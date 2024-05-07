#pragma once
#include "AABBVolume.h"
#include "SphereVolume.h"
#include "Vector2.h" 
#include <algorithm> 
#include <math.h>  

class CollisionDetection {
public:

    // AABB vs AABB collision detection:
    static bool AABBvsAABB(const AABBVolume& a, const AABBVolume& b) {
        Vector2 aMin = a.GetCenter() - a.GetHalfDimensions();
        Vector2 aMax = a.GetCenter() + a.GetHalfDimensions();
        Vector2 bMin = b.GetCenter() - b.GetHalfDimensions();
        Vector2 bMax = b.GetCenter() + b.GetHalfDimensions();

        // check for overlap in all axes:
        bool overlapX = (aMax.x > bMin.x) && (aMin.x < bMax.x);
        bool overlapY = (aMax.y > bMin.y) && (aMin.y < bMax.y);

        return overlapX && overlapY;
    }

    // sphere vs sphere collision detection:
    static bool SpherevsSphere(const SphereVolume& a, const SphereVolume& b) {
        Vector2 diff = a.GetCenter() - b.GetCenter();
        float distanceSquared = diff.x * diff.x + diff.y * diff.y;
        float radiusSum = a.GetRadius() + b.GetRadius();
        return distanceSquared < (radiusSum* radiusSum);
    }

    // AABB vs Sphere collision detection:
    // ...

    // AABB vs AABB conflict resolution:
    static void ResolveAABBvsAABB(AABBVolume& a, AABBVolume& b) {
        Vector2 aCenter = a.GetCenter();
        Vector2 bCenter = b.GetCenter();

        Vector2 delta = aCenter - bCenter;
        Vector2 dimensionSum = a.GetHalfDimensions() + b.GetHalfDimensions();

        Vector2 separation;

        if (std::abs(delta.x) < dimensionSum.x) {
            separation.x = dimensionSum.x - std::abs(delta.x);
            separation.x *= (delta.x > 0) ? 1 : -1;
        }

        if (std::abs(delta.y) < dimensionSum.y) {
            separation.y = dimensionSum.y - std::abs(delta.y);
            separation.y *= (delta.y > 0) ? 1 : -1;
        }

        // apply the separation to move the objects apart:
        a.SetCenter(aCenter + separation * 0.5f);
        b.SetCenter(bCenter - separation * 0.5f);
    }
};
