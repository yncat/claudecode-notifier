# Claude Code Notifier

> **🤖 This project was created entirely by Claude Code. 100% Claude Code powered development. Amazing!**

[日本語版 README](README_ja.md) | English

A Windows command-line application that provides voice notifications for Claude Code hooks. This tool announces when Claude Code sessions are waiting for user input, making it easier to manage multiple sessions.

## Features

- **Voice Notifications**: Uses Windows SAPI 5.0 for text-to-speech announcements
- **Hook Integration**: Designed to work with Claude Code's notification hooks
- **Session Awareness**: Identifies sessions by their ID for clear notifications
- **Multiple Message Types**: Handles various notification scenarios:
  - User input waiting
  - Tool permission requests
  - Agent response completion (Stop hook)
  - General notifications

## Installation

### Prerequisites

- Windows operating system
- Visual Studio Build Tools or Visual C++ compiler
- Windows SAPI 5.0 (included with Windows)

### Building from Source

1. Clone or download this repository
2. Open a Visual Studio Developer Command Prompt
3. Navigate to the project directory
4. Build using the provided Makefile:

```cmd
nmake all
```

### Testing

To test the application:

```cmd
nmake test
```

This will send a sample notification and speak the message through your system's speakers.

## Usage

### Claude Code Hook Configuration

Add the following configuration to your Claude Code settings file (typically `~/.claude/settings.json`):

```json
{
  "hooks": {
    "Notification": [
      {
        "hooks": [
          {
            "type": "command",
            "command": "C:\\path\\to\\claudecode-notifier\\claudecode-notifier.exe"
          }
        ]
      }
    ],
    "Stop": [
      {
        "hooks": [
          {
            "type": "command",
            "command": "C:\\path\\to\\claudecode-notifier\\claudecode-notifier.exe"
          }
        ]
      }
    ]
  }
}
```

Replace `C:\\path\\to\\claudecode-notifier\\` with the actual path where you built the executable.

### WSL2 (Ubuntu) Setup

If you're using Claude Code from within WSL2, you can still use this Windows executable:

1. **Place the executable on Windows side:**
   ```cmd
   # Build the project on Windows first
   nmake all
   # Copy to a permanent location, e.g.:
   copy claudecode-notifier.exe C:\Tools\claudecode-notifier.exe
   ```

2. **Create a symbolic link in WSL:**
   ```bash
   # Create a directory for tools in WSL
   mkdir -p ~/.local/bin
   
   # Create symbolic link to Windows executable
   ln -s /mnt/c/Tools/claudecode-notifier.exe ~/.local/bin/claudecode-notifier
   
   # Make sure ~/.local/bin is in PATH (add to ~/.bashrc if needed)
   echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
   source ~/.bashrc
   ```

3. **Configure Claude Code hook in WSL:**
   ```json
   {
     "hooks": {
       "Notification": [
         {
           "hooks": [
             {
               "type": "command",
               "command": "/home/yourusername/.local/bin/claudecode-notifier"
             }
           ]
         }
       ],
       "Stop": [
         {
           "hooks": [
             {
               "type": "command",
               "command": "/home/yourusername/.local/bin/claudecode-notifier"
             }
           ]
         }
       ]
     }
   }
   ```

4. **Test the setup:**
   ```bash
   echo '{"session_id":"test123","message":"Claude is waiting for your input","hook_event_name":"Notification","cwd":"/home/user"}' | claudecode-notifier
   ```

### Manual Testing

You can test the application manually by piping JSON data to it:

```cmd
echo {"session_id":"test123","message":"Claude is waiting for your input","hook_event_name":"Notification","cwd":"C:\\test"} | claudecode-notifier.exe
```

## How It Works

1. **Input**: Receives JSON data from Claude Code via stdin containing session information
2. **Parsing**: Extracts session ID and message content from the JSON
3. **Processing**: Generates appropriate voice notification based on message content
4. **Output**: Speaks the notification using Windows SAPI 5.0

## Notification Messages

The application generates different voice messages based on the notification content:

- **Input Waiting**: "Session [ID] is waiting for your input."
- **Permission Request**: "Session [ID] needs permission to use a tool."
- **Response Completion**: "Session [ID] has stopped."
- **General Notification**: "Session [ID] has a notification."

Session IDs are truncated to the first 8 characters for brevity.

### Stop Hook Benefits

The Stop hook runs immediately when the Claude Code agent finishes its response, providing faster notifications than waiting for the standard "waiting for user input" message (which typically appears after a 60-second delay). This allows you to be notified as soon as the agent has completed its response and is ready for your next input, improving workflow efficiency when managing multiple sessions.

## Project Structure

```
claudecode-notifier/
├── src/
│   ├── main.cpp          - Main application logic
│   ├── tts_speaker.cpp   - SAPI 5.0 text-to-speech implementation
│   ├── tts_speaker.h     - TTS speaker header
│   ├── json_parser.cpp   - JSON parsing logic
│   └── json_parser.h     - JSON parser header
├── build/                - Build artifacts (created during build)
├── Makefile             - Build configuration
├── README.md            - This file
├── README_ja.md         - Japanese version
├── claude_code_hook.md  - Hook investigation report (Japanese)
└── design_doc.md        - Design document (Japanese)
```

## Technical Details

- **Language**: C++ with Visual C++
- **Audio**: Windows Speech API (SAPI) 5.0
- **JSON Parsing**: Custom lightweight parser
- **Build System**: nmake with Visual Studio compiler
- **Dependencies**: Windows system libraries (ole32.lib, oleaut32.lib, sapi.lib)

## Troubleshooting

### Build Issues

- Ensure you're using a Visual Studio Developer Command Prompt
- Verify Windows SDK is installed
- Check that SAPI development headers are available

### Runtime Issues

- **No audio output**: Check system volume and audio drivers
- **Permission errors**: Ensure the executable has proper permissions
- **JSON parsing errors**: Verify the input format matches Claude Code's hook output

## Contributing

Contributions to this project are welcome! Here's how you can help:

### Reporting Issues

- **Bug Reports**: Include your Windows version, Visual Studio version, and steps to reproduce
- **Feature Requests**: Describe the use case and expected behavior
- **Documentation**: Help improve README files or code comments

### Development Setup

1. Fork this repository
2. Set up Visual Studio Build Tools or Visual C++ compiler
3. Clone your fork and create a feature branch
4. Make your changes and test with `nmake test`
5. Submit a pull request with a clear description

### Code Guidelines

- Follow existing code style and naming conventions
- Add appropriate error handling
- Test your changes thoroughly
- Update documentation as needed

### Areas for Improvement

- Additional notification message patterns
- Configuration file support for custom messages
- Support for different TTS voices or speech rates
- Enhanced error logging and debugging features

## License

This project is provided as-is for use with Claude Code. Please respect Anthropic's terms of service when using Claude Code and related tools.

### Third-party Libraries

This project uses the following third-party library:

- **PicoJSON** (https://github.com/kazuho/picojson)  
  Copyright (c) 2009-2010 Cybozu Labs, Inc.  
  Copyright (c) 2011-2014 Kazuho Oku  
  Licensed under the BSD-2-Clause License

## Related Documentation

- [Claude Code Hooks Documentation](https://docs.anthropic.com/en/docs/claude-code/hooks)
- [Windows SAPI Documentation](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms723627(v=vs.85))