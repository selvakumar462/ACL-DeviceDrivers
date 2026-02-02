// Pseudocode: 
const char* ERROR_CODES[] = {"ERROR01", "ERROR02", ...};
bool is_error_code(const char* data) { 
  for (int i = 0; i < ERROR_CODES_LEN; ++i) {
    if (strcmp(data, ERROR_CODES[i]) == 0) return true;
  }
  return false;
}