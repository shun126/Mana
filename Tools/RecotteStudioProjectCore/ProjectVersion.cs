namespace RecotteStudioProjectCore;

/// <summary>
/// Preserves the project format version without normalization.
/// プロジェクト形式のバージョンを正規化せず保持します。
/// </summary>
public sealed record ProjectVersion
{
    /// <summary>
    /// Gets an unknown project version.
    /// 不明なプロジェクトバージョンを取得します。
    /// </summary>
    public static ProjectVersion Unknown { get; } = new(null);

    /// <summary>
    /// Initializes a project version from its original value.
    /// 元の値からプロジェクトバージョンを初期化します。
    /// </summary>
    public ProjectVersion(string? rawValue)
    {
        RawValue = string.IsNullOrWhiteSpace(rawValue) ? null : rawValue;
    }

    /// <summary>
    /// Gets the original version value, or null when it was not detected.
    /// 元のバージョン値を取得します。検出できなかった場合は null です。
    /// </summary>
    public string? RawValue { get; }

    /// <summary>
    /// Gets whether a version value is available.
    /// バージョン値が存在するかどうかを取得します。
    /// </summary>
    public bool IsKnown => RawValue is not null;
}
