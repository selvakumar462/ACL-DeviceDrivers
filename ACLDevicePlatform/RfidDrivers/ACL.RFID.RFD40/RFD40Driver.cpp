#include "RFD40Driver.h"
#include <iostream>
#include <algorithm>
#include <string>

namespace ACLDevicePlatform {
namespace Rfid {
namespace RFD40 {

    RFD40Driver::RFD40Driver()
        : m_status(ACLDevicePlatform::Core::DeviceStatus::Disconnected)
        , m_lastError("")
        , m_activeComm(nullptr)
    {
    }

    RFD40Driver::~RFD40Driver()
    {
        Disconnect();
    }

    bool RFD40Driver::Connect(const std::string& connectionString)
    {
        try
        {
            if (!InitializeCommunication(connectionString))
            {
                m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
                m_lastError = "Failed to initialize communication";
                return false;
            }

            // Send initialization command
            std::string response;
            if (!SendCommand("INIT", response, 2000))
            {
                m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
                m_lastError = "Failed to initialize RFD40 reader";
                return false;
            }

            if (!ValidateResponse(response))
            {
                m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
                m_lastError = "Invalid response from RFD40 reader";
                return false;
            }

            m_status = ACLDevicePlatform::Core::DeviceStatus::Connected;
            m_lastError = "";
            return true;
        }
        catch (const std::exception& e)
        {
            m_status = ACLDevicePlatform::Core::DeviceStatus::Error;
            m_lastError = e.what();
            return false;
        }
    }

    bool RFD40Driver::Disconnect()
    {
        try
        {
            if (m_activeComm != nullptr)
            {
                m_activeComm->Disconnect();
            }

            m_status = ACLDevicePlatform::Core::DeviceStatus::Disconnected;
            m_lastError = "";
            return true;
        }
        catch (const std::exception& e)
        {
            m_lastError = e.what();
            return false;
        }
    }

    bool RFD40Driver::Send(const std::string& data)
    {
        try
        {
            if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected)
            {
                m_lastError = "Device not connected";
                return false;
            }

            if (m_activeComm == nullptr)
            {
                m_lastError = "No active communication channel";
                return false;
            }

            return m_activeComm->Send(data);
        }
        catch (const std::exception& e)
        {
            m_lastError = e.what();
            return false;
        }
    }

    std::string RFD40Driver::GetModel() const
    {
        return "RFD40";
    }

    ACLDevicePlatform::Core::DeviceType RFD40Driver::GetDeviceType() const
    {
        return ACLDevicePlatform::Core::DeviceType::RFID;
    }

    ACLDevicePlatform::Core::DeviceStatus RFD40Driver::GetStatus() const
    {
        return m_status;
    }

    std::string RFD40Driver::GetLastError() const
    {
        return m_lastError;
    }

    bool RFD40Driver::Initialize()
    {
        // RFD40 specific initialization
        std::string response;
        if (!SendCommand("INIT", response, 2000))
        {
            m_lastError = "Failed to initialize RFD40 reader";
            return false;
        }

        return ValidateResponse(response);
    }

    std::vector<std::string> RFD40Driver::ReadTags(int timeoutMs)
    {
        try
        {
            if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected)
            {
                m_lastError = "Device not connected";
                return {};
            }

            std::string response;
            std::string command = "READ_TAGS";
            
            if (!SendCommand(command, response, timeoutMs))
            {
                m_lastError = "Failed to read tags";
                return {};
            }

            if (!ValidateResponse(response))
            {
                m_lastError = "Invalid response from reader";
                return {};
            }

            // Parse response to extract tag IDs
            std::vector<std::string> tags;
            // Implementation would parse the response format specific to RFD40
            // For now, return empty vector
            return tags;
        }
        catch (const std::exception& e)
        {
            m_lastError = e.what();
            return {};
        }
    }

    std::string RFD40Driver::GetReaderStatus()
    {
        try
        {
            if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected)
            {
                return "Disconnected";
            }

            std::string response;
            if (!SendCommand("STATUS", response, 1000))
            {
                return "Error: " + m_lastError;
            }

            return response;
        }
        catch (const std::exception& e)
        {
            return "Error: " + std::string(e.what());
        }
    }

    bool RFD40Driver::PerformSelfTest()
    {
        try
        {
            if (m_status != ACLDevicePlatform::Core::DeviceStatus::Connected)
            {
                m_lastError = "Device not connected";
                return false;
            }

            std::string response;
            if (!SendCommand("SELF_TEST", response, 5000))
            {
                m_lastError = "Self-test command failed";
                return false;
            }

            return ValidateResponse(response);
        }
        catch (const std::exception& e)
        {
            m_lastError = e.what();
            return false;
        }
    }

    bool RFD40Driver::InitializeCommunication(const std::string& connectionString)
    {
        try
        {
            // Parse connection string
            if (connectionString.substr(0, 4) == "USB:")
            {
                m_usbComm = std::make_unique<ACLDevicePlatform::Core::Communication::UsbComm>();
                m_activeComm = m_usbComm.get();
                return m_activeComm->Connect(connectionString);
            }
            else if (connectionString.substr(0, 4) == "TCP:")
            {
                m_tcpComm = std::make_unique<ACLDevicePlatform::Core::Communication::TcpComm>();
                m_activeComm = m_tcpComm.get();
                return m_activeComm->Connect(connectionString);
            }
            else if (connectionString.substr(0, 3) == "BT:")
            {
                m_btComm = std::make_unique<ACLDevicePlatform::Core::Communication::BluetoothComm>();
                m_activeComm = m_btComm.get();
                return m_activeComm->Connect(connectionString);
            }
            else
            {
                m_lastError = "Invalid connection string format";
                return false;
            }
        }
        catch (const std::exception& e)
        {
            m_lastError = e.what();
            return false;
        }
    }

    bool RFD40Driver::SendCommand(const std::string& command, std::string& response, int timeoutMs)
    {
        try
        {
            if (m_activeComm == nullptr)
            {
                m_lastError = "No active communication channel";
                return false;
            }

            // Send command
            if (!m_activeComm->Send(command))
            {
                m_lastError = "Failed to send command";
                return false;
            }

            // Receive response
            if (!m_activeComm->Receive(response, timeoutMs))
            {
                m_lastError = "Failed to receive response";
                return false;
            }

            return true;
        }
        catch (const std::exception& e)
        {
            m_lastError = e.what();
            return false;
        }
    }

    bool RFD40Driver::ValidateResponse(const std::string& response)
    {
        // Basic validation - check if response is not empty and doesn't contain error indicators
        if (response.empty())
        {
            return false;
        }

        // Check for common error indicators
        std::string upperResponse = response;
        // Convert to uppercase for case-insensitive comparison
        std::transform(upperResponse.begin(), upperResponse.end(), upperResponse.begin(), ::toupper);

        if (upperResponse.find("ERROR") != std::string::npos ||
            upperResponse.find("FAIL") != std::string::npos ||
            upperResponse.find("INVALID") != std::string::npos)
        {
            return false;
        }

        return true;
    }

} // namespace RFD40
} // namespace Rfid
} // namespace ACLDevicePlatform