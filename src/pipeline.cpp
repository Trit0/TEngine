//
// Created by Tristan Brault on 2023-11-23.
//

#include "pipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace te {
    Pipeline::Pipeline(const std::string&vertFilePath, const std::string&fragFilePath) {
        createGraphicsPipeline(vertFilePath, fragFilePath);
    }



    std::vector<char> Pipeline::readFile(const std::string&filePath) {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filePath);
        }
        const size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void Pipeline::createGraphicsPipeline(const std::string&vertFilePath, const std::string&fragFilePath) {
        auto vertCode = readFile(vertFilePath);
        auto fragCode = readFile(fragFilePath);

        printf("Vertex Shader Code Size: %lu\n", vertCode.size());
        printf("Fragment Shader Code Size: %lu\n", fragCode.size());
    }


}
