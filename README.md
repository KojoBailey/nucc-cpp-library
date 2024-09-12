# [NUCC++](https://github.com/KojoBailey/nucc-cpp-library/)
Welcome to the **[NUCC C++ Library](https://github.com/KojoBailey/nucc-cpp-library/)**, a **multi-include** library of headers for **modern C++** (from **C++17**).

Its purpose is to allow easy interaction with data from [CyberConnect2](https://jojomodding.miraheze.org/wiki/CyberConnect2)'s in-house **NUCC** engine, that mostly being data found in the [**XFBIN**](https://jojomodding.miraheze.org/wiki/XFBIN) container format, which sports the file signature: `NUCC` / `4E 55 43 43`.

This library is nowhere near finished, and many design choices are **subject to change**, but otherwise, its internal functionality is more-or-less already figured out, and the library should be updated at least semi-frequently.

## Contributing
Although I intend to work on this library myself for the most part, largely for my own learning and development as a programmer, **help and feedback** is much appreciated - granted it's constructive!

More importantly though, the best way to contribute to this project by expanding its support of the various `nuccChunkBinary` formats for the various different games that use them. I'll release more info about this as the library's basic functionality nears completion.

## Design Goals
While there are XFBIN libraries out there for [Rust](https://github.com/SutandoTsukai181/xfbin-lib-rs), [Python](https://github.com/SutandoTsukai181/xfbin_lib), and [C#](https://github.com/TheLeonX/XFBIN_LIB), this is the first for **C++**.

This library aims for:
- **Ease of integration** → Everything is intended to be as abstracted as is useful, and although there are multiple headers you may need to include, they are also intentionally structured to be easy to organise. Most projects focusing on one game shouldn't need to include more than 2.
- **A myriad of features** → Across the CC2 NUCC games, there's a lot that happens with XFBINs behind the scenes, such as en/decryption, hashing, and more - all actions which this library will support.
- **Efficiency** → For a library intended to be used in bigger projects, the faster it can do its part, the better. As such, it should be as optimal as possible.
- **Flexibility** → This library should be able to achieve any (reasonable) task without driving users insane.

## Features
Below is a list of all the things this library **can already** do...
- Load XFBIN files by path and deserialise them into an `XFBIN` class.
- Access deserialised NUCC data for `nuccChunkPage` and `nuccChunkBinary`.
- Serialise XFBIN files from scratch or otherwise.

... and here is a list of things **to-be-added** in the near future:
- More chunk support, eventually adding for them all.
- CRC32 hashing, used in the CC2 games for various things.
- Encryption and decryption.
- And much more...

## How To Use
<sub>The docs for this library are incredibly lacking for the timebeing, and will be updated once I'm more confident in the design.</sub>

Everything in the library is under the `nucc` namespace.

To access specific chunk information from an `XFBIN` object, you must first initialise an object depending on what chunk type you need.
```cpp
#include "include/nucc/xfbin.hpp" // Including the library

int main() {
  nucc::XFBIN xfbin{"./PlayerColorParam.bin.xfbin"};
  nucc::Binary binary_chunk{ xfbin.fetch("PlayerColorParam") };
}
```

In the case of `Binary` objects, there is a vast library of the different formats they can contain.

```cpp
#include "include/nucc/xfbin.hpp"
#include "include/nucc/chunks/binary/asbr.hpp" // JoJo's Bizarre Adventure: All-Star Battle R formats

#include <iostream>

int main() {
  nucc::XFBIN xfbin{"./PlayerColorParam.bin.xfbin"};
  nucc::Binary binary_chunk{ xfbin.fetch("PlayerColorParam") };
  nucc::ASBR::PlayerColorParam pcp{ binary_chunk.data() };
  std::cout << pcp.entries[0].character_id; // "1jnt01"
}
```

## Examples
A lot of us learn best from examples, so here are some common (and not so common) tasks you may want to do with this library.

### Drag-and-Drop Loading
This can take an XFBIN file dragged onto the application and immediately parse it into an `XFBIN` object.
```cpp
int main(int argc, char* argv[]) {
  nucc::XFBIN xfbin{argv[1]};
}
```