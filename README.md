# Minecraft Classic Clone in C

![](https://i.imgur.com/x6YY4Ic.png)

This was my first large-scale OpenGL project. I've personally never used OpenGL, and so I apologize for any bizzare/sloppy practices while using the library. However, I think I've done rather well for my first time using OpenGL, and I hope you have fun playing the game as much as I enjoyed making it.

This project was heavily inspired by the user [jdah](https://github.com/jdah), who did a similar project to me, except only in 48 hours. (He has more experience in OpenGL than me!)


### Compilation

  
I must preface this with a quick note:
> The whole game is roughly 338kB in size, and is compiled with the `-O2` flag. If you are having trouble compiling, or the game is compiling extremely slowly, then remove the `-O2` flag from the Makefile.
The game as of now also only supports Mac devices, however I may port it to Windows and Linux in the near future.
First, clone the repository with:
`$ git clone --recurse-submodules https://github.com/AxolotlC/Minecraft-Classic-in-C.git

In order to compile the game, you first have to clean up the submodules:
`$ sh ./setupworkspace.sh`  

Then, run the following command in your terminal (Mac, should work in Linux):

`$ make`

If you've made any changes, or `make` is returning a message similar to `make: 'bin/minecraft' is up to date.`, pleased compile with the `-B` option on `make`.

Before running this, make sure you are in the root directory of the project.



### Running

To run the game, execute the following command:

`$ make run`

#### Copyright Notice:

```
The OpenGL Extension Wrangler Library
Copyright (C) 2008-2016, Nigel Stewart <nigels[]users sourceforge net>
Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>
Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>
Copyright (C) 2002, Lev Povalahev
All rights reserved.
```