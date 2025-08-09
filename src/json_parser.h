#pragma once

#include <string>
#include "picojson.h"

struct NotificationData {
    std::string session_id;
    std::string message;
    std::string hook_event_name;
    std::string cwd;
};

class JsonParser {
public:
    static bool ParseStdin(NotificationData& data);
    static std::string ReadStdin();
};