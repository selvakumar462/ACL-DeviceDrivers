#pragma once
#include <string>

namespace ACLDevicePlatform {
namespace Core {
namespace Communication {

    /**
     * @brief Abstract interface for communication with ACL devices
     * 
     * Provides a common interface for different communication protocols
     * (USB, TCP/IP, Bluetooth) used by ACL devices.
     */
    class ICommunication {
    public:
        virtual ~ICommunication() = default;

        /**
         * @brief Connect to the device
         * @param connectionString Connection parameters specific to the protocol
         * @return true if connection successful, false otherwise
         */
        virtual bool Connect(const std::string& connectionString) = 0;

        /**
         * @brief Disconnect from the device
         * @return true if disconnection successful, false otherwise
         */
        virtual bool Disconnect() = 0;

        /**
         * @brief Send data to the device
         * @param data Data to send
         * @return true if send successful, false otherwise
         */
        virtual bool Send(const std::string& data) = 0;

        /**
         * @brief Receive data from the device
         * @param data Buffer to store received data
         * @param timeoutMs Timeout in milliseconds
         * @return true if receive successful, false otherwise
         */
        virtual bool Receive(std::string& data, int timeoutMs = 1000) = 0;

        /**
         * @brief Get the last error message
         * @return Error message string
         */
        virtual std::string GetLastError() const = 0;

        /**
         * @brief Check if device is connected
         * @return true if connected, false otherwise
         */
        virtual bool IsConnected() const = 0;
    };

} // namespace Communication
} // namespace Core
} // namespace ACLDevicePlatform