using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ACLDevicePlatform.CoreFramework.Managers;
using ACLDevicePlatform.CoreFramework.Models;

namespace ACLDevicePlatform.TestApps.ScannerTestApp
{
    /// <summary>
    /// Console application for testing ACL scanner functionality
    /// </summary>
    class Program
    {
        private static ACLDeviceManager _deviceManager;

        static void Main(string[] args)
        {
            Console.WriteLine("=== ACL Scanner Test Application ===");
            Console.WriteLine();

            try
            {
                _deviceManager = new ACLDeviceManager();

                ShowMainMenu();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error initializing application: {ex.Message}");
                Console.WriteLine("Press any key to exit...");
                Console.ReadKey();
            }
            finally
            {
                _deviceManager?.Dispose();
            }
        }

        /// <summary>
        /// Display the main menu and handle user input
        /// </summary>
        static void ShowMainMenu()
        {
            while (true)
            {
                Console.WriteLine("Please select an option:");
                Console.WriteLine("1. List supported scanner models");
                Console.WriteLine("2. Test scanner connection");
                Console.WriteLine("3. Scan barcode");
                Console.WriteLine("4. Check scanner status");
                Console.WriteLine("5. Perform scanner self-test");
                Console.WriteLine("6. Exit");
                Console.WriteLine();

                Console.Write("Enter your choice (1-6): ");
                var input = Console.ReadLine();

                Console.WriteLine();

                switch (input)
                {
                    case "1":
                        ListSupportedScanners();
                        break;
                    case "2":
                        TestScannerConnection();
                        break;
                    case "3":
                        ScanBarcode();
                        break;
                    case "4":
                        CheckScannerStatus();
                        break;
                    case "5":
                        PerformSelfTest();
                        break;
                    case "6":
                        Console.WriteLine("Exiting application...");
                        return;
                    default:
                        Console.WriteLine("Invalid choice. Please enter a number between 1 and 6.");
                        break;
                }

                Console.WriteLine();
                Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
                Console.WriteLine();
            }
        }

        /// <summary>
        /// List all supported scanner models
        /// </summary>
        static void ListSupportedScanners()
        {
            try
            {
                var supportedModels = _deviceManager.GetSupportedModels();
                var scanners = supportedModels.Where(m => m.GetDeviceType() == DeviceType.Scanner).ToArray();

                if (scanners.Length == 0)
                {
                    Console.WriteLine("No scanner models are currently supported.");
                    return;
                }

                Console.WriteLine("Supported Scanner Models:");
                Console.WriteLine("========================");

                foreach (var scanner in scanners)
                {
                    Console.WriteLine($"- {scanner} ({scanner.ToModelString()})");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error listing supported scanners: {ex.Message}");
            }
        }

        /// <summary>
        /// Test connection to a specific scanner
        /// </summary>
        static void TestScannerConnection()
        {
            try
            {
                Console.Write("Enter scanner model (e.g., DS2208): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid scanner model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Scanner)
                {
                    Console.WriteLine("Selected model is not a scanner.");
                    return;
                }

                Console.Write("Enter connection string (e.g., USB:VID_0A5F&PID_0001): ");
                var connectionString = Console.ReadLine();

                if (string.IsNullOrEmpty(connectionString))
                {
                    Console.WriteLine("Connection string cannot be empty.");
                    return;
                }

                Console.WriteLine($"Testing connection to {model}...");
                
                // For testing purposes, we'll simulate the connection
                // In a real implementation, this would actually connect to the device
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would test the actual device connection.");
                Console.WriteLine("Connection test completed successfully (simulated).");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error testing scanner connection: {ex.Message}");
            }
        }

        /// <summary>
        /// Scan a barcode
        /// </summary>
        static void ScanBarcode()
        {
            try
            {
                Console.Write("Enter scanner model (e.g., DS2208): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid scanner model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Scanner)
                {
                    Console.WriteLine("Selected model is not a scanner.");
                    return;
                }

                Console.Write("Enter connection string: ");
                var connectionString = Console.ReadLine();

                if (string.IsNullOrEmpty(connectionString))
                {
                    Console.WriteLine("Connection string cannot be empty.");
                    return;
                }

                Console.WriteLine($"Scanning barcode with {model}...");
                
                // For testing, we'll simulate the scan operation
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would trigger the actual scanner.");
                Console.WriteLine("Simulated scan result: 1234567890123");
                Console.WriteLine("Barcode scanned successfully (simulated).");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error scanning barcode: {ex.Message}");
            }
        }

        /// <summary>
        /// Check scanner status
        /// </summary>
        static void CheckScannerStatus()
        {
            try
            {
                Console.Write("Enter scanner model (e.g., DS2208): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid scanner model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Scanner)
                {
                    Console.WriteLine("Selected model is not a scanner.");
                    return;
                }

                Console.Write("Enter connection string: ");
                var connectionString = Console.ReadLine();

                if (string.IsNullOrEmpty(connectionString))
                {
                    Console.WriteLine("Connection string cannot be empty.");
                    return;
                }

                Console.WriteLine($"Checking status of {model}...");
                
                // For testing, we'll simulate the status check
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would query the actual scanner.");
                Console.WriteLine("Simulated scanner status:");
                Console.WriteLine("- Status: Ready");
                Console.WriteLine("- Trigger: Idle");
                Console.WriteLine("- Error: None");
                Console.WriteLine("- Firmware: V70.19.12S");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error checking scanner status: {ex.Message}");
            }
        }

        /// <summary>
        /// Perform scanner self-test
        /// </summary>
        static void PerformSelfTest()
        {
            try
            {
                Console.Write("Enter scanner model (e.g., DS2208): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid scanner model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Scanner)
                {
                    Console.WriteLine("Selected model is not a scanner.");
                    return;
                }

                Console.Write("Enter connection string: ");
                var connectionString = Console.ReadLine();

                if (string.IsNullOrEmpty(connectionString))
                {
                    Console.WriteLine("Connection string cannot be empty.");
                    return;
                }

                Console.WriteLine($"Performing self-test on {model}...");
                
                // For testing, we'll simulate the self-test
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would send a self-test command to the scanner.");
                Console.WriteLine("Self-test completed successfully (simulated).");
                Console.WriteLine("Test results:");
                Console.WriteLine("- Laser: OK");
                Console.WriteLine("- Sensors: OK");
                Console.WriteLine("- Decoding: OK");
                Console.WriteLine("- Communication: OK");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error performing self-test: {ex.Message}");
            }
        }
    }
}