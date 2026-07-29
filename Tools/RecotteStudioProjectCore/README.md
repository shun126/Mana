# Recotte Studio Project Core

Recotte Studio の `.ccproj` を読み込み、保存、検証するためのマルチプラットフォーム C# ライブラリです。

## 採用する .NET

- **`.NET 10` (`net10.0`) を推奨します。** 新規開発向けの LTS であり、Windows、Linux、macOS で同じ SDK とランタイムを利用できます。
- 旧称の「.NET Core」は `.NET 5` 以降 `.NET` に統合されています。そのため、新規ライブラリを `.NET Core` 固有として構成する必要はありません。
- SDK-style project、nullable reference types、implicit global usings、warnings as errors を有効にし、初期段階から API の曖昧さを減らします。
- JSON の構文検証にはランタイム同梱の `System.Text.Json`、XML には `System.Xml.Linq`、ファイル操作には `System.IO` を使用します。外部 NuGet パッケージを必須にしないため、配布と更新を単純化できます。
- 将来、古いアプリからの参照が必須になった場合だけ `netstandard2.0` などの追加ターゲットを検討します。現時点でのマルチターゲット化は、API 制約とテスト対象を不必要に増やすため行いません。

## 設計方針

- 読み込んだファイルは元のバイト列を保持し、編集していない未知フィールドを再シリアライズで失わないようにします。
- JSON または XML と判定できる文書について、標準ライブラリで基本構文を検証します。
- ID の定義と参照を分けて登録し、重複 ID と未解決参照を診断します。
- バージョン値は正規化せず保持し、既知バージョンごとの変換処理を将来追加できる境界にします。
- スキーマが確定した段階で、形式別の codec と型付き内部モデルを追加します。MCP サーバーはこのライブラリを利用する薄いアダプターとし、ファイル形式の知識を MCP 層へ重複させません。

## 文字コード

- Encoding: UTF-8 (BOM なし)
- Line ending: CRLF
