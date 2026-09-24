# 式

式は値を作る、参照する、計算する、または代入するための構文です。

## リテラル

```mana
10
1.5
true
"Hello"
Nil
```

## 変数参照

```mana
count
speed
mOpened
```

## 算術式

```mana
count + 1
speed * 2.0
(a + b) * c
```

## 比較式

```mana
count == 0
count != 0
count < 10
count >= 1
```

比較結果は条件式などで使用できます。

## 論理式

```mana
enabled && visible
ready || force
!finished
```

## 代入式

```mana
count = 10
count += 1
speed *= 2.0
```

代入先には、書き込み可能な変数やメンバーなどを指定します。

## 条件演算子

`?:` を使用できます。

```mana
int value = enabled ? 1 : 0;
```

## Function 呼び出し

```mana
calculate(10, 20)
```

Struct のメンバー Function は `.` で呼び出します。

```mana
value.reset()
```

## メンバー参照

Struct のメンバーには `.` でアクセスします。

```mana
position.x
```

## 配列要素

配列要素は `[]` で参照します。

```mana
values[index]
```

## Action 参照

Actor の Action は `->` で参照します。

```mana
Enemy->think
Game::AI::Enemy->think
```

Action 参照は `request`、`awaitStart`、`awaitCompletion` などで使用します。

```mana
request(10, Enemy->think);
```

`::` は namespace の名前修飾、`->` は Action 参照です。

## 定義済みの値

Mana には実行状況を参照するための定義済みシンボルがあります。

```text
priority
self
sender
this
Nil
```

それぞれの有効な位置と正確な意味は「定義済みシンボル」リファレンスで扱います。

## `sizeof`

`sizeof` 演算子が用意されています。

詳細な対象と結果は演算子リファレンスで扱います。

## 型チェック

式の演算や代入について、Compiler が型の整合性を確認します。

## 関連項目

- [型](./reference-types.md)
- [変数](./reference-variables.md)
- [演算子](./reference-operators.md)
- [文](./reference-statements.md)
