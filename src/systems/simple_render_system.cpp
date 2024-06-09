//
// Created by Tristan Brault on 2023-11-22.
//

#include "simple_render_system.hpp"
#include "ecs_core/scene_manager.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/constants.hpp>

#include <stdexcept>

namespace te {
    extern SceneManager gSceneManager;

    struct SimplePushConstantData {
        glm::mat4 modelMatrix{1.0f};
        glm::mat4 normalMatrix{1.0f};
    };

    SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : device{device} {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() {
        vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
    }

    void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        renderSystemLayout =
                DescriptorSetLayout::Builder(device)
                        .addBinding(
                                0,
                                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT)
                        .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
                        .build();

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{
                globalSetLayout,
                renderSystemLayout->getDescriptorSetLayout()};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pipelineTextured = std::make_unique<Pipeline>(
            device,
            "../shaders/simple_shader_texture.vert.spv",
            "../shaders/simple_shader_texture.frag.spv",
            pipelineConfig
        );
        pipeline = std::make_unique<Pipeline>(
                device,
                "../shaders/simple_shader_color.vert.spv",
                "../shaders/simple_shader_color.frag.spv",
                pipelineConfig
        );
    }

    void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo, std::set<Entity>& entities, const std::unordered_map<Entity, VkDescriptorBufferInfo>& descriptors) {
        //pipeline->bind(frameInfo.commandBuffer);
        //pipelineTextured->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                                &frameInfo.globalDescriptorSet, 0, nullptr);

        int count = 0;
        for (auto const& entity : entities) {
            auto& model = gSceneManager.getComponent<std::shared_ptr<Model>>(entity);
            if (model == nullptr) continue;

            auto& transform = gSceneManager.getComponent<TransformComponent>(entity);

            auto hasTexture = gSceneManager.componentExist<std::shared_ptr<Texture>>(entity);

            count++;

            SimplePushConstantData push{};
            push.modelMatrix = transform.mat4();
            push.normalMatrix = transform.normalMatrix();

            // writing descriptor set each frame can slow performance
            // would be more efficient to implement some sort of caching
            auto bufferInfo = descriptors.at(entity);
            VkDescriptorSet gameObjectDescriptorSet;

            if (hasTexture) {
                pipelineTextured->bind(frameInfo.commandBuffer);
                auto& texture = gSceneManager.getComponent<std::shared_ptr<Texture>>(entity);
                auto imageInfo = texture->getImageInfo();
                DescriptorWriter(*renderSystemLayout, frameInfo.frameDescriptorPool)
                        .writeBuffer(0, &bufferInfo)
                        .writeImage(1, &imageInfo)
                        .build(gameObjectDescriptorSet);
            } else {
                pipeline->bind(frameInfo.commandBuffer);
                DescriptorWriter(*renderSystemLayout, frameInfo.frameDescriptorPool)
                        .writeBuffer(0, &bufferInfo)
                        .build(gameObjectDescriptorSet);
            }

            vkCmdBindDescriptorSets(
                    frameInfo.commandBuffer,
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipelineLayout,
                    1,  // starting set (0 is the globalDescriptorSet, 1 is the set specific to this system)
                    1,  // set count
                    &gameObjectDescriptorSet,
                    0,
                    nullptr);

            vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
                               sizeof(SimplePushConstantData), &push);
            model->bind(frameInfo.commandBuffer);
            model->draw(frameInfo.commandBuffer);
        }

        std::cout << count << " entities in render system" << std::endl;

//        for (auto& kv: frameInfo.gameObjects) {
//            auto& obj = kv.second;
//            if (obj.model == nullptr) continue;
//
//            SimplePushConstantData push{};
//            push.modelMatrix = obj.transform.mat4();
//            push.normalMatrix = obj.transform.normalMatrix();
//
//            vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout,
//                               VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
//                               sizeof(SimplePushConstantData), &push);
//            obj.model->bind(frameInfo.commandBuffer);
//            obj.model->draw(frameInfo.commandBuffer);
//        }
    }
}
