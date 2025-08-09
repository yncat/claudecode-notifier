#include "json_parser.h"
#include <iostream>
#include <sstream>

std::string JsonParser::ReadStdin() {
    std::string input;
    std::string line;
    
    while (std::getline(std::cin, line)) {
        input += line;
    }
    
    return input;
}

bool JsonParser::ParseStdin(NotificationData& data) {
    try {
        std::string input = ReadStdin();
        
        if (input.empty()) {
            std::cerr << "No input received from stdin" << std::endl;
            return false;
        }
        
        // Parse JSON using picojson
        picojson::value json_value;
        std::string err = picojson::parse(json_value, input);
        
        if (!err.empty()) {
            std::cerr << "JSON parse error: " << err << std::endl;
            return false;
        }
        
        if (!json_value.is<picojson::object>()) {
            std::cerr << "JSON root is not an object" << std::endl;
            return false;
        }
        
        const picojson::object& obj = json_value.get<picojson::object>();
        
        // Extract values with error checking
        auto session_id_it = obj.find("session_id");
        if (session_id_it != obj.end() && session_id_it->second.is<std::string>()) {
            data.session_id = session_id_it->second.get<std::string>();
        } else {
            std::cerr << "Failed to parse session_id from JSON" << std::endl;
            return false;
        }
        
        auto message_it = obj.find("message");
        if (message_it != obj.end() && message_it->second.is<std::string>()) {
            data.message = message_it->second.get<std::string>();
        }
        
        auto hook_event_name_it = obj.find("hook_event_name");
        if (hook_event_name_it != obj.end() && hook_event_name_it->second.is<std::string>()) {
            data.hook_event_name = hook_event_name_it->second.get<std::string>();
        }
        
        auto cwd_it = obj.find("cwd");
        if (cwd_it != obj.end() && cwd_it->second.is<std::string>()) {
            data.cwd = cwd_it->second.get<std::string>();
        }
        
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception during JSON parsing: " << e.what() << std::endl;
        return false;
    }
}