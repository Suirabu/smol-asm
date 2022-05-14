# SmolAsm

Smol is a retro-inspired fantasy game console.
This repository contains the source code for the Smol assembler,
an assembler which produces binaries using the [Smol CPU architecture](https://github.com/Suirabu/smol-emu/blob/06887793308d7268502e9fe4802ac34713d1956c/docs/cpu_arch.md).

See also
- [SmolEmu](https://github.com/Suirabu/smol-emu)
    The Smol emulator

## Quick Start

### Build SmolAsm

```
$ make
```

The resulting binary can then be found at `./bin/smol-asm`.

### Install SmolAsm

```
# make install
```

### Compile a binary

```
$ smol-asm <source path>
```

## Contributing

Bug fixes and patches are welcome and would be greatly appreciated!
I will not be accepting PRs containing feature additions or major changes to the functionality of the program at this time.


## License

SmolAsm is licensed under the MIT license ([see here](./LICENSE)).
