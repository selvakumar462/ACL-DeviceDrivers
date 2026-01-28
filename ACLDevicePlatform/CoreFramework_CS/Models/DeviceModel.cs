namespace ACLDevicePlatform.CoreFramework.Models
{
    /// <summary>
    /// Enumeration of supported ACL device models
    /// </summary>
    public enum DeviceModel
    {
        // Printers
        /// <summary>
        /// ACL GK420 printer
        /// </summary>
        GK420,

        /// <summary>
        /// ACL ZD420 printer
        /// </summary>
        ZD420,

        /// <summary>
        /// ACL ZT411 printer
        /// </summary>
        ZT411,

        /// <summary>
        /// ACL ZT421 printer
        /// </summary>
        ZT421,

        /// <summary>
        /// ACL ZQ521 printer
        /// </summary>
        ZQ521,

        // Scanners
        /// <summary>
        /// ACL DS2208 scanner
        /// </summary>
        DS2208,

        /// <summary>
        /// ACL DS4608 scanner
        /// </summary>
        DS4608,

        /// <summary>
        /// ACL LI4278 scanner
        /// </summary>
        LI4278,

        /// <summary>
        /// ACL DS3678 scanner
        /// </summary>
        DS3678,

        // RFID
        /// <summary>
        /// ACL RFD40 RFID reader
        /// </summary>
        RFD40,

        /// <summary>
        /// ACL FX9600 RFID reader
        /// </summary>
        FX9600
    }

    /// <summary>
    /// Extension methods for DeviceModel enum
    /// </summary>
    public static class DeviceModelExtensions
    {
        /// <summary>
        /// Gets the string representation of the device model
        /// </summary>
        /// <param name="model">The device model</param>
        /// <returns>String representation of the model</returns>
        public static string ToModelString(this DeviceModel model)
        {
            return model switch
            {
                DeviceModel.GK420 => "GK420",
                DeviceModel.ZD420 => "ZD420",
                DeviceModel.ZT411 => "ZT411",
                DeviceModel.ZT421 => "ZT421",
                DeviceModel.ZQ521 => "ZQ521",
                DeviceModel.DS2208 => "DS2208",
                DeviceModel.DS4608 => "DS4608",
                DeviceModel.LI4278 => "LI4278",
                DeviceModel.DS3678 => "DS3678",
                DeviceModel.RFD40 => "RFD40",
                DeviceModel.FX9600 => "FX9600",
                _ => model.ToString()
            };
        }

        /// <summary>
        /// Gets the device type for the model
        /// </summary>
        /// <param name="model">The device model</param>
        /// <returns>Device type</returns>
        public static DeviceType GetDeviceType(this DeviceModel model)
        {
            return model switch
            {
                DeviceModel.GK420 or DeviceModel.ZD420 or DeviceModel.ZT411 or 
                DeviceModel.ZT421 or DeviceModel.ZQ521 => DeviceType.Printer,
                DeviceModel.DS2208 or DeviceModel.DS4608 or DeviceModel.LI4278 or 
                DeviceModel.DS3678 => DeviceType.Scanner,
                DeviceModel.RFD40 or DeviceModel.FX9600 => DeviceType.RFID,
                _ => DeviceType.Printer // Default fallback
            };
        }
    }
}