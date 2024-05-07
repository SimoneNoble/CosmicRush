#include "stdafx.h"
#include "PhysicsManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Vector2.h"
#include "CollisionDetection.h"

PhysicsManager::PhysicsManager()
    : position(0.0f, 0.0f), angle(0.0f), velocity(0.0f, 0.0f), angularVelocity(0.0f) {
}

PhysicsManager::~PhysicsManager() {
}

void PhysicsManager::Update(float deltaTime, Player* player, Enemy* enemy, bool areShipsColliding) {
    // simple Euler integration for velocity
    position += velocity * deltaTime;
    angle += angularVelocity * deltaTime;

    // damping:
    velocity *= 0.99f;  // adjust 
    angularVelocity *= 0.98f;  // adjust 

    if (areShipsColliding) {
        ResolvePlayerEnemyCollision(player, enemy);
    }
}

void PhysicsManager::SetPosition(const Vector2& newPosition) {
    position = newPosition;
}

Vector2 PhysicsManager::GetPosition() const {
    return position;
}

void PhysicsManager::SetAngle(float newAngle) {
    angle = newAngle;
}

float PhysicsManager::GetAngle() const {
    return angle;
}

void PhysicsManager::ApplyForce(const Vector2& force, float deltaTime) {
    // a = F/m
    Vector2 acceleration = force;  // If ships end up having mass, divide force by mass
    velocity += acceleration * deltaTime;
}

void PhysicsManager::ApplyImpulse(const Vector2& impulse, const Vector2& contactVector) {
    velocity += impulse;

    //angularVelocity += /* ... calculate torque based on impulse and contact vector */;
}

Vector2 PhysicsManager::CalculateCollisionNormal(const AABBVolume& aabb1, const AABBVolume& aabb2) {
    Vector2 centerA = aabb1.GetCenter();
    Vector2 centerB = aabb2.GetCenter();

    Vector2 diff = centerB - centerA;

    Vector2 halfSizeA = aabb1.GetHalfDimensions();
    Vector2 halfSizeB = aabb2.GetHalfDimensions();

    float overlapX = halfSizeA.x + halfSizeB.x - std::abs(diff.x);
    float overlapY = halfSizeA.y + halfSizeB.y - std::abs(diff.y);

    if (overlapX < overlapY) {
        return Vector2(diff.x < 0 ? -1.0f : 1.0f, 0.0f);
    }
    else {
        return Vector2(0.0f, diff.y < 0 ? -1.0f : 1.0f);
    }
}

void PhysicsManager::ResolvePlayerEnemyCollision(Player* player, Enemy* enemy) {

    Vector2 collisionNormal = CalculateCollisionNormal(player->playerCollisionVolume, enemy->enemyCollisionVolume);

    // placeholder for penetration depth - replace with actual calculation:
    float penetrationDepth = 1.2f;
    Vector2 correction = collisionNormal * penetrationDepth * -1;

    Vector2 relativeVelocity = player->GetVelocity() - enemy->GetVelocity();

    float restitution = 0.999f;
    float impulseScalar = -(1 + restitution) * Vector2::Dot(relativeVelocity, collisionNormal);
    impulseScalar /= player->GetInverseMass() + enemy->GetInverseMass();

    Vector2 impulse = collisionNormal * impulseScalar;
    player->SetVelocity(player->GetVelocity() + impulse * player->GetInverseMass());
    enemy->SetVelocity(enemy->GetVelocity() - impulse * enemy->GetInverseMass());

    float playerX, playerY, enemyX, enemyY;
    player->GetPosition(playerX, playerY);
    enemy->GetPosition(enemyX, enemyY);

    Vector2 newPlayerPos = Vector2(playerX + correction.x * player->GetInverseMass(), playerY + correction.y * player->GetInverseMass());
    Vector2 newEnemyPos = Vector2(enemyX - correction.x * enemy->GetInverseMass(), enemyY - correction.y * enemy->GetInverseMass());

    player->SetPosition(newPlayerPos.x, newPlayerPos.y);
    enemy->SetPosition(newEnemyPos.x, newEnemyPos.y);

    player->playerCollisionVolume.SetCenter(newPlayerPos);
    enemy->enemyCollisionVolume.SetCenter(newEnemyPos);
}

void PhysicsManager::ResolvePlayerWallCollision(Player* player, Wall* wall) {
    Vector2 collisionNormal;

    if (CollisionDetection::AABBvsAABB(player->playerCollisionVolume, wall->wallCollisionVolumeA)) {
        collisionNormal = CalculateCollisionNormal(player->playerCollisionVolume, wall->wallCollisionVolumeA);
    }
    else if (CollisionDetection::AABBvsAABB(player->playerCollisionVolume, wall->wallCollisionVolumeB)) {
        collisionNormal = CalculateCollisionNormal(player->playerCollisionVolume, wall->wallCollisionVolumeB);
    }
    else {
        return;
    }

    float penetrationDepth = 1.2f;
    Vector2 correction = collisionNormal * penetrationDepth * -1;

    Vector2 relativeVelocity = player->GetVelocity();
    float dotProduct = Vector2::Dot(relativeVelocity, collisionNormal);
    Vector2 reflection = collisionNormal * (2 * dotProduct);
    player->SetVelocity(relativeVelocity - reflection);

    float playerX, playerY;
    player->GetPosition(playerX, playerY);
    Vector2 newPlayerPos = Vector2(playerX + correction.x, playerY + correction.y);
    player->SetPosition(newPlayerPos.x, newPlayerPos.y);
    player->playerCollisionVolume.SetCenter(newPlayerPos);

    while (CollisionDetection::AABBvsAABB(player->playerCollisionVolume, wall->wallCollisionVolumeA) ||
        CollisionDetection::AABBvsAABB(player->playerCollisionVolume, wall->wallCollisionVolumeB)) {
        player->SetPosition(player->Vector2GetPosition().x + correction.x * 0.1f, player->Vector2GetPosition().y + correction.y * 0.1f);
        player->playerCollisionVolume.SetCenter(player->Vector2GetPosition());
    }
}

void PhysicsManager::SetVelocity(const Vector2& newVelocity) {
    velocity = newVelocity;
}

Vector2 PhysicsManager::GetVelocity() const {
    return velocity;
}

void PhysicsManager::SetAngularVelocity(float newAngularVelocity) {
    angularVelocity = newAngularVelocity;
}

float PhysicsManager::GetAngularVelocity() const {
    return angularVelocity;
}
