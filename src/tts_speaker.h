#pragma once

#include <string>
#include <sapi.h>
#include <sphelper.h>

class TTSSpeaker {
private:
    ISpVoice* pVoice;
    bool initialized;

public:
    TTSSpeaker();
    ~TTSSpeaker();
    bool Initialize();
    bool Speak(const std::string& text);
    void Cleanup();
};