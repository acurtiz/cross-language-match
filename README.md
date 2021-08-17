# Cross Language Match

## What is this?

Cross Language Match is a language learning practice game. The user uploads a file containing word pairs (each word
defined in two languages), and then must match each word in one language with that word's translation in the other
language.

See **sample-word-pairs.txt** for an example file.

## What languages can I use?

At the moment, most if not all Romance languages.

Why not others? The current limitation is the font; a single TrueType font (TTF) gets loaded in to render the text, and
no single TTF exists that supports all existing languages. Such a TTF would have a relatively large data size and thus
it would not be practical even if it did exist. Thus, further work must happen to support handling multiple fonts.

## What was used to make it?

C++14 along with [SDL2](https://www.libsdl.org/) and [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) libraries,
with CMake being used to simplify building. This code is to be compiled with [Emscripten](https://emscripten.org/),
which is a C++ compiler which produces Javascript and WebAssembly (WASM).

## How do I use it?

1. [Build the project](#how-do-i-build-it) to produce three files: a Javascript file, a WASM file, and a data file.
1. Host those files on a web server, with the hosting page containing some small Javascript to properly glue everything
   together. See example HTML file which does this in **sample-web-page.html**.

## How do I build it?

1. Install [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) and follow any other setup
   instructions (such as setting up environment).
1. In the root of this project directory,
   run `rm -rf build && mkdir build && emcmake cmake -S . -B build && cmake --build build --parallel 8`. This will
   generate the three files (JS, WASM, and data) mentioned above, and you can then host them.

You can set up your IDE to utilize the Emscripten tools, so you can click a button instead of doing this command line
process. Generally this just entails setting a custom toolchain and compilation profile. I prefer using the terminal for
compilation, but proper IDE setup is also very much necessary to be able to find library headers.