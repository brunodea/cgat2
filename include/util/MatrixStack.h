#ifndef _CGA_T2_MATRIX_STACK_HPP_
#define _CGA_T2_MATRIX_STACK_HPP_

#include <stack>
#include <vector>
#include "math/matrix_functions.hpp"

namespace util
{
    class MatrixStack
    {
    public:
        ~MatrixStack()
        {
            while(!m_MatrixStack->empty())
                m_MatrixStack->pop();
            delete m_MatrixStack;
        }

        static MatrixStack *instance()
        {
            if(m_sInstance == NULL)
                m_sInstance = new MatrixStack();
            return m_sInstance;
        }

        void pushMatrix() { m_MatrixStack->push(*m_pTop); }
        void popMatrix()
        {
            m_MatrixStack->pop();
            if(m_MatrixStack->empty())
            {
                m_MatrixStack->push(math::identity<4>());
            }

            m_pTop = &m_MatrixStack->top();
        }

        math::Matrix4 top() { return *m_pTop; }

        void loadIdentity() { *m_pTop = math::identity<4>(); }

        void transform(const math::Matrix4 &mat)
        {
            m_pTop = &m_MatrixStack->top();
            *m_pTop = mat*(*m_pTop);
        }

        void translate(float x, float y, float z) { transform(math::translate3f(x,y,z)); }
        void translate(const math::Vector3 &vec) { transform(math::translate(math::toVector4f(vec))); }

        void rotate(float angle, float x, float y, float z) { transform(math::rotate(angle,x,y,z)); }
        void rotate(float angle, math::Vector3 &axis) { transform(math::rotate(angle, axis)); }

        void scale(float sx, float sy, float sz) { transform(math::scale3f(sx,sy,sz)); }
        void scale(const math::Vector3 &vec) { transform(math::scale3f(vec(0,0),vec(1,0),vec(2,0))); }
        
        void setProjection(const math::Matrix4 &proj) { m_Projection = proj; }
        math::Matrix4 projection() { return m_Projection; }

    private:
        MatrixStack()
        {
            m_Projection = math::identity<4>();

            m_MatrixStack = new std::stack<math::Matrix4, std::vector<math::Matrix4> >();
            m_MatrixStack->push(math::identity<4>());
            m_pTop = &m_MatrixStack->top();
        }
    private:
        static MatrixStack *m_sInstance;

        std::stack<math::Matrix4, std::vector<math::Matrix4> > *m_MatrixStack;
        math::Matrix4 *m_pTop;

        math::Matrix4 m_Projection;
    }; //end of class MatrixStack.
} //end of namespace util.

#endif
