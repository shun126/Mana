## Chapter 3: Let's learn the basics
## How to write a program
Unfortunately, writing a program does not allow the use of Kanji codes for actor names. Please use the character set shown in the table.

### Table 2-1 Mana Character Set
- | Character
---------- | ---------- 
lower case | abcdefghijklmnopqrstuvwxyz
Upper case letters | ABCDEFGHIJKLMNOPQRSTUVWXYZ
Numbers | 0123456789
Symbols | ! @ # $ % ^ & | ( ) { } [ ] = < > , . + - / ? ' " : ;
Characters not displayed | spaces, tabs, newlines
	
Mana's program source (hereafter referred to as "source") can basically be written in free form. Except for special positions, it is possible to include spaces, tabs, or newlines, which are arbitrarily called whitespace characters. Thus, a line containing only line feeds is also possible. Annotations can be placed in the positions where whitespace can be inserted. Annotations are ignored by the compilation process. There are two examples of annotations, and the contents from / to / are annotations. Line breaks are also included in the annotation. In the other form, the annotation is from example symbol // to the end of the line. See Example 2-2 below.

### Example 2-2 Example of annotation description

```
/* annotation description */
// annotation description
```
## Types and Numbers
Data processed by computers can be broadly classified into constants and variables. A constant is something whose content does not change, while a variable is something whose content changes. They have types. Variables also have attributes such as valid range of reference and lifetime. Numbers can be classified as integers and real numbers, each of which has a different size type. Type conversion is required between different types.

## Constants
A constant is a value that does not change. Constants are very easy to define.

### Table 2-2 How to write constants

```
define PI 3.14;
define MyHeight 170;
define MyName "Shun";
```
Now PI is defined to be 3.14 and MyHeight is 170. Integer constants can be in hexadecimal or binary, in addition to the usual decimal numbers.

*Binary and hexadecimal numbers We usually use decimal numbers in our daily lives. Money is in decimal because it is carried forward by 10. A dozen is 12, so it is in decimal. A day is 24 hours, so it is in 24 decimal, and an hour is 60 minutes, so it is in 60 decimal. In other words, a decimal number is a way of counting numbers whose digits are carried forward by that number.

### Table 2-4 Binary, decimal, and hexadecimal numbers
Binary | Decimal | Hexadecimal
---------- | ---------- | ------ 
0b0_0000 | 0 | 0x00 
0b0_0001 | 1 | 0x01 
0b0_0010 | 2 | 0x02 
0b0_0011 | 3 | 0x03 
0b0_0100 | 4 | 0x04 
0b0_0101 | 5 | 0x05 
0b0_0110 | 6 | 0x06 
0b0_0111 | 7 | 0x07 
0b0_1000 | 8 | 0x08 
0b0_1001 | 9 | 0x09 
0b0_1010 | 10 | 0x0a 
0b0_1011 | 11 | 0x0b 
0b0_1100 | 12 | 0x0c 
0b0_1101 | 13 | 0x0d 
0b0_1110 | 14 | 0x0e 
0b0_1111 | 15 | 0x0f 
0b1_0000 | 16 | 0x10 

Binary numbers are prefixed with 0b, while hexadecimal numbers are prefixed with 0x. For example, 0b1100 and 0xc both point to 12.

## Types and Variables
Mana intentionally supports only primitive types. Primitive types are byte, short, bool, int, float, and string.

### Table 2-3 Data Types

Name | Byte Number | Range
---------- | ---------- | ------ 
byte | 1 | -128 to 127 
short | 2 | -32768 to 32767 
bool | 1 | true or false
int | 4 | -2147483648 to 2147483647 
float | 4 | real number 
string | 4 | string 

Variables must be declared or defined before they can be used. The declaration informs the compiler of the type and attributes of the variable. The definition allocates the variable space. A declaration may also serve as a definition. There are no restrictions on where declarations and definitions may be written, and they may be defined by reference. There are three types of variable declarations: global, actor, and local. global variables are stored in external memory, such as a memory card, and can be referenced by any actor in the game. actor variables are variables that can be shared within an actor. local variables can be referenced only within a block. Local variables are variables that can only be referenced within a block.

### Table 2-4: How to define variables

```
int global;
actor Girl
{
    int instance;

    action init
    {
        int local;

        instance = 12;
        global = 100;
    }

    action sleep
    {
        local = 1; // NG: local cannot be referenced because it is out of scope
        instance = 13; // OK
    }
}

actor Boy
{
    action init
    {
        global = 0; // OK
        instance = 0; // NG: local cannot be referenced because it is out of scope
    }
}
```
*Note: In Table 2-4, blocks are written in the declaration for clarity.

## Operators
Mana can use arithmetic, logical, and relational operators.

### Arithmetic Operators
Arithmetic operators are the four arithmetic operations such as addition, subtraction, and remainder.

- | Symbol
---------- | ----------
addition | +
Subtraction | -
multiplication | *
division | /
remainder | % % ## Logical operators

### Logical Operators
Logical operators are not familiar to most people, but there are four types: logical negation, logical conjunction, logical disjunction, and exclusive disjunction. The values of these operations shown in the table are not numbers, such as 100 or 256, but values such as true (value 1) or false (value 0).

- | Symbol
---------- | ----------
Logical negation | !
1's complement | ~
Logical conjunction | &
Logical disjunction | | | Exclusive disjunction | ^
exclusive disjunction | ^

### Calculating Logical Operators
Logical negation (not) reverses the logic. Logical conjunction (and) makes the result true if and only if both sides are true. logical disjunction (or) is true if either side is true or both sides are true. Exclusive disjunction (eor) is true if and only if either side is true.

Expression | Result
---------- | ----------
true | false
false | true
false & false | false
false & true | false
true & false | false
true & true | true
false | false | false | false
true | false | true | true
true | false | false | true
true | true | true
false ^ false | true
false ^ true | true
true ^ false | true
true ^ true | false

### Relational Operators
The relational operator compares two operators and returns a logical value.

- | symbol
---------- | ----------
true if they are equal | ==
true if they are not equal | ! = true if the left-hand side is smaller than the right-hand side
true if left side is less than right side | <
true if the left side is less than or equal to the right side | <=
true if the left side is greater than the right side | >
true if the left-hand side is greater than or equal to the right-hand side | >=

## Operator Precedence
The precedence order of operators is as follows

Operator | Meaning | Combining Rules
---------- | ---------- | ------ 
++, -- | postfix increment/decrement | 
++, -- | Precedence increment/decrement | 
@, & | Indirect (reference), address | 
+, - | positive sign, negative sign | ! 
! right sign | logical negation (NOT), bitwise logical negation (1's complement) | right 
sizeof(), (TYPE) | size, cast | right 
.      | reference | left 
*, /, % | multiply, divide, remainder | left 
+, - | addition, subtraction | left 
<<,>> | shift integer left, shift integer right | left 
<, >, <=, >= | less than, greater than, less than or equal to, greater than or equal to | Left 
Left ==, ! = | equal not equal | left 
&, "|", ^ | bitwise logical conjunction, bitwise exclusive disjunction, bitwise disjunction | left 
&&, "||" | logical conjunction (AND),logical disjunction (OR) | left 
? : | simple assignment, multiplication assignment, division assignment, remainder assignment, addition assignment, subtraction assignment, bitwise logical conjunction assignment, bitwise logical disjunction assignment, bitwise exclusive OR | left &&, /=, %? 
=, =, /=, %=, +=, -=, <<=, >>=, &=, |=, ^= | ternary operator | right 

## Execution statement

- | right ## Execution statement
---------- | ----------
Execution | 
Branching | if, if-eles or switch-case
Repetition #1 | while or for
Repetition 2 | do-while

[[Embed(2-1.png, caption=Figure 2-1 Program execution steps)]]

### if
An if statement, which specifies a statement to be processed when a conditional expression is satisfied, is written in the following format.

```
if(conditional expression)
    A statement that should be processed when the conditional expression is satisfied
```
However, to specify multiple statements, use a block.

```
if(conditional expression)
{
    A statement that should be processed when the conditional expression is satisfied.
    statement that should be processed when the conditional expression is satisfied.
}
```
To specify the execution of one of the statements according to the result of the evaluation of the conditional expression, use the following form with an else statement appended immediately after the conditional expression.

```
if(conditional expression)
    A statement to be processed when the conditional expression is satisfied.
else
    A statement that should be processed when the conditional expression fails
```
This format also uses blocks to specify multiple statements.

```
if(conditional expression)
{
　　Statements that should be processed when the conditional expression is satisfied.
}else{
　　statement to be processed if the conditional expression fails
}
```
### switch
The switch statement branches to the position of the case statement in which the integer constant expression is written and executes the statement from that position. If there is no matching case, the statement is executed from the default statement if there is a default statement. If there is no case that matches, the switch statement is terminated.

```
switch(formula)
{
　　case Integer constant expression:
　　　　A statement that should be processed when an expression and an integer constant expression match.
　　case Integer constant expression:
　　　　statement that should be processed when an expression and an integer constant expression match
　　default:
　　　　statement that should be processed when there is no match
}
```
There are some points to note. After branching, ignore case and default statements until a break statement is found.

```
switch(formula){
　　case Integer constant expression:
　　　　Statement to be processed when an expression and an integer constant expression match
　　　　break;
　　case integer constant expression:
　　case Integer constant expression:
　　　　statement to be processed when an expression and an integer constant expression match
　　　　break;
　　default:
　　　　statement to be processed when there is no match
　　　　break;
}
```
### while
The conditional expression is evaluated before the iterative part is executed, and if it does not match, the while statement is terminated. If the conditional expression is satisfied, the repeating part is processed and the conditional expression is evaluated again.

```
while(conditional expression)
　　A statement that should be processed when the condition is satisfied.

while(conditional expression)
{ { while(conditional expression)
　　Statement that should be processed when the condition is satisfied.
}
```
### do
The do statement evaluates a conditional expression in the iterative process go.

```
do {
　　Iteration
}while(conditional expression);
```
The difference between a do statement and a while statement is twofold: a while statement evaluates at the beginning of the loop, while a do statement evaluates at the end of the loop. The while statement is executed while the conditional expression is satisfied, while the do statement terminates when the conditional expression is satisfied.

### for
The for statement is used to perform repetitive processing.

```
for initialization expression ; conditional expression ; arithmetic expression)
　　A statement that repeatedly processes

for(initialization expression ; conditional expression ; arithmetic expression)
{
　　Statements to be iterated over
}
```
The initialization expression is a statement that is executed only once at the beginning. The conditional expression is evaluated before the loop is executed, and if it does not hold, the for statement is terminated. If it is satisfied, an arithmetic expression is executed at the end of the loop.

### break
A break statement exits the innermost do, for, while, or switch statement that contains this statement.

### continue
The continue statement skips the remaining statements in the innermost do, for or while statement containing this statement and moves on to the next operation.

### goto

```
actor Luyue
{
    action main
    {
        if(flag == TRUE)
            goto ABORT;
        setAnime(KICK);
        setMessage("I hate you!");
ABORT:
    }
}
```
### request
Requests an actor, including yourself. If the requestor has already executed or reserved a request of the same level, the request will be invalid.

request(Priority, ActorRef->ActionName);
Priority ... Priority
ActorRef ... Actor name
ActionName ... Action name

```
actor Mom
{
    action main
    {
        request(8, Kid->cleaning);
    }
}
actor Kid
{
    action cleaning
    {
        setAnimation(CLEANING);
    }
}
```
###return
Returns from the current action to a lower request tag within action; returns to the caller within Function.

```
void function(int count, string name)
{
    return;
}
int function_value(int count, string name)
{
    return 2;
}
actor Dad
{
    action talk
    {
        if(sender ! = Mom)
            return;
        setAnimation(SLEEPING);
    }
}
```
### rollback
Returns to actions below the specified priority.

```
actor Kid
{
    action main
    {
        request(5, Kid->sleep);
    }
    action sleep
    {
    }
    action work
    {
    }
    action talk
    {
        if(sender == Boss)
        {
            rollback 4; // cancel sleeping action
            request(5, Kid->work); // request working action
        }
    }
}
```
### join
Wait until the actor's priority is below the specified level.

```
actor Boss
{
    action order
    {
        request(1, Employee->doWork);
        join(1, Employee); // wait until the job is finished
    }
}
```
