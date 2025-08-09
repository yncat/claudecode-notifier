#include <iostream>
#include <string>
#include "tts_speaker.h"
#include "json_parser.h"

std::string GenerateNotificationMessage(const std::string& sessionId, const std::string& message) {
    size_t maxLen = 8;
    if (sessionId.length() < maxLen) {
        maxLen = sessionId.length();
    }
    std::string shortSessionId = sessionId.substr(0, maxLen);
    
    if (message.find("waiting for your input") != std::string::npos) {
        return "Session " + shortSessionId + " is waiting for your input.";
    }
    
    if (message.find("needs your permission") != std::string::npos) {
        return "Session " + shortSessionId + " needs permission to use a tool.";
    }
    
    if (message.find("permission to use") != std::string::npos) {
        return "Session " + shortSessionId + " needs permission to use a tool.";
    }
    
    return "Session " + shortSessionId + " has a notification.";
}

int main() {
    NotificationData data;
    
    if (!JsonParser::ParseStdin(data)) {
        std::cerr << "Failed to parse input JSON" << std::endl;
        return 1;
    }
    
    TTSSpeaker speaker;
    if (!speaker.Initialize()) {
        std::cerr << "Failed to initialize TTS speaker" << std::endl;
        return 1;
    }
    
    std::string notificationMessage = GenerateNotificationMessage(data.session_id, data.message);
    
    if (!speaker.Speak(notificationMessage)) {
        std::cerr << "Failed to speak notification" << std::endl;
        return 1;
    }
    
    std::cout << "Notification spoken: " << notificationMessage << std::endl;
    return 0;
}