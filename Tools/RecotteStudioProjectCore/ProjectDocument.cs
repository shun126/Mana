using System.Text;
using System.Text.Json;
using System.Text.RegularExpressions;
using System.Xml.Linq;

namespace RecotteStudioProjectCore;

/// <summary>
/// Owns a Recotte Studio project document while preserving unsupported fields verbatim.
/// 未対応フィールドをそのまま保持しながら Recotte Studio プロジェクト文書を管理します。
/// </summary>
public sealed partial class ProjectDocument
{
    private static readonly UTF8Encoding Utf8WithoutBom = new(false, true);
    private byte[] rawContent = [];

    /// <summary>
    /// Gets the version detected in the current document.
    /// 現在の文書から検出したバージョンを取得します。
    /// </summary>
    public ProjectVersion Version { get; private set; } = ProjectVersion.Unknown;

    /// <summary>
    /// Gets the registry used for identifier and reference validation.
    /// 識別子と参照の検証に使用するレジストリを取得します。
    /// </summary>
    public IdRegistry Ids { get; } = new();

    /// <summary>
    /// Gets the project text decoded as strict UTF-8.
    /// 厳密な UTF-8 としてデコードしたプロジェクトテキストを取得します。
    /// </summary>
    public string RawText => Utf8WithoutBom.GetString(rawContent);

    /// <summary>
    /// Loads a .ccproj file and keeps its original bytes for lossless round trips.
    /// .ccproj ファイルを読み込み、無損失で再保存できるよう元のバイト列を保持します。
    /// </summary>
    public static ProjectDocument Load(string path, ICollection<Diagnostic>? diagnostics = null)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(path);
        var document = new ProjectDocument();
        try
        {
            document.SetRawContent(File.ReadAllBytes(path));
        }
        catch (Exception exception) when (exception is IOException or UnauthorizedAccessException or DecoderFallbackException)
        {
            diagnostics?.Add(new Diagnostic(DiagnosticSeverity.Error, "RECOTTE_CORE_LOAD_FAILED", $"Failed to load the .ccproj file: {exception.Message}"));
        }

        return document;
    }

    /// <summary>
    /// Saves the current bytes to a .ccproj file without rewriting unknown fields.
    /// 未知フィールドを書き換えず、現在のバイト列を .ccproj ファイルに保存します。
    /// </summary>
    public bool Save(string path, ICollection<Diagnostic>? diagnostics = null)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(path);
        try
        {
            var fullPath = Path.GetFullPath(path);
            var directory = Path.GetDirectoryName(fullPath);
            if (directory is not null)
            {
                Directory.CreateDirectory(directory);
            }

            File.WriteAllBytes(fullPath, rawContent);
            return true;
        }
        catch (Exception exception) when (exception is IOException or UnauthorizedAccessException)
        {
            diagnostics?.Add(new Diagnostic(DiagnosticSeverity.Error, "RECOTTE_CORE_SAVE_FAILED", $"Failed to save the .ccproj file: {exception.Message}"));
            return false;
        }
    }

    /// <summary>
    /// Replaces the document with UTF-8 text and refreshes derived metadata.
    /// 文書を UTF-8 テキストで置き換え、派生メタデータを更新します。
    /// </summary>
    public void SetRawText(string text)
    {
        ArgumentNullException.ThrowIfNull(text);
        SetRawContent(Utf8WithoutBom.GetBytes(text));
    }

    /// <summary>
    /// Validates encoding, recognizable JSON or XML syntax, version metadata, and registered references.
    /// エンコーディング、認識可能な JSON または XML 構文、バージョン情報、登録済み参照を検証します。
    /// </summary>
    public IReadOnlyList<Diagnostic> Validate()
    {
        var diagnostics = new List<Diagnostic>();
        if (rawContent.Length == 0)
        {
            diagnostics.Add(new Diagnostic(DiagnosticSeverity.Error, "RECOTTE_CORE_EMPTY_DOCUMENT", "The project document is empty."));
            diagnostics.AddRange(Ids.Validate());
            return diagnostics;
        }

        string text;
        try
        {
            text = RawText;
        }
        catch (DecoderFallbackException exception)
        {
            diagnostics.Add(new Diagnostic(DiagnosticSeverity.Error, "RECOTTE_CORE_INVALID_UTF8", $"The project document is not valid UTF-8: {exception.Message}"));
            diagnostics.AddRange(Ids.Validate());
            return diagnostics;
        }

        ValidateSyntax(text, diagnostics);
        if (!Version.IsKnown)
        {
            diagnostics.Add(new Diagnostic(DiagnosticSeverity.Warning, "RECOTTE_CORE_UNKNOWN_VERSION", "The project format version was not detected."));
        }

        diagnostics.AddRange(Ids.Validate());
        return diagnostics;
    }

    private void SetRawContent(byte[] content)
    {
        rawContent = content;
        Version = DetectVersion(content);
    }

    private static ProjectVersion DetectVersion(byte[] content)
    {
        string text;
        try
        {
            text = Utf8WithoutBom.GetString(content);
        }
        catch (DecoderFallbackException)
        {
            return ProjectVersion.Unknown;
        }

        var match = VersionPattern().Match(text);
        return match.Success ? new ProjectVersion(match.Groups["value"].Value) : ProjectVersion.Unknown;
    }

    private static void ValidateSyntax(string text, ICollection<Diagnostic> diagnostics)
    {
        var trimmed = text.AsSpan().TrimStart();
        try
        {
            if (trimmed.StartsWith("{".AsSpan(), StringComparison.Ordinal) || trimmed.StartsWith("[".AsSpan(), StringComparison.Ordinal))
            {
                using var document = JsonDocument.Parse(text);
            }
            else if (trimmed.StartsWith("<".AsSpan(), StringComparison.Ordinal))
            {
                _ = XDocument.Parse(text, LoadOptions.PreserveWhitespace | LoadOptions.SetLineInfo);
            }
            else
            {
                diagnostics.Add(new Diagnostic(DiagnosticSeverity.Warning, "RECOTTE_CORE_UNKNOWN_SYNTAX", "The project syntax is not recognized as JSON or XML."));
            }
        }
        catch (Exception exception) when (exception is JsonException or System.Xml.XmlException)
        {
            diagnostics.Add(new Diagnostic(DiagnosticSeverity.Error, "RECOTTE_CORE_INVALID_SYNTAX", $"The project syntax is invalid: {exception.Message}"));
        }
    }

    [GeneratedRegex("(?:\\\"version\\\"\\s*:\\s*\\\"|\\bversion\\s*=\\s*\\\")(?<value>[^\\\"]+)\\\"", RegexOptions.IgnoreCase | RegexOptions.CultureInvariant)]
    private static partial Regex VersionPattern();
}
