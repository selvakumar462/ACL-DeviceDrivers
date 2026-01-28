#pragma once

#ifdef COREFRAMEWORK_EXPORTS
#define COREFRAMEWORK_API __declspec(dllexport)
#else
#define COREFRAMEWORK_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @brief Print a label using the specified ACL printer model
     * @param model Printer model name (e.g., "GK420", "ZD420")
     * @param connectionString Connection parameters (USB, TCP, Bluetooth)
     * @param text Text content for the label
     * @return true if print successful, false otherwise
     */
    COREFRAMEWORK_API bool PrintLabel(const char* model, const char* connectionString, const char* text);

    /**
     * @brief Scan a barcode using the specified ACL scanner model
     * @param model Scanner model name (e.g., "DS2208", "DS4608")
     * @param connectionString Connection parameters
     * @param resultBuffer Buffer to store scanned result
     * @param bufferSize Size of the result buffer
     * @return true if scan successful, false otherwise
     */
    COREFRAMEWORK_API bool ScanBarcode(const char* model, const char* connectionString, 
                                      char* resultBuffer, int bufferSize);

    /**
     * @brief Read RFID tags using the specified ACL RFID reader model
     * @param model RFID reader model name (e.g., "RFD40", "FX9600")
     * @param connectionString Connection parameters
     * @param resultBuffer Buffer to store RFID data
     * @param bufferSize Size of the result buffer
     * @return true if read successful, false otherwise
     */
    COREFRAMEWORK_API bool ReadRfid(const char* model, const char* connectionString, 
                                   char* resultBuffer, int bufferSize);

    /**
     * @brief Get the last error message
     * @param buffer Buffer to store error message
     * @param bufferSize Size of the buffer
     * @return true if error message available, false otherwise
     */
    COREFRAMEWORK_API bool GetACLDeviceLastError(char* buffer, int bufferSize);

    /**
     * @brief Check if a device model is supported
     * @param model Device model name
     * @return true if supported, false otherwise
     */
    COREFRAMEWORK_API bool IsModelSupported(const char* model);

    /**
     * @brief Get list of supported device models
     * @param buffer Buffer to store comma-separated model list
     * @param bufferSize Size of the buffer
     * @return true if successful, false otherwise
     */
    COREFRAMEWORK_API bool GetSupportedModels(char* buffer, int bufferSize);
#ifdef __cplusplus
}
#endif
