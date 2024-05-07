#pragma once
#include "CollisionVolume.h"
#include "Vector2.h"

class SphereVolume : public CollisionVolume {
public:
    SphereVolume(float sphereRadius = 1.0f) : center(Vector2()), radius(sphereRadius) {
        type = VolumeType::Sphere;
    }

    Vector2 GetCenter() const {
        return center;
    }

    void SetCenter(const Vector2& newCenter) {
        center = newCenter;
    }

    float GetRadius() const {
        return radius;
    }

private:
    Vector2 center;
    float radius;
};
