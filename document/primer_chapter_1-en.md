# Chapter 1 Getting Started with Mana
## What kind of language is Mana?
Mana is a script developed for the purpose of efficiently developing game events. Things in the world are always being done in parallel.
For example, in a school classroom, there are many different things going on at the same time: the teacher lecturing, the students seriously studying, the students looking away, the students talking in vain, the students speaking quickly, and so on.

However, the processing of a program is a step-by-step process. With conventional programming methods, this kind of parallel processing is difficult to achieve. Mana is a script that attempts to reproduce this in a pseudo-parallel manner. Mana is a script that aims to perform pseudo-parallel processing and is composed of multiple units called actors. Actors, which is the literal translation of the word "actor" in Japanese, exist in the source files of a scene (situation). Each actor then reacts to each other's movements to create a scene.

To put it more simply, the person writing the script is the director, the screenwriter, and the director in a movie. Each actor is an actor who follows the script's instructions. The actors follow the script and perform their lines and acts according to each other's lines and movements. A scene is created when all the actors perform their roles in parallel.
Mana is the script that makes this possible.

## Actors and Requests
A concept that symbolizes Mana is the "request. Actors command each other to move, and this allows for parallel movement and coordination between actors. For example, consider a situation in which you are riding a bus, and when the next stop is announced, several people get off the bus and try to press the button simultaneously.

First, the "bus" actor sends a request to all the "passenger" actors to "press the button if you want to get off at the next stop. The corresponding actor among the "passenger" actors receives the request and executes it. The corresponding "Passenger" actor will then execute the button press even if it is doing something else (e.g., reading a book, talking to someone else, looking out the window, etc.). In other words, the request to "press the button" takes precedence over the action the actor is performing at the time. This is "priority.

The actor is performing some other action unless otherwise commanded. This is the same as executing the request at the lowest level. The priority range now allows broader integer values, but the main action remains priority 0. Requests at priority 0 or below are treated as no-ops, and only priorities greater than 0 are executed. However, when a request comes in that should have a higher priority than it does, it will interrupt what it was doing and attempt to execute that request. Conversely, if a request of a lower level comes in while executing a request of a certain level, it waits until the request currently being performed is finished, without executing the new request that came in.

Figure 1-1 makes it easier to understand the behavior of requests. Requests with a higher priority than the level at which they are being executed are executed immediately, while requests with a lower priority are executed after they are finished.

[[Embed(1-1.png, caption=Figure 1-1 Priority vs. Request)]]

Figure 1-1 is my weekend. At Priority 1 I was napping, then at Priority 3 I had a desire to watch TV. I remembered that I had not washed the dishes, but since it was a Priority 2 request, I decided to do it after watching TV.
However, at Priority 4, my wife demanded that I clean the bathroom (and toilet).
I interrupt watching TV and start cleaning.
After the cleaning is finished, I return to the Priority 3 process of watching TV. Then, since the TV program has ended, I begin the Priority 2 dishwashing process, which I do not want to do.

When the action ends as described above, it returns to the action registered at a lower priority than the current one. It is also possible to cancel the previous process. If you consider the event of falling asleep at work, you may not want to go back to the action of falling asleep again after being warned by your boss. For this situation, Mana allows you to specify the priority to which the event will drop at the end of the action (rollback instruction).

## How to create an event in Mana
So, what is the format of the actual source code? Example 1-2 shows the Mana script notation.

### Example 1-2 My first Mana source

```
actor Mr_M
{
    action init
    {
        print("hello world\n");
    }
}
```
The part enclosed by {~} or the line up to ; is called a block.

The actor block is the part that corresponds to the actor introduced earlier. action block is the block that defines the action of the actor.
The name of the action block can be freely chosen, except for reserved names. By describing actions in the action block, you can define the actions of the character.

Now let's create an environment to actually move the character.
Many recent applications make full use of GUI (Graphical User Interface), such as opening a window and displaying text and pictures in it, but the author's situation is that "GUI depends on the OS and toolkit used, so it is difficult to create a generic one. Therefore, Mana humbly uses the console.
The console here refers to the command prompt (MS-DOS prompt) in the case of Windows; in the case of Linux and other Unix-like operating systems, it refers to applications called "terminal emulators" such as kterm and rxvt.

[[Embed(1-2.png, caption=Figure 1-2 Example screens of command prompt and terminal emulator)]]

If you are using Windows, follow these steps to start the console

From the "Start" button, click "Programs" -> "Accessories" -> "Command Prompt".
If you are using Unix, you are probably familiar with the console, so we will skip the explanation.

Let's get started. Enter the Mana program from Example 1-2 using an editor such as Notepad. After saving the file as test.mn, type `mana test.mn` in the DOS console for Windows or in the terminal for Linux to convert it into an executable, and the Mana executable will be saved in the current directory with the extension .mx (in this case This conversion process is called compilation. This conversion process is called compilation. The extension `.mn` indicates that it is a Mana program.

In an environment where keyboard macros can be executed, such as the Hidemaru editor, it is very easy to assign the compile operation to a key, so please consult your programmer.

Even if compilation is done correctly, it does not mean that the program is complete. The compiled program must be checked to make sure that it works as intended, just free of grammatical errors. This process is called debugging. If a problem is found during the debugging process, it is corrected in the editor, side-compiled, and the operation is checked. When it is confirmed that the program works as intended, the program is complete. The following is a diagram of the workflow.

[[Embed(1-3.png, caption=Figure 1-3 Program Development Flow)]]

Executing the mx file starts the game events, but this manual cannot explain how to execute it because the execution method differs for each game title. Please contact the programmer of each project.
