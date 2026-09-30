# Mana quick start

This archive contains the Mana command-line executable and a sample program.
No compiler toolchain is needed to run the sample.

## Windows (PowerShell)

Extract the ZIP archive, open PowerShell in the extracted `mana-VERSION-windows-ARCH`
directory, and run:

```powershell
.\mana.exe --version
.\mana.exe .\examples\03-request.mn
```

## Ubuntu

Extract the archive and run these commands from its parent directory:

```sh
tar -xzf mana-VERSION-ubuntu-x64.tar.gz
cd mana-VERSION-ubuntu-x64
./mana --version
./mana examples/03-request.mn
```

## macOS

Choose the `macos-arm64` archive for Apple Silicon or `macos-x64` for an
Intel Mac. Extract it and run these commands from its parent directory:

```sh
tar -xzf mana-VERSION-macos-ARCH.tar.gz
cd mana-VERSION-macos-ARCH
./mana --version
./mana examples/03-request.mn
```

Replace `VERSION` and `ARCH` with the values in the downloaded filename.

The macOS executable is not Developer ID signed or notarized. If macOS blocks
its first launch, see [Apple's instructions for opening software from an
unidentified developer](https://support.apple.com/en-us/guide/mac-help/mh40616/mac).

The sample prints `Event: Request.` followed by `Guide: Welcome!`.

Run `.\mana.exe --help` on Windows or `./mana --help` on Ubuntu/macOS to see the
command-line options.
For language examples and documentation, see the
[Mana repository](https://github.com/shun126/Mana).
