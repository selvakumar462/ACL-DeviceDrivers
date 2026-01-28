#include "GK420Driver.h"
#include <sstream>
#include <thread>
#include <chrono>

namespace ACLDevicePlatform {
namespace Printer {
namespace GK420 {

    GK420Driver::GK420Driver() 
        : m_status(ACLDevicePlatform::Core::DeviceStatus::Disconnected),
          m_lastError(),
          m_usbComm(),
          m_tcpComm(),
          m_btComm(),
          m_activeComm(nullptr) {
    }

    GK420Driver::~GK420Driver() {
        Disconnect();
    }

    bool GK420Driver::Connect(const std::string& connectionString) {
        if (m_status == ACLDevicePlatform::Core::DeviceStatus::Connected) {
            Disconnect();
        }

        if (!InitializeCommunication(connectionString)) {
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        // Test connection with a simple status command
        std::string response;
        if (!SendCommand("^XA^HON^XZ", response, 2000)) {
            m_lastError = "Failed to communicate with GK420 printer";
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        m_status = ACLDevicePlatform::Core::DeviceStatus::Connected;
        m_lastError.clear();
        return true;
    }

    bool GK420Driver::Disconnect() {
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

    bool GK420Driver::Send(const std::string& data) {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected || !m_activeComm) {
            m_lastError = "Printer not connected";
            return false;
        }

        if (!m_activeComm->Send(data)) {
            m_lastError = "Failed to send data to GK420 printer: " + m_activeComm->GetLastError();
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        return true;
    }

    std::string GK420Driver::GetModel() const {
        return "GK420";
    }

    ACLDevicePlatform::Core::DeviceType GK420Driver::GetDeviceType() const {
        return ACLDevicePlatform::Core::DeviceType::Printer;
    }

    ACLDevicePlatform::Core::DeviceStatus GK420Driver::GetStatus() const {
        return m_status;
    }

    std::string GK420Driver::GetLastError() const {
        return m_lastError;
    }

    bool GK420Driver::Initialize() {
        // Perform any model-specific initialization
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "Printer not connected";
            return false;
        }

        // Send initialization commands
        std::string initCommands = "^XA"
                                 "^MNN"  // Media tracking: continuous
                                 "^LL600" // Label length
                                 "^PW400" // Print width
                                 "^XZ";

        std::string response;
        if (!SendCommand(initCommands, response, 1000)) {
            m_lastError = "Failed to initialize GK420 printer";
            return false;
        }

        return true;
    }

    bool GK420Driver::PrintLabel(const std::string& labelContent) {
        if (!Send(labelContent)) {
            return false;
        }

        // Wait a moment for the printer to process
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return true;
    }

    std::string GK420Driver::GetPrinterStatus() {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            return "Printer not connected";
        }

        std::string statusCommand = "^XA"
                                  "^HON"  // Host status on
                                  "^XZ";

        std::string response;
        if (SendCommand(statusCommand, response, 1000)) {
            return "Status: " + response;
        }

        return "Unable to retrieve status: " + m_lastError;
    }

    bool GK420Driver::PerformSelfTest() {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "Printer not connected";
            return false;
        }

        // Send self-test command
        std::string selfTestCommand = "^XA"
                                    "^JUS"  // Self-test
                                    "^XZ";

        std::string response;
        if (!SendCommand(selfTestCommand, response, 5000)) { // Longer timeout for self-test
            m_lastError = "Self-test failed: " + m_lastError;
            return false;
        }

        return true;
    }

    bool GK420Driver::InitializeCommunication(const std::string& connectionString) {
        // Parse connection string to determine communication type
        if (connectionString.substr(0, 4) == "USB:") {
            m_usbComm = std::make_unique<ACLDevicePlatform::Core::Communication::UsbComm>();
            m_activeComm = m_usbComm.get();
            return m_activeComm->Connect(connectionString);
        }
        else if (connectionString.substr(0, 4) == "TCP:") {
            m_tcpComm = std::make_unique<ACLDevicePlatform::Core::Communication::TcpComm>();
            m_activeComm = m_tcpComm.get();
            return m_activeComm->Connect(connectionString);
        }
        else if (connectionString.substr(0, 3) == "BT:") {
            m_btComm = std::make_unique<ACLDevicePlatform::Core::Communication::BluetoothComm>();
            m_activeComm = m_btComm.get();
            return m_activeComm->Connect(connectionString);
        }
        else {
            m_lastError = "Unsupported connection type: " + connectionString;
            return false;
        }
    }

    bool GK420Driver::SendCommand(const std::string& command, std::string& response, int timeoutMs) {
        if (!m_activeComm || !m_activeComm->IsConnected()) {
            m_lastError = "No active communication channel";
            return false;
        }

        if (!m_activeComm->Send(command)) {
            m_lastError = "Failed to send command: " + m_activeComm->GetLastError();
            return false;
        }

        // For some commands, we might want to read a response
        // For GK420, most ZPL commands don't return responses, but status commands do
        if (command.find("^HON") != std::string::npos) {
            if (!m_activeComm->Receive(response, timeoutMs)) {
                m_lastError = "Failed to receive response: " + m_activeComm->GetLastError();
                return false;
            }
        }

        return true;
    }

    bool GK420Driver::ValidateResponse(const std::string& response) {
        // Basic validation - check if response contains expected content
        // For GK420, responses are typically simple status indicators
        return !response.empty();
    }

} // namespace GK420
} // namespace Printer
} // namespace ACLDevicePlatform