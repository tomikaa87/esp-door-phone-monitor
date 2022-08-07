#include "Utils.h"

#include <cstring>
#include <string>

std::string Utils::pgmToStdString(PGM_P str)
{
    const auto len = strlen_P(str);
    std::string ss(len, 0);
    memcpy_P(&ss[0], str, len);
    return ss;
}