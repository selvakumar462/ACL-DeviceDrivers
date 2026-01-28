#pragma once
#include <string>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ICommunication.h"

#pragma comment(lib, "ws2_32.lib")

namespace ACLDevicePlatform {
namespace Core {
namespace Communication {

    /**
     * @brief TCP/IP communication interface for ACL devices
     * 
     * Provides TCP communication capabilities for network-connected ACL devices.
     * Handles socket operations, connection management, and data transfer.
     */
    class TcpComm : public ICommunication {
    public:
        TcpComm();
        ~TcpComm();

        /**
         * @brief Connect to a ACL TCP device
         * @param connectionString Format: "TCP:IP_ADDRESS:PORT"
         * @return true if connection successful, false otherwise
         */
        bool Connect(const std::string& connectionString);

        /**
         * @brief Disconnect from the TCP device
         * @return true if disconnection successful, false otherwise
         */
        bool Disconnect();

        /**
         * @brief Send data to the TCP device
         * @param data Data to send
         * @return true if send successful, false otherwise
         */
        bool Send(const std::string& data);

        /**
         * @brief Receive data from the TCP device
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

    private:
        SOCKET m_socket;
        bool m_isConnected;
        std::string m_lastError;
        std::string m_ipAddress;
        int m_port;

        /**
         * @brief Initialize Winsock library
         * @return true if initialization successful, false otherwise
         */
        bool InitializeWinsock();

        /**
         * @brief Parse connection string for IP and port
         * @param connectionString Connection string in format "TCP:IP:PORT"
         * @return true if parsing successful, false otherwise
         */
        bool ParseConnectionString(const std::string& connectionString);
    };

} // namespace Communication
} // namespace Core
} // namespace ACLDevicePlatform