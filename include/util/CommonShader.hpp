/*
    funções utilizadas apenas para setar o valor correto de variáveis uniformes em determinados shaders que as utilizam.
*/

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

    static inline void setSunlight(Glsl *shader,float angle)
    {
        float s = sin((double)angle);
        float c = cos((double)angle);
        glUniform3f(shader->getUniformLoc("sunlightpos"), c,s,c);
    }

} //end of namespace Util.

#endif
