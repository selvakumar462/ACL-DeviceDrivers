#include "DeviceFactory.h"
#include <algorithm>

namespace ACLDevicePlatform {
namespace Core {
namespace Factory {

    DeviceFactory& DeviceFactory::GetInstance() {
        static DeviceFactory instance;
        return instance;
    }

    void DeviceFactory::RegisterDevice(const std::string& model, 
                                      std::function<DevicePtr()> creator) {
        m_deviceCreators[model] = creator;
    }

    DevicePtr DeviceFactory::CreateDevice(const std::string& model, 
                                         const std::string& connectionString) {
        auto it = m_deviceCreators.find(model);
        if (it != m_deviceCreators.end()) {
            DevicePtr device = it->second();
            if (device && device->Connect(connectionString)) {
                return device;
            }
        }
        return nullptr;
    }

    bool DeviceFactory::IsModelSupported(const std::string& model) const {
        return m_deviceCreators.find(model) != m_deviceCreators.end();
    }

    std::vector<std::string> DeviceFactory::GetSupportedModels() const {
        std::vector<std::string> models;
        for (const auto& pair : m_deviceCreators) {
            models.push_back(pair.first);
        }
        return models;
    }

    void DeviceFactory::UnregisterDevice(const std::string& model) {
        m_deviceCreators.erase(model);
    }

} // namespace Factory
} // namespace Core
} // namespace ACLDevicePlatform
