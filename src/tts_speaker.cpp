#include "tts_speaker.h"
#include <iostream>
#include <comdef.h>

TTSSpeaker::TTSSpeaker() : pVoice(nullptr), initialized(false) {
}

TTSSpeaker::~TTSSpeaker() {
    Cleanup();
}

bool TTSSpeaker::Initialize() {
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM" << std::endl;
        return false;
    }

    hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    if (FAILED(hr)) {
        std::cerr << "Failed to create SAPI voice instance" << std::endl;
        CoUninitialize();
        return false;
    }

    initialized = true;
    return true;
}

bool TTSSpeaker::Speak(const std::string& text) {
    if (!initialized || !pVoice) {
        return false;
    }

    std::wstring wtext(text.begin(), text.end());
    HRESULT hr = pVoice->Speak(wtext.c_str(), 0, nullptr);
    
    return SUCCEEDED(hr);
}

void TTSSpeaker::Cleanup() {
    if (pVoice) {
        pVoice->Release();
        pVoice = nullptr;
    }
    if (initialized) {
        CoUninitialize();
        initialized = false;
    }
}