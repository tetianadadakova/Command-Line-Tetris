# Command line Tetris in C++

Mac OS version of the Javidx9's [original code](https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_Tetris.cpp) and a great [YouTube tutorial](https://www.youtube.com/watch?v=8OK8_tHeCIA&t=1806s).
In addition, I used [this pull request](https://github.com/OneLoneCoder/CommandLineFPS/pull/1) for the example use of ncurses library, specifically [the following code](https://github.com/rohanliston/CommandLineFPS/blob/linux-port/linux/CommandLineFPS.cpp).


![](media/Tetris_mov.gif)


## Build:

```
mkdir build
cd build
cmake ..
make
```


## Run
```
./Tetris
```

## To set up and run directly in Xcode

* Add `-lncurses` to Build Settings -> Other Linker Flags
* Add `_XOPEN_SOURCE_EXTENDED=500` to Preprocessor Macros
* Change Product -> Scheme -> Edit Scheme -> Options -> Use Terminal


## License
[GNU GPLv3](https://www.gnu.org/licenses/gpl-3.0.html)