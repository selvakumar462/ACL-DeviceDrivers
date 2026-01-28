#pragma once
#include <string>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#include "ICommunication.h"

namespace ACLDevicePlatform {
namespace Core {
namespace Communication {

    /**
     * @brief USB communication interface for ACL devices
     * 
     * Provides USB communication capabilities using Windows Win32 API.
     * Handles device enumeration, connection, and data transfer.
     */
    class UsbComm : public ICommunication {
    public:
        UsbComm();
        ~UsbComm();

        /**
         * @brief Connect to a ACL USB device
         * @param connectionString Format: "USB:VID_XXXX&PID_XXXX"
         * @return true if connection successful, false otherwise
         */
        bool Connect(const std::string& connectionString);

        /**
         * @brief Disconnect from the USB device
         * @return true if disconnection successful, false otherwise
         */
        bool Disconnect();

        /**
         * @brief Send data to the USB device
         * @param data Data to send
         * @return true if send successful, false otherwise
         */
        bool Send(const std::string& data);

        /**
         * @brief Receive data from the USB device
         * @param data Buffer to store received data
         * @param timeoutMs Timeout in milliseconds
         * @return true if receive successful, false otherwise
         */
        bool Receive(std::string& data, int timeoutMs = 1000);

        /**
         * @brief Get the last error message
         * @return Error message string
         */
        virtual std::string GetLastError() const override;

        /**
         * @brief Check if device is connected
         * @return true if connected, false otherwise
         */
        virtual bool IsConnected() const override;

    private:
        HANDLE m_hDevice;
        bool m_isConnected;
        std::string m_lastError;

        /**
         * @brief Find ACL USB device by VID/PID
         * @param vidPid Vendor and Product ID string
         * @return Handle to device or INVALID_HANDLE_VALUE
         */
        HANDLE FindACLUsbDevice(const std::string& vidPid);
    };

} // namespace Communication
} // namespace Core
} // namespace ACLDevicePlatform