#pragma once
#include "../../CoreFramework_CPP/Device/IDevice.h"
#include "../../CoreFramework_CPP/Communication/UsbComm.h"
#include "../../CoreFramework_CPP/Communication/TcpComm.h"
#include "../../CoreFramework_CPP/Communication/BluetoothComm.h"
#include "../../CoreFramework_CPP/Factory/DeviceFactory.h"

namespace ACLDevicePlatform {
namespace Scanner {
namespace DS4608 {

    class DS4608Driver : public ACLDevicePlatform::Core::IDevice {
    public:
        DS4608Driver();
        virtual ~DS4608Driver();

        virtual bool Connect(const std::string& connectionString) override;
        virtual bool Disconnect() override;
        virtual bool Send(const std::string& data) override;
        virtual std::string GetModel() const override;
        virtual ACLDevicePlatform::Core::DeviceType GetDeviceType() const override;
        virtual ACLDevicePlatform::Core::DeviceStatus GetStatus() const override;
        virtual std::string GetLastError() const override;
        virtual bool Initialize() override;

        std::string ScanBarcode(int timeoutMs = 5000);
        std::string GetScannerStatus();
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

    REGISTER_DEVICE("DS4608", DS4608Driver);

} // namespace DS4608
} // namespace Scanner
} // namespace ACLDevicePlatform