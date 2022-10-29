#pragma once

#include "../engine/Entity.h"
#include "../engine/Logger.h"
#include "../engine/Polygon.h"
#include "../engine/Rectangle.h"

#include "Camera.h"
#include "BlockMovement.h"
#include "TetrisShaders.h"
#include "TetrisProperties.h"

#include <vector>

class IBlock : public Entity {
public:
    IBlock(Camera* camera, TetrisProperties& tetrisProperties) : Entity(camera), _tetrisProperties(tetrisProperties) {}

    ~IBlock() override {
        delete _block;

        LOGGER(info, "iblock " << _blockIdentifier << " delete succeeded");
    }

    void update(float deltaTime) {
        _tetrisProperties.timeElapsed += deltaTime;
    }

    void render() override {}

    void display() override {
        render();
        _block->display();
    }

    virtual void generateBlock() {
        generateBlockPositions();

        _block = new Polygon(_camera);
        _block->initShader(vertexShader, fragmentShader, /*readFromFile*/ false);
        _block->initMVP();
        _block->setDrawMode(aux::DrawMode::triangles);
        _block->dataBuffer({ /*size*/ sizeof(_blockPositions[0]) * _blockPositions.size(), /*data*/ _blockPositions.data() });
        _block->attribute({ /*index*/ 0, /*size*/ 2, /*pointer*/ 0 });
    }

    bool move(BlockMovement blockMovement) {
        if (!down()) {
            return false;
        }

        switch (blockMovement) {
            case BlockMovement::faster_down:
            {
                return fasterDown();
            }
            case BlockMovement::right:
            {
                return right();
            }
            case BlockMovement::left:
            {
                return left();
            }
            case BlockMovement::rotate_left:
            {
                return rotateLeft();
            }
            case BlockMovement::rotate_right:
            {
                return rotateRight();
            }
            case BlockMovement::noop:;
            default:
                return true;
        }
    };

    bool down() {
        if (!isBottom()) {
            --_row;
            generateBlockPositions();
            _block->dataBuffer({ /*size*/ sizeof(_blockPositions[0]) * _blockPositions.size(), /*data*/ _blockPositions.data() });
            return true;
        }
        return false;
    }

    bool fasterDown() {
        if (!isBottom()) {
            --_row;
            generateBlockPositions();
            _block->dataBuffer({ /*size*/ sizeof(_blockPositions[0]) * _blockPositions.size(), /*data*/ _blockPositions.data() });
            return true;
        }
        return false;
    }

    bool right() {
        if (isWall()) {
            return true;
        }
        ++_col;
        return true;
    }

    bool left() {
        if (isWall()) {
            return true;
        }
        --_col;
        return true;
    }

    virtual bool rotateLeft() {
        return true;
    }

    virtual bool rotateRight() {
        return true;
    }

    bool isBottom() {
        return _row <= 0;
    }

    bool isWall() {
        return _col < 0 || _col > _tetrisProperties.totalCols;
    }

    std::vector<glm::vec2> createSquarePosition(const int x, const int y) {
        const auto interval = _tetrisProperties.interval;
        const auto d1 = (interval * 0) / 2;
        const auto d2 = (interval * 2) / 2;
        return {
            { x * interval + d1, y * interval + d1 },
            { x * interval + d1, y * interval + d2 },
            { x * interval + d2, y * interval + d2 },
            { x * interval + d2, y * interval + d2 },
            { x * interval + d2, y * interval + d1 },
            { x * interval + d1, y * interval + d1 },
        };
    }

    void generateBlockPositions() {
        _blockPositions.clear();
        _blockPositions.reserve(4 * 6);

        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                if (_layout[y][x] == 1) {
                    const auto s1 = createSquarePosition(_col + x, _row - y);
                    _blockPositions.insert(_blockPositions.end(), s1.begin(), s1.end());
                }
            }
        }
    }

    const glm::ivec2& getBlockPosition() {
        return _blockPosition;
    }

protected:
    TetrisProperties& _tetrisProperties;
    Entity* _block{ nullptr };
    glm::ivec2 _blockPosition{};
    std::string _blockIdentifier;
    std::vector<glm::vec2> _blockPositions;
    BlockMovement _blockMovement = BlockMovement::noop;

    glm::imat4x4 _layout{};
    int _row{ _tetrisProperties.totalRows };
    int _col{ _tetrisProperties.totalCols / 3 };

};
