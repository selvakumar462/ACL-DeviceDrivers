#include "FX9600Driver.h"
#include <sstream>
#include <thread>
#include <chrono>

namespace ACLDevicePlatform {
namespace Rfid {
namespace FX9600 {

    FX9600Driver::FX9600Driver() 
        : m_status(ACLDevicePlatform::Core::DeviceStatus::Disconnected),
          m_lastError(),
          m_usbComm(),
          m_tcpComm(),
          m_btComm(),
          m_activeComm(nullptr) {
    }

    FX9600Driver::~FX9600Driver() {
        Disconnect();
    }

    bool FX9600Driver::Connect(const std::string& connectionString) {
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
            m_lastError = "Failed to communicate with FX9600 RFID reader";
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        m_status = ACLDevicePlatform::Core::DeviceStatus::Connected;
        m_lastError.clear();
        return true;
    }

    bool FX9600Driver::Disconnect() {
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

    bool FX9600Driver::Send(const std::string& data) {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected || !m_activeComm) {
            m_lastError = "RFID reader not connected";
            return false;
        }

        if (!m_activeComm->Send(data)) {
            m_lastError = "Failed to send data to FX9600 RFID reader: " + m_activeComm->GetLastError();
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            return false;
        }

        return true;
    }

    std::string FX9600Driver::GetModel() const {
        return "FX9600";
    }

    ACLDevicePlatform::Core::DeviceType FX9600Driver::GetDeviceType() const {
        return ACLDevicePlatform::Core::DeviceType::RFID;
    }

    ACLDevicePlatform::Core::DeviceStatus FX9600Driver::GetStatus() const {
        return m_status;
    }

    std::string FX9600Driver::GetLastError() const {
        return m_lastError;
    }

    bool FX9600Driver::Initialize() {
        // Perform any model-specific initialization
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "RFID reader not connected";
            return false;
        }

        // Send initialization commands
        std::string initCommands = "CONFIG:DEFAULT";

        std::string response;
        if (!SendCommand(initCommands, response, 1000)) {
            m_lastError = "Failed to initialize FX9600 RFID reader";
            return false;
        }

        return true;
    }

    std::vector<std::string> FX9600Driver::ReadTags(int timeoutMs) {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "RFID reader not connected";
            return {};
        }

        // Send read tags command
        std::string readCommand = "READ_TAGS";
        std::string response;

        if (!SendCommand(readCommand, response, timeoutMs)) {
            m_lastError = "Failed to read RFID tags: " + m_lastError;
            return {};
        }

        // Parse response and extract tag IDs
        std::vector<std::string> tagIds;
        if (ValidateResponse(response)) {
            // Simple parsing - split by delimiter
            std::istringstream iss(response);
            std::string tagId;
            while (std::getline(iss, tagId, ',')) {
                if (!tagId.empty()) {
                    tagIds.push_back(tagId);
                }
            }
        }

        return tagIds;
    }

    std::string FX9600Driver::GetReaderStatus() {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            return "RFID reader not connected";
        }

        std::string statusCommand = "STATUS";

        std::string response;
        if (SendCommand(statusCommand, response, 1000)) {
            return "Status: " + response;
        }

        return "Unable to retrieve status: " + m_lastError;
    }

    bool FX9600Driver::PerformSelfTest() {
        if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected) {
            m_lastError = "RFID reader not connected";
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

    bool FX9600Driver::InitializeCommunication(const std::string& connectionString) {
        // Parse connection string to determine communication type
        if (connectionString.substr(0, 4) == "USB:") {
            m_usbComm = std::make_unique<ACLDevicePlatform::Core::Communication::UsbComm>();
            m_activeComm = m_usbComm.get();
            return m_usbComm->Connect(connectionString);
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

    bool FX9600Driver::SendCommand(const std::string& command, std::string& response, int timeoutMs) {
        if (!m_activeComm || !m_activeComm->IsConnected()) {
            m_lastError = "No active communication channel";
            return false;
        }

        if (!m_activeComm->Send(command)) {
            m_lastError = "Failed to send command: " + m_activeComm->GetLastError();
            return false;
        }

        // For RFID commands, we typically expect a response
        if (!m_activeComm->Receive(response, timeoutMs)) {
            m_lastError = "Failed to receive response: " + m_activeComm->GetLastError();
            return false;
        }

        return true;
    }

    bool FX9600Driver::ValidateResponse(const std::string& response) {
        // Basic validation - check if response contains expected content
        // For FX9600, responses should be non-empty and contain valid data
        return !response.empty();
    }

} // namespace FX9600
} // namespace Rfid
} // namespace ACLDevicePlatform