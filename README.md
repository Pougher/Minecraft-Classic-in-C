# Minecraft Classic Clone in C

![](https://i.imgur.com/x6YY4Ic.png)

This was my first large-scale OpenGL project. I've personally never used OpenGL, and so I apologize for any bizzare/sloppy practices while using the library. However, I think I've done rather well for my first time using OpenGL, and I hope you have fun playing the game as much as I enjoyed making it.

  

### Compilation

  
I must preface this with a quick note:
> The whole game is roughly 338kB in size, and is compiled with the `-O2` flag. If you are having trouble compiling, or the game is compiling extremely slowly, then remove the `-O2` flag from the Makefile.

> The game as of now also only supports Mac devices, however I may port it to Windows and Linux in the near future.

  

In order to compile the game, run the following command in your terminal (Mac, should work in Linux):

`$ make`

If you've made any changes, or `make` is returning a message similar to `make: 'bin/minecraft' is up to date.`, pleased compile with the `-B` option on `make`.

Before running this, make sure you are in the root directory of the project.

  

### Running

In order to run the game, the following command must be used:

`$ make run`