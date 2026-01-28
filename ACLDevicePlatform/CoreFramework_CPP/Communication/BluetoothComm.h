#pragma once
#include <string>
#include <vector>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#include <bluetoothapis.h>
#include "ICommunication.h"

#pragma comment(lib, "Bthprops.lib")

namespace ACLDevicePlatform {
namespace Core {
namespace Communication {

    /**
     * @brief Bluetooth communication interface for ACL devices
     * 
     * Provides Bluetooth communication capabilities using Windows Bluetooth API.
     * Handles device discovery, pairing, and data transfer.
     */
    class BluetoothComm : public ICommunication {
    public:
        BluetoothComm();
        ~BluetoothComm();

        /**
         * @brief Connect to a ACL Bluetooth device
         * @param connectionString Format: "BT:DEVICE_ADDRESS"
         * @return true if connection successful, false otherwise
         */
        bool Connect(const std::string& connectionString);

        /**
         * @brief Disconnect from the Bluetooth device
         * @return true if disconnection successful, false otherwise
         */
        bool Disconnect();

        /**
         * @brief Send data to the Bluetooth device
         * @param data Data to send
         * @return true if send successful, false otherwise
         */
        bool Send(const std::string& data);

        /**
         * @brief Receive data from the Bluetooth device
         * @param data Buffer to store received data
         * @param timeoutMs Timeout in milliseconds
         * @return true if receive successful, false otherwise
         */
        bool Receive(std::string& data, int timeoutMs = 5000);

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

        /**
         * @brief Discover nearby Bluetooth devices
         * @param devices Output vector to store discovered device addresses
         * @return true if discovery successful, false otherwise
         */
        bool DiscoverDevices(std::vector<std::string>& devices);

    private:
        HANDLE m_hDevice;
        bool m_isConnected;
        std::string m_lastError;
        BLUETOOTH_ADDRESS m_deviceAddress;

        /**
         * @brief Parse connection string for Bluetooth address
         * @param connectionString Connection string in format "BT:ADDRESS"
         * @return true if parsing successful, false otherwise
         */
        bool ParseConnectionString(const std::string& connectionString);

        /**
         * @brief Convert string address to Bluetooth address structure
         * @param addressStr Bluetooth address string
         * @param address Output Bluetooth address structure
         * @return true if conversion successful, false otherwise
         */
        bool StringToBtAddress(const std::string& addressStr, BLUETOOTH_ADDRESS& address);
    };

} // namespace Communication
} // namespace Core
} // namespace ACLDevicePlatform