# ACL Device Platform for Windows

An enterprise-grade device platform for managing ACL printers, scanners, and RFID readers on Windows systems.

## Overview

The ACL Device Platform provides a unified, scalable solution for integrating multiple ACL device models into Windows applications. Built with a layered architecture, it supports USB, TCP/IP, and Bluetooth communication protocols with comprehensive error handling and logging.

## Supported Devices

### Printers
- **GK420** - Desktop thermal printer
- **ZD420** - Industrial desktop printer
- **ZT411** - Industrial printer
- **ZT421** - Industrial printer
- **ZQ521** - Mobile printer

### Scanners
- **DS2208** - 2D area imager scanner
- **DS4608** - Industrial scanner
- **LI4278** - Laser scanner
- **DS3678** - Rugged scanner

### RFID Readers
- **RFD40** - Handheld RFID reader
- **FX9600** - Fixed RFID reader

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Test Applications                        │
│  (PrinterTestApp, ScannerTestApp, RfidTestApp)              │
├─────────────────────────────────────────────────────────────┤
│                    C# SDK Layer                             │
│  (CoreFramework_CS - P/Invoke wrapper)                      │
├─────────────────────────────────────────────────────────────┤
│                    Windows Service                          │
│  (Background monitoring, logging, device management)        │
├─────────────────────────────────────────────────────────────┤
│                    C++ Core Framework                       │
│  (Shared engine, communication, ZPL, device factory)        │
├─────────────────────────────────────────────────────────────┤
│                    Device Driver Projects                   │
│  (Model-specific implementations for each ACL device)       │
└─────────────────────────────────────────────────────────────┘
```

### Key Components

1. **C++ Core Framework** (`CoreFramework_CPP/`)
   - Shared communication layers (USB, TCP/IP, Bluetooth)
   - ZPL (ACL Programming Language) engine
   - Device factory pattern for dynamic driver loading
   - Exported DLL functions for C# integration

2. **C# SDK Layer** (`CoreFramework_CS/`)
   - P/Invoke wrapper around C++ DLL
   - High-level API for application development
   - Type-safe device model and type enums
   - Comprehensive error handling

3. **Device Drivers** (`PrinterDrivers/`, `ScannerDrivers/`, `RfidDrivers/`)
   - Individual C++ projects for each device model
   - Model-specific behavior and configuration
   - Automatic registration with device factory

4. **Windows Service** (`WindowsService/`)
   - Background device monitoring
   - Periodic health checks and diagnostics
   - Structured logging to Windows Event Log

5. **Test Applications** (`TestApps/`)
   - Console applications for testing device functionality
   - Example implementations for each device category

## Installation

### Prerequisites
- Windows 10/11 (64-bit recommended)
- Visual Studio 2022 or later
- .NET Framework 4.8
- C++17 compiler support

### Building the Solution

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd ACLDevicePlatform
   ```

2. **Open the solution:**
   ```bash
   start ACLDevicePlatform.sln
   ```

3. **Build the solution:**
   - In Visual Studio: `Build > Build Solution`
   - Or use MSBuild: `msbuild ACLDevicePlatform.sln`

4. **Output locations:**
   - C++ DLL: `bin\Debug\CoreFramework.dll` or `bin\Release\CoreFramework.dll`
   - C# SDK: `bin\Debug\ACLDevicePlatform.CoreFramework.dll`
   - Windows Service: `bin\Debug\ACLDevicePlatform.WindowsService.exe`

### Deployment

1. **Copy required files:**
   - `CoreFramework.dll` (C++ core)
   - `ACLDevicePlatform.CoreFramework.dll` (C# SDK)
   - `ACLDevicePlatform.snk` (strong name key)

2. **Install Windows Service:**
   ```bash
   # Install the service
   ACLDevicePlatform.WindowsService.exe -install
   
   # Start the service
   net start ACLDeviceService
   
   # Uninstall the service
   ACLDevicePlatform.WindowsService.exe -uninstall
   ```

## Usage

### Basic C# Usage

```csharp
using ACLDevicePlatform.CoreFramework.Managers;
using ACLDevicePlatform.CoreFramework.Models;

class Program
{
    static void Main()
    {
        using (var deviceManager = new ACLDeviceManager())
        {
            // List supported devices
            var supportedModels = deviceManager.GetSupportedModels();
            
            // Print a label
            bool success = deviceManager.PrintLabel(
                DeviceModel.GK420,
                "USB:VID_0A5F&PID_0001",
                "Hello, ACL!"
            );
            
            // Scan a barcode
            string barcode = deviceManager.ScanBarcode(
                DeviceModel.DS2208,
                "USB:VID_0A5F&PID_0002"
            );
            
            // Read RFID
            string rfidData = deviceManager.ReadRfid(
                DeviceModel.RFD40,
                "BT:00:11:22:33:44:55"
            );
        }
    }
}
```

### ZPL Label Creation

```csharp
using ACLDevicePlatform.CoreFramework.Managers;

var deviceManager = new ACLDeviceManager();

// Create a ZPL label
var zplLabel = @"^XA
^FO50,50^A0,30,30^FDHello World^FS
^FO50,100^B3,100,Y,N^FD123456789012^FS
^XZ";

bool success = deviceManager.PrintLabel(
    DeviceModel.GK420,
    "USB:VID_0A5F&PID_0001",
    zplLabel
);
```

## Configuration

### Connection Strings

Each device type supports different connection string formats:

**USB:**
```
USB:VID_0A5F&PID_0001
```

**TCP/IP:**
```
TCP:192.168.1.100:9100
```

**Bluetooth:**
```
BT:00:11:22:33:44:55
```

### Windows Service Configuration

The service can be configured through the Windows Registry:

```
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\ACLDeviceService
```

Key settings:
- `MonitoringInterval` - Device monitoring frequency (seconds)
- `HealthCheckInterval` - Health check frequency (minutes)
- `LogLevel` - Logging level (0-3)

## Adding New Device Models

### 1. Create Device Driver Project

Create a new C++ project in the appropriate driver directory:
- `PrinterDrivers/` for printers
- `ScannerDrivers/` for scanners  
- `RfidDrivers/` for RFID readers

### 2. Implement IDevice Interface

```cpp
class NewDeviceDriver : public ACLDevicePlatform::Core::Device::IDevice
{
public:
    bool Connect(const std::string& connectionString) override;
    bool Disconnect() override;
    bool Send(const std::string& data) override;
    std::string GetModel() const override;
    // ... other interface methods
};
```

### 3. Register with Device Factory

Add the registration macro at the end of your driver header:
```cpp
REGISTER_DEVICE("NEW_MODEL", NewDeviceDriver);
```

### 4. Update C# Enums

Add the new model to the `DeviceModel` enum in `CoreFramework_CS/Models/DeviceModel.cs`:
```csharp
/// <summary>
/// New device model
/// </summary>
NEW_MODEL,
```

### 5. Update Extension Methods

Add the model to the `ToModelString()` and `GetDeviceType()` extension methods.

## Error Handling

The platform provides comprehensive error handling:

```csharp
try
{
    var deviceManager = new ACLDeviceManager();
    deviceManager.PrintLabel(model, connectionString, text);
}
catch (InvalidOperationException ex)
{
    Console.WriteLine($"Operation failed: {ex.Message}");
    Console.WriteLine($"Last error: {deviceManager.GetLastError()}");
}
catch (ArgumentException ex)
{
    Console.WriteLine($"Invalid parameters: {ex.Message}");
}
```

## Logging

### Windows Service Logging

The Windows Service logs to the Windows Event Log under the "Application" log with source "ACLDeviceService".

### Application Logging

For custom logging in your applications:

```csharp
var deviceManager = new ACLDeviceManager();
// Perform operations...
var lastError = deviceManager.GetLastError();
if (!string.IsNullOrEmpty(lastError))
{
    // Log the error using your preferred logging framework
    Console.WriteLine($"Device error: {lastError}");
}
```

## Troubleshooting

### Common Issues

1. **Device Not Found**
   - Verify connection string format
   - Check device drivers are installed
   - Ensure device is powered and connected

2. **Permission Errors**
   - Run application as administrator
   - Check USB/COM port permissions
   - Verify network access for TCP devices

3. **Communication Timeouts**
   - Check cable connections
   - Verify device is responsive
   - Increase timeout settings if needed

### Debugging

1. **Enable verbose logging** in the Windows Service
2. **Check Windows Event Log** for service errors
3. **Use test applications** to isolate issues
4. **Verify ZPL syntax** for label printing

## Performance Considerations

- **Connection pooling**: Reuse device connections when possible
- **Asynchronous operations**: Use async methods for long-running operations
- **Error handling**: Implement proper retry logic for transient failures
- **Resource management**: Always dispose of device managers properly

## Security

- **Strong naming**: All assemblies are strongly named
- **Input validation**: All user inputs are validated
- **Secure communication**: Supports encrypted communication where available
- **Access control**: Follow Windows security best practices

## Contributing

1. Fork the repository
2. Create a feature branch
3. Implement your changes with tests
4. Update documentation
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Support

For support and questions:
- Check the troubleshooting section
- Review the test applications for examples
- Consult ACL device documentation
- Contact your ACL representative for hardware-specific issues

## Version History

- **1.0.0** - Initial release with core framework and basic device support
- **1.1.0** - Added Windows Service and comprehensive logging
- **1.2.0** - Enhanced error handling and performance improvements
- **2.0.0** - Enterprise features, scalability improvements, and documentation

## Contributing Organizations

This platform was developed by ACL Technologies for enterprise device management solutions.
