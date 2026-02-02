// Example snippet
for (auto& device : ConnectedACLUSBDevices) {
    deviceID = GenerateUniqueID(device);
    deviceMap[deviceID] = device;
}
// When routing print jobs
if (deviceMap.contains(targetDeviceID)) {
    deviceMap[targetDeviceID]->SendPrintJob(printData);
} else {
    // Error: Device ID not found
}