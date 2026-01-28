#pragma once
#include <string>
#include <memory>

namespace ACLDevicePlatform {
namespace Core {

    enum class DeviceStatus {
        Disconnected,
        Connected,
        Error
    };

    enum class DeviceType {
        Printer,
        Scanner,
        RFID
    };

    /**
     * @brief Abstract interface for all ACL device operations
     * 
     * This interface defines the contract that all ACL device drivers must implement.
     * It provides a consistent API for device communication regardless of the specific model.
     */
    class IDevice {
    public:
        virtual ~IDevice() = default;

        /**
         * @brief Connect to the device
         * @param connectionString Connection parameters (USB, TCP, Bluetooth)
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
         * @brief Get the device model name
         * @return Device model identifier
         */
        virtual std::string GetModel() const = 0;

        /**
         * @brief Get the device type
         * @return Device type (Printer, Scanner, RFID)
         */
        virtual DeviceType GetDeviceType() const = 0;

        /**
         * @brief Get current device status
         * @return Current connection status
         */
        virtual DeviceStatus GetStatus() const = 0;

        /**
         * @brief Get detailed error information
         * @return Error message if available, empty string otherwise
         */
        virtual std::string GetLastError() const = 0;

        /**
         * @brief Perform device-specific initialization
         * @return true if initialization successful, false otherwise
         */
        virtual bool Initialize() = 0;
    };

    /**
     * @brief Smart pointer type for device instances
     */
    using DevicePtr = std::unique_ptr<IDevice>;

} // namespace Core
} // namespace ACLDevicePlatform
