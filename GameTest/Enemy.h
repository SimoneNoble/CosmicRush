//-----------------------------------------------------------------------------
// Enemy.h
//-----------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "../glut/include/GL/freeglut.h"
#include <map>
#include <vector>
#include "Vector2.h" 
#include <iostream>
#include <ctime>
#include "AABBVolume.h" 

struct EnemyBullet {
    float x, y;
    float speedX, speedY; // directional speeds
    AABBVolume collisionVolume; // collision radius

    EnemyBullet(float x, float y, float speedX, float speedY) : x(x), y(y), speedX(speedX), speedY(speedY), collisionVolume(AABBVolume(Vector2(2.0f, 2.0f))) {}
};


//-----------------------------------------------------------------------------
// Enemy
//-----------------------------------------------------------------------------
class Enemy {
public:
    Enemy(const char* fileName, unsigned int nColumns, unsigned int nRows, float initialMass = 1.0f);
    void Update(float dt);
    void Draw();
    void SetPosition(float x, float y) { m_xpos = x; m_ypos = y; }
    void SetAngle(float a) { m_angle = a; }
    void SetScale(float s) { m_scale = s; }
    void SetFrame(unsigned int f);
    void SetAnimation(int id);
    void GetPosition(float& x, float& y) { x = m_xpos; y = m_ypos; }

    Vector2 Vector2GetPosition() const {
        return Vector2(m_xpos, m_ypos);
    }

    float GetWidth()  const { return m_width; }
    float GetHeight() const { return m_height; }
    float GetAngle()  const { return m_angle; }
    float GetScale()  const { return m_scale; }
    unsigned int GetFrame()  const { return m_frame; }
    void SetColor(float r, float g, float b) { m_red = r; m_green = g; m_blue = b; }

    // initialise the sprite's collision volume:
    float enemyWidth = GetWidth();
    float enemyHeight = GetHeight();
    AABBVolume enemyCollisionVolume = AABBVolume(Vector2(enemyWidth / 2.0f, enemyHeight / 2.0f));

    static float blowUpTimer;

    void CreateAnimation(unsigned int id, float speed, const std::vector<int>& frames) {
        sAnimation anim;
        anim.m_id = id;
        anim.m_speed = speed;
        anim.m_frames = frames;
        m_animations.push_back(anim);
    };

    bool IsInAnimation(unsigned int animId) const;

    std::vector<EnemyBullet> bullets;
    float lastShotTime; // will make the enemy be able to shoot every n seconds

    void Shoot();
    void UpdateBullets(float deltaTime);
    void RenderBullets();

    Vector2 GetVelocity() const {
        return velocity;
    }

    float GetInverseMass() const {
        return (mass > 0.0f) ? 1.0f / mass : 0.0f; // assuming mass cannot be negative
    }

    void SetVelocity(const Vector2& newVelocity) {
        velocity = newVelocity;
    }

private:
    void CalculateUVs();
    GLuint m_texture;
    float m_xpos = 0.0F;
    float m_ypos = 0.0F;
    float m_width = 0.0F;
    float m_height = 0.0F;
    int   m_texWidth = 0;
    int   m_texHeight = 0;
    float m_angle = 0.0F;
    float m_scale = 1.0F;
    float m_points[8];
    float m_uvcoords[8];
    unsigned int   m_frame;
    unsigned int   m_nColumns;
    unsigned int   m_nRows;
    float m_red = 1.0f;
    float m_green = 1.0f;
    float m_blue = 1.0f;
    int     m_currentAnim = -1;
    float   m_animTime = 0.0F;
    Vector2 velocity;
    float mass;

    struct sAnimation {
        unsigned int m_id = 0;
        float m_speed = 0.0f;
        std::vector<int> m_frames;
    };
    std::vector<sAnimation> m_animations;

    struct sTextureDef {
        unsigned int m_width;
        unsigned int m_height;
        GLuint m_textureID;
    };
    bool LoadTexture(const char*);
    static std::map<const char*, sTextureDef > m_textures;
};

#endif // _ENEMY_H_
