using System;
using System.Runtime.InteropServices;

namespace ACLDevicePlatform.CoreFramework.Interop
{
    /// <summary>
    /// P/Invoke declarations for the native C++ CoreFramework DLL
    /// </summary>
    internal static class NativeMethods
    {
        private const string DllName = "CoreFramework.dll";

        /// <summary>
        /// Print a label using the specified ACL printer model
        /// </summary>
        /// <param name="model">Printer model name (e.g., "GK420", "ZD420")</param>
        /// <param name="connectionString">Connection parameters (USB, TCP, Bluetooth)</param>
        /// <param name="text">Text content for the label</param>
        /// <returns>true if print successful, false otherwise</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal static extern bool PrintLabel(
            [MarshalAs(UnmanagedType.LPStr)] string model,
            [MarshalAs(UnmanagedType.LPStr)] string connectionString,
            [MarshalAs(UnmanagedType.LPStr)] string text);

        /// <summary>
        /// Scan a barcode using the specified ACL scanner model
        /// </summary>
        /// <param name="model">Scanner model name (e.g., "DS2208", "DS4608")</param>
        /// <param name="connectionString">Connection parameters</param>
        /// <param name="resultBuffer">Buffer to store scanned result</param>
        /// <param name="bufferSize">Size of the result buffer</param>
        /// <returns>true if scan successful, false otherwise</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal static extern bool ScanBarcode(
            [MarshalAs(UnmanagedType.LPStr)] string model,
            [MarshalAs(UnmanagedType.LPStr)] string connectionString,
            IntPtr resultBuffer,
            int bufferSize);

        /// <summary>
        /// Read RFID tags using the specified ACL RFID reader model
        /// </summary>
        /// <param name="model">RFID reader model name (e.g., "RFD40", "FX9600")</param>
        /// <param name="connectionString">Connection parameters</param>
        /// <param name="resultBuffer">Buffer to store RFID data</param>
        /// <param name="bufferSize">Size of the result buffer</param>
        /// <returns>true if read successful, false otherwise</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal static extern bool ReadRfid(
            [MarshalAs(UnmanagedType.LPStr)] string model,
            [MarshalAs(UnmanagedType.LPStr)] string connectionString,
            IntPtr resultBuffer,
            int bufferSize);

        /// <summary>
        /// Get the last error message from the native library
        /// </summary>
        /// <param name="buffer">Buffer to store error message</param>
        /// <param name="bufferSize">Size of the buffer</param>
        /// <returns>true if error message available, false otherwise</returns>
        [DllImport(DllName, EntryPoint = "GetACLDeviceLastError", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal static extern bool GetLastError(
            IntPtr buffer,
            int bufferSize);

        /// <summary>
        /// Check if a device model is supported
        /// </summary>
        /// <param name="model">Device model name</param>
        /// <returns>true if supported, false otherwise</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal static extern bool IsModelSupported(
            [MarshalAs(UnmanagedType.LPStr)] string model);

        /// <summary>
        /// Get list of supported device models
        /// </summary>
        /// <param name="buffer">Buffer to store comma-separated model list</param>
        /// <param name="bufferSize">Size of the buffer</param>
        /// <returns>true if successful, false otherwise</returns>
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal static extern bool GetSupportedModels(
            IntPtr buffer,
            int bufferSize);
    }
}