#pragma once

#include "Game.h"
#include "Maze.h"
#include "IRunner.h"
#include "CameraMaze.h"
#include "ControlMaze.h"

class RunnerMaze : public IRunner {
public:
    RunnerMaze() {}

    virtual ~RunnerMaze() {}

    void operator()() {
        WindowProperties windowProperties{ /*title*/ "Maze", /*fullscreen*/ true };
        WindowManager windowManager{ windowProperties };
        windowManager.disableCursor();

        float mazeWidth{ 20.0f }, mazeHeight{ 20.0f }, mazeThickness{ 5.0f };
        Maze* maze = new Maze{ mazeWidth, mazeHeight, mazeThickness };

        CameraProperties cameraProperties;
        cameraProperties.position = glm::vec3{ 0, -mazeThickness * 5, -mazeThickness * 4 - mazeHeight * mazeThickness };
        std::unique_ptr<CameraMaze> camera = std::make_unique<CameraMaze>(cameraProperties);
        camera->setMazeDimensions(mazeWidth, mazeHeight, mazeThickness);
        camera->setMazeWallPositions(maze->getVerticalWallPosition(), maze->getHorizontalWallPosition());

        Light light;
        Skybox skybox;
        ControlMaze control{ &windowManager, camera.get(), &maze};
        Game game{ static_cast<float>(windowManager.getWidth()), static_cast<float>(windowManager.getHeight()) };

        LOGGER(info, "starting maze game loop");

        while (!glfwWindowShouldClose(windowManager.getWindow())) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera->handleInput(windowManager.getWindow());
            camera->update();

            const glm::mat4& view = camera->getView();
            const glm::mat4& projection = camera->getProjection();

            maze->display(camera->showEscapePath());

            skybox.display(view, projection);

            light.setPosition(cameraProperties.position + glm::vec3{ 0.0f, -10.0f, 20.0f });
            light.setViewProjection(view, projection);
            light.light();

            game.display(control.getUserInput(), camera->getIsPlaying(), camera->isRestartTriggered(), camera->checkWin());

            glfwPollEvents();
            glfwSwapBuffers(windowManager.getWindow());
        }

        LOGGER(info, "seri game loop stopped");
    }

private:

};
