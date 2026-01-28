using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using ACLDevicePlatform.CoreFramework.Interop;
using ACLDevicePlatform.CoreFramework.Models;

namespace ACLDevicePlatform.CoreFramework.Managers
{
    /// <summary>
    /// High-level manager for ACL device operations
    /// Provides a clean C# API wrapper around the native C++ CoreFramework
    /// </summary>
    public class ACLDeviceManager : IDisposable
    {
        private const int DefaultBufferSize = 4096;
        private bool _disposed = false;

        /// <summary>
        /// Initializes a new instance of the ACLDeviceManager class
        /// </summary>
        public ACLDeviceManager()
        {
            // Ensure the native library is loaded
            EnsureNativeLibraryLoaded();
        }

        /// <summary>
        /// Print a label using the specified ACL printer model
        /// </summary>
        /// <param name="model">Printer model</param>
        /// <param name="connectionString">Connection parameters (USB, TCP, Bluetooth)</param>
        /// <param name="text">Text content for the label</param>
        /// <returns>true if print successful, false otherwise</returns>
        /// <exception cref="ArgumentNullException">Thrown when parameters are null</exception>
        /// <exception cref="InvalidOperationException">Thrown when operation fails</exception>
        public bool PrintLabel(DeviceModel model, string connectionString, string text)
        {
            ValidateParameters(connectionString, text);

            if (model.GetDeviceType() != DeviceType.Printer)
            {
                throw new ArgumentException($"Model {model} is not a printer", nameof(model));
            }

            bool result = NativeMethods.PrintLabel(
                model.ToModelString(),
                connectionString,
                text);

            if (!result)
            {
                throw new InvalidOperationException($"Failed to print label: {GetLastError()}");
            }

            return result;
        }

        /// <summary>
        /// Scan a barcode using the specified ACL scanner model
        /// </summary>
        /// <param name="model">Scanner model</param>
        /// <param name="connectionString">Connection parameters</param>
        /// <returns>Scanned barcode data</returns>
        /// <exception cref="ArgumentNullException">Thrown when parameters are null</exception>
        /// <exception cref="InvalidOperationException">Thrown when operation fails</exception>
        public string ScanBarcode(DeviceModel model, string connectionString)
        {
            ValidateParameters(connectionString, null);

            if (model.GetDeviceType() != DeviceType.Scanner)
            {
                throw new ArgumentException($"Model {model} is not a scanner", nameof(model));
            }

            IntPtr buffer = Marshal.AllocHGlobal(DefaultBufferSize);
            try
            {
                bool result = NativeMethods.ScanBarcode(
                    model.ToModelString(),
                    connectionString,
                    buffer,
                    DefaultBufferSize);

                if (!result)
                {
                    throw new InvalidOperationException($"Failed to scan barcode: {GetLastError()}");
                }

                return Marshal.PtrToStringAnsi(buffer) ?? string.Empty;
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        /// <summary>
        /// Read RFID tags using the specified ACL RFID reader model
        /// </summary>
        /// <param name="model">RFID reader model</param>
        /// <param name="connectionString">Connection parameters</param>
        /// <returns>RFID tag data</returns>
        /// <exception cref="ArgumentNullException">Thrown when parameters are null</exception>
        /// <exception cref="InvalidOperationException">Thrown when operation fails</exception>
        public string ReadRfid(DeviceModel model, string connectionString)
        {
            ValidateParameters(connectionString, null);

            if (model.GetDeviceType() != DeviceType.RFID)
            {
                throw new ArgumentException($"Model {model} is not an RFID reader", nameof(model));
            }

            IntPtr buffer = Marshal.AllocHGlobal(DefaultBufferSize);
            try
            {
                bool result = NativeMethods.ReadRfid(
                    model.ToModelString(),
                    connectionString,
                    buffer,
                    DefaultBufferSize);

                if (!result)
                {
                    throw new InvalidOperationException($"Failed to read RFID: {GetLastError()}");
                }

                return Marshal.PtrToStringAnsi(buffer) ?? string.Empty;
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        /// <summary>
        /// Check if a device model is supported
        /// </summary>
        /// <param name="model">Device model to check</param>
        /// <returns>true if supported, false otherwise</returns>
        public bool IsModelSupported(DeviceModel model)
        {
            return NativeMethods.IsModelSupported(model.ToModelString());
        }

        /// <summary>
        /// Get list of all supported device models
        /// </summary>
        /// <returns>Array of supported device models</returns>
        public DeviceModel[] GetSupportedModels()
        {
            IntPtr buffer = Marshal.AllocHGlobal(DefaultBufferSize);
            try
            {
                bool result = NativeMethods.GetSupportedModels(buffer, DefaultBufferSize);
                if (!result)
                {
                    return new DeviceModel[0];
                }

                string modelsString = Marshal.PtrToStringAnsi(buffer) ?? string.Empty;
                if (string.IsNullOrEmpty(modelsString))
                {
                    return new DeviceModel[0];
                }

                string[] modelNames = modelsString.Split(',');
                List<DeviceModel> models = new List<DeviceModel>();

                foreach (string modelName in modelNames)
                {
                    if (Enum.TryParse(modelName.Trim(), out DeviceModel model))
                    {
                        models.Add(model);
                    }
                }

                return models.ToArray();
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        /// <summary>
        /// Get the last error message from the native library
        /// </summary>
        /// <returns>Error message string</returns>
        public string GetLastError()
        {
            IntPtr buffer = Marshal.AllocHGlobal(DefaultBufferSize);
            try
            {
                bool result = NativeMethods.GetLastError(buffer, DefaultBufferSize);
                if (!result)
                {
                    return "No error available";
                }

                return Marshal.PtrToStringAnsi(buffer) ?? "Unknown error";
            }
            finally
            {
                Marshal.FreeHGlobal(buffer);
            }
        }

        /// <summary>
        /// Dispose of resources
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <summary>
        /// Protected implementation of Dispose pattern
        /// </summary>
        /// <param name="disposing">True if called from Dispose(), false if called from finalizer</param>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    // Dispose managed resources here if any
                }

                // Dispose unmanaged resources here if any

                _disposed = true;
            }
        }

        /// <summary>
        /// Finalizer
        /// </summary>
        ~ACLDeviceManager()
        {
            Dispose(false);
        }

        /// <summary>
        /// Validate input parameters
        /// </summary>
        /// <param name="connectionString">Connection string</param>
        /// <param name="text">Text parameter (can be null)</param>
        private void ValidateParameters(string connectionString, string? text)
        {
            if (string.IsNullOrEmpty(connectionString))
            {
                throw new ArgumentNullException(nameof(connectionString), "Connection string cannot be null or empty");
            }

            if (text != null && string.IsNullOrEmpty(text))
            {
                throw new ArgumentException("Text cannot be null or empty", nameof(text));
            }
        }

        /// <summary>
        /// Ensure the native library is loaded
        /// </summary>
        private void EnsureNativeLibraryLoaded()
        {
            // This method ensures the native library is loaded
            // In a real implementation, you might want to check if the DLL exists
            // and handle loading errors more gracefully
        }
    }
}