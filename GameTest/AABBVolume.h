#pragma once
#include "CollisionVolume.h"
#include "Vector2.h" 

class AABBVolume : public CollisionVolume {
public:
    AABBVolume() : center(Vector2(0.0f, 0.0f)), halfSizes(Vector2(0.5f, 0.5f)), m_rotation(0) {
        type = VolumeType::AABB;
    }

    AABBVolume(const Vector2& halfDims) : halfSizes(halfDims), center(Vector2(0.0f, 0.0f)), m_rotation(0) {
        type = VolumeType::AABB;
    }


    ~AABBVolume() {}

    Vector2 GetCenter() const {
        return center;
    }

    void SetCenter(const Vector2& newCenter) {
        center = newCenter;
    }

    //Vector2 GetHalfDimensions() const {
    //    return halfSizes;
    //}

    Vector2 AABBVolume::GetHalfDimensions() const
    {
        // Consider the rotation angle when computing half dimensions
        float cosA = cos(m_rotation);
        float sinA = sin(m_rotation);
        return Vector2(fabs(halfSizes.x * cosA) + fabs(halfSizes.y * sinA), fabs(halfSizes.x * sinA) + fabs(halfSizes.y * cosA));
    }

    void SetRotation(float newRotation)
    {
        m_rotation = newRotation;
    }

protected:
    Vector2 center; 
    Vector2 halfSizes; 
    float m_rotation;
};
