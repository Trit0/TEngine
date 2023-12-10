//
// Created by Tristan Brault on 2023-12-07.
//
#pragma once

#include "camera.hpp"

#include <vulkan/vulkan.h>

namespace te {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera& camera;
        VkDescriptorSet globalDescriptorSet;
    };
} // namespace te
