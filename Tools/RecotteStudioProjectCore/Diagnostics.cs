namespace RecotteStudioProjectCore;

/// <summary>
/// Indicates the severity of a project diagnostic.
/// プロジェクト診断の重要度を示します。
/// </summary>
public enum DiagnosticSeverity
{
    /// <summary>Informational diagnostic. 情報を示す診断。</summary>
    Info,

    /// <summary>Potential problem that does not prevent processing. 処理を妨げない潜在的な問題。</summary>
    Warning,

    /// <summary>Problem that prevents correct processing. 正しい処理を妨げる問題。</summary>
    Error,
}

/// <summary>
/// Describes a stable, machine-readable project diagnostic.
/// 安定した機械可読コードを持つプロジェクト診断を表します。
/// </summary>
/// <param name="Severity">Diagnostic severity. 診断の重要度。</param>
/// <param name="Code">Stable diagnostic code. 安定した診断コード。</param>
/// <param name="Message">Human-readable message. 人間が読めるメッセージ。</param>
public sealed record Diagnostic(DiagnosticSeverity Severity, string Code, string Message);
