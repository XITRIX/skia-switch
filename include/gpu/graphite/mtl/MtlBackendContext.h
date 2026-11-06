/*
 * Copyright 2021 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skgpu_graphite_MtlBackendContext_DEFINED
#define skgpu_graphite_MtlBackendContext_DEFINED

#include "include/gpu/graphite/Context.h"
#include "include/ports/SkCFObject.h"
#include "include/private/SkAPI.h"

#import <CoreFoundation/CoreFoundation.h>
#include <memory>
#include <string_view>

class SkContext;
struct SkContextOptions;

namespace skgpu::graphite {

// Client-owned shader-library and binary-pipeline cache. Callbacks may run on
// compilation workers; implementations must be thread safe. CF return values
// carry one owning reference. The Context retains the cache for its lifetime.
class SK_API MtlPipelineCache {
public:
    virtual ~MtlPipelineCache() = default;
    virtual sk_cfp<CFTypeRef> loadLibrary(std::string_view msl) = 0;
    virtual bool allowsRuntimeCompilation() const { return true; }
    virtual void didCompileLibrary(std::string_view msl, CFTypeRef library) = 0;
    virtual sk_cfp<CFTypeRef> makeRenderPipeline(CFTypeRef device,
                                               CFTypeRef descriptor,
                                               sk_cfp<CFTypeRef>* error) = 0;
};

// Linking this symbol also detects stale prebuilt Skia archives.
SK_API int MetalPipelineCacheVersion();

// The MtlBackendContext contains all of the base Metal objects needed by the graphite Metal
// backend. The client will create this object and pass it into the Context::MakeMetal factory call
// when setting up Skia.
struct SK_API MtlBackendContext {
    sk_cfp<CFTypeRef> fDevice;
    sk_cfp<CFTypeRef> fQueue;
    std::shared_ptr<MtlPipelineCache> fPipelineCache;
};

namespace ContextFactory {
SK_API std::unique_ptr<Context> MakeMetal(const MtlBackendContext&, const ContextOptions&);
} // namespace ContextFactory

} // namespace skgpu::graphite

namespace SkContexts {

// Creates a context wrapping a Graphite GPU backend using Metal
std::unique_ptr<SkContext> MakeGraphite(const skgpu::graphite::MtlBackendContext& mtlContext,
                                        const SkContextOptions& contextOptions,
                                        const skgpu::graphite::ContextOptions& graphiteOptions);

}  // namespace SkContexts

#endif // skgpu_graphite_MtlBackendContext_DEFINED
