# [NUCC++](https://github.com/KojoBailey/nucc-cpp-library/)
This is a **single-include** header **C++** library for interacting with data from [CyberConnect2](https://jojomodding.miraheze.org/wiki/CyberConnect2)'s in-house **NUCC** engine. This mostly includes data found in the [**XFBIN**](https://jojomodding.miraheze.org/wiki/XFBIN) container format, which have the magic: `NUCC` / `4E 55 43 43`.

This library is nowhere near finished, but with time and energy, it should eventually rise to be a very powerful multi-purpose library.

## Design Goals
While there are XFBIN libraries out there for [Rust](https://github.com/SutandoTsukai181/xfbin-lib-rs), [Python](https://github.com/SutandoTsukai181/xfbin_lib), and [C#](https://github.com/TheLeonX/XFBIN_LIB), this is the first for **C++**.

Besides simply supporting C++ NUCC projects however, this library also aims for:
- **Ease of integration** → As a simple single-include-header file, all you'll need to do is `#include` one file for access to everything. Although there are dependencies of other useful libraries, they too are easy to install, and should be downloaded alongside this library as well.
- **A myriad of features** → Across the CC2 NUCC games, there's a lot that happens with XFBINs behind the scenes, such as en/decryption, hashing, and more - actions which this library will support.
- **Efficient yet accurate memory** → The more unnecessary memory usage that can be avoided, the better, and so optimisation is key. Yet, the structure of XFBIN objects will still prove faithful to the originals - mostly for documentation sake.
- **Speed** → For a library intended to be used in bigger projects, the faster it can do its part, the better. Therefore, like with the goal of memory efficiency, this library is minimal in regards to overhead.
- **Ease of use** → It's not necessarily easy to have something be efficient *and* "easy" to use in C++, but that is a gap this library aims to bridge by minimising complicated syntax and excessive use of pointers on the user-end.
- **Flexibility** → This library should be able to achieve any (reasonable) task without driving users insane.
- **Moddability** → In true modding fashion, this library will aim to be "moddable", loading external plugins and scripts to assist wider gaps that are either too big to really feasibly "finish", or too niche to force upon all users.

## Features
Below is a list of all the things this library **can already** do...
- Load XFBIN files by path and deserialise them into an `XFBIN` class.
- Access deserialised NUCC data for nuccIndex and nuccPage.

... and here is a list of things **to-be-added** in the near future:
- Serialisation - i.e. writing XFBIN files.
- More chunk support, eventually adding for them all.
- An external script/plugin loading system for the parsing of nuccBinary data, since there are many sub-types of nuccBinary that are uniquely structured.
- CRC32 hashing, used in the CC2 games for various things.
- Encryption and decryption.
- And much more...

## How To Use
With this library, rather than all the difference `nuccChunk` types being parsed when a XFBIN file is loaded into the class, you must create different `nucc-Type` objects yourself, and initialise them with the `nuccChunk` you want.
```cpp
// Creates an `nuccBinary` object using the 3rd `nuccChunk` stored in the `xfbin` object.
nuccBinary binary_chunk{ xfbin.chunks[2] };
```

However, you may not initialise an `nuccChunk-Type` object with a `nuccChunk` of a different type.
```cpp
// Checks if type matches nuccBinary first.
if ( xfbin.chunks[2].type == nuccChunkType::Binary )
  nuccBinary binary_chunk{ xfbin.chunks[2] };
```

If you do try to do otherwise, the object will throw a runtime error, which may be useful for debugging purposes:
```
terminate called after throwing an instance of 'std::runtime_error'
  what():  Cannot initialise nuccBinary with non-nuccBinary data.
```

## Examples
A lot of us learn best from examples, so here are some common (and not so common) tasks you may want to do with this library.

### Drag-and-Drop Loading
This can take an XFBIN file dragged onto the application and immediately parse it into an `XFBIN` object.
```cpp
using kojo::XFBIN;

int main(int argc, char* argv[]) {
    XFBIN xfbin;
    xfbin.load(argv[1]);
}
```

### Iterating through Chunks
This iterates through all of an XFBIN's chunks, looking for nuccPages and outputting their `map_offset` value to the console if found.
```cpp
for (auto& chunk : xfbin.chunks) {
    if (chunk.type == kojo::nuccChunkType::Page) {
        kojo::nuccPage page{&chunk};
        std::cout << page.map_offset << "\n";
    }
}
```
