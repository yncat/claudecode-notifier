# Makefile for Claude Code Notifier

# Compiler and flags
CXX = cl
CXXFLAGS = /EHsc /std:c++17 /W3
LDFLAGS = ole32.lib oleaut32.lib sapi.lib

# Directories
SRC_DIR = src
BUILD_DIR = build
TARGET = claudecode-notifier.exe

# Source files
SOURCES = $(SRC_DIR)\main.cpp $(SRC_DIR)\tts_speaker.cpp $(SRC_DIR)\json_parser.cpp

# Object files
OBJECTS = $(BUILD_DIR)\main.obj $(BUILD_DIR)\tts_speaker.obj $(BUILD_DIR)\json_parser.obj

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR) 2>nul || echo Build directory already exists

# Build target
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) /Fe:$(TARGET) $(LDFLAGS)

# Compile source files
$(BUILD_DIR)\main.obj: $(SRC_DIR)\main.cpp $(SRC_DIR)\tts_speaker.h $(SRC_DIR)\json_parser.h
	$(CXX) $(CXXFLAGS) /c $(SRC_DIR)\main.cpp /Fo:$(BUILD_DIR)\main.obj

$(BUILD_DIR)\tts_speaker.obj: $(SRC_DIR)\tts_speaker.cpp $(SRC_DIR)\tts_speaker.h
	$(CXX) $(CXXFLAGS) /c $(SRC_DIR)\tts_speaker.cpp /Fo:$(BUILD_DIR)\tts_speaker.obj

$(BUILD_DIR)\json_parser.obj: $(SRC_DIR)\json_parser.cpp $(SRC_DIR)\json_parser.h
	$(CXX) $(CXXFLAGS) /c $(SRC_DIR)\json_parser.cpp /Fo:$(BUILD_DIR)\json_parser.obj

# Clean build artifacts
clean:
	del /q $(BUILD_DIR)\*.obj 2>nul || echo No object files to clean
	del /q $(TARGET) 2>nul || echo No executable to clean
	rmdir $(BUILD_DIR) 2>nul || echo Build directory not empty or doesn't exist

# Test target
test: $(TARGET)
	@echo Testing claudecode-notifier...
	@echo {"session_id":"test123","message":"Claude is waiting for your input","hook_event_name":"Notification","cwd":"C:\\test"} | $(TARGET)

# Help
help:
	@echo Available targets:
	@echo   all     - Build the application (default)
	@echo   clean   - Remove build artifacts
	@echo   test    - Build and run a test
	@echo   help    - Show this help message

.PHONY: all clean test help