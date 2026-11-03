# Nintendo Switch / Horizon

This port builds Skia with devkitPro's devkitA64 and libnx. The native Graphite
Vulkan backend can use a Horizon NVK driver supplied by the application. Skia
does not create the window, Vulkan instance/device, or presentation surface.

Install the Switch portlibs (including FreeType, libpng, libjpeg-turbo, Expat,
and zlib), sync the pinned dependencies, then build:

```sh
python3 tools/git-sync-deps
export DEVKITPRO=/opt/devkitpro # or your complete SDK installation
tools/build-horizon.sh
```

The default output is `out/horizon-vulkan`; an optional first argument chooses
another output directory. `SKIA_BUILD_JOBS` controls parallelism. The output
contains `libskia.a`, `libskparagraph.a`, `libskshaper.a`, `libskunicode_core.a`,
`libskunicode_icu.a`, `libicu.a`, and `libskcms.a`. Consumers must use matching
headers, `SK_RELEASE`, and the same ABI configuration. Link libnx and the Switch
portlibs required by your application and selected codecs.

The build preserves libnx's software TLS convention (`-mtp=soft` and
`-ftls-model=local-exec`). SkSL's pool and the optional per-thread strike cache
use pthread keys. The Horizon semaphore implements counted signals. Serialized
Graphite pipeline keys can initialize known runtime-effect metadata before
the first draw, which is needed when replaying bundled shader caches.

The previous Dawn/OpenGL ES compatibility port is retained. Its GN/CMake bridge
applies `third_party/dawn/horizon.patch` and `abseil-horizon.patch` idempotently
to the exact revisions in `DEPS`. It is separate from the native Vulkan build
above. Applications using Dawn must supply their Horizon EGL surface and the
appropriate portlibs. The native build does not require those patches.
