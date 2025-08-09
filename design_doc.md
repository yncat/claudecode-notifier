# Claude Code Notifier - 設計書

## プロジェクト概要

Claude Code のフック機能を利用して、ユーザー入力待ち状態を音声で通知するWindows用コマンドラインアプリケーション。

## アーキテクチャ

### 基本構成

```
claudecode-notifier.exe
├── main.cpp          - エントリーポイント、JSON解析
├── tts_speaker.cpp   - SAPI5音声合成処理
├── tts_speaker.h     - SAPI5関連のヘッダー
└── json_parser.cpp   - JSON解析処理
```

### 技術スタック

- **言語**: C++ (Visual C++)
- **JSON解析**: Windows標準のJSON API または軽量ライブラリ
- **音声合成**: Windows SAPI 5.0
- **ビルドシステム**: Visual Studio + Makefile

## データフロー

1. **入力**: 標準入力からJSONデータを受信
2. **解析**: session_id, message フィールドを抽出
3. **判定**: 通知が必要かどうかを判定
4. **生成**: 適切な通知メッセージを生成
5. **出力**: SAPI5で音声合成・再生

### 入力JSON形式

```json
{
  "session_id": "abc123",
  "transcript_path": "/path/to/transcript.jsonl", 
  "cwd": "/current/working/directory",
  "hook_event_name": "Notification",
  "message": "Claude is waiting for your input"
}
```

## 機能仕様

### 通知対象メッセージ

以下のメッセージパターンを検出して通知：

1. "Claude is waiting for your input" - 入力待ち
2. "Claude needs your permission to use" - ツール使用許可待ち
3. その他の通知メッセージ

### 音声メッセージ生成ロジック

```cpp
string generateNotificationMessage(const string& sessionId, const string& message) {
    if (message.find("waiting for your input") != string::npos) {
        return "Session " + sessionId.substr(0, 8) + " is waiting for your input.";
    }
    if (message.find("needs your permission") != string::npos) {
        return "Session " + sessionId.substr(0, 8) + " needs permission to use a tool.";
    }
    return "Session " + sessionId.substr(0, 8) + " has a notification.";
}
```

## クラス設計

### TTSSpeaker クラス

```cpp
class TTSSpeaker {
private:
    ISpVoice* pVoice;
    
public:
    TTSSpeaker();
    ~TTSSpeaker();
    bool Initialize();
    bool Speak(const std::string& text);
    void Cleanup();
};
```

### JsonParser クラス

```cpp
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
```

## エラーハンドリング

1. **SAPI5初期化失敗**: ログ出力してサイレント終了
2. **JSON解析失敗**: エラーメッセージを標準エラー出力
3. **音声合成失敗**: 代替手段（beep音など）で通知

## パフォーマンス要件

- **起動時間**: 500ms以下
- **音声再生開始**: 1秒以内
- **メモリ使用量**: 10MB以下
- **CPU使用率**: 通知時のみ短時間使用

## セキュリティ考慮事項

- 入力されるJSONデータの検証（最大サイズ制限）
- session_idの機密情報漏洩防止（先頭8文字のみ使用）
- 音声出力時の情報漏洩防止

## ファイル構成

```
claudecode-notifier/
├── src/
│   ├── main.cpp
│   ├── tts_speaker.cpp
│   ├── tts_speaker.h
│   ├── json_parser.cpp
│   └── json_parser.h
├── Makefile
├── claudecode-notifier.vcxproj
├── README.md
├── README_ja.md
├── claude_code_hook.md
└── design_doc.md
```

## 実装手順

1. Visual Studio プロジェクト作成
2. SAPI5を使用したTTSSpeakerクラス実装
3. JSON解析処理の実装
4. main関数での統合処理実装
5. Makefile作成とビルドテスト
6. ドキュメント整備

## 設定例

Claude Code settings.json への追加内容：

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
    ]
  }
}
```