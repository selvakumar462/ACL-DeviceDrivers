using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ACLDevicePlatform.CoreFramework.Managers;
using ACLDevicePlatform.CoreFramework.Models;

namespace ACLDevicePlatform.TestApps.PrinterTestApp
{
    /// <summary>
    /// Console application for testing ACL printer functionality
    /// </summary>
    class Program
    {
        private static ACLDeviceManager _deviceManager;

        static void Main(string[] args)
        {
            Console.WriteLine("=== ACL Printer Test Application ===");
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
                Console.WriteLine("1. List supported printer models");
                Console.WriteLine("2. Test printer connection");
                Console.WriteLine("3. Print test label");
                Console.WriteLine("4. Print custom label");
                Console.WriteLine("5. Check printer status");
                Console.WriteLine("6. Perform printer self-test");
                Console.WriteLine("7. Exit");
                Console.WriteLine();

                Console.Write("Enter your choice (1-7): ");
                var input = Console.ReadLine();

                Console.WriteLine();

                switch (input)
                {
                    case "1":
                        ListSupportedPrinters();
                        break;
                    case "2":
                        TestPrinterConnection();
                        break;
                    case "3":
                        PrintTestLabel();
                        break;
                    case "4":
                        PrintCustomLabel();
                        break;
                    case "5":
                        CheckPrinterStatus();
                        break;
                    case "6":
                        PerformSelfTest();
                        break;
                    case "7":
                        Console.WriteLine("Exiting application...");
                        return;
                    default:
                        Console.WriteLine("Invalid choice. Please enter a number between 1 and 7.");
                        break;
                }

                Console.WriteLine();
                Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
                Console.WriteLine();
            }
        }

        /// <summary>
        /// List all supported printer models
        /// </summary>
        static void ListSupportedPrinters()
        {
            try
            {
                var supportedModels = _deviceManager.GetSupportedModels();
                var printers = supportedModels.Where(m => m.GetDeviceType() == DeviceType.Printer).ToArray();

                if (printers.Length == 0)
                {
                    Console.WriteLine("No printer models are currently supported.");
                    return;
                }

                Console.WriteLine("Supported Printer Models:");
                Console.WriteLine("========================");

                foreach (var printer in printers)
                {
                    Console.WriteLine($"- {printer} ({printer.ToModelString()})");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error listing supported printers: {ex.Message}");
            }
        }

        /// <summary>
        /// Test connection to a specific printer
        /// </summary>
        static void TestPrinterConnection()
        {
            try
            {
                Console.Write("Enter printer model (e.g., GK420): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid printer model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Printer)
                {
                    Console.WriteLine("Selected model is not a printer.");
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
                Console.WriteLine($"Error testing printer connection: {ex.Message}");
            }
        }

        /// <summary>
        /// Print a test label
        /// </summary>
        static void PrintTestLabel()
        {
            try
            {
                Console.Write("Enter printer model (e.g., GK420): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid printer model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Printer)
                {
                    Console.WriteLine("Selected model is not a printer.");
                    return;
                }

                Console.Write("Enter connection string: ");
                var connectionString = Console.ReadLine();

                if (string.IsNullOrEmpty(connectionString))
                {
                    Console.WriteLine("Connection string cannot be empty.");
                    return;
                }

                // Create a simple test label
                var testLabel = CreateTestLabel();

                Console.WriteLine($"Printing test label on {model}...");
                
                // For testing, we'll simulate the print operation
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would send the label to the actual printer.");
                Console.WriteLine("Test label content:");
                Console.WriteLine(testLabel);
                Console.WriteLine();
                Console.WriteLine("Test label printed successfully (simulated).");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error printing test label: {ex.Message}");
            }
        }

        /// <summary>
        /// Print a custom label with user-defined text
        /// </summary>
        static void PrintCustomLabel()
        {
            try
            {
                Console.Write("Enter printer model (e.g., GK420): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid printer model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Printer)
                {
                    Console.WriteLine("Selected model is not a printer.");
                    return;
                }

                Console.Write("Enter connection string: ");
                var connectionString = Console.ReadLine();

                if (string.IsNullOrEmpty(connectionString))
                {
                    Console.WriteLine("Connection string cannot be empty.");
                    return;
                }

                Console.Write("Enter custom text for the label: ");
                var customText = Console.ReadLine();

                if (string.IsNullOrEmpty(customText))
                {
                    Console.WriteLine("Custom text cannot be empty.");
                    return;
                }

                var customLabel = CreateCustomLabel(customText);

                Console.WriteLine($"Printing custom label on {model}...");
                
                // For testing, we'll simulate the print operation
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would send the label to the actual printer.");
                Console.WriteLine("Custom label content:");
                Console.WriteLine(customLabel);
                Console.WriteLine();
                Console.WriteLine("Custom label printed successfully (simulated).");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error printing custom label: {ex.Message}");
            }
        }

        /// <summary>
        /// Check printer status
        /// </summary>
        static void CheckPrinterStatus()
        {
            try
            {
                Console.Write("Enter printer model (e.g., GK420): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid printer model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Printer)
                {
                    Console.WriteLine("Selected model is not a printer.");
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
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would query the actual printer.");
                Console.WriteLine("Simulated printer status:");
                Console.WriteLine("- Status: Ready");
                Console.WriteLine("- Media: Loaded");
                Console.WriteLine("- Error: None");
                Console.WriteLine("- Firmware: V70.19.12Z");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error checking printer status: {ex.Message}");
            }
        }

        /// <summary>
        /// Perform printer self-test
        /// </summary>
        static void PerformSelfTest()
        {
            try
            {
                Console.Write("Enter printer model (e.g., GK420): ");
                var modelInput = Console.ReadLine();

                if (!Enum.TryParse(modelInput, out DeviceModel model))
                {
                    Console.WriteLine("Invalid printer model.");
                    return;
                }

                if (model.GetDeviceType() != DeviceType.Printer)
                {
                    Console.WriteLine("Selected model is not a printer.");
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
                Console.WriteLine("Note: This is a simulation. In a real implementation, this would send a self-test command to the printer.");
                Console.WriteLine("Self-test completed successfully (simulated).");
                Console.WriteLine("Test results:");
                Console.WriteLine("- Print head: OK");
                Console.WriteLine("- Sensors: OK");
                Console.WriteLine("- Memory: OK");
                Console.WriteLine("- Communication: OK");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error performing self-test: {ex.Message}");
            }
        }

        /// <summary>
        /// Create a simple test label in ZPL format
        /// </summary>
        /// <returns>ZPL label content</returns>
        static string CreateTestLabel()
        {
            return @"^XA
^FO50,50^A0,30,30^FDACL Test Label^FS
^FO50,100^B3,100,Y,N^FD123456789012^FS
^FO50,220^A0,20,20^FDTest Barcode^FS
^FO50,260^A0,20,20^FDModel: GK420^FS
^FO50,300^A0,20,20^FDDate: " + DateTime.Now.ToString("yyyy-MM-dd") + @"^FS
^XZ";
        }

        /// <summary>
        /// Create a custom label with user text
        /// </summary>
        /// <param name="customText">Text to include in the label</param>
        /// <returns>ZPL label content</returns>
        static string CreateCustomLabel(string customText)
        {
            return $@"^XA
^FO50,50^A0,30,30^FD{customText}^FS
^FO50,100^B3,100,Y,N^FD{DateTime.Now.Ticks % 1000000000000}^FS
^FO50,220^A0,20,20^FDGenerated Label^FS
^FO50,260^A0,20,20^FDDate: {DateTime.Now.ToString("yyyy-MM-dd")}^FS
^XZ";
        }
    }
}