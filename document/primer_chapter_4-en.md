# Chapter 4 Using Instructions
## Loading external files
It is very inefficient to write instructions common to all maps, such as external instruction definitions, in each event script.
Therefore, each event script file can use the import instruction to read the common file and achieve definition sharing.
There is also a separate include instruction, but the import instruction checks for duplicate reads, while the include does not. Other than that, the operation is the same as that of import.

## Example 2-9 Defining shared files
common.mn
```
int ending_flag;
native void GameFlagOn(int a);
native void GameFlagOff(int a);
```
yongfu.mn
```
import "common.mn";

action Luyue::init
{
    GameFlagOn(ending_flag);
    GameFlagOff(ending_flag);
}
```
## Let's use Mana!
Under construction. Plans to write how to create save data, write constants that can be shared, etc.
