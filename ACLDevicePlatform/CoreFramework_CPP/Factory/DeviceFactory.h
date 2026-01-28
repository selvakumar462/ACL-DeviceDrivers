#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include "../Device/IDevice.h"

namespace ACLDevicePlatform {
namespace Core {
namespace Factory {

    /**
     * @brief Device factory for creating ACL device instances
     * 
     * Implements the factory pattern to dynamically create device instances
     * based on model names. Supports registration of custom device drivers.
     */
    class DeviceFactory {
    public:
        /**
         * @brief Get the singleton instance of the device factory
         * @return Reference to the device factory instance
         */
        static DeviceFactory& GetInstance();

        /**
         * @brief Register a device driver with the factory
         * @param model Device model name
         * @param creator Function to create the device instance
         */
        void RegisterDevice(const std::string& model, 
                           std::function<DevicePtr()> creator);

        /**
         * @brief Create a device instance by model name
         * @param model Device model name
         * @param connectionString Connection parameters
         * @return Unique pointer to the created device, or nullptr if not found
         */
        DevicePtr CreateDevice(const std::string& model, 
                              const std::string& connectionString);

        /**
         * @brief Check if a device model is supported
         * @param model Device model name
         * @return true if supported, false otherwise
         */
        bool IsModelSupported(const std::string& model) const;

        /**
         * @brief Get list of all supported device models
         * @return Vector of supported model names
         */
        std::vector<std::string> GetSupportedModels() const;

        /**
         * @brief Unregister a device driver
         * @param model Device model name
         */
        void UnregisterDevice(const std::string& model);

    private:
        DeviceFactory() = default;
        ~DeviceFactory() = default;
        DeviceFactory(const DeviceFactory&) = delete;
        DeviceFactory& operator=(const DeviceFactory&) = delete;

        std::unordered_map<std::string, std::function<DevicePtr()>> m_deviceCreators;
    };

    /**
     * @brief Macro for easy device registration
     * @param model Device model name
     * @param className Device class name
     */
    #define REGISTER_DEVICE(model, className) \
        static bool Register##className() { \
            ACLDevicePlatform::Core::Factory::DeviceFactory::GetInstance().RegisterDevice( \
                model, []() -> ACLDevicePlatform::Core::DevicePtr { \
                    return std::make_unique<className>(); \
                } \
            ); \
            return true; \
        } \
        static bool g_##className##_registered = Register##className();

} // namespace Factory
} // namespace Core
} // namespace ACLDevicePlatform