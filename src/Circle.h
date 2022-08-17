#pragma once

#include "Entity.h"

class Circle : public Entity {
public:
    Circle(EntityProperties& circleProperties) : _circleProperties(circleProperties) {
        _entityType = EntityType::CIRCLE;
        setProperties(_circleProperties);
        _renderCount = (GLsizei)_circleProperties.coordinates.size();
    }

    virtual ~Circle() {
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
    }

    void update() override {}

    void render() override {
        _shader.use();
        _texture.bind();
        glBindVertexArray(_VAO);
        glDrawArrays(_circleProperties.drawMode, 0, _renderCount);
    }

private:
    GLsizei _renderCount = 0;
    EntityProperties _circleProperties;
};