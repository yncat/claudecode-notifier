# Claude Code フック機能調査レポート

## 概要

Claude Code には「フック」と呼ばれる機能があり、特定のイベント発生時に外部のスクリプトやコマンドを自動実行できる。これにより、Claude Code の動作を拡張・カスタマイズすることが可能。

## フックの種類

Claude Code では以下のフック種類が利用できる：

1. **PreToolUse**: ツール実行前に発火
2. **PostToolUse**: ツール実行後に発火  
3. **UserPromptSubmit**: ユーザーがプロンプトを送信した際に発火
4. **Notification**: 通知発生時に発火 ★今回のターゲット
5. **Stop/SubagentStop**: メインエージェントまたはサブエージェントの応答終了時に発火

## Notification フックの詳細

### 発火タイミング

Notification フックは以下の2つのシナリオで発火する：

1. **ツール使用許可待ち**: Claude がツール使用許可を求める際（例：「Claude needs your permission to use Bash」）
2. **ユーザー入力待ち**: プロンプト入力が60秒以上アイドル状態の際（例：「Claude is waiting for your input」）

### 受信データ構造

フックスクリプトは標準入力（stdin）経由でJSON形式のデータを受信する：

```json
{
  "session_id": "abc123",
  "transcript_path": "/path/to/transcript.jsonl", 
  "cwd": "/current/working/directory",
  "hook_event_name": "Notification",
  "message": "Claude is waiting for your input"
}
```

### 環境変数

- `CLAUDE_PROJECT_DIR`: プロジェクトルートディレクトリの絶対パス
- `CLAUDE_NOTIFICATION`: 通知メッセージの内容（Notificationフック専用）

## 設定方法

フックの設定は `~/.claude/settings.json` ファイルで行う：

```json
{
  "hooks": {
    "Notification": [
      {
        "hooks": [
          {
            "type": "command",
            "command": "C:\\path\\to\\notifier.exe"
          }
        ]
      }
    ]
  }
}
```

## 実装要件

今回作成するアプリケーションは：

1. **入力**: 標準入力からJSONデータを受信
2. **処理**: session_idとmessageを解析し、通知内容を決定
3. **出力**: Windows SAPI5を使用して音声通知を実行

## セキュリティ注意事項

フックは任意のシェルコマンドを実行するため、設定時は十分注意が必要。信頼できないコマンドの実行は避けること。

## 参考情報

- Claude Code 公式ドキュメント: https://docs.anthropic.com/en/docs/claude-code/hooks
- 通知は最低60秒のアイドル時間後に「Claude is waiting for your input」メッセージで発火
- フックスクリプトは同期的に実行されるため、長時間の処理は避けるべき