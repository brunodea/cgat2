/*
    Structs criadas para manter um padr�o nas structs dos v�rtices dos diversos modelos e para n�o ser necess�ria a cria��o
    de structs identicas para as diferentes classes.
*/
#ifndef _CGA_T2_VERTEX_STRUCTS_HPP_
#define _CGA_T2_VERTEX_STRUCTS_HPP_

namespace model
{
    enum VertexStruct
    {
        POSVERT,
        TEXVERT,
        COLORVERT
    }; //end of enum VertexStructs.

    struct PosVert
    {
        float pos[4];
    }; //end of class VertexStructs.

    struct TexVert
    {
        float pos[4];
        float uv[2]; //u,v
    }; //end of struct TexVert.

    struct ColorVert
    {
        float pos[4];
        float color[4]; //rgba
    }; //end of struct ColorVert.
} //end of namespace model.
#endif
