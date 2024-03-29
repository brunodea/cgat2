#ifndef _BRUNODEA_CG_T3_MATRIX_FUNCTIONS_HPP_
#define _BRUNODEA_CG_T3_MATRIX_FUNCTIONS_HPP_

/*
 * Funcoes auxiliares relacionado a matrizes a vetores.
 */

#include "math/Matrix.hpp"
#include "math/Vector.hpp"

#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace math
{
    typedef Vector<float, 4> Vector4;
    typedef Vector<float, 3> Vector3;
    typedef Vector<float, 2> Vector2;
    
    typedef Matrix<float, 4> Matrix4;
    typedef Matrix<float, 3> Matrix3;
    typedef Matrix<float, 2> Matrix2;

    /* Retorna a matriz identidade de tamanho M. */
    template<unsigned int M>
    const Matrix<float, M> identity()
    {
        Matrix<float, M> res;
        for(unsigned int i = 0; i < M; i++)
        {
            for(unsigned int j = 0; j < M; j++)
            {
                if(i == j)
                    res.set(1.f, i, j);
                else
                    res.set(0.f, i, j);
            }
        }

        return res;
    }
    
    template<unsigned int M>
    float determinant(const Matrix<float, M, M> &mat)
    {
        float mul = 1;
        float sum = 0;
        for(unsigned int i = 0; i < M*M; i++)
        {
            if(i % M == 0 && i != 0)
            {
                sum += mul;
                mul = 1;
            }
            mul *= mat(i%M,((i%M) + i/M)%M);
        }
        mul = 1;
        for(unsigned int i = 0; i < M*M; i++)
        {
            if(i % M == 0 && i != 0)
            {
                sum -= mul;
                mul = 1;
            }
            mul *= mat(M-1-(i%M),((i%M) + i/M)%M);
        }

        return sum;
    }

    template<unsigned int M>
    Matrix<float, M, M> invert(const Matrix<float, M, M> &mat)
    {
        float det = determinant(mat);
        math::Matrix<float, M, M> res = mat;
        if(det != 0)
        {
            res.print();
            res = math::transpose(res);
            res.print();
            res *= ((float)1.f/det);
            res.print();
        }
        return res;
    }

    template<unsigned int M, unsigned int N>
    Matrix<float, N, M> transpose(const Matrix<float, M, N> &mat)
    {
        Matrix<float, N, M> res;
        for(int i = 0; i < N; i++)
            for(int j = 0; j < M; j++)
                res.set(mat(i,j), j, i);
        return res;
    }

    template<unsigned int M>
    Matrix<float, M> translate(const Vector<float, M> &vec)
    {
        Matrix<float, M> res = identity<M>();

        for(unsigned int i = 0; i < M-1; i++)
            res.set(vec(i, 0), i, M-1);

        return res;
    }

    inline Matrix4 translate3f(float x, float y, float z)
    {
        Vector4 vec(1.f);
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;

        return translate(vec);
    }
    

    template<unsigned int M>
    const math::Vector<float, M>normalize(const Vector<float, M> &vec)
    {
        math::Vector<float, M> res;
        float n = norm(vec);
        for(unsigned int i = 0; i < M; i++)
            res.set(vec(i,0)/n, i, 0);
        return res;
    }

    

    template<unsigned int M>
    inline Matrix<float, M> scale(const Vector<float, M> &vec)
    {
        Matrix<float, M> res = identity<M>();
        for(unsigned int i = 0; i < M-1; i++)
            res.set(vec(i,0),i,i);

        return res;
    }

    inline Matrix4 scale3f(float sx, float sy, float sz)
    {
        Vector4 vec(1.f);
        vec[0] = sx;
        vec[1] = sy;
        vec[2] = sz;

        return scale(vec);
    }

    template<unsigned int M>
    const float norm(const Vector<float, M> &vec)
    {
        float sum = 0.f;
        for(unsigned int i = 0; i < M; i++)
            sum += (vec(i,0)*vec(i,0));
        return sqrt((float)sum);
    }
    
    template<unsigned int M>
    const float angle(const Vector<float, M> &v1, const Vector<float, M> &v2)
    {
        Vector<float, M> v1_unit = normalize(v1);
        Vector<float, M> v2_unit = normalize(v2);

        float ang = acos(v1_unit.dotProduct(v2_unit));
        return ang;
    }

    template<unsigned int M>
    const Matrix<float, M, 1> toMatrix(const Vector<float, M> &vec)
    {
        Matrix<float, M, 1> res;
        for(unsigned int i = 0; i < M; i++)
            res.set(vec[i], i, 0);

        return res;
    }

    template<unsigned int M>
    const Vector<float, M> toVector(const Matrix<float, M, 1> &mat)
    {
        Vector<float, M> res;
        for(unsigned int i = 0; i < M; i++)
            res[i] = mat(i, 0);
        return res;
    }

    inline Vector3 vector3f(float x, float y, float z)
    {
        Vector<float, 3> res;
        res[0] = x;
        res[1] = y;
        res[2] = z;
        return res;
    }

    inline Vector4 vector4f(float x, float y, float z, float w)
    {
        Vector<float, 4> res;
        res[0] = x;
        res[1] = y;
        res[2] = z;
        res[3] = w;

        return res;
    }

    inline Vector4 toVector4f(const Vector3 &v)
    {
        return vector4f(v(0,0),v(1,0),v(2,0),1);
    }

    inline Vector3 toVector3f(const Vector4 &v)
    {
        return vector3f(v(0,0),v(1,0),v(2,0));
    }

    inline float distance(const Vector3 &p1, const Vector3 &p2)
    {
        return sqrt(pow(p2(0,0)-p1(0,0),2)+pow(p2(1,0)-p1(1,0),2)+pow(p2(2,0)+p1(2,0),2));
    }

    //verifica se position esta dentro do plano formado por d1 e d2.
    inline bool isInside(Vector3 &position, Vector3 &d1, Vector3 &d2)
    {
        float x = position[0];
        float y = position[1];
        float z = position[2];

        if(x >= d1[0] && x <= d2[0] && y >= d1[1] && y <= d2[1] && z >= d1[2] && z <= d2[2])
            return true;
        return false;
    }

    inline Matrix4 perspective(float fovy, float aspect, float zNear, float zFar)
    {
        Matrix4 res(0.f);

        float f = (float)tan((PI/2.F) - (fovy/2.f));

        res.set(f/aspect, 0,0);
        res.set(f, 1,1);
        res.set((zFar+zNear)/(zNear-zFar), 2,2);
        res.set((2*zFar*zNear)/(zNear-zFar), 2,3);
        res.set(-1.f, 3,2);

        return res;
    }

    inline Matrix4 ortho(float left, float right, float bottom, float top, float near, float far)
    {
        Matrix4 res(0.f);

        float tx = -1*((right+left)/(right-left));
        float ty = -1*((top+bottom)/(top-bottom));
        float tz = -1*((far+near)/(far-near));

        res.set(2/(right-left),0,0);
        res.set(2/(top-bottom),1,1);
        res.set(-2/(far-near),2,2);
        res.set(tx,0,3);
        res.set(ty,1,3);
        res.set(tz,2,3);
        res.set(1.f,3,3);

        return res;
    }
    
    inline Matrix4 rotate(float ang, Vector3 &axis)
    {
        math::Matrix4 res = math::identity<4>();
        axis = normalize(axis);
        float x = axis[0];
        float y = axis[1];
        float z = axis[2];

        float c = cos(ang);
        float s = sin(ang);

        res.set(c+(x*x*(1-c)), 0,0);
        res.set(((x*y)*(1-c))-(z*s), 0,1);
        res.set((x*z*(1-c))+(y*s), 0,2);
        
        res.set((x*y*(1-c))+(z*s), 1,0);
        res.set(c+(y*y*(1-c)), 1,1);
        res.set((y*z*(1-c))-(x*s), 1,2);
        
        res.set((x*z*(1-c))-(y*s), 2,0);
        res.set((y*z*(1-c))+(x*s), 2,1);
        res.set(c+(z*z*(1-c)), 2,2);

        return res;

        //Matrix4 res = identity<4>();
        //float t = 1-(cos(ang));
        //float c = cos(ang);
        //float s = sin(ang);

        //axis = normalize(axis);
        //float x = axis[0];
        //float y = axis[1];
        //float z = axis[2];

        //res.set((t*x*x)+c, 0,0);
        //res.set((t*x*y)+(s*z), 0,1);
        //res.set((t*x*z)-(s*y), 0,2);

        //res.set((t*x*y)-(s*z), 1,0);
        //res.set((t*y*y)+c, 1,1);
        //res.set((t*y*z)+(s*x), 1,2);

        //res.set((t*x*y)+(s*y), 2,0);
        //res.set((t*y*z)-(s*x), 2,1);
        //res.set((t*z*z)+c, 2,2);

        //return res;
    }

    inline Matrix4 rotate(float ang, float x, float y, float z)
    {
        return rotate(ang, math::vector3f(x,y,z));
        //Matrix4 res = identity<4>();
        //float t = 1-(cos(ang));
        //float c = cos(ang);
        //float s = sin(ang);

        //res.set((t*x*x)+c, 0,0);
        //res.set((t*x*y)+(s*z), 0,1);
        //res.set((t*x*z)-(s*y), 0,2);

        //res.set((t*x*y)-(s*z), 1,0);
        //res.set((t*y*y)+c, 1,1);
        //res.set((t*y*z)+(s*x), 1,2);

        //res.set((t*x*y)+(s*y), 2,0);
        //res.set((t*y*z)-(s*x), 2,1);
        //res.set((t*z*z)+c, 2,2);
    }

    inline Matrix4 rotateX(float ang)
    {
        return rotate(ang, 1.f,0.f,0.f);
    }

    inline Matrix4 rotateY(float ang)
    {
        return rotate(ang, 0.f,1.f,0.f);
    }

    inline Matrix4 rotateZ(float ang)
    {
        return rotate(ang, 0.f,0.f,1.f);
    }

    inline Vector3 transform(const Vector3 &v, const Matrix4 &m)
    {
        Vector4 aux = toVector4f(v);
        aux = m*aux;
        return toVector3f(aux);
    }

} //end of namespace math.


#endif
