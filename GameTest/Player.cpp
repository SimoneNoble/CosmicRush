///////////////////////////////////////////////////////////////////////////////
// Filename: Player.cpp
// Privides a very simple DSOUND wrapper to load and play wav files.
///////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
#include "stdafx.h"
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <assert.h>

//-----------------------------------------------------------------------------

#include "../App/app.h"
#include "../App/AppSettings.h"
#include "Player.h"

#include "../stb_image/stb_image.h"
#include "../glut/include/GL/freeglut_ext.h"

std::map<const char*, Player::sTextureDef > Player::m_textures;

float Player::blowUpTimer = 0.0f;

//-----------------------------------------------------------------------------
Player::Player(const char* fileName, unsigned int nColumns, unsigned int nRows, float initialMass)
    : m_nColumns(nColumns)
    , m_nRows(nRows)
    , mass(initialMass)
    , velocity(0.0f, 0.0f) // initialising velocity to zero
{
    if (LoadTexture(fileName))
    {
        CalculateUVs();
        m_points[0] = -(m_width / 2.0f);
        m_points[1] = -(m_height / 2.0f);
        m_points[2] = m_width / 2.0f;
        m_points[3] = -(m_height / 2.0f);
        m_points[4] = m_width / 2.0f;
        m_points[5] = m_height / 2.0f;
        m_points[6] = -(m_width / 2.0f);
        m_points[7] = m_height / 2.0f;
    }

    playerCollisionVolume = AABBVolume(Vector2(((m_scale * m_width) / 2.0f, (m_scale * m_height) / 2.0f)));
}

void Player::Update(float dt)
{
    if (m_currentAnim >= 0)
    {
        m_animTime += dt / 1000.0f;
        sAnimation& anim = m_animations[m_currentAnim];
        float duration = anim.m_speed * anim.m_frames.size();

        //Looping around if reached the end of animation
        if (m_animTime > duration)
        {
            //If we've gone farther than twice the duration, we have to remove as many full durations as possible
            if (m_animTime >= 2 * duration)
            {
                m_animTime = m_animTime - duration * floorf(m_animTime / duration);
            }
            else //Otherwise, we can just do a simple loop around
            {
                m_animTime = m_animTime - duration;
            }
        }
        int frame = (int)(m_animTime / anim.m_speed);
        SetFrame(anim.m_frames[frame]);
    }
}

void Player::CalculateUVs()
{
    float u = 1.0f / m_nColumns;
    float v = 1.0f / m_nRows;
    int row = m_frame / m_nColumns;
    int column = m_frame % m_nColumns;

    m_width = (m_texWidth * u) / 3;
    m_height = (m_texHeight * v) / 3;
    m_uvcoords[0] = u * column;
    m_uvcoords[1] = v * (float)(row + 1);

    m_uvcoords[2] = u * (float)(column + 1);
    m_uvcoords[3] = v * (float)(row + 1);

    m_uvcoords[4] = u * (float)(column + 1);
    m_uvcoords[5] = v * row;

    m_uvcoords[6] = u * column;
    m_uvcoords[7] = v * row;
}

void Player::Draw()
{
#if APP_USE_VIRTUAL_RES
    float scalex = (m_scale / APP_VIRTUAL_WIDTH) * 2.0f;
    float scaley = (m_scale / APP_VIRTUAL_HEIGHT) * 2.0f;
#else
    float scalex = m_scale;
    float scaley = m_scale;
#endif
    float x = m_xpos;
    float y = m_ypos;
#if APP_USE_VIRTUAL_RES
    APP_VIRTUAL_TO_NATIVE_COORDS(x, y);
#endif

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scalex, scaley, 1.0f);
    glRotatef(m_angle * 180 / PI, 0.0f, 0.0f, 1.0f);
    glColor3f(m_red, m_green, m_blue);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBegin(GL_QUADS);
    for (unsigned int i = 0; i < 8; i += 2)
    {
        glTexCoord2f(m_uvcoords[i], m_uvcoords[i + 1]);
        glVertex2f(m_points[i], m_points[i + 1]);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Player::SetFrame(unsigned int f)
{
    m_frame = f;
    if (m_frame > m_nRows * m_nColumns)
    {
        m_frame = 0;
    }
    CalculateUVs();
}

void Player::SetAnimation(int id)
{
    for (int i = 0; i < m_animations.size(); i++)
    {
        if (m_animations[i].m_id == id)
        {
            m_currentAnim = i;
            return;
        }
    }
    m_currentAnim = -1; // stops animation
}

//void Player::SetPosition(float x, float y) {
//    m_xpos = x; m_ypos = y;
//    collisionVolume.SetCenter(Vector2(x, y));
//}


void Player::Shoot() {
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    if (currentTime - lastShotTime >= 0.0f) { // no delay for now
        float bulletSpeed = 1.5f; 
        float bulletDirX = cos(m_angle); 
        float bulletDirY = sin(m_angle); 

        Bullet newBullet(m_xpos, m_ypos, bulletDirX * bulletSpeed, bulletDirY * bulletSpeed);
        bullets.push_back(newBullet);
        lastShotTime = currentTime;
    }
}

void Player::UpdateBullets(float deltaTime) {

    for (auto& bullet : bullets) {
        bullet.x += bullet.speedX * deltaTime; 
        bullet.y += bullet.speedY * deltaTime; 

        bullet.collisionVolume.SetCenter(Vector2(bullet.x, bullet.y));
    }

    // remove bullets that have gone off screen:
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
        return b.x > 1000; // 1000 is temporary screen dimension!
        }), bullets.end());
}

void Player::RenderBullets() {
    const float bulletLength = 10.0f; 
    const float bulletSpeed = 0.5f; // make sure this matches the value in Shoot method

    for (const auto& bullet : bullets) {
        float endX = bullet.x + (bullet.speedX / bulletSpeed) * bulletLength;
        float endY = bullet.y + (bullet.speedY / bulletSpeed) * bulletLength;

        float r = 1.0f, g = 0.0f, b = 0.0f;

        glLineWidth(2.0f);
        App::DrawLine(bullet.x, bullet.y, endX, endY, r, g, b);
        glLineWidth(1.0f);
    }
}

bool Player::IsInAnimation(unsigned int animId) const {
    return m_currentAnim != -1 && m_animations[m_currentAnim].m_id == animId;
}

bool Player::LoadTexture(const char* filename)
{
    if (m_textures.find(filename) != m_textures.end())
    {
        sTextureDef& texDef = m_textures[filename];
        m_texture = texDef.m_textureID;
        m_texWidth = texDef.m_width;
        m_texHeight = texDef.m_height;
        return true;
    }

    //unsigned char *imageData = loadBMPRaw(filename, m_texWidth, m_texHeight, true);

    int channels;
    unsigned char* imageData = stbi_load(filename, &m_texWidth, &m_texHeight, &channels, 4);

    GLuint texture = 0;
    if (imageData)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, m_texWidth, m_texHeight, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        stbi_image_free(imageData);
        sTextureDef textureDef = { (unsigned int)m_texWidth, (unsigned int)m_texHeight, texture };
        m_textures[filename] = textureDef;
        m_texture = texture;
        return true;
    }
    return false;
}
