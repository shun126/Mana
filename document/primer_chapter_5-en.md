# Chapter 5: Let's use it even more!
This chapter explains how to incorporate Mana extension instructions into your game program.

## Modules and Mix-in
Incomplete as it is being implemented. Debugging in progress.

```
module TestModule
{
    action sleep
    {
        wait(1.0);
    }
}
actor NiceGuy
{
    extend TestModule;
}
```
## actor and phantom
When an actor is defined, the actor exists. However, when Mana is used for unspecified AI such as enemy thoughts in battle, it is not realistic to define an actor for each number of enemies. Therefore, it is necessary to have a concept that defines actions but duplicates the entities at runtime. In Mana, this is called a phantom. The request for the replicated phantom has not been resolved.

```
phantom TestPhantom.
{
    action dead;
}
```
## actor inheritance
''Specification under consideration''

```
actor ChildActor : ParentActor
{
    action dead;
}
```
