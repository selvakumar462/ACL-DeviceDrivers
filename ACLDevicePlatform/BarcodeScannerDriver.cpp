// Example patch
std::string result = GetScanResult();
if (IsErrorCode(result)) {
  LogError("Scan error received: " + result);
  return ERROR;
}
if (!IsValidBarcode(result)) {
  LogError("Invalid barcode format: " + result);
  return ERROR;
}
// else process as success