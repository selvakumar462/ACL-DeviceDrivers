// Pseudocode
bool IsErrorCode(const std::string& data) {
  static const std::set<std::string> errorCodes = {"ERR01", "ERR_TIMEOUT", "NO_READ"};
  return errorCodes.count(data) > 0;
}