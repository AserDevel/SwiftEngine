#ifndef ISYSTEM_H
#define ISYSTEM_H

#include "managers/EventManager.h"

class ISystem {
protected:
    uint32_t requiredComponents = 0; // Bitmask defining components this system requires

public:
    bool match(Entity entity) {
        return (requiredComponents & entity.componentMask) == requiredComponents;
    }

    virtual int getPriority() = 0;

    virtual void processEvent(const Event& event, float deltaTime) = 0;

    virtual void update(float deltaTime) = 0;
};

#endif