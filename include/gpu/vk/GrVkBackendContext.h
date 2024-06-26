/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrVkBackendContext_DEFINED
#define GrVkBackendContext_DEFINED

#include "include/core/SkRefCnt.h"
#include "include/gpu/vk/GrVkTypes.h"
#include "include/gpu/vk/VulkanMemoryAllocator.h"

namespace skgpu { class VulkanExtensions; }

enum GrVkExtensionFlags {
    kEXT_debug_report_GrVkExtensionFlag    = 0x0001,
    kNV_glsl_shader_GrVkExtensionFlag      = 0x0002,
    kKHR_surface_GrVkExtensionFlag         = 0x0004,
    kKHR_swapchain_GrVkExtensionFlag       = 0x0008,
    kKHR_win32_surface_GrVkExtensionFlag   = 0x0010,
    kKHR_android_surface_GrVkExtensionFlag = 0x0020,
    kKHR_xcb_surface_GrVkExtensionFlag     = 0x0040,
};

enum GrVkFeatureFlags {
    kGeometryShader_GrVkFeatureFlag    = 0x0001,
    kDualSrcBlend_GrVkFeatureFlag      = 0x0002,
    kSampleRateShading_GrVkFeatureFlag = 0x0004,
};

// It is not guaranteed VkPhysicalDeviceProperties2 will be in the client's header so we forward
// declare it here to be safe.
struct VkPhysicalDeviceFeatures2;

// The BackendContext contains all of the base Vulkan objects needed by the GrVkGpu. The assumption
// is that the client will set these up and pass them to the GrVkGpu constructor. The VkDevice
// created must support at least one graphics queue, which is passed in as well.
// The QueueFamilyIndex must match the family of the given queue. It is needed for CommandPool
// creation, and any GrBackendObjects handed to us (e.g., for wrapped textures) needs to be created
// in or transitioned to that family. The refs held by members of this struct must be released
// (either by deleting the struct or manually releasing the refs) before the underlying vulkan
// device and instance are destroyed.
struct SK_API GrVkBackendContext {
    VkInstance                          fInstance = VK_NULL_HANDLE;
    VkPhysicalDevice                    fPhysicalDevice = VK_NULL_HANDLE;
    VkDevice                            fDevice = VK_NULL_HANDLE;
    VkQueue                             fQueue = VK_NULL_HANDLE;
    uint32_t                            fGraphicsQueueIndex = 0;
    uint32_t                            fMinAPIVersion = 0; // Deprecated. Use fInstanceVersion
                                                            // instead.
    uint32_t                            fInstanceVersion = 0; // Deprecated. Use fMaxApiVersion
    // The max api version set here should match the value set in VkApplicationInfo::apiVersion when
    // then VkInstance was created.
    uint32_t                            fMaxAPIVersion = 0;
    uint32_t                            fExtensions = 0; // Deprecated. Use fVkExtensions instead.
    const skgpu::VulkanExtensions*      fVkExtensions = nullptr;
    uint32_t                            fFeatures = 0; // Deprecated. Use fDeviceFeatures[2]
                                                       // instead.
    // The client can create their VkDevice with either a VkPhysicalDeviceFeatures or
    // VkPhysicalDeviceFeatures2 struct, thus we have to support taking both. The
    // VkPhysicalDeviceFeatures2 struct is needed so we know if the client enabled any extension
    // specific features. If fDeviceFeatures2 is not null then we ignore fDeviceFeatures. If both
    // fDeviceFeatures and fDeviceFeatures2 are null we will assume no features are enabled.
    const VkPhysicalDeviceFeatures*     fDeviceFeatures = nullptr;
    const VkPhysicalDeviceFeatures2*    fDeviceFeatures2 = nullptr;
    sk_sp<skgpu::VulkanMemoryAllocator> fMemoryAllocator;
    skgpu::VulkanGetProc                fGetProc = nullptr;
    // This is deprecated and should be set to false. The client is responsible for managing the
    // lifetime of the VkInstance and VkDevice objects.
    bool                                fOwnsInstanceAndDevice = false;
    // Indicates that we are working with protected content and all CommandPool and Queue operations
    // should be done in a protected context.
    skgpu::Protected                    fProtectedContext = skgpu::Protected::kNo;
    // Optional callback which will be invoked if a VK_ERROR_DEVICE_LOST error code is received from
    // the driver. Debug information from the driver will be provided to the callback if the
    // VK_EXT_device_fault extension is supported and enabled (VkPhysicalDeviceFaultFeaturesEXT must
    // be in the pNext chain of VkDeviceCreateInfo).
    skgpu::VulkanDeviceLostContext      fDeviceLostContext = nullptr;
    skgpu::VulkanDeviceLostProc         fDeviceLostProc = nullptr;
};

#endif
