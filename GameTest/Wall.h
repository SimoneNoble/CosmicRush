//-----------------------------------------------------------------------------
// Wall.h
//-----------------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_

#include "../glut/include/GL/freeglut.h"
#include <map>
#include <vector>
#include "Vector2.h" 
#include <iostream>
#include <ctime>
#include "AABBVolume.h" 


//-----------------------------------------------------------------------------
// Wall
//-----------------------------------------------------------------------------
class Wall {
public:
    Wall(const char* fileName, unsigned int nColumns, unsigned int nRows, float initialMass = 1.0f);
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

    float wallWidth = GetWidth();
    float wallHeight = GetHeight();
    //float m_collisionVolumeAngle;
    AABBVolume wallCollisionVolumeA = AABBVolume(Vector2(wallWidth / 2.0f, wallHeight / 2.0f));
    AABBVolume wallCollisionVolumeB = AABBVolume(Vector2(wallHeight / 2.0f, wallWidth / 2.0f)); // rotated 90 degrees

    static float blowUpTimer;

    void CreateAnimation(unsigned int id, float speed, const std::vector<int>& frames) {
        sAnimation anim;
        anim.m_id = id;
        anim.m_speed = speed;
        anim.m_frames = frames;
        m_animations.push_back(anim);
    };

    bool IsInAnimation(unsigned int animId) const;


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

#endif // _WALL_H_
