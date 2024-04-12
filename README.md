# XFBIN++
This is a **single-include-header** C++ library for interaction with the [CyberConnect2](https://jojomodding.miraheze.org/wiki/CyberConnect2) [XFBIN](https://jojomodding.miraheze.org/wiki/XFBIN) file container format. That is, any file with the `.xfbin` file extension for use in CC2's in-house NUCC engine, hence the magic of XFBINs: `NUCC` / `4E 55 43 43`.

This library is nowhere near finished, but with time and energy, it should eventually rise to be a very powerful multi-purpose library. (It might even make more sense to rename this to **NUCC++** at some point).

## Design Goals
While there are XFBIN libraries out there for [Rust](https://github.com/SutandoTsukai181/xfbin-lib-rs), [Python](https://github.com/SutandoTsukai181/xfbin_lib), and [C#](https://github.com/TheLeonX/XFBIN_LIB), this is the first for **C++**.

Besides simply supporting C++ XFBIN projects however, this library also aims for:
- **Ease of integration** → As a simple single-include-header file, all you'll need to do is `#include` one file for access to everything. Although there are dependencies of other useful libraries, they too are easy to install, and should be downloaded alongside this library as well.
- **A myriad of features** → Across the CC2 NUCC games, there's a lot that happens with XFBINs behind the scenes, such as en/decryption, hashing, and more - actions which this library will support.
- **Efficient yet accurate memory** → The more unnecessary memory usage that can be avoided, the better, and so optimisation is key. Yet, the structure of XFBIN objects will still prove faithful to the originals - mostly for documentation sake.
- **Speed** → For a library intended to be used in bigger projects, the faster it can do its part, the better. Therefore, like with the goal of memory efficiency, this library is minimal in regards to overhead.
- **Ease of use** → It's not necessarily easy to have something be efficient *and* "easy" to use in C++, but that is a gap this library aims to bridge by minimising complicated syntax and excessive use of pointers on the user-end.
- **Flexibility** → This library should be able to achieve any (reasonable) task without driving users insane.
- **Moddability** → In true modding fashion, this library will aim to be "moddable", loading external plugins and scripts to assist wider gaps that are either too big to really feasibly "finish", or too niche to force upon all users.

## Features
Below is a list of all the things this library **can already** do...
- Load XFBIN files by path and deserialise them into an `XFBIN` object.
- Access deserialised chunk data for nuccChunkIndex and nuccChunkPage.

... and here is a list of things **to-be-added** in the near future:
- Serialisation - i.e. writing XFBIN files.
- More chunk support, eventually adding for them all.
- An external script/plugin loading system for the parsing of nuccChunkBinary data, since there are many sub-types of nuccChunkBinary that are uniquely structured.
- CRC32 hashing, used in the CC2 games for various things, including some XFBIN data.
- Encryption and decryption, once again used in some XFBIN files.
- And much more...
