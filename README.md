# Rainbow Joe
## About
Rainbow Joe is a classic 2D platformer game in the spirit of the 1990s written
in C using SDL2 and the
[TMX Map Format](http://doc.mapeditor.org/en/stable/reference/tmx-map-format/)
(Tile Map XML).  It is also a constantly evolving project I work on to exercise
C programming and to learn about the basic concepts of game development.

Last but not least: follow-on project of
[Janitor Jamboree](https://github.com/mupfelofen-de/janitor-jamboree) which I
have discarded.

### Trivia
The quite odd game title was generated using
[The Video Game Name Generator](https://www.videogamena.me/).

## Why C?

A lot of people ask me why I wrote this game in C instead of a modern language
like Rust, Haskell or `[insert your preferred language here]`, because writing
games in C is neither a easy thing to do nor very common (these days).

The main reason is obvious and I already mentioned it in the introduction: to
get some exercise in C programming.  And writing a game in C seemed like a fun
way to accomplish this.

Nevertheless: I hope you find the code of this project useful.  If so, feel free
to use it in any way you want. Just consider buying me a beer in case we meet
someday.

## Dependencies and how to compile
The program has been successfully compiled with the following libraries:
```
libxml2    2.9.8
sdl2       2.0.8
sdl2_image 2.0.3
sdl2_mixer 2.0.2
zlib       1.2.11
```

To compile _Rainbow Joe_ simply enter:
```
make
```

To generate the documentation using doxygen enter:
```
doxygen
```

## Controls

```
A:      move left
D:      move right
LSHIFT: hold to run
F:      toggle free camera mode
UP:     move camera up
DOWN:   move camera down
LEFT:   move camera left
RIGHT:  move camera right
Q:      quit
```

## License
This project is licensed under the "THE BEER-WARE LICENCE".  See the file
[LICENCE.md](LICENCE.md) for details.

[TMX C Loader](https://github.com/baylej/tmx/) by Bayle Jonathan is licensed
under a BSD 2-Clause "Simplified" License.  See the file
[COPYING](src/tmx/COPYING) for details.

All graphic, sound & music resources are dedicated to
[public domain](https://creativecommons.org/publicdomain/zero/1.0/) and have
been found on [OpenGameArt.org](https://opengameart.org/).
