#pragma once
#include <vector>
#include <string>

namespace driver
{
    const std::vector<std::string> forbiddenPatchKeyStrings = {"command"};
    const int WAIT_TIME_MILLI_SEC = 120000; // 2 minutes
    // const int WAIT_TIME_MILLI_SEC = 30000;
}