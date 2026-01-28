#include "CoreExports.h"
#include "../Factory/DeviceFactory.h"
#include "../ZPL/ZplBuilder.h"
#include <string>
#include <memory>

namespace {
    // Global error message storage
    std::string g_lastError;
    
    // Helper function to set last error
    void SetLastError(const std::string& error) {
        g_lastError = error;
    }
    
    // Helper function to clear last error
    void ClearError() {
        g_lastError.clear();
    }
}



    COREFRAMEWORK_API bool PrintLabel(const char* model, const char* connectionString, const char* text) {
        try {
            ClearError();
            
            if (!model || !connectionString || !text) {
                SetLastError("Invalid parameters: model, connectionString, or text is null");
                return false;
            }

            auto& factory = ACLDevicePlatform::Core::Factory::DeviceFactory::GetInstance();
            auto device = factory.CreateDevice(std::string(model), std::string(connectionString));
            
            if (!device) {
                SetLastError("Failed to create device: " + std::string(model));
                return false;
            }

            // Create ZPL label
            ACLDevicePlatform::Core::ZPL::ZplBuilder builder;
            builder.SetLabelDimensions(400, 600)
                   .AddText(50, 50, 0, 30, 30, std::string(text))
                   .SetQuantity(1);
            
            std::string zpl = builder.Build();
            
            // Send to device
            bool success = device->Send(zpl);
            if (!success) {
                SetLastError("Failed to send ZPL to device: " + device->GetLastError());
            }
            
            return success;
        }
        catch (const std::exception& e) {
            SetLastError("Exception in PrintLabel: " + std::string(e.what()));
            return false;
        }
    }

    COREFRAMEWORK_API bool ScanBarcode(const char* model, const char* connectionString, 
                                      char* resultBuffer, int bufferSize) {
        try {
            ClearError();
            
            if (!model || !connectionString || !resultBuffer || bufferSize <= 0) {
                SetLastError("Invalid parameters for ScanBarcode");
                return false;
            }

            auto& factory = ACLDevicePlatform::Core::Factory::DeviceFactory::GetInstance();
            auto device = factory.CreateDevice(std::string(model), std::string(connectionString));
            
            if (!device) {
                SetLastError("Failed to create scanner device: " + std::string(model));
                return false;
            }

            // Send scan command (this would be device-specific in a real implementation)
            std::string scanCommand = "^SC"; // Hypothetical scan command
            bool success = device->Send(scanCommand);
            
            if (success) {
                std::string response;
                // In a real implementation, this would wait for and parse the scan response
                response = "1234567890123"; // Mock response
                
                if (response.length() < static_cast<size_t>(bufferSize)) {
                    strcpy_s(resultBuffer, bufferSize, response.c_str());
                } else {
                    SetLastError("Response buffer too small for scan result");
                    return false;
                }
            } else {
                SetLastError("Failed to send scan command: " + device->GetLastError());
            }
            
            return success;
        }
        catch (const std::exception& e) {
            SetLastError("Exception in ScanBarcode: " + std::string(e.what()));
            return false;
        }
    }

    COREFRAMEWORK_API bool ReadRfid(const char* model, const char* connectionString, 
                                   char* resultBuffer, int bufferSize) {
        try {
            ClearError();
            
            if (!model || !connectionString || !resultBuffer || bufferSize <= 0) {
                SetLastError("Invalid parameters for ReadRfid");
                return false;
            }

            auto& factory = ACLDevicePlatform::Core::Factory::DeviceFactory::GetInstance();
            auto device = factory.CreateDevice(std::string(model), std::string(connectionString));
            
            if (!device) {
                SetLastError("Failed to create RFID device: " + std::string(model));
                return false;
            }

            // Send RFID read command (this would be device-specific in a real implementation)
            std::string readCommand = "^RF"; // Hypothetical RFID read command
            bool success = device->Send(readCommand);
            
            if (success) {
                std::string response;
                // In a real implementation, this would wait for and parse the RFID response
                response = "E2801160600002019A004456"; // Mock RFID tag ID
                
                if (response.length() < static_cast<size_t>(bufferSize)) {
                    strcpy_s(resultBuffer, bufferSize, response.c_str());
                } else {
                    SetLastError("Response buffer too small for RFID result");
                    return false;
                }
            } else {
                SetLastError("Failed to send RFID read command: " + device->GetLastError());
            }
            
            return success;
        }
        catch (const std::exception& e) {
            SetLastError("Exception in ReadRfid: " + std::string(e.what()));
            return false;
        }
    }

    COREFRAMEWORK_API bool GetACLDeviceLastError(char* buffer, int bufferSize) {
        if (!buffer || bufferSize <= 0) {
            return false;
        }
        
        if (!g_lastError.empty()) {
            if (g_lastError.length() < static_cast<size_t>(bufferSize)) {
                strcpy_s(buffer, bufferSize, g_lastError.c_str());
                return true;
            }
        }
        
        return false;
    }

    COREFRAMEWORK_API bool IsModelSupported(const char* model) {
        if (!model) {
            return false;
        }
        
        auto& factory = ACLDevicePlatform::Core::Factory::DeviceFactory::GetInstance();
        return factory.IsModelSupported(std::string(model));
    }

    COREFRAMEWORK_API bool GetSupportedModels(char* buffer, int bufferSize) {
        if (!buffer || bufferSize <= 0) {
            return false;
        }
        
        auto& factory = ACLDevicePlatform::Core::Factory::DeviceFactory::GetInstance();
        auto models = factory.GetSupportedModels();
        
        std::string result;
        for (size_t i = 0; i < models.size(); ++i) {
            result += models[i];
            if (i < models.size() - 1) {
                result += ",";
            }
        }
        
        if (result.length() < static_cast<size_t>(bufferSize)) {
            strcpy_s(buffer, bufferSize, result.c_str());
            return true;
        }
        
        return false;
    }

// extern "C"