#include "first_app.hpp"
#include "scene.hpp"


#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
    auto scene = te::Scene::fromJson("../scenes/sceneTS.json");
    te::FirstApp app = te::FirstApp(std::move(scene));

    try {
        app.run();
    }catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}