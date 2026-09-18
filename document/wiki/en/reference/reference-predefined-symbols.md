# Predefined symbols

Mana has predefined symbols for referring to the context of the running Actor and Request.

## List

| Name | Type / kind | Meaning |
| --- | --- | --- |
| `self` | `actor` | The Actor currently running |
| `sender` | `actor` | The Actor that Requested the current Action |
| `priority` | `int` | The Priority of the Action currently running |
| `this` | The Struct receiver | A reserved word for referring to the current Struct instance in a Struct member Function |
| `Nil` | `Nil` | A special value that represents an empty reference |

## self

```mana
actor Worker
{
    action main
    {
        request(1, self->update);
    }

    action update
    {
    }
}
```

`self` represents the current Actor.

It can be used, for example, to send a Request to yourself from an Action or from a Function running on the Actor.

## sender

```mana
actor Receiver
{
    action receive
    {
        request(1, sender->reply);
    }
}
```

`sender` represents the Actor that Requested that Action.

However, for system Requests the VM itself sends at startup, such as `init` / `main`, there is no sending Actor. Don't assume that `sender` always points to a valid Actor.

## priority

```mana
actor Worker
{
    action work
    {
        print("%d\n", priority);
    }
}
```

`priority` gets the Priority of the Action currently running, as an `int`.

Use it when you want to check the Priority given in the Request, and which interrupt level the code is currently running at.

## this

`this` is a reserved word for referring to the current Struct instance in a Struct's member Function.

Inside the compiler it is resolved as the receiver identifier `this`. It is not for general self-reference to an Actor in ordinary global Functions or Actions. To refer to the Actor itself, use `self`.

## Nil

`Nil` is a special value that represents an empty reference.

```mana
Nil
```

**The leading `N` is upper case.** The current Lexer recognises `Nil` as a reserved word, and does not treat `nil` as the same token.

`Nil` has a type of its own, different from ordinary numeric constants, and cannot be used in constant expressions.

## true / false

`true` and `false` can also be used as Boolean literals.

```mana
bool visible = true;
bool finished = false;
```

These are literals of type `bool`.

## Related topics

- [Actor](./reference-actor.md)
- [Action](./reference-action.md)
- [Request](./reference-request.md)
- [Struct](./reference-struct.md)
