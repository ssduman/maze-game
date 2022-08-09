#pragma once

#include "ICommand.h"

class MoveCommand : public ICommand {
public:
    MoveCommand(Entity* entity, glm::vec3 moveUnit) : _entity(entity), _moveUnit(moveUnit) {};

    virtual ~MoveCommand() {}

    void execute() override {}

private:
    Entity* _entity;
    glm::vec3 _moveUnit;

};
