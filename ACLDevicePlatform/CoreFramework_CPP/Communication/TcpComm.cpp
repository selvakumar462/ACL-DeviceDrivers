#include "TcpComm.h"
#include <iostream>
#include <sstream>

namespace ACLDevicePlatform {
namespace Core {
namespace Communication {

    TcpComm::TcpComm() : m_socket(INVALID_SOCKET), m_isConnected(false), m_port(0) {
        InitializeWinsock();
    }

    TcpComm::~TcpComm() {
        Disconnect();
        WSACleanup();
    }

    bool TcpComm::Connect(const std::string& connectionString) {
        if (m_isConnected) {
            Disconnect();
        }

        if (!ParseConnectionString(connectionString)) {
            m_lastError = "Invalid connection string format";
            return false;
        }

        struct addrinfo* result = nullptr;
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        std::string portStr = std::to_string(m_port);
        int iResult = getaddrinfo(m_ipAddress.c_str(), portStr.c_str(), &hints, &result);

        if (iResult != 0) {
            m_lastError = "getaddrinfo failed: " + std::to_string(iResult);
            return false;
        }

        m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (m_socket == INVALID_SOCKET) {
            m_lastError = "socket failed: " + std::to_string(WSAGetLastError());
            freeaddrinfo(result);
            return false;
        }

        iResult = connect(m_socket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            m_lastError = "connect failed: " + std::to_string(WSAGetLastError());
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
            freeaddrinfo(result);
            return false;
        }

        freeaddrinfo(result);
        m_isConnected = true;
        m_lastError.clear();
        return true;
    }

    bool TcpComm::Disconnect() {
        if (m_socket != INVALID_SOCKET) {
            closesocket(m_socket);
            m_socket = INVALID_SOCKET;
        }
        m_isConnected = false;
        return true;
    }

    bool TcpComm::Send(const std::string& data) {
        if (!m_isConnected || m_socket == INVALID_SOCKET) {
            m_lastError = "Device not connected";
            return false;
        }

        int iResult = send(m_socket, data.c_str(), static_cast<int>(data.length()), 0);
        if (iResult == SOCKET_ERROR) {
            m_lastError = "send failed: " + std::to_string(WSAGetLastError());
            return false;
        }

        return true;
    }

    bool TcpComm::Receive(std::string& data, int timeoutMs) {
        if (!m_isConnected || m_socket == INVALID_SOCKET) {
            m_lastError = "Device not connected";
            return false;
        }

        // Set socket timeout
        DWORD timeout = timeoutMs;
        setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

        char buffer[4096];
        int iResult = recv(m_socket, buffer, sizeof(buffer) - 1, 0);
        
        if (iResult > 0) {
            buffer[iResult] = '\0';
            data = std::string(buffer, iResult);
            return true;
        } else if (iResult == 0) {
            m_lastError = "Connection closed by server";
            return false;
        } else {
            m_lastError = "recv failed: " + std::to_string(WSAGetLastError());
            return false;
        }
    }

    std::string TcpComm::GetLastError() const {
        return m_lastError;
    }

    bool TcpComm::IsConnected() const {
        return m_isConnected;
    }

    bool TcpComm::InitializeWinsock() {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            m_lastError = "WSAStartup failed: " + std::to_string(iResult);
            return false;
        }
        return true;
    }

    bool TcpComm::ParseConnectionString(const std::string& connectionString) {
        // Expected format: "TCP:IP_ADDRESS:PORT"
        size_t firstColon = connectionString.find(':');
        if (firstColon == std::string::npos || connectionString.substr(0, firstColon) != "TCP") {
            return false;
        }

        size_t secondColon = connectionString.find(':', firstColon + 1);
        if (secondColon == std::string::npos) {
            return false;
        }

        m_ipAddress = connectionString.substr(firstColon + 1, secondColon - firstColon - 1);
        std::string portStr = connectionString.substr(secondColon + 1);

        try {
            m_port = std::stoi(portStr);
            if (m_port <= 0 || m_port > 65535) {
                return false;
            }
        } catch (const std::exception&) {
            return false;
        }

        return true;
    }

} // namespace Communication
} // namespace Core
} // namespace ACLDevicePlatform
