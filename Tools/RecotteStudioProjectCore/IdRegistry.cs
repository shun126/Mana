namespace RecotteStudioProjectCore;

/// <summary>
/// Tracks project identifiers and references using ordinal comparison.
/// 序数比較を使用してプロジェクトの識別子と参照を追跡します。
/// </summary>
public sealed class IdRegistry
{
    private readonly HashSet<string> definitions = new(StringComparer.Ordinal);
    private readonly List<string> duplicateDefinitions = [];
    private readonly HashSet<string> references = new(StringComparer.Ordinal);

    /// <summary>
    /// Gets registered definitions in no specified order.
    /// 登録済みの定義を順序保証なしで取得します。
    /// </summary>
    public IReadOnlyCollection<string> Definitions => definitions;

    /// <summary>
    /// Gets registered references in no specified order.
    /// 登録済みの参照を順序保証なしで取得します。
    /// </summary>
    public IReadOnlyCollection<string> References => references;

    /// <summary>
    /// Registers an identifier definition and returns false for a duplicate.
    /// 識別子の定義を登録し、重複している場合は false を返します。
    /// </summary>
    public bool RegisterDefinition(string id)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(id);
        if (definitions.Add(id))
        {
            return true;
        }

        duplicateDefinitions.Add(id);
        return false;
    }

    /// <summary>
    /// Registers an identifier reference for later integrity validation.
    /// 後で整合性を検証するために識別子参照を登録します。
    /// </summary>
    public void RegisterReference(string id)
    {
        ArgumentException.ThrowIfNullOrWhiteSpace(id);
        references.Add(id);
    }

    /// <summary>
    /// Returns true when an identifier definition is registered.
    /// 識別子の定義が登録済みの場合は true を返します。
    /// </summary>
    public bool ContainsDefinition(string id) => definitions.Contains(id);

    /// <summary>
    /// Removes all definitions, references, and duplicate history.
    /// すべての定義、参照、重複履歴を削除します。
    /// </summary>
    public void Clear()
    {
        definitions.Clear();
        duplicateDefinitions.Clear();
        references.Clear();
    }

    internal IEnumerable<Diagnostic> Validate()
    {
        foreach (var id in duplicateDefinitions.Distinct(StringComparer.Ordinal))
        {
            yield return new Diagnostic(DiagnosticSeverity.Error, "RECOTTE_CORE_DUPLICATE_ID", $"The identifier '{id}' is defined more than once.");
        }

        foreach (var id in references.Where(id => !definitions.Contains(id)))
        {
            yield return new Diagnostic(DiagnosticSeverity.Error, "RECOTTE_CORE_MISSING_REFERENCE", $"The identifier reference '{id}' has no matching definition.");
        }
    }
}
