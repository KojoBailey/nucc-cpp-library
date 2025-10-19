# [NUCC++](https://github.com/KojoBailey/nucc-cpp-library/)
**DISCLAIMER:** This documentation is a work in progress. Information may be inaccurate!

---

Welcome to the **[NUCC C++ Library](https://github.com/KojoBailey/nucc-cpp-library/)** for **modern C++**, C++20 and onwards.

Its purpose is to allow easy interaction with the [**XFBIN**](https://jojomodding.miraheze.org/wiki/XFBIN) container format (file sig: `NUCC` / `4E 55 43 43`), originating from [CyberConnect2](https://jojomodding.miraheze.org/wiki/CyberConnect2)'s in-house **NUCC** engine. 

<!-- ## Contributing
Although I intend to work on this library myself for the most part, largely for my own learning and development as a programmer, **help and feedback** is much appreciated - granted it's constructive!

More importantly though, the best way to contribute to this project by expanding its support of the various `nuccChunkBinary` formats for the various different games that use them. I'll release more info about this as the library's basic functionality nears completion. -->

## Design Goals
While there are XFBIN libraries out there for [Rust](https://github.com/SutandoTsukai181/xfbin-lib-rs), [Python](https://github.com/SutandoTsukai181/xfbin_lib), and [C#](https://github.com/TheLeonX/XFBIN_LIB), this is the first for **C++**.

This library aims for:
- **Ease of integration** → Everything is intended to be as abstracted as is useful, and although there are multiple headers you may need to include, they are also intentionally structured to be easy to organise. Most projects focusing on one game shouldn't need to include more than 2.
- **A myriad of features** → Across the CC2 NUCC games, there's a lot that happens with XFBINs behind the scenes, such as en/decryption, hashing, and more - all actions which this library will support.
- **Efficiency** → For a library intended to be used in bigger projects, the faster it can do its part, the better. As such, it should be as optimal as possible.
- **Flexibility** → This library should be able to achieve any (reasonable) task without driving users insane.

<!-- ## Features
Below is a list of all the things this library **can already** do...
- Load XFBIN files by path and deserialise them into an `XFBIN` class.
- Access deserialised NUCC data for `nuccChunkPage` and `nuccChunkBinary`.
- Serialise XFBIN files from scratch or otherwise.

... and here is a list of things **to-be-added** in the near future:
- More chunk support, eventually adding for them all.
- CRC32 hashing, used in the CC2 games for various things.
- Encryption and decryption.
- And much more... -->

## Usage
Everything in this library is under the `nucc` namespace.

### Loading from path
```cpp
#include <nucc/xfbin.hpp>

int main(int argc, char* argv[])
{
        nucc::xfbin xfbin{argv[1]};
}
```

### Loading from binary data
```cpp
#include <nucc/xfbin.cpp>

int main(int argc, char* argv[])
{
        kojo::binary data{argv[1]};
        nucc::xfbin xfbin{data, data.size()};
}
```