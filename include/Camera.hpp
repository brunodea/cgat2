/**
    Classe que lida com as funcoes de uma camera sintetica.
 **/
#ifndef _BRUNODEA_CAMERA_HPP_
#define _BRUNODEA_CAMERA_HPP_

#include "math/matrix_functions.hpp"
#include "math/math_aux.hpp"

class Camera
{
public:
    Camera(const math::Vector3 &eye, const math::Vector3 &target, const math::Vector3 &up)
        : m_vEye3(eye), m_vTarget3(target), m_vUp3(up)
    {
        adjustAxes();
        m_fSpeed = 1.f;
    }

    void rotate(float angle, float x, float y, float z)
    {
    }

    math::Matrix4 lookAt()
    {
        math::Matrix4 res = math::identity<4>();

        float eyex = m_vEye3[0];
        float eyey = m_vEye3[1];
        float eyez = m_vEye3[2];
        
        float centerx = m_vTarget3[0];
        float centery = m_vTarget3[1];
        float centerz = m_vTarget3[2];
        
        float upx = m_vUp3[0];
        float upy = m_vUp3[1];
        float upz = m_vUp3[2];

        math::Vector3 f = math::vector3f(centerx-eyex,centery-eyey,centerz-eyez);
        math::Vector3 up = math::vector3f(upx,upy,upz);
        f = math::normalize(f);
        up = math::normalize(up);

        math::Vector3 s = f.crossProduct(up);
        math::Vector3 u = s.crossProduct(f);
        
        res.set(s[0], 0,0);
        res.set(s[1], 0,1);
        res.set(s[2], 0,2);
        
        res.set(u[0], 1,0);
        res.set(u[1], 1,1);
        res.set(u[2], 1,2);

        res.set(-f[0], 2,0);
        res.set(-f[1], 2,1);
        res.set(-f[2], 2,2);

        return res;
    }

    math::Matrix4 lookAt(float eyex, float eyey, float eyez, float centerx,
                         float centery, float centerz, float upx, float upy, float upz)
    {
        math::Matrix4 res = math::identity<4>();

        math::Vector3 f = math::vector3f(centerx-eyex,centery-eyey,centerz-eyez);
        math::Vector3 up = math::vector3f(upx,upy,upz);
        f = math::normalize(f);
        up = math::normalize(up);

        math::Vector3 s = f.crossProduct(up);
        math::Vector3 u = s.crossProduct(f);
        
        res.set(s[0], 0,0);
        res.set(s[1], 0,1);
        res.set(s[2], 0,2);
        
        res.set(u[0], 1,0);
        res.set(u[1], 1,1);
        res.set(u[2], 1,2);

        res.set(-f[0], 2,0);
        res.set(-f[1], 2,1);
        res.set(-f[2], 2,2);

        return res;
    }

    void moveForward() { m_vEye3 += m_vDirection3*m_fSpeed; }
    void moveBackwards() { m_vEye3 -= m_vDirection3*m_fSpeed; }
    void moveLeft() { m_vEye3 += m_vRight3*m_fSpeed; }
    void moveRight() { m_vEye3 -= m_vRight3*m_fSpeed; }
    void moveUp() { m_vEye3 -= m_vUp3*m_fSpeed; }
    void moveDown() { m_vEye3 += m_vUp3*m_fSpeed; }
    
    math::Vector3 eye() { return m_vEye3; }
    void setEye(const math::Vector3 &eye) { m_vEye3 = eye; }

    math::Vector3 target() { return m_vTarget3; }
    void setTarget(const math::Vector3 &target) { m_vTarget3 = target; }

    math::Vector3 up() { return m_vUp3; }
    void setUp(const math::Vector3 &up) { m_vUp3 = up; }

    math::Vector3 direction() { return m_vDirection3; }
    void setDirection(const math::Vector3 &direction) { m_vDirection3 = direction; }

    math::Vector3 right() { return m_vRight3; }
    void setRight(const math::Vector3 &right) { m_vRight3 = right; }

    void setSpeed(float speed) { m_fSpeed = speed; }
    float getSpeed() { return m_fSpeed; }

    void adjustAxes()
    {
        m_vDirection3 = math::normalize(m_vTarget3 - m_vEye3);
        m_vRight3 = math::normalize(m_vUp3.crossProduct(m_vDirection3));
        m_vUp3 = m_vDirection3.crossProduct(m_vRight3);
    }

private:
    math::Vector3 m_vEye3;
    math::Vector3 m_vTarget3;
    math::Vector3 m_vUp3;
    math::Vector3 m_vDirection3;
    math::Vector3 m_vRight3;

    float m_fSpeed;

}; //end of class Camera.
#endif
