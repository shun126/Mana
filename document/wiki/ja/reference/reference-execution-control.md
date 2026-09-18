# 実行制御

このページでは、Action の待機・中断・再開や Request の受付状態を制御する文をまとめます。

## `yield()`

```mana
yield();
```

現在の Action を終了せず、その時点の実行をいったん中断して Mana VM に制御を返します。

次に実行機会が来たとき、`yield()` の後から再開します。

```mana
print("step 1\n");
yield();
print("step 2\n");
```

`yield()` は時間指定の待機ではありません。「1フレーム待つ」「1秒待つ」とは保証されず、再実行のタイミングはホスト側が VM をどのように更新するかにも依存します。

## `join`

```mana
join(priority, actorExpression);
```

新しい Request は送らず、既に動作している対象 Actor を待ちます。

```mana
join(0, NPC);
```

現行VMでは、対象 Actor の現在 Priority が指定 Priority 以下になるまで待機します。

`awaitCompletion` が「Request を送ってその完了を待つ」のに対し、`join` は「既存の実行状態を待つ」ための命令です。

## `rollback`

```mana
rollback priorityExpression;
```

例:

```mana
rollback 1;
```

`rollback` は現在の Action 実行を終了し、Actor に保存されている Priority の実行状態を指定値の方向へ巻き戻します。

現行VMの `Actor::Rollback` は、現在の Priority を解放し、必要に応じて指定値より上側に残っている実行状態を取り除いた後、再開可能な保存済み Action を復元します。再開できる Action がなければ Actor は停止状態へ戻ります。

通常の Action 終了時にも VM 内部では同じ Rollback 機構が使われています。

`rollback` は制御フローへの影響が大きいため、通常の順次処理よりも Priority の実行状態を明示的に戻したい場合に使用します。

## `halt()`

```mana
halt();
```

現在の Actor の実行を停止します。

現行VMでは Actor を halt 状態にし、保持している割り込み実行状態をクリアします。halt 状態の Actor に対する新しい Request は `Actor::Request` で受理されません。

VM 側で Actor を Restart すると halt 状態は解除されます。

## `refuse()`

```mana
refuse();
```

Actor を Request 拒否状態にします。

現行VMでは Actor の `Refused` フラグを立て、それ以降に届く新しい Request を受理しなくします。

既に登録済みの実行状態をクリアする命令ではありません。

## `comply()`

```mana
comply();
```

`refuse()` による Request 拒否状態を解除します。

現行VMでは `Refused` フラグをクリアします。

```mana
refuse();
// 新しい Request を拒否する区間
comply();
```

## `lock`

構文:

```mana
lock statement
```

通常はブロックと組み合わせます。

```mana
lock
{
    // statements
}
```

現行コンパイラは `lock` の開始時に `NonPreEmptive`、終了時に `PreEmptive` 命令を生成します。現行VMでは、これらは現在の Priority 実行状態にある `Synchronized` フラグを ON / OFF します。

重要な点として、現在の `Actor::Request` 実装はこの `Synchronized` フラグを Request の受付判定や Priority の割り込み判定には直接参照していません。

そのため、現行実装の `lock` を C++ の mutex や「必ず割り込まれない atomic 区間」と同じ意味として扱わないでください。リファレンス上は、**現在の実装では同期実行フラグを切り替える構文**として扱います。

## `request` / `awaitStart` / `awaitCompletion`

これらは Action の実行要求と待機を組み合わせた命令です。

```mana
request(10, NPC->talk);
awaitStart(10, NPC->talk);
awaitCompletion(10, NPC->talk);
```

詳細は [Request](./reference-request.md) を参照してください。

## Priority の取得

現在実行中の Priority は定義済み値 `priority` で取得できます。

```mana
print("%d\n", priority);
```

## 関連項目

- [Action](./reference-action.md)
- [Request](./reference-request.md)
- [Actor](./reference-actor.md)
- [Mana の実行モデル](../concepts/concept-execution-model.md)
