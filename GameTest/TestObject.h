#pragma once
#include "AABBVolume.h"
#include "Vector2.h" 
class TestObject {
public:
    TestObject(const Vector2& position, const Vector2& halfSize)
        : aabbVolume(halfSize), position(position) {}

    const AABBVolume& GetCollisionVolume() const {
        return aabbVolume;
    }

    void SetPosition(const Vector2& newPosition) {
        position = newPosition;
        aabbVolume.SetCenter(newPosition);
    }

    Vector2 GetPosition() const {
        return position;
    }

private:
    AABBVolume aabbVolume;
    Vector2 position;
};
