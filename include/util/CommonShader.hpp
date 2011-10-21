#ifndef _CGA_T2_COMMON_SHADER_HPP_
#define _CGA_T2_COMMON_SHADER_HPP_

#include "GL/glsl.h"
#include "GL/glfw.h"
#include "GL/gl3w.hpp"

#include "macros.h"
#include "util/MatrixStack.h"

namespace util
{
    static inline void setMatrices(Glsl *shader)
    {
        glUniformMatrix4fv(shader->getUniformLoc("projection"), 1, GL_TRUE, util::MATRIXSTACK->projection().elements());
        glUniformMatrix4fv(shader->getUniformLoc("modelview"), 1, GL_TRUE, util::MATRIXSTACK->top().elements());
    }

    static inline void setSunlight(Glsl *shader)
    {
                ////m_fAngle = (m_fAngle > 1000000.f) ? 4.5f : m_fAngle + .01f;
        float s = sin((double)15.f);
        float c = cos((double)15.f);
        glUniform3f(shader->getUniformLoc("sunlightpos"), c,s,c);
    }

} //end of namespace Util.

#endif
