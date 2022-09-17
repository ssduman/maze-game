#pragma once

#include "../engine/Typer.h"
#include "../engine/Entity.h"

#include "Camera.h"
#include "Control.h"
#include "Stopwatch.h"

class Game : public Entity {
public:
    Game(ICamera* camera, Control& control, float width, float height) : Entity(camera), _control(control), _width(width), _height(height) {
        Game::init();
        Game::initShader();
        initTyper();
        Entity::initTexture("assets/textures/gameWindow.png");
        initStopwatch();
        setProjection();

        _cameraMaze = dynamic_cast<Camera*>(_camera);

        LOGGER(info, "game init succeeded");
    }

    ~Game() override {
        delete _typer;
        delete _stopwatch;
    }

    void display() override {
        if (_cameraMaze->getIsPlaying()) {
            _stopwatch->run(_cameraMaze->isRestartTriggered(), _cameraMaze->checkWin());
            return;
        }

        render();

        float x = _width / 3.0f + 45.0f;
        float y = _height / 2.0f - 45.0f;
        _stopwatch->renderText(_control.getUserInput(), x + 8.0f * 46.5f, y, glm::vec3{ 0.3f, 0.8f, 0.9f }, 0.5f);
        _stopwatch->renderText("Enter maze size: __________", x, y, glm::vec3{ 0.5f, 0.8f, 0.2f }, 0.5f);
    }

private:
    void init() override {
        _vertices = {
            1460.0f, 15.0f, 1.0f, 1.0f, // top right
            460.0f, 15.0f, 0.0f, 1.0f, // top left
            460.0f, 1015.0f, 0.0f, 0.0f, // bottom left

            1460.0f, 15.0f, 1.0f, 1.0f, // top right
            1460.0f, 1015.0f, 1.0f, 0.0f, // bottom right
            460.0f, 1015.0f, 0.0f, 0.0f, // bottom left
        };

        auto w = _width, h = _height;
        _vertices[0] = _vertices[12] = _vertices[16] = w - w / 6.0f;
        _vertices[9] = _vertices[17] = _vertices[21] = h - h / (6.0f * w / h);
        _vertices[1] = _vertices[5] = _vertices[13] = h / (6.0f * w / h);
        _vertices[4] = _vertices[8] = _vertices[20] = w / 6.0f;

        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);

        glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void initShader(const std::string& vsCodePath = "assets/shaders/game_vs.shader", const std::string& fsCodePath = "assets/shaders/game_fs.shader") override {
        _shader.init(vsCodePath, fsCodePath);
    }

    void initTyper() {
        _typer = new Typer(_camera, static_cast<int>(_width), static_cast<int>(_height));
    }

    void initStopwatch() {
        _stopwatch = new Stopwatch(_typer, _width, _height);
    }

    void setProjection() {
        _shader.use();
        auto projection = glm::ortho(0.0f, _width, 0.0f, _height);
        _shader.setMat4("u_projection", projection);
    }

    void update() override {}

    void render() override {
        _shader.use();
        _texture.bind();

        glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, _vertices.size() * sizeof(GLfloat), _vertices.data());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    Typer* _typer = nullptr;
    Control& _control;
    Stopwatch* _stopwatch = nullptr;
    Camera* _cameraMaze = nullptr;
    float _width{ 0.0f };
    float _height{ 0.0f };

};