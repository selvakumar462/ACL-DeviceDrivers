// Example logic
switch (protocolResponse) {
    case ERROR_TIMEOUT:
    case ERROR_NO_READ:
        return ERROR;
    default:
        return VALID_DATA;
}