# Claude Code Notifier

> **🤖 このプロジェクトは Claude Code により作成されています。100% Claude Code です。すごい！**

[English README](README.md) | 日本語版

Claude Code のフック機能と連携し、音声通知を行う Windows 用コマンドラインアプリケーションです。Claude Code セッションがユーザー入力待ち状態になった際に音声でお知らせし、複数のセッション管理を効率化します。

## 機能

- **音声通知**: Windows SAPI 5.0 を使用した音声合成によるアナウンス
- **フック連携**: Claude Code の通知フックとの統合設計
- **セッション識別**: セッション ID による明確な通知
- **複数メッセージタイプ対応**:
  - ユーザー入力待ち
  - ツール使用許可要求
  - エージェント応答完了（Stopフック）
  - 一般的な通知

## インストール

### 前提条件

- Windows オペレーティングシステム
- Visual Studio Build Tools または Visual C++ コンパイラ
- Windows SAPI 5.0（Windows に標準搭載）

### ソースからのビルド

1. このリポジトリをクローンまたはダウンロード
2. Visual Studio Developer Command Prompt を開く
3. プロジェクトディレクトリに移動
4. 提供されている Makefile を使用してビルド:

```cmd
nmake all
```

### テスト

アプリケーションをテストするには:

```cmd
nmake test
```

これによりサンプル通知が送信され、システムスピーカーからメッセージが読み上げられます。

## 使用方法

### Claude Code フック設定

Claude Code 設定ファイル（通常は `~/.claude/settings.json`）に以下の設定を追加してください:

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

`C:\\path\\to\\claudecode-notifier\\` を実際に実行ファイルをビルドしたパスに置き換えてください。

### WSL2 (Ubuntu) での設定

WSL2 内で Claude Code を使用している場合でも、この Windows 実行ファイルを使用できます：

1. **実行ファイルを Windows 側に配置:**
   ```cmd
   # まず Windows でプロジェクトをビルド
   nmake all
   # 永続的な場所にコピー（例）:
   copy claudecode-notifier.exe C:\Tools\claudecode-notifier.exe
   ```

2. **WSL でシンボリックリンクを作成:**
   ```bash
   # WSL にツール用ディレクトリを作成
   mkdir -p ~/.local/bin
   
   # Windows 実行ファイルへのシンボリックリンクを作成
   ln -s /mnt/c/Tools/claudecode-notifier.exe ~/.local/bin/claudecode-notifier
   
   # ~/.local/bin が PATH に含まれることを確認（必要に応じて ~/.bashrc に追加）
   echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
   source ~/.bashrc
   ```

3. **WSL で Claude Code フックを設定:**
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

4. **設定をテスト:**
   ```bash
   echo '{"session_id":"test123","message":"Claude is waiting for your input","hook_event_name":"Notification","cwd":"/home/user"}' | claudecode-notifier
   ```

### 手動テスト

JSON データをパイプで渡すことで、アプリケーションを手動でテストできます:

```cmd
echo {"session_id":"test123","message":"Claude is waiting for your input","hook_event_name":"Notification","cwd":"C:\\test"} | claudecode-notifier.exe
```

## 動作原理

1. **入力**: Claude Code から標準入力経由でセッション情報を含む JSON データを受信
2. **解析**: JSON からセッション ID とメッセージ内容を抽出
3. **処理**: メッセージ内容に基づいて適切な音声通知を生成
4. **出力**: Windows SAPI 5.0 を使用して通知を音声で再生

## 通知メッセージ

アプリケーションは通知内容に基づいて異なる音声メッセージを生成します:

- **入力待ち**: "Session [ID] is waiting for your input."
- **許可要求**: "Session [ID] needs permission to use a tool."
- **応答完了**: "Session [ID] has stopped."
- **一般通知**: "Session [ID] has a notification."

セッション ID は簡潔性のため最初の8文字に短縮されます。

### Stopフックの利点

Stopフックは Claude Code エージェントが応答を完了した時点で即座に実行されるため、通常の「ユーザー入力待ち」メッセージ（通常60秒後に表示）よりも早い通知を受け取ることができます。これによりエージェントが応答を完了して次の入力を受け付ける準備が整ったことをすぐに知ることができ、複数セッション管理時のワークフロー効率が向上します。

## プロジェクト構成

```
claudecode-notifier/
├── src/
│   ├── main.cpp          - メインアプリケーションロジック
│   ├── tts_speaker.cpp   - SAPI 5.0 音声合成実装
│   ├── tts_speaker.h     - TTS スピーカーヘッダー
│   ├── json_parser.cpp   - JSON 解析ロジック
│   └── json_parser.h     - JSON パーサーヘッダー
├── build/                - ビルド成果物（ビルド時に作成）
├── Makefile             - ビルド設定
├── README.md            - 英語版 README
├── README_ja.md         - このファイル
├── claude_code_hook.md  - フック調査レポート
└── design_doc.md        - 設計文書
```

## 技術詳細

- **言語**: Visual C++ による C++
- **音声**: Windows Speech API (SAPI) 5.0
- **JSON 解析**: カスタム軽量パーサー
- **ビルドシステム**: Visual Studio コンパイラによる nmake
- **依存関係**: Windows システムライブラリ（ole32.lib、oleaut32.lib、sapi.lib）

## トラブルシューティング

### ビルド問題

- Visual Studio Developer Command Prompt を使用していることを確認
- Windows SDK がインストールされていることを確認
- SAPI 開発ヘッダーが利用可能であることを確認

### 実行時問題

- **音声出力なし**: システム音量とオーディオドライバーを確認
- **権限エラー**: 実行ファイルが適切な権限を持っていることを確認
- **JSON 解析エラー**: 入力形式が Claude Code のフック出力と一致することを確認

## 貢献

このプロジェクトへの貢献を歓迎します！以下の方法でご協力いただけます：

### 問題報告

- **バグレポート**: Windows バージョン、Visual Studio バージョン、再現手順を含めてください
- **機能要求**: 使用事例と期待する動作を説明してください
- **ドキュメント**: README ファイルやコード コメントの改善にご協力ください

### 開発環境セットアップ

1. このリポジトリをフォーク
2. Visual Studio Build Tools または Visual C++ コンパイラをセットアップ
3. フォークをクローンし、機能ブランチを作成
4. 変更を行い、`nmake test` でテスト
5. 明確な説明とともにプルリクエストを送信

### コードガイドライン

- 既存のコードスタイルと命名規則に従ってください
- 適切なエラーハンドリングを追加してください
- 変更を十分にテストしてください
- 必要に応じてドキュメントを更新してください

### 改善が期待される分野

- 追加の通知メッセージパターン
- カスタムメッセージ用の設定ファイルサポート
- 異なる TTS 音声や読み上げ速度のサポート
- 強化されたエラーログとデバッグ機能

## ライセンス

このプロジェクトは Claude Code との使用を目的として提供されています。Claude Code および関連ツール使用時は Anthropic の利用規約を遵守してください。

### サードパーティライブラリ

このプロジェクトは以下のサードパーティライブラリを使用しています：

- **PicoJSON** (https://github.com/kazuho/picojson)  
  Copyright (c) 2009-2010 Cybozu Labs, Inc.  
  Copyright (c) 2011-2014 Kazuho Oku  
  BSD-2-Clause ライセンス

## 関連ドキュメント

- [Claude Code フックドキュメント](https://docs.anthropic.com/en/docs/claude-code/hooks)
- [Windows SAPI ドキュメント](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms723627(v=vs.85))