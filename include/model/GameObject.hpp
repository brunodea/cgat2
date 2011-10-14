#ifndef _CGA_T2_GAME_OBJECT_HPP_
#define _CGA_T2_GAME_OBJECT_HPP_

#include "math/matrix_functions.hpp"

namespace model
{
    class GameObject
    {
    public:
        GameObject() 
            : m_Pos(math::Vector3(0.f)), m_Dir(math::Vector3(0.f)), m_fSpeed(0.f), m_fMaxSpeed(0.f), m_fAccel(0.f)
        {}
        GameObject(const math::Vector3 &pos, const math::Vector3 &dir, float speed, float maxspeed, float accel)
            : m_Pos(pos), m_Dir(dir), m_fSpeed(speed), m_fMaxSpeed(maxspeed), m_fAccel(accel)
        {}

        /* Virtual Functions */
        virtual void onRender() = 0;
        virtual void onUpdate() = 0;
        virtual void onKeyEvent(int key, int state) = 0;

        virtual void move() { m_Pos += (m_Dir*m_fSpeed); }

        void speedUp() 
        {
            m_fSpeed += m_fAccel;
            if(m_fSpeed > m_fMaxSpeed)
                m_fSpeed = m_fMaxSpeed;
        }
        void speedDown()
        {
            m_fSpeed -= m_fAccel;
            if(m_fSpeed < 0.f)
                m_fSpeed = 0.f;
        }

        void setPos(const math::Vector3 &pos) { m_Pos = pos; }
        math::Vector3 pos() { return m_Pos; }

        void setDir(const math::Vector3 &dir) { m_Dir = dir; }
        math::Vector3 dir() { return m_Dir; }

        void setSpeed(float speed) { m_fSpeed = speed; }
        float speed() { return m_fSpeed; }

        void setMaxSpeed(float maxspeed) { m_fMaxSpeed = maxspeed; }
        float maxSpeed() { return m_fMaxSpeed; }

        void setAccel(float accel) { m_fAccel = accel; }
        float accel() { return m_fAccel; }

    private:
        math::Vector3 m_Pos;
        math::Vector3 m_Dir;

        float m_fSpeed;
        float m_fMaxSpeed;

        float m_fAccel;
    };
} //end of namespace model.

#endif
