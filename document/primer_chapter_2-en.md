## Chapter 2 Program Structure
## actor and action
In Mana, one Actor block corresponds one-to-one to one character in the game.
In other words, if there is no single Actor block in the source, there is no character in the game.
Within the Actor block, an Action block is placed to define the character's behavior by describing the character's actions.
Statements within an Action must always end with a ; or be enclosed in {}.
The part enclosed in {} is called a block and can be treated as a whole process.

## Who called me?
The local variable sender in the action block contains information about the called actor. Therefore, by using sender, you can change the action depending on who received the request.

### How to use sender

```
actor GoodLookingGuy
{
}

actor Girl
{
    action talk
    {
        switch(sender)
        {
        case GoodLookingGuy:
            setMessage("Hello, nice to meet you.");
            break;
    
        default:
            setMessage("...") ;
            break;
        }
    }
}
```

## Synchronous and Asynchronous
As mentioned previously, Mana is a language designed to facilitate control of quasi-parallel processing, but there may be situations where some actors do not want processing to be interrupted until a certain process is completed.

For example, an actor that manages the entire animation may lose synchronization with other actors if a frame is switched in the middle of the overall animation instructions.
Therefore, a block called "lock" is used to lock a period of time during which the actor's processing should not be switched without permission, and during this time, processing is not switched to another actor until a yield() instruction is issued.

### Example of creating a synchronous actor

```
actor Wife
{
    action talk
    {
        lock
        {
            Punch(Husband);
            Kick(Husband);
            Chop(Husband);
            yield();
        }
    }
}
```

## Actor Actions
Mana not only allows you to associate actors with characters, but also to associate them with space-defining objects to define actions that cannot be realized by characters.
For example, a box can be placed in a space, and various conditions, such as "the character passes through the box," can trigger an interrupt. Using this function, it is easy to create a situation where "a box is placed on the road, and when an actor enters the box, the actor-guard will be alerted.

## Functions
Functions can be used to externalize a process common to each actor that is independent of the ACTION. Statements within functions must likewise always end in ; or be enclosed in {}.
