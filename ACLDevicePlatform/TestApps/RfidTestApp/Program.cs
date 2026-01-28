using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ACLDevicePlatform.CoreFramework.Managers;
using ACLDevicePlatform.CoreFramework.Models;

namespace ACLDevicePlatform.TestApps.RfidTestApp
{
    /// <summary>
    /// Console application for testing ACL RFID functionality
    /// </summary>
    class Program
    {
        private static ACLDeviceManager _deviceManager;

        static void Main(string[] args)
        {
            Console.WriteLine("=== ACL RFID Test Application ===");
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
                Console.WriteLine("1. List supported RFID reader models");
                Console.WriteLine("2. Test RFID reader connection");
                Console.WriteLine("3. Read RFID tags");
                Console.WriteLine("4. Check RFID reader status");
                Console.WriteLine("5. Perform RFID reader self-test");
                Console.WriteLine("6. Exit");
                Console.WriteLine();

                Console.Write("Enter your choice (1-6): ");
                var input = Console.ReadLine();

                Console.WriteLine();

                switch (input)
                {
                    case "1":
                        ListSupportedRfidReaders();
                        break;
                    case "2":
                        TestRfidReaderConnection();
                        break;
                    case "3":
                        ReadRfidTags();
                        break;
                    case "4":
                        CheckRfidReaderStatus();
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
        /// List all supported RFID reader models
        /// </summary>
        static void ListSupportedRfidReaders()
        {
            try
            {
                var supportedModels = _deviceManager.GetSupportedModels();
                var rfidReaders = supportedModels.Where(m => m.GetDeviceType() == DeviceType.RFID).ToArray();

                if (rfidReaders.Length == 0)
                {
                    Console.WriteLine("No RFID reader models are currently supported.");
                    return;
                }

                Console.WriteLine("Supported RFID Reader Models:");
                Console.WriteLine("==============================");

                foreach (var reader in rfidReaders)
                {
                    Console.WriteLine($"- {reader} ({reader.ToModelString()})");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error listing supported RFID readers: {ex.Message}");
            }
        }

        /// <summary>
        /// Test connection to a specific RFID reader
        /// </summary>
        static void TestRfidReaderConnection()
        {
            try
            {
                Console.Write("Enter RFID reader model (e.g., FX9600): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid RFID reader model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.RFID)
                {
                    Console.WriteLine("Selected model is not an RFID reader.");
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
                Console.WriteLine($"Error testing RFID reader connection: {ex.Message}");
            }
        }

        /// <summary>
        /// Read RFID tags
        /// </summary>
        static void ReadRfidTags()
        {
            try
            {
                Console.Write("Enter RFID reader model (e.g., FX9600): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid RFID reader model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.RFID)
                {
                    Console.WriteLine("Selected model is not an RFID reader.");
                    return;
                }

                Console.Write("Enter connection string: ");
                var connectionString = Console.ReadLine();

                if (string.IsNullOrEmpty(connectionString))
                {
                    Console.WriteLine("Connection string cannot be empty.");
                    return;
                }

                Console.WriteLine($"Reading RFID tags with {model}...");
                
                // For testing, we'll simulate the tag reading operation
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would trigger the actual RFID reader.");
                Console.WriteLine("Simulated RFID tag reads:");
                Console.WriteLine("- EPC: 300833B2DDD9014000000000");
                Console.WriteLine("- EPC: 300833B2DDD9014000000001");
                Console.WriteLine("- EPC: 300833B2DDD9014000000002");
                Console.WriteLine("RFID tags read successfully (simulated).");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error reading RFID tags: {ex.Message}");
            }
        }

        /// <summary>
        /// Check RFID reader status
        /// </summary>
        static void CheckRfidReaderStatus()
        {
            try
            {
                Console.Write("Enter RFID reader model (e.g., FX9600): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid RFID reader model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.RFID)
                {
                    Console.WriteLine("Selected model is not an RFID reader.");
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
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would query the actual RFID reader.");
                Console.WriteLine("Simulated RFID reader status:");
                Console.WriteLine("- Status: Ready");
                Console.WriteLine("- Antenna: Active");
                Console.WriteLine("- Power: 27 dBm");
                Console.WriteLine("- Error: None");
                Console.WriteLine("- Firmware: V70.19.12R");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error checking RFID reader status: {ex.Message}");
            }
        }

        /// <summary>
        /// Perform RFID reader self-test
        /// </summary>
        static void PerformSelfTest()
        {
            try
            {
                Console.Write("Enter RFID reader model (e.g., FX9600): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid RFID reader model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.RFID)
                {
                    Console.WriteLine("Selected model is not an RFID reader.");
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
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would send a self-test command to the RFID reader.");
                Console.WriteLine("Self-test completed successfully (simulated).");
                Console.WriteLine("Test results:");
                Console.WriteLine("- RF Module: OK");
                Console.WriteLine("- Antenna: OK");
                Console.WriteLine("- Memory: OK");
                Console.WriteLine("- Communication: OK");
                Console.WriteLine("- Firmware: OK");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error performing self-test: {ex.Message}");
            }
        }
    }
}