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

    struct TexVert : public PosVert
    {
        float pos[4];
        float uv[2]; //u,v
    }; //end of struct TexVert.

    struct ColorVert : public PosVert
    {
        float pos[4];
        float color[4]; //rgba
    }; //end of struct ColorVert.
} //end of namespace model.
#endif
