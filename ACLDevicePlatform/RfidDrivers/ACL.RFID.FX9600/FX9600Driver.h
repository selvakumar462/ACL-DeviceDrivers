#pragma once
#include "../../CoreFramework_CPP/Device/IDevice.h"
#include "../../CoreFramework_CPP/Communication/UsbComm.h"
#include "../../CoreFramework_CPP/Communication/TcpComm.h"
#include "../../CoreFramework_CPP/Communication/BluetoothComm.h"
#include "../../CoreFramework_CPP/Factory/DeviceFactory.h"

namespace ACLDevicePlatform {
namespace Rfid {
namespace FX9600 {

    class FX9600Driver : public ACLDevicePlatform::Core::IDevice {
    public:
        FX9600Driver();
        virtual ~FX9600Driver();

        virtual bool Connect(const std::string& connectionString) override;
        virtual bool Disconnect() override;
        virtual bool Send(const std::string& data) override;
        virtual std::string GetModel() const override;
        virtual ACLDevicePlatform::Core::DeviceType GetDeviceType() const override;
        virtual ACLDevicePlatform::Core::DeviceStatus GetStatus() const override;
        virtual std::string GetLastError() const override;
        virtual bool Initialize() override;

        std::vector<std::string> ReadTags(int timeoutMs = 5000);
        std::string GetReaderStatus();
        bool PerformSelfTest();

    private:
        ACLDevicePlatform::Core::DeviceStatus m_status;
        std::string m_lastError;
        std::unique_ptr<ACLDevicePlatform::Core::Communication::UsbComm> m_usbComm;
        std::unique_ptr<ACLDevicePlatform::Core::Communication::TcpComm> m_tcpComm;
        std::unique_ptr<ACLDevicePlatform::Core::Communication::BluetoothComm> m_btComm;
        ACLDevicePlatform::Core::Communication::ICommunication* m_activeComm;

        bool InitializeCommunication(const std::string& connectionString);
        bool SendCommand(const std::string& command, std::string& response, int timeoutMs = 1000);
        bool ValidateResponse(const std::string& response);
    };

    REGISTER_DEVICE("FX9600", FX9600Driver);

} // namespace FX9600
} // namespace Rfid
} // namespace ACLDevicePlatform