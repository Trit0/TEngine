#include "../../TEngine/src/first_app.hpp"


#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    auto scene = te::Scene::fromJson("assets/scenes/sceneTS.json");
    te::FirstApp app = te::FirstApp(std::move(scene));

    try {
        app.run();
    }catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}