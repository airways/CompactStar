/****************************************************************************
 * ==> CSR_Models ----------------------------------------------------------*
 ****************************************************************************
 * Description : This module provides the basic models functions and types  *
 * Developer   : Jean-Milost Reymond                                        *
 * Copyright   : 2017 - 2018, this file is part of the CompactStar Engine.  *
 *               You are free to copy or redistribute this file, modify it, *
 *               or use it for your own projects, commercial or not. This   *
 *               file is provided "as is", WITHOUT ANY WARRANTY OF ANY      *
 *               KIND. THE DEVELOPER IS NOT RESPONSIBLE FOR ANY DAMAGE OF   *
 *               ANY KIND, ANY LOSS OF DATA, OR ANY LOSS OF PRODUCTIVITY    *
 *               TIME THAT MAY RESULT FROM THE USAGE OF THIS SOURCE CODE,   *
 *               DIRECTLY OR NOT.                                           *
 ****************************************************************************/

#ifndef CSR_VertexH
#define CSR_VertexH

// compactStar engine
#include "CSR_Geometry.h"

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

/**
* Vertex type
*/
typedef enum
{
    CSR_VT_Triangles,
    CSR_VT_TriangleStrip,
    CSR_VT_TriangleFan,
    CSR_VT_Quads,
    CSR_VT_QuadStrip
} CSR_EVertexType;

/**
* Vertex format
*/
typedef struct
{
    CSR_EVertexType m_Type;
    int             m_UseNormals;
    int             m_UseTextures;
    int             m_UseColors;
    unsigned        m_Stride;
} CSR_VertexFormat;

/**
* Vertex buffer
*/
typedef struct
{
    CSR_VertexFormat m_Format;
    float*           m_pData;
    size_t           m_Count;
} CSR_VertexBuffer;

/**
* Mesh
*/
typedef struct
{
    CSR_VertexBuffer* m_pVB;
    size_t            m_Count;
} CSR_Mesh;

#ifdef __cplusplus
    extern "C"
    {
#endif
        //-------------------------------------------------------------------
        // Vertex functions
        //-------------------------------------------------------------------

        /**
        * Creates a vertex buffer
        *@return newly created vertex buffer, 0 on error
        *@note The vertex buffer must be released when no longer used, see csrVertexBufferRelease()
        */
        CSR_VertexBuffer* csrVertexBufferCreate(void);

        /**
        * Releases a vertex buffer
        *@param pV - vertex buffer to release
        */
        void csrVertexBufferRelease(CSR_VertexBuffer* pV);

        /**
        * Calculates the vertex stride
        *@param[in, out] pVertexFormat - vertex format for which the stride should be calculated
        */
        void csrVertexCalculateStride(CSR_VertexFormat* pVertexFormat);

        /**
        * Adds a vertex to a vertex buffer
        *@param pVertex - vertex
        *@param pNormal - normal
        *@param pUV - texture coordinate
        *@param color - color
        *@param[in, out] pVB - vertex buffer to add to
        */
        void csrVertexAdd(CSR_Vector3*      pVertex,
                          CSR_Vector3*      pNormal,
                          CSR_Vector2*      pUV,
                          unsigned          color,
                          CSR_VertexBuffer* pVB);

        //-------------------------------------------------------------------
        // Mesh functions
        //-------------------------------------------------------------------

        /**
        * Creates a mesh
        *@return newly created mesh, 0 on error
        *@note The mesh must be released when no longer used, see csrMeshRelease()
        */
        CSR_Mesh* csrMeshCreate(void);

        /**
        * Releases a mesh
        *@param pMesh - mesh to release
        */
        void csrMeshRelease(CSR_Mesh* pMesh);

#ifdef __cplusplus
    }
#endif

//---------------------------------------------------------------------------
// Compiler
//---------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "CSR_Models.c"
#endif

#endif
