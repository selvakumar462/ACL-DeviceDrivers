using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using ACLDevicePlatform.CoreFramework.Managers;
using ACLDevicePlatform.CoreFramework.Models;

namespace ACLDevicePlatform.WindowsService
{
    /// <summary>
    /// Windows Service for managing ACL devices in the background
    /// </summary>
    public class ACLDeviceService : ServiceBase
    {
        private Timer _monitoringTimer;
        private Timer _healthCheckTimer;
        private ACLDeviceManager _deviceManager;
        private List<DeviceModel> _monitoredDevices;
        private bool _isRunning;
        private readonly object _lockObject = new object();

        public ACLDeviceService()
        {
            ServiceName = "ACLDeviceService";
            CanPauseAndContinue = true;
            CanShutdown = true;
            CanHandleSessionChangeEvent = true;
            EventLog.Log = "Application";
            
            // Set up event log source
            if (!EventLog.SourceExists(ServiceName))
            {
                EventLog.CreateEventSource(ServiceName, "Application");
            }
        }

        /// <summary>
        /// Service entry point
        /// </summary>
        /// <param name="args">Command line arguments</param>
        public static void Main(string[] args)
        {
            ServiceBase[] ServicesToRun;
            ServicesToRun = new ServiceBase[]
            {
                new ACLDeviceService()
            };
            ServiceBase.Run(ServicesToRun);
        }

        /// <summary>
        /// Called when the service starts
        /// </summary>
        /// <param name="args">Start arguments</param>
        protected override void OnStart(string[] args)
        {
            try
            {
                lock (_lockObject)
                {
                    _isRunning = true;
                    _deviceManager = new ACLDeviceManager();
                    _monitoredDevices = new List<DeviceModel>();

                    // Initialize monitoring
                    InitializeMonitoring();
                    
                    // Start periodic health checks
                    StartHealthChecks();

                    LogEvent("ACL Device Service started successfully", EventLogEntryType.Information);
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Failed to start service: {ex.Message}", EventLogEntryType.Error);
                throw;
            }
        }

        /// <summary>
        /// Called when the service stops
        /// </summary>
        protected override void OnStop()
        {
            try
            {
                lock (_lockObject)
                {
                    _isRunning = false;
                    
                    // Stop timers
                    StopMonitoring();
                    StopHealthChecks();

                    // Clean up resources
                    _deviceManager?.Dispose();
                    _deviceManager = null;

                    LogEvent("ACL Device Service stopped", EventLogEntryType.Information);
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Error stopping service: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Called when the service is paused
        /// </summary>
        protected override void OnPause()
        {
            try
            {
                lock (_lockObject)
                {
                    StopMonitoring();
                    LogEvent("ACL Device Service paused", EventLogEntryType.Information);
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Error pausing service: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Called when the service is resumed
        /// </summary>
        protected override void OnContinue()
        {
            try
            {
                lock (_lockObject)
                {
                    if (_isRunning)
                    {
                        InitializeMonitoring();
                        LogEvent("ACL Device Service resumed", EventLogEntryType.Information);
                    }
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Error resuming service: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Called during system shutdown
        /// </summary>
        protected override void OnShutdown()
        {
            try
            {
                lock (_lockObject)
                {
                    _isRunning = false;
                    StopMonitoring();
                    StopHealthChecks();
                    
                    LogEvent("ACL Device Service shutdown initiated", EventLogEntryType.Information);
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Error during shutdown: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Initialize device monitoring
        /// </summary>
        private void InitializeMonitoring()
        {
            try
            {
                // Get supported devices
                var supportedDevices = _deviceManager.GetSupportedModels();
                _monitoredDevices.AddRange(supportedDevices);

                // Start monitoring timer (check every 30 seconds)
                _monitoringTimer = new Timer(MonitorDevices, null, TimeSpan.Zero, TimeSpan.FromSeconds(30));

                LogEvent($"Started monitoring {supportedDevices.Length} supported devices", EventLogEntryType.Information);
            }
            catch (Exception ex)
            {
                LogEvent($"Error initializing monitoring: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Stop device monitoring
        /// </summary>
        private void StopMonitoring()
        {
            try
            {
                if (_monitoringTimer != null)
                {
                    _monitoringTimer.Dispose();
                    _monitoringTimer = null;
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Error stopping monitoring: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Start health checks
        /// </summary>
        private void StartHealthChecks()
        {
            try
            {
                // Perform health checks every 5 minutes
                _healthCheckTimer = new Timer(PerformHealthCheck, null, TimeSpan.FromMinutes(5), TimeSpan.FromMinutes(5));
            }
            catch (Exception ex)
            {
                LogEvent($"Error starting health checks: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Stop health checks
        /// </summary>
        private void StopHealthChecks()
        {
            try
            {
                if (_healthCheckTimer != null)
                {
                    _healthCheckTimer.Dispose();
                    _healthCheckTimer = null;
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Error stopping health checks: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Monitor connected devices
        /// </summary>
        /// <param name="state">Timer state</param>
        private void MonitorDevices(object state)
        {
            try
            {
                if (!_isRunning || _deviceManager == null)
                    return;

                foreach (var device in _monitoredDevices)
                {
                    try
                    {
                        // Test device connectivity with a simple operation
                        if (device.GetDeviceType() == DeviceType.Printer)
                        {
                            // For printers, we could send a status request
                            // This is a simplified check
                            var isSupported = _deviceManager.IsModelSupported(device);
                            if (!isSupported)
                            {
                                LogEvent($"Device {device} is no longer supported", EventLogEntryType.Warning);
                            }
                        }
                        else if (device.GetDeviceType() == DeviceType.Scanner)
                        {
                            // For scanners, we could check if they're responsive
                            var isSupported = _deviceManager.IsModelSupported(device);
                            if (!isSupported)
                            {
                                LogEvent($"Scanner {device} is no longer supported", EventLogEntryType.Warning);
                            }
                        }
                        else if (device.GetDeviceType() == DeviceType.RFID)
                        {
                            // For RFID readers, we could check if they're responsive
                            var isSupported = _deviceManager.IsModelSupported(device);
                            if (!isSupported)
                            {
                                LogEvent($"RFID reader {device} is no longer supported", EventLogEntryType.Warning);
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        LogEvent($"Error monitoring device {device}: {ex.Message}", EventLogEntryType.Error);
                    }
                }
            }
            catch (Exception ex)
            {
                LogEvent($"Error in device monitoring: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Perform periodic health checks
        /// </summary>
        /// <param name="state">Timer state</param>
        private void PerformHealthCheck(object state)
        {
            try
            {
                if (!_isRunning || _deviceManager == null)
                    return;

                var supportedDevices = _deviceManager.GetSupportedModels();
                var currentError = _deviceManager.GetLastError();

                if (!string.IsNullOrEmpty(currentError))
                {
                    LogEvent($"Health check detected error: {currentError}", EventLogEntryType.Warning);
                }
                else
                {
                    LogEvent($"Health check passed - {supportedDevices.Length} devices supported", EventLogEntryType.Information);
                }

                // Log service status
                LogEvent($"Service running - monitoring {_monitoredDevices.Count} devices", EventLogEntryType.Information);
            }
            catch (Exception ex)
            {
                LogEvent($"Error in health check: {ex.Message}", EventLogEntryType.Error);
            }
        }

        /// <summary>
        /// Log events to Windows Event Log
        /// </summary>
        /// <param name="message">Event message</param>
        /// <param name="entryType">Event type</param>
        private void LogEvent(string message, EventLogEntryType entryType)
        {
            try
            {
                EventLog.WriteEntry(ServiceName, message, entryType);
            }
            catch
            {
                // If event log fails, we can't do much - this shouldn't crash the service
            }
        }
    }
}