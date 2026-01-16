# What is Mana?

There are two types of game scripts: highly functional scripts for programmers and simple scripts for planners. Lua and Squirrel are examples of the former, and Mana was developed as a script for the latter.

Mana was developed with a simple structure in mind, allowing the planner to control the movement and animation of the characters it controls, as well as messages. Therefore, Mana does not have modern features such as dynamic types and classes. The compiler and execution environment are separated, considering the use on low-memory platforms where it is impossible to install an interpreter. For this reason, we have enhanced the compiler to detect errors before execution as much as possible.

# Mana Features

Mana is a script developed for the purpose of efficient development of game events. Things in the world are always happening in parallel. For example, in a school classroom, there are many different things going on at the same time: the teacher lecturing, the students studying earnestly, the students looking away, the students talking in vain, the students speaking quickly, and so on.

However, the processing of a program is a step-by-step process. With conventional programming methods, this kind of parallel processing is difficult to achieve. Mana is a script that attempts to reproduce this in a pseudo-parallel manner.

Mana is a script that aims to perform pseudo-parallel processing and is composed of multiple units called actors. Actors, which is the literal translation of the word "actor" in Japanese, exist in the source files of a scene (situation). Each actor then reacts to each other's movements to create a scene.

# Advantages of Mana

I created an orthodox scripting system incorporating classes, but I thought scripting would be too complicated with threads and object management. I wanted a simple scripting system that allowed the planner to describe the direction, and I think Mana is suited to programmers who want similar conditions.

Mana is a system that runs on interrupts called requests. Complex processing can be left to the program side and only the directing part, such as timing and branching, can be left to the script.

# Mana for Beginners
* [Chapter 1: Getting Started with Mana](./primer_chapter_1-en)
* [Chapter 2: Program Structure](./primer_chapter_2-en)
* [Chapter 3: Let's learn the basics](./primer_chapter_3-en)
* [Chapter 4: Let's try using instructions](./primer_chapter_4-en)
* [Chapter 5: Let's use it even more!](./primer_chapter_5-en)
* [At the end](./primer_afterwords-en)
