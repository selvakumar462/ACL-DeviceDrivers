#include "UsbComm.h"
#include <windows.h>
#include <initguid.h>
#include <setupapi.h>
#include <devguid.h>
#include <usbiodef.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "setupapi.lib")

namespace ACLDevicePlatform {
namespace Core {
namespace Communication {

    UsbComm::UsbComm() : m_hDevice(INVALID_HANDLE_VALUE), m_isConnected(false) {
    }

    UsbComm::~UsbComm() {
        Disconnect();
    }

    bool UsbComm::Connect(const std::string& connectionString) {
        if (m_isConnected) {
            Disconnect();
        }

        // Parse connection string format: "USB:VID_XXXX&PID_XXXX"
        std::string vidPid = connectionString.substr(4); // Remove "USB:" prefix
        
        m_hDevice = FindACLUsbDevice(vidPid);
        if (m_hDevice == INVALID_HANDLE_VALUE) {
            m_lastError = "Failed to find ACL USB device: " + connectionString;
            return false;
        }

        m_isConnected = true;
        m_lastError.clear();
        return true;
    }

    bool UsbComm::Disconnect() {
        if (m_hDevice != INVALID_HANDLE_VALUE) {
            CloseHandle(m_hDevice);
            m_hDevice = INVALID_HANDLE_VALUE;
        }
        m_isConnected = false;
        return true;
    }

    bool UsbComm::Send(const std::string& data) {
        if (!m_isConnected || m_hDevice == INVALID_HANDLE_VALUE) {
            m_lastError = "Device not connected";
            return false;
        }

        DWORD bytesWritten;
        BOOL result = WriteFile(m_hDevice, data.c_str(), static_cast<DWORD>(data.length()), &bytesWritten, nullptr);
        
        if (!result || bytesWritten != data.length()) {
            m_lastError = "Failed to write to USB device";
            return false;
        }

        return true;
    }

    bool UsbComm::Receive(std::string& data, int timeoutMs) {
        if (!m_isConnected || m_hDevice == INVALID_HANDLE_VALUE) {
            m_lastError = "Device not connected";
            return false;
        }

        char buffer[4096];
        DWORD bytesRead;
        BOOL result = ReadFile(m_hDevice, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);
        
        if (!result) {
            m_lastError = "Failed to read from USB device";
            return false;
        }

        buffer[bytesRead] = '\0';
        data = std::string(buffer, bytesRead);
        return true;
    }

    std::string UsbComm::GetLastError() const {
        return m_lastError;
    }

    bool UsbComm::IsConnected() const {
        return m_isConnected;
    }

    HANDLE UsbComm::FindACLUsbDevice(const std::string& vidPid) {
        // ACL USB Vendor ID: 0x0A5F
        GUID guid = GUID_DEVINTERFACE_USB_DEVICE;
        HDEVINFO deviceInfo = SetupDiGetClassDevs(&guid, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
        
        if (deviceInfo == INVALID_HANDLE_VALUE) {
            return INVALID_HANDLE_VALUE;
        }

        SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
        deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

        for (DWORD i = 0; SetupDiEnumDeviceInterfaces(deviceInfo, nullptr, &guid, i, &deviceInterfaceData); i++) {
            DWORD requiredSize = 0;
            SetupDiGetDeviceInterfaceDetail(deviceInfo, &deviceInterfaceData, nullptr, 0, &requiredSize, nullptr);

            if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                continue;
            }

            PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = 
                reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(new BYTE[requiredSize]);
            detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            if (SetupDiGetDeviceInterfaceDetail(deviceInfo, &deviceInterfaceData, detailData, requiredSize, nullptr, nullptr)) {
                HANDLE deviceHandle = CreateFile(detailData->DevicePath, 
                    GENERIC_READ | GENERIC_WRITE, 
                    FILE_SHARE_READ | FILE_SHARE_WRITE, 
                    nullptr, 
                    OPEN_EXISTING, 
                    0, 
                    nullptr);

                if (deviceHandle != INVALID_HANDLE_VALUE) {
                    // Verify this is an ACL device by checking the path
#ifdef UNICODE
                    std::wstring wpath(detailData->DevicePath);
                    std::string path(wpath.begin(), wpath.end());
#else
                    std::string path(detailData->DevicePath);
#endif
                    if (path.find("VID_0A5F") != std::string::npos) { // ACL VID
                        delete[] detailData;
                        SetupDiDestroyDeviceInfoList(deviceInfo);
                        return deviceHandle;
                    } else {
                        CloseHandle(deviceHandle);
                    }
                }
            }

            delete[] detailData;
        }

        SetupDiDestroyDeviceInfoList(deviceInfo);
        return INVALID_HANDLE_VALUE;
    }

} // namespace Communication
} // namespace Core
} // namespace ACLDevicePlatform
