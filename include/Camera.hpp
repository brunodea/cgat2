/**
    Classe que lida com as funcoes de uma camera sintetica.
 **/
#ifndef _BRUNODEA_CAMERA_HPP_
#define _BRUNODEA_CAMERA_HPP_

#include "math/matrix_functions.hpp"
#include "math/math_aux.hpp"

#include <cmath>

class Camera
{
public:
    Camera(const math::Vector3 &eye, const math::Vector3 &target, const math::Vector3 &up)
        : m_vEye3(eye), m_vTarget3(target), m_vUp3(up)
    {
        adjustAxes();
        m_fSpeed = 1.f;
    }
    
    math::Matrix4 rotate(float angle, math::Vector3 &v)
    {
        v = math::normalize(v);
        return rotate(angle, v(0,0), v(1,0), v(2,0));
    }

    math::Matrix4 rotate(float angle, float x, float y, float z)
    {
        math::Matrix4 res = math::identity<4>();

        float c = cos(angle);
        float s = sin(angle);

        res.set(c+(x*x*(1-c)), 0,0);
        res.set(((x*y)*(1-c))-(z*s), 0,1);
        res.set((x*z*(1-c))+(y*s), 0,2);
        
        res.set((x*y*(1-c))+(z*s), 1,0);
        res.set(c+(y*y*(1-c)), 1,1);
        res.set((y*z*(1-c))-(x*s), 1,2);
        
        res.set((x*z*(1-c))-(y*s), 2,0);
        res.set((y*z*(1-c))+(x*s), 2,1);
        res.set(c+(z*z*(1-c)), 2,2);
        
        /*float bx = m_vEye3[0];
        float by = m_vEye3[1];
        float bz = m_vEye3[2];

        math::Vector3 n = math::vector3f(x,y,z);
        res.set(((bx-(m_vEye3.dotProduct(n)*x))*(1-c))+(z*by)-(y*bz), 0,3);
        res.set(((by-(m_vEye3.dotProduct(n)*y))*(1-c))+(z*by)-(x*bz), 1,3);
        res.set(((bz-(m_vEye3.dotProduct(n)*z))*(1-c))+(x*by)-(y*bz), 2,3);*/

        return res;
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
        m_vEye3 = math::vector3f(eyex, eyey, eyez);
        m_vTarget3 = math::vector3f(centerx, centery, centerz);
        m_vUp3 = math::vector3f(upx, upy, upz);

        return lookAt();
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


    void transform(const math::Matrix4 &m)
    {
        //m_vTarget3 = transform(m_vTarget3, m);
        //adjustAxes();
        //m_vDirection3 = math::normalize(transform(m_vDirection3, m));
        //m_vDirection3.print();
        //m_vRight3 = math::normalize(transform(m_vRight3, m));
        //m_vUp3 = math::normalize(transform(m_vUp3, m));
    }

    void adjustAxes()
    {
        m_vDirection3 = math::normalize(m_vTarget3 - m_vEye3);
        m_vRight3 = math::normalize(m_vUp3.crossProduct(m_vDirection3));
        m_vUp3 = m_vDirection3.crossProduct(m_vRight3);
    }
private:
    math::Vector3 transform(const math::Vector3 &v, const math::Matrix4 &m)
    {
        math::Vector4 t = math::toVector4f(v);
        t =  m*t;
        return math::toVector3f(t);
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
