//
// Created by Trist on 2024-05-11.
//

#pragma once

#include "../system.hpp"
#include "frame_info.hpp"
#include "systems/simple_render_system.hpp"

#include <numeric>

namespace te {
//TODO optimize that shit
    class RenderSystemTextured : public System {
    public:
        void render(FrameInfo & frameInfo, SimpleRenderSystem& simpleRenderSystem) {
            simpleRenderSystem.renderGameObjects(frameInfo, mEntities, map(frameInfo));
        }

        void init(Device& device) {
            int alignment = std::lcm(
                    device.properties.limits.nonCoherentAtomSize,
                    device.properties.limits.minUniformBufferOffsetAlignment);
            for (int i = 0; i < uboBuffers.size(); i++) {
                uboBuffers[i] = std::make_unique<Buffer>(
                        device,
                        sizeof(GameObjectBufferData),
                        MAX_ENTITIES,
                        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                        alignment);
                uboBuffers[i]->map();
            }
        }


        void updateBuffers(FrameInfo & frameInfo) {
            int frameIndex = frameInfo.frameIndex;
            // copy model matrix and normal matrix for each gameObj into
            // buffer for this frame
            for (auto& entity: mEntities) {
                auto& transform = gSceneManager.getComponent<TransformComponent>(entity);
                GameObjectBufferData data{};
                data.modelMatrix = transform.mat4();
                data.normalMatrix = transform.normalMatrix();
                uboBuffers[frameIndex]->writeToIndex(&data, entity);
            }
            uboBuffers[frameIndex]->flush();
        }

        std::unordered_map<Entity, VkDescriptorBufferInfo> map(FrameInfo& frameInfo) {
            int frameIndex = frameInfo.frameIndex;
            std::unordered_map<Entity, VkDescriptorBufferInfo> myMap{};
            myMap.reserve(mEntities.size());
            for (auto& entity: mEntities) {
                myMap.insert({entity, getBufferInfoForEntity(frameIndex, entity)});
            }
            return myMap;
        }

        VkDescriptorBufferInfo getBufferInfoForEntity(int frameIndex, Entity entity) const {
            return uboBuffers[frameIndex]->descriptorInfoForIndex(entity);
        }

    private:
        std::vector<std::unique_ptr<Buffer>> uboBuffers{SwapChain::MAX_FRAMES_IN_FLIGHT};
    };

} // te
