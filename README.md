# tetriX    {#mainpage}
_tetriX_ is a Tetris game written in C, meant to be runned on any Linux distribution or in a Raspberry Pi with a matrix display + joystick module attached.

<br>

## Building instructions
___

### Allegro

For building to run in Linux using Allegro, enter to your terminal inside the project directory and run

```console
bash -x nbproject/Package-Release_Allegro.bash
```

After that, a new file called tetrix.tar will be created. Uncompress it wherever you want and execute from the new "tetrix" directory
```console
bin/tetrix 
```

### Raspberry Pi

For building to run in a Raspberry Pi using the led Matrix, enter to your terminal inside the project directory and run

```console
bash -x nbproject/Package-Release_RaspberryPi.bash
```

After that, a new file called tetrix.tar will be created. Uncompress it wherever you want and execute from the new "tetrix" directory
```console
bin/tetrix 
```

<br>

## Release information
___
Release date: 2019-02-19

Current version: 0.0.1

<br>

## Source code organization
___

The project source code is located inside the `src` folder and it is clearly devided in `backend` and `frontend` related sources.
Also, the whole backend is tested using CUnit test framework and its tests are located in `tests` folder, which is in the project parent directory.

The source code documentation is generated with Doxygen using _tetriCs\_Doxyfile_ file inside `docs` folder. To read the documentation, open index.html in `docs/doxygen/html/index.html`

> Altough all tests are properly documented, that documentation is not located inside `docs` folder, but inside of each test.
> This is done to keep the documentation more clear and show the developer only the most important stuff.

<br>

## Sources
___

### The inspiration for the game rules and options was taken from
[Tetris Wiki: Tetris Guideline](https://tetris.wiki/Tetris_Guideline)
[Tetris Wiki: Nintendo Rotation System](https://tetris.wiki/Nintendo_Rotation_System)
[Tetris Wiki: Random Generator](https://tetris.wiki/Random_Generator)
[Tetris Wiki: Tetris (NES, Nintendo)](https://tetris.wiki/Tetris_(NES,_Nintendo))
[Nintendo (NES) Version of the Tetris](https://www.playemulator.com/nes-online/classic-tetris/)

### Some inspiration for ideas / pieces of code was taken from
[stackoverflow: How would one write object-oriented code in C?](https://stackoverflow.com/a/351745)
[C - Pointers and Two Dimensional Array: Address mapping](https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array)
[stackoverflow: How to use /dev/random or urandom in C?](https://stackoverflow.com/a/2572373)
[Linux Programmer's Manual: GETRANDOM](http://man7.org/linux/man-pages/man2/getrandom.2.html)
[Allegro Vivace: Input](https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace%3A-Input)
[stackoverflow: Creating a new directory in C](https://stackoverflow.com/a/7430262)
[stackoverflow: linux mkdir function can't authorize full permission](https://stackoverflow.com/a/8719916)

### Sound effects where taken from
[Blop Sound](http://soundbible.com/2067-Blop.html)
[Game Bleeps 3](https://freesound.org/people/korgchops/sounds/268694/)
[Game Over Arcade](https://freesound.org/people/myfox14/sounds/382310/)

<br>

## Authors
___
Gino Minnucci

<gminnucci@itba.edu.ar>

Martín E. Zahnd

<mzahnd@itba.edu.ar>

<br>

## Copyright
___

GNU General Public License v3
