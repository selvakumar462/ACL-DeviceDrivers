// Example pseudo-code:
if (IsKnownErrorCode(scanResult)) {
  LogError("FX7500 scan error: " + scanResult);
  return ERROR;
} else {
  return scanResult;
}

// IsKnownErrorCode implementation could consult a list/set of device error codes and compare.