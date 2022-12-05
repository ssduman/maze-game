#pragma once

#include "../engine/IRunner.h"

#include "GUI.h"
#include "Camera.h"
#include "Control.h"
#include "Factory.h"

class RunnerEditor : public IRunner {
public:
    RunnerEditor() = default;

    ~RunnerEditor() override = default;

    void operator()() {
        WindowProperties windowProperties{ /*title*/ "Seri Game Engine - Editor", /*fullscreen*/ false, /*w*/ 1280, /*h*/ 720 };
        std::shared_ptr<WindowManager> windowManager = std::make_shared<WindowManager>(windowProperties);

        std::shared_ptr<State> state = std::make_shared<State>();
        state->gameState() = GameState::menu;

        CameraProperties cameraProperties{};
        cameraProperties.aspect = windowManager->getAspect();
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(cameraProperties, state.get());
        camera->init();

        Layer layers{};

        Control control{ windowManager, state, camera };
        control.init();

        GUI gui{ windowManager, camera, layers, state };
        gui.init();

        LOGGER(info, "starting seri game engine - editor loop");

        while (!glfwWindowShouldClose(windowManager->getWindow())) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            control.processInput(windowManager->updateDeltaTime());

            for (auto& entity : layers.getLayers()) {
                entity->display();
                gui.registerEntity(entity);
            }

            gui.display();

            glfwPollEvents();
            glfwSwapBuffers(windowManager->getWindow());
        }

        LOGGER(info, "seri game engine - editor loop stopped");
    }

};
