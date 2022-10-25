#pragma once

#include <random>
#include <vector>

#include "../engine/Entity.h"
#include "../engine/Logger.h"
#include "../engine/Polygon.h"
#include "../engine/Rectangle.h"
#include "../engine/Triangle.h"
#include "Camera.h"
#include "IBlock.h"
#include "TetrisProperties.h"

class BlockL : public IBlock {
public:
    BlockL(Camera* camera, TetrisProperties& tetrisProperties) : IBlock(camera, tetrisProperties) {
        _blockIdentifier = "L";
        _layout = {
            { 0, 1, 0, 0 },
            { 0, 1, 0, 0 },
            { 0, 1, 1, 0 },
            { 0, 0, 0, 0 },
        };
        _row = 3;

        LOGGER(info, "block L init succeeded");
    }

    ~BlockL() override {
        delete _block;

        LOGGER(info, "block L delete succeeded");
    }

    void init() override {
        generateBlock();
    }

    void update() override {}

    void display() override {
        Object::display();
        _block->display();
    }

    void generateBlock() override {
        generateBlockPositions();

        _block = new Polygon(_camera);
        _block->initShader(vertexShader, fragmentShader, /*readFromFile*/ false);
        _block->setDrawMode(GL_TRIANGLES);
        _block->setTexture("tetris-assets/textures/block-" + _blockIdentifier + ".png");
        _block->initMVP();
        _block->setPositions(_blockPositions);
        _block->init();
    }

};
