#ifndef _PHYSICS_MANAGER_H_
#define _PHYSICS_MANAGER_H_

#include "Vector2.h"
#include "AABBVolume.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "CollisionDetection.h"

class PhysicsManager {
public:
    PhysicsManager();
    ~PhysicsManager();

    void Update(float deltaTime, Player* player, Enemy* enemy, bool areShipsColliding);

    void ApplyForce(const Vector2& force, float deltaTime);

    void ApplyImpulse(const Vector2& impulse, const Vector2& contactVector);
    void ApplyTorque(float torque);

    void ResolvePlayerEnemyCollision(Player* player, Enemy* enemy);
    void ResolvePlayerWallCollision(Player* player, Wall* wall);


    void SetVelocity(const Vector2& velocity);
    Vector2 GetVelocity() const;

    void SetAngularVelocity(float angularVelocity);
    float GetAngularVelocity() const;

    void SetPosition(const Vector2& position);
    Vector2 GetPosition() const;

    void SetAngle(float angle);
    float GetAngle() const;

    Vector2 Abs(const Vector2& vec) {
        return Vector2(std::abs(vec.x), std::abs(vec.y));
    }

    Vector2 CalculateCollisionNormal(const AABBVolume& aabb1, const AABBVolume& aabb2);

private:
    Vector2 position;
    float angle;
    Vector2 velocity;
    float angularVelocity;
};

#endif // _PHYSICS_MANAGER_H_
