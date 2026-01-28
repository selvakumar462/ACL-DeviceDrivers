#include "LI4278Driver.h"
#include <sstream>
#include <thread>
#include <chrono>

namespace ACLDevicePlatform {
namespace Scanner {
namespace LI4278 {

    LI4278Driver::LI4278Driver() 
        : m_status(ACLDevicePlatform::Core::DeviceStatus::Disconnected),
          m_lastError(),
          m_usbComm(),
          m_tcpComm(),
          m_btComm(),
          m_activeComm(nullptr) {
    }

    LI4278Driver::~LI4278Driver() {
        Disconnect();
    }

    bool LI4278Driver::Connect(const std::string& connectionString) {
        if (m_status == ACLDevicePlatform::Core::DeviceStatus::Connected) {
            Disconnect();
        }

        if (!InitializeCommunication(connectionString)) {
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        // Test connection with a simple status command
        std::string response;
        if (!SendCommand("STATUS", response, 2000)) {
            m_lastError = "Failed to communicate with LI4278 scanner";
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        m_status = ACLDevicePlatform::Core::DeviceStatus::Connected;
        m_lastError.clear();
        return true;
    }

    bool LI4278Driver::Disconnect() {
        if (m_usbComm && m_usbComm->IsConnected()) {
            m_usbComm->Disconnect();
        }
        if (m_tcpComm && m_tcpComm->IsConnected()) {
            m_tcpComm->Disconnect();
        }
        if (m_btComm && m_btComm->IsConnected()) {
            m_btComm->Disconnect();
        }

        m_usbComm.reset();
        m_tcpComm.reset();
        m_btComm.reset();
        m_activeComm = nullptr;
        m_status = ACLDevicePlatform::Core::DeviceStatus::Disconnected;
        return true;
    }

    bool LI4278Driver::Send(const std::string& data) {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected || !m_activeComm) {
            m_lastError = "Scanner not connected";
            return false;
        }

        if (!m_activeComm->Send(data)) {
            m_lastError = "Failed to send data to LI4278 scanner: " + m_activeComm->GetLastError();
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        return true;
    }

    std::string LI4278Driver::GetModel() const {
        return "LI4278";
    }

    ACLDevicePlatform::Core::DeviceType LI4278Driver::GetDeviceType() const {
        return ACLDevicePlatform::Core::DeviceType::Scanner;
    }

    ACLDevicePlatform::Core::DeviceStatus LI4278Driver::GetStatus() const {
        return m_status;
    }

    std::string LI4278Driver::GetLastError() const {
        return m_lastError;
    }

    bool LI4278Driver::Initialize() {
        // Perform any model-specific initialization
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "Scanner not connected";
            return false;
        }

        // Send initialization commands
        std::string initCommands = "CONFIG:DEFAULT";

        std::string response;
        if (!SendCommand(initCommands, response, 1000)) {
            m_lastError = "Failed to initialize LI4278 scanner";
            return false;
        }

        return true;
    }

    std::string LI4278Driver::ScanBarcode(int timeoutMs) {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "Scanner not connected";
            return "";
        }

        // Send scan command
        std::string scanCommand = "SCAN";
        std::string response;

        if (!SendCommand(scanCommand, response, timeoutMs)) {
            m_lastError = "Failed to scan barcode: " + m_lastError;
            return "";
        }

        // Validate and return the scanned data
        if (ValidateResponse(response)) {
            return response;
        }

        m_lastError = "Invalid barcode data received";
        return "";
    }

    std::string LI4278Driver::GetScannerStatus() {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            return "Scanner not connected";
        }

        std::string statusCommand = "STATUS";

        std::string response;
        if (SendCommand(statusCommand, response, 1000)) {
            return "Status: " + response;
        }

        return "Unable to retrieve status: " + m_lastError;
    }

    bool LI4278Driver::PerformSelfTest() {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "Scanner not connected";
            return false;
        }

        // Send self-test command
        std::string selfTestCommand = "SELFTEST";

        std::string response;
        if (!SendCommand(selfTestCommand, response, 5000)) { // Longer timeout for self-test
            m_lastError = "Self-test failed: " + m_lastError;
            return false;
        }

        return true;
    }

    bool LI4278Driver::InitializeCommunication(const std::string& connectionString) {
        // Parse connection string to determine communication type
        if (connectionString.substr(0, 4) == "USB:") {
            m_usbComm = std::make_unique<ACLDevicePlatform::Core::Communication::UsbComm>();
            m_activeComm = m_usbComm.get();
            return m_usbComm->Connect(connectionString);
        }
        else if (connectionString.substr(0, 4) == "TCP:") {
            m_tcpComm = std::make_unique<ACLDevicePlatform::Core::Communication::TcpComm>();
            m_activeComm = m_tcpComm.get();
            return m_tcpComm->Connect(connectionString);
        }
        else if (connectionString.substr(0, 3) == "BT:") {
            m_btComm = std::make_unique<ACLDevicePlatform::Core::Communication::BluetoothComm>();
            m_activeComm = m_btComm.get();
            return m_btComm->Connect(connectionString);
        }
        else {
            m_lastError = "Unsupported connection type: " + connectionString;
            return false;
        }
    }

    bool LI4278Driver::SendCommand(const std::string& command, std::string& response, int timeoutMs) {
        if (!m_activeComm || !m_activeComm->IsConnected()) {
            m_lastError = "No active communication channel";
            return false;
        }

        if (!m_activeComm->Send(command)) {
            m_lastError = "Failed to send command: " + m_activeComm->GetLastError();
            return false;
        }

        // For scanner commands, we typically expect a response
        if (!m_activeComm->Receive(response, timeoutMs)) {
            m_lastError = "Failed to receive response: " + m_activeComm->GetLastError();
            return false;
        }

        return true;
    }

    bool LI4278Driver::ValidateResponse(const std::string& response) {
        // Basic validation - check if response contains expected content
        // For LI4278, responses should be non-empty and contain valid data
        return !response.empty();
    }

} // namespace LI4278
} // namespace Scanner
} // namespace ACLDevicePlatform