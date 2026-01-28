#pragma once
#include "../../CoreFramework_CPP/Device/IDevice.h"
#include "../../CoreFramework_CPP/Communication/UsbComm.h"
#include "../../CoreFramework_CPP/Communication/TcpComm.h"
#include "../../CoreFramework_CPP/Communication/BluetoothComm.h"
#include "../../CoreFramework_CPP/Factory/DeviceFactory.h"

namespace ACLDevicePlatform {
namespace Rfid {
namespace RFD40 {

    /**
     * @brief ACL RFD40 RFID reader driver implementation
     * 
     * Implements the IDevice interface for the ACL RFD40 RFID reader model.
     * Supports USB, TCP/IP, and Bluetooth communication.
     */
    class RFD40Driver : public ACLDevicePlatform::Core::IDevice {
    public:
        RFD40Driver();
        virtual ~RFD40Driver();

        // IDevice interface implementation
        virtual bool Connect(const std::string& connectionString) override;
        virtual bool Disconnect() override;
        virtual bool Send(const std::string& data) override;
        virtual std::string GetModel() const override;
        virtual ACLDevicePlatform::Core::DeviceType GetDeviceType() const override;
        virtual ACLDevicePlatform::Core::DeviceStatus GetStatus() const override;
        virtual std::string GetLastError() const override;
        virtual bool Initialize() override;

        /**
         * @brief Read RFID tags
         * @param timeoutMs Timeout in milliseconds
         * @return List of detected RFID tag IDs
         */
        std::vector<std::string> ReadTags(int timeoutMs = 5000);

        /**
         * @brief Get RFID reader status
         * @return Reader status information
         */
        std::string GetReaderStatus();

        /**
         * @brief Perform reader self-test
         * @return true if self-test successful, false otherwise
         */
        bool PerformSelfTest();

    private:
        ACLDevicePlatform::Core::DeviceStatus m_status;
        std::string m_lastError;
        std::unique_ptr<ACLDevicePlatform::Core::Communication::UsbComm> m_usbComm;
        std::unique_ptr<ACLDevicePlatform::Core::Communication::TcpComm> m_tcpComm;
        std::unique_ptr<ACLDevicePlatform::Core::Communication::BluetoothComm> m_btComm;
        ACLDevicePlatform::Core::Communication::ICommunication* m_activeComm;

        /**
         * @brief Parse connection string and initialize appropriate communication
         * @param connectionString Connection parameters
         * @return true if initialization successful, false otherwise
         */
        bool InitializeCommunication(const std::string& connectionString);

        /**
         * @brief Send command to RFID reader and wait for response
         * @param command Command to send
         * @param response Buffer to store response
         * @param timeoutMs Timeout in milliseconds
         * @return true if command successful, false otherwise
         */
        bool SendCommand(const std::string& command, std::string& response, int timeoutMs = 1000);

        /**
         * @brief Validate RFID reader response
         * @param response Response from reader
         * @return true if response is valid, false otherwise
         */
        bool ValidateResponse(const std::string& response);
    };

} // namespace RFD40
} // namespace Rfid
} // namespace ACLDevicePlatform

// Register this driver with the factory
REGISTER_DEVICE("RFD40", ACLDevicePlatform::Rfid::RFD40::RFD40Driver);