# macOS (x86_64), for an AMD RDNA4 card under AMDRadeonRDNA4's RADV

Not the fork's supported route (`./omw` builds on Linux and Windows): a port for one Tahoe machine.

1. The pinned vcpkg registry (`tools/omw/pins.py` VCPKG_REGISTRY) in `out/mac/vcpkg`, with vcpkg-tool's
   `vcpkg-macos` as `out/mac/vcpkg/vcpkg`; `brew install nasm` (ffmpeg's assembler).
2. `vcpkg.json` (the fork's without clang-tidy, Qt and the Linux-only ports) in `out/mac/manifest`, beside a
   `vcpkg-configuration.json` naming `vcpkg/ports` and this folder as overlays; then, there:
   `VCPKG_ROOT=../vcpkg VCPKG_FORCE_SYSTEM_BINARIES=1 ../vcpkg/vcpkg install --triplet x64-osx-openmw
   --host-triplet arm64-osx --x-install-root=../installed`.
3. `env/macos/configure.sh` (moved to `out/mac/`), then `cmake --build out/mac/build --target openmw openmw-iniimporter`:
   Homebrew's Clang 23 against the SDK's libc++, macOS 26, no Qt programs, no crash catcher.
4. Run with `DYLD_LIBRARY_PATH` naming a folder holding the Khronos loader (`libvulkan.1.dylib`) and
   `VK_ICD_FILENAMES` naming the RADV ICD manifest; `openmw-iniimporter` writes `~/.config/openmw/openmw.cfg`
   as `./omw release setup` does.
