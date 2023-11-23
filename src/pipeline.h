//
// Created by Tristan Brault on 2023-11-23.
//
#pragma once

#include <string>
#include <vector>

namespace te {
 class Pipeline {
 public:
  Pipeline(const std::string& vertFilePath, const std::string& fragFilePath);

 private:
  static std::vector<char> readFile(const std::string& filePath);
  void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);

 };
}
