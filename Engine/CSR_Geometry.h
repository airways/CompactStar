/****************************************************************************
 * ==> CSR_Geometry --------------------------------------------------------*
 ****************************************************************************
 * Description : This module provides the geometric functions and structs   *
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

#ifndef CSR_GeometryH
#define CSR_GeometryH

// std
#include <stddef.h>

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

/**
* 2D vector
*/
typedef struct
{
    float m_X;
    float m_Y;
} CSR_Vector2;

/**
* 3D vector
*/
typedef struct
{
    float m_X;
    float m_Y;
    float m_Z;
} CSR_Vector3;

/**
* 4x4 matrix
*/
typedef struct
{
    float m_Table[4][4];
} CSR_Matrix4;

/**
* Quaternion
*/
typedef struct
{
    float m_X;
    float m_Y;
    float m_Z;
    float m_W;
} CSR_Quaternion;

/**
* Plane
*/
typedef struct
{
    float m_A;
    float m_B;
    float m_C;
    float m_D;
} CSR_Plane;

/**
* 2D Ray
*/
typedef struct
{
    CSR_Vector2 m_Pos;
    CSR_Vector2 m_Dir;
    CSR_Vector2 m_InvDir;
} CSR_Ray2;

/**
* 2D Line segment
*/
typedef struct
{
    CSR_Vector2 m_Start;
    CSR_Vector2 m_End;
} CSR_Segment2;

/**
* 3D Ray
*/
typedef struct
{
    CSR_Vector3 m_Pos;
    CSR_Vector3 m_Dir;
    CSR_Vector3 m_InvDir;
} CSR_Ray3;

/**
* 3D Line segment
*/
typedef struct
{
    CSR_Vector3 m_Start;
    CSR_Vector3 m_End;
} CSR_Segment3;

/**
* Circle
*/
typedef struct
{
    CSR_Vector2 m_Center;
    float       m_Radius;
} CSR_Circle;

/**
* Aligned-axis rect
*/
typedef struct
{
    CSR_Vector2 m_Min;
    CSR_Vector2 m_Max;
} CSR_Rect;

/**
* Sphere
*/
typedef struct
{
    CSR_Vector3 m_Center;
    float       m_Radius;
} CSR_Sphere;

/**
* Aligned-axis box
*/
typedef struct
{
    CSR_Vector3 m_Min;
    CSR_Vector3 m_Max;
} CSR_Box;

/**
* 2D Polygon
*/
typedef struct
{
    CSR_Vector2 m_Vertex[3];
} CSR_Polygon2;

/**
* 3D Polygon
*/
typedef struct
{
    CSR_Vector3 m_Vertex[3];
} CSR_Polygon3;

#ifdef __cplusplus
    extern "C"
    {
#endif
        //-------------------------------------------------------------------
        // 2D vector functions
        //-------------------------------------------------------------------

        /**
        * Adds vector contents
        *@param pV1 - vector that contains values to be added
        *@param pV2 - vector to add to
        *@param[out] pR - resulting vector
        */
        void csrVec2Add(const CSR_Vector2* pV1, const CSR_Vector2* pV2, CSR_Vector2* pR);

        /**
        * Subtracts vector contents
        *@param pV1 - vector that contains values to be subtracted
        *@param pV2 - vector to subtract by
        *@param[out] pR - resulting vector
        */
        void csrVec2Sub(const CSR_Vector2* pV1, const CSR_Vector2* pV2, CSR_Vector2* pR);

        /**
        * Calculates vector length
        *@param pV - vector to calculate length
        *@param[out] pR - vector length
        */
        void csrVec2Length(const CSR_Vector2* pV, float* pR);

        /**
        * Normalizes the vector
        *@param pV - vector to normalize
        *@param[out] pR - normailzed vector
        */
        void csrVec2Normalize(const CSR_Vector2* pV, CSR_Vector2* pR);

        /**
        * Calculates cross product between 2 vectors
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param[out] pR - resulting vector
        */
        void csrVec2Cross(const CSR_Vector2* pV1, const CSR_Vector2* pV2, CSR_Vector2* pR);

        /**
        * Calculates dot product between 2 vectors
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param[out] pR - resulting angle
        */
        void csrVec2Dot(const CSR_Vector2* pV1, const CSR_Vector2* pV2, float* pR);

        /**
        * Checks if a vector is between a range of values
        *@param pV - vector to test
        *@param pRS - range start
        *@param pRE - range end
        *@param tolerance - tolerance for calculation
        *@return 1 if vector is between range, otherwise 0
        */
        int csrVec2BetweenRange(const CSR_Vector2* pV,
                                const CSR_Vector2* pRS,
                                const CSR_Vector2* pRE,
                                      float        tolerance);

        //-------------------------------------------------------------------
        // 3D vector functions
        //-------------------------------------------------------------------

        /**
        * Adds vector contents
        *@param pV1 - vector that contains values to be added
        *@param pV2 - vector to add to
        *@param[out] pR - resulting vector
        */
        void csrVec3Add(const CSR_Vector3* pV1, const CSR_Vector3* pV2, CSR_Vector3* pR);

        /**
        * Subtracts vector contents
        *@param pV1 - vector that contains values to be subtracted
        *@param pV2 - vector to subtract by
        *@param[out] pR - resulting vector
        */
        void csrVec3Sub(const CSR_Vector3* pV1, const CSR_Vector3* pV2, CSR_Vector3* pR);

        /**
        * Calculates vector length
        *@param pV - vector to calculate length
        *@param[out] pR - vector length
        */
        void csrVec3Length(const CSR_Vector3* pV, float* pR);

        /**
        * Normalizes the vector
        *@param pV - vector to normalize
        *@param[out] pR - normailzed vector
        */
        void csrVec3Normalize(const CSR_Vector3* pV, CSR_Vector3* pR);

        /**
        * Calculates cross product between 2 vectors
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param[out] pR - resulting vector
        */
        void csrVec3Cross(const CSR_Vector3* pV1, const CSR_Vector3* pV2, CSR_Vector3* pR);

        /**
        * Calculates dot product between 2 vectors
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param[out] pR - resulting angle
        */
        void csrVec3Dot(const CSR_Vector3* pV1, const CSR_Vector3* pV2, float* pR);

        /**
        * Checks if a vector is between a range of values
        *@param pV - vector to test
        *@param pRS - range start
        *@param pRE - range end
        *@param tolerance - tolerance for calculation
        *@return 1 if vector is between range, otherwise 0
        */
        int csrVec3BetweenRange(const CSR_Vector3* pV,
                                const CSR_Vector3* pRS,
                                const CSR_Vector3* pRE,
                                      float        tolerance);

        //-------------------------------------------------------------------
        // Matrix functions
        //-------------------------------------------------------------------

        /**
        * Gets matrix identity
        *@param[out] pR - resulting identity matrix
        */
        void csrMat4Identity(CSR_Matrix4* pR);

        /**
        * Gets orthogonal matrix
        *@param left - left view rect edge
        *@param right - right view rect edge
        *@param bottom - bottom view rect edge
        *@param top - top view rect edge
        *@param zNear - z near clipping value
        *@param zFar - z far clipping value
        *@param[out] pR - resulting orthogonal matrix
        */
        void csrMat4Ortho(float        left,
                          float        right,
                          float        bottom,
                          float        top,
                          float        zNear,
                          float        zFar,
                          CSR_Matrix4* pR);

        /**
        * Gets frustrum matrix
        *@param left - left view rect edge
        *@param right - right view rect edge
        *@param bottom - bottom view rect edge
        *@param top - top view rect edge
        *@param zNear - z near clipping value
        *@param zFar - z far clipping value
        *@param[out] pR - resulting frustrum matrix
        */
        void csrMat4Frustum(float        left,
                            float        right,
                            float        bottom,
                            float        top,
                            float        zNear,
                            float        zFar,
                            CSR_Matrix4* pR);

        /**
        * Gets perspective matrix
        *@param fovyDeg - field of view angle in radian
        *@param aspect - aspect ratio
        *@param zNear - z near clipping value
        *@param zFar - z far clipping value
        *@param[out] pR - resulting perspective matrix
        */
        void csrMat4Perspective(float        fovyDeg,
                                float        aspect,
                                float        zNear,
                                float        zFar,
                                CSR_Matrix4* pR);

        /**
        * Gets translation matrix
        *@param pT - translation vector
        *@param[out] pR - resulting translation matrix
        */
        void csrMat4Translate(const CSR_Vector3* pT, CSR_Matrix4* pR);

        /**
        * Gets rotation matrix
        *@param pAngle - rotation angle, in radian
        *@param pAxis - rotation axis
        *@param[out] pR - resulting orthogonal matrix
        */
        void csrMat4Rotate(const float* pAngle, const CSR_Vector3* pAxis, CSR_Matrix4* pR);

        /**
        * Gets scale matrix
        *@param pFactor - scale factor
        *@param[out] pR - resulting scale matrix
        */
        void csrMat4Scale(const CSR_Vector3* pFactor, CSR_Matrix4* pR);

        /**
        * Multiplies matrix by another matrix
        *@param pM1 - first matrix to multiply
        *@param pM2 - second matrix to multiply with
        *@param[out] pR - resulting orthogonal matrix
        */
        void csrMat4Multiply(const CSR_Matrix4* pM1, const CSR_Matrix4* pM2, CSR_Matrix4* pR);

        /**
        * Inverses a matrix
        *@param pM - matrix to inverse
        *@param[out] pR - inversed matrix
        *@param[out] pDeterminant - matrix determinant
        */
        void csrMat4Inverse(const CSR_Matrix4* pM, CSR_Matrix4* pR, float* pDeterminant);

        /**
        * Applies matrix to a vector
        *@param pM - matrix to apply
        *@param pV - vector on which matrix should be applied
        *@param[out] pR - resulting vector
        */
        void csrMat4ApplyToVector(const CSR_Matrix4* pM, const CSR_Vector3* pV, CSR_Vector3* pR);

        /**
        * Applies matrix to a normal
        *@param pM - matrix to apply
        *@param pN - normal on which matrix should be applied
        *@param[out] pR - resulting normal
        */
        void csrMat4ApplyToNormal(const CSR_Matrix4* pM, const CSR_Vector3* pN, CSR_Vector3* pR);

        /**
        * Unprojects a ray (i.e. transforms it in viewport coordinates)
        *@param pP - projection matrix
        *@param pV - view matrix
        *@param[in, out] pR - ray to unproject, unprojected ray on function ends
        *@note The inverted direction is also calculated in the resulting ray
        */
        void csrMat4Unproject(const CSR_Matrix4* pP, const CSR_Matrix4* pV, CSR_Ray3* pR);

        //-------------------------------------------------------------------
        // Quaternion functions
        //-------------------------------------------------------------------

        /**
        * Gets a quaternion from an angle and a vector representing a rotation axis
        *@param pAngle - rotation angle
        *@param pAxis- rotation axis
        *@param[out] pR - resulting quaternion
        */
        void csrQuatFromAxis(float angle, const CSR_Vector3* pAxis, CSR_Quaternion* pR);

        /**
        * Gets a quaternion from Euler angles
        *@param pAngleX - rotation angle on x axis
        *@param pAngleY - rotation angle on y axis
        *@param pAngleZ - rotation angle on z axis
        *@param[out] pR - resulting quaternion
        */
        void csrQuatFromEuler(float angleX, float angleY, float angleZ, CSR_Quaternion* pR);

        /**
        * Gets a quaternion from a pitch, yaw and roll angles
        *@param pitch - the pitch angle in radians, pitch is comparable to a head up/down movement
        *@param yaw - the yaw angle in radians, yaw is comparable to a head left/right movement
        *@param roll - the roll angle in radians, roll is comparable to an aircraft rolling movement
        *@param[out] pR - resulting quaternion
        */
        void csrQuatFromPYR(float pitch, float yaw, float roll, CSR_Quaternion* pR);

        /**
        * Gets the squared length of a quaternion
        *@param pQ - quaternion for which the norm should be get
        *@param[out] pR - the resulting norm
        */
        void csrQuatLengthSquared(const CSR_Quaternion* pQ, float* pR);

        /**
        * Gets the quaternion length
        *@param pQ - quaternion for which the length should be get
        *@param[out] pR - the resulting length
        */
        void csrQuatLength(const CSR_Quaternion* pQ, float* pR);

        /**
        * Normalizes the quaternion
        *@param pV - vector to normalize
        *@param[out] pR - normailzed vector
        */
        void csrQuatNormalize(const CSR_Quaternion* pQ, CSR_Quaternion* pR);

        /**
        * Calculates dot product between 2 quaternions
        *@param pQ1 - first quaternion
        *@param pQ2 - second quaternion
        *@param[out] pR - resulting angle
        */
        void csrQuatDot(const CSR_Quaternion* pQ1, const CSR_Quaternion* pQ2, float* pR);

        /**
        * Scales a quaternion
        *@param pQ - quaternion to scale
        *@param s - scale factor
        *@param[out] pR - scaled quaternion
        */
        void csrQuatScale(const CSR_Quaternion* pQ, float s, CSR_Quaternion* pR);

        /**
        * Conjugates the quaternion
        *@param pQ - quaternion to conjugate
        *@param[out] pR - conjugated quaternion
        */
        void csrQuatConjugate(const CSR_Quaternion* pQ, CSR_Quaternion* pR);

        /**
        * Multiplies a quaternion by another
        *@param pQ1 - first quaternion to multiply
        *@param pQ2 - second quaternion to multiply with
        *@param[out] pR - multiplied quaternion
        */
        void csrQuatMultiply(const CSR_Quaternion* pQ1, const CSR_Quaternion* pQ2, CSR_Quaternion* pR);

        /**
        * Inverse the quaternion
        *@param pQ - quaternion to inverse
        *@param[out] pR - inverted quaternion
        */
        void csrQuatInverse(const CSR_Quaternion* pQ, CSR_Quaternion* pR);

        /**
        * Rotates a vector by a quaternion
        *@param pQ - quaternion by which the vector will be rotated
        *@param pV  vector to rotate
        *@param[out] pR - rotated vector
        */
        void csrQuatRotate(const CSR_Quaternion* pQ, const CSR_Vector3* pV, CSR_Vector3* pR);

        /**
        * Gets the spherical linear interpolated quaternion between 2 quaternions
        *@param pQ1 - quaternion to interpolate
        *@param pQ2 - quaternion to interpolate with
        *@param p - interpolation position, in percent (between 0.0f and 1.0f)
        *@param[out] pR - the resulting spherical linear interpolated quaternion
        *@return 1 on success, otherwise 0
        */
        int csrQuatSlerp(const CSR_Quaternion* pQ1,
                         const CSR_Quaternion* pQ2,
                               float           p,
                               CSR_Quaternion* pR);

        /**
        * Get a quaternion from a matrix
        *@param pM - rotation matrix
        *@param[out] pR - quaternion
        *@return 1 on success, otherwise 0
        */
        int csrQuatFromMatrix(const CSR_Matrix4* pM, CSR_Quaternion* pR);

        /**
        * Gets a rotation matrix from a quaternion
        *@param pQ - quaternion from which the matrix should be get
        *@param[out] pR - rotation matrix
        */
        void csrQuatToMatrix(const CSR_Quaternion* pQ, CSR_Matrix4* pR);

        //-------------------------------------------------------------------
        // Plane functions
        //-------------------------------------------------------------------

        /**
        * Creates a plane from a point and a normal
        *@param pP - point
        *@param pN - normal
        *@param[out] pR - resulting plane
        */
        void csrPlaneFromPointNormal(const CSR_Vector3* pP, const CSR_Vector3* pN, CSR_Plane* pR);

        /**
        * Creates a plane from 3 vertex
        *@param pV1 - first vector
        *@param pV2 - second vector
        *@param pV3 - third vector
        *@param[out] pR - resulting plane
        */
        void csrPlaneFromPoints(const CSR_Vector3* pV1,
                                const CSR_Vector3* pV2,
                                const CSR_Vector3* pV3,
                                      CSR_Plane*   pR);

        /**
        * Calculates a distance between a point and a plane
        *@param pP - point
        *@param pPl - plane
        *@param[out] - resulting distance
        */
        void csrPlaneDistanceTo(const CSR_Vector3* pP, const CSR_Plane* pPl, float* pR);

        //-------------------------------------------------------------------
        // 3D segment functions
        //-------------------------------------------------------------------

        /**
        * Gets the shortest distance between 2 line segments
        *@param pS1 - first line segment
        *@param pS2 - second line segment from which distance should be calculated
        *@param tolerance - tolerance for calculation
        *@param[out] pR - resulting distance
        */
        void csrSeg3DistanceBetween(const CSR_Segment3* pS1,
                                    const CSR_Segment3* pS2,
                                          float         tolerance,
                                          float*        pR);

        /**
        * Calculates and gets the closest point on a line segment from a point
        *@param pS - line segment
        *@param pP - point
        *@param pR - closest point on the line segment
        *@note The closest point is calculated as follow:
        *      pS->m_End
        *          |
        *          |
        *          |
        *       pR x------x pP
        *          |
        *          |
        *          |
        *      pS->m_Start
        */
        void csrSeg3ClosestPoint(const CSR_Segment3* pS, const CSR_Vector3* pP, CSR_Vector3* pR);

        //-------------------------------------------------------------------
        // 3D polygon functions
        //-------------------------------------------------------------------

        /**
        * Calculates and gets the closest point on a polygon edge from a point
        *@param pP - point
        *@param pPo - polygon
        *@param pR - closest point on the polygon edge
        *@note The closest point is calculated as follow:
        *                      pPo->m_Vertex[0]
        *                            /\
        *                           /  \
        *                          /    \
        *                         /      \
        *                        /   pR   \
        *      pPo->m_Vertex[1] /____x_____\ pPo->m_Vertex[2]
        *                            |
        *                            |
        *                            x pP
        */
        void csrPolygon3ClosestPoint(const CSR_Vector3* pP, const CSR_Polygon3* pPo, CSR_Vector3* pR);

        //-------------------------------------------------------------------
        // Inside checks
        //-------------------------------------------------------------------

        /**
        * Checks if a point is inside a 2D polygon
        *@param pP - point to check
        *@param pPo - polygon to check against
        *@return 1 if point is inside the polygon, otherwise 0
        */
        int csrInsidePolygon2(const CSR_Vector2* pP, const CSR_Polygon2* pPo);

        /**
        * Checks if a point is inside a polygon
        *@param pP - point to check
        *@param pPo - polygon to check against
        *@return 1 if point is inside the polygon, otherwise 0
        */
        int csrInsidePolygon3(const CSR_Vector3* pP, const CSR_Polygon3* pPo);

        /**
        * Checks if a point is inside a rectangle
        *@param pP - point to check
        *@param pR - rectangle to check against
        *@return 1 if point is inside the rectangle, otherwise 0
        */
        int csrInsideRect(const CSR_Vector2* pP, const CSR_Rect* pR);

        /**
        * Checks if a point is inside a circle
        *@param pP - point to check
        *@param pC - circle to check against
        *@return 1 if point is inside the circle, otherwise 0
        */
        int csrInsideCircle(const CSR_Vector2* pP, const CSR_Circle* pC);

        /**
        * Checks if a point is inside a box
        *@param pP - point to check
        *@param pB - box to check against
        *@return 1 if point is inside the box, otherwise 0
        */
        int csrInsideBox(const CSR_Vector3* pP, const CSR_Box* pB);

        /**
        * Checks if a point is inside a sphere
        *@param pP - point to check
        *@param pS - sphere to check against
        *@return 1 if point is inside the sphere, otherwise 0
        */
        int csrInsideSphere(const CSR_Vector3* pP, const CSR_Sphere* pS);

        //-------------------------------------------------------------------
        // Intersection checks
        //-------------------------------------------------------------------

        /**
        * Checks if a point intersects a ray
        *@param pP - point
        *@param pR - ray
        *@return 1 if the point intersects the ray, otherwise 0
        */
        int csrIntersectRay2Point(const CSR_Vector2* pP, const CSR_Ray2* pR);

        /**
        * Checks if a ray intesects a circle
        *@param pRa - ray
        *@param pC - circle
        *@param[out] pR - array of 2 vectors containing the instersection points if the shapes intersect
        *@return 1 if the ray intersects the circle, otherwise 0
        *@note The array passed as parameter for pR should be declared as follow: CSR_Vector2 points[2]
        */
        int csrIntersectRay2Circle(const CSR_Ray2* pRa, const CSR_Circle* pC, CSR_Vector2* pR);

        /**
        * Checks if a line segment intesects a circle
        *@param pS - line segment
        *@param pC - circle
        *@param[out] pR - array of 2 vectors containing the instersection points if the shapes intersect
        *@return 1 if the line segment intersects the circle, otherwise 0
        *@note The array passed as parameter for pR should be declared as follow: CSR_Vector2 points[2]
        */
        int csrIntersectSeg2Circle(const CSR_Segment2* pS, const CSR_Circle* pC, CSR_Vector2* pR);

        /**
        * Checks if a ray intesects a plane
        *@param pRa - ray
        *@param pPl - plane
        *@param[out] pR - in case of intersection, the point where the plane intersects the ray
        *@return 1 if the ray intersects the plane, otherwise 0
        */
        int csrIntersectRay3Plane(const CSR_Ray3* pRa, const CSR_Plane* pPl, CSR_Vector3* pR);

        /**
        * Checks if a line segment intesects a plane
        *@param pS - line segment
        *@param pPl - plane
        *@param[out] pR - in case of intersection, the point where the plane intersects the segment
        *@return 1 if the line segment intersects the plane, otherwise 0
        */
        int csrIntersectSeg3Plane(const CSR_Segment3* pS, const CSR_Plane* pPl, CSR_Vector3* pR);

        /**
        * Checks if a ray intersects a polygon
        *@param pRay - ray
        *@param pP - polygon
        *@return 1 if the ray intersects the polygon, otherwise 0
        */
        int csrIntersectRay3Polygon3(const CSR_Ray3* pRay, const CSR_Polygon3* pP);

        /**
        * Checks if a line segment intersects a polygon
        *@param pS - line segment
        *@param pP - polygon
        *@return 1 if the line segment intersects the polygon, otherwise 0
        */
        int csrIntersectSeg3Polygon3(const CSR_Segment3* pS, const CSR_Polygon3* pP);

        /**
        * Checks if a circle intersects another circle
        *@param pC1 - first circle to check
        *@param pC2 - second circle to check against
        *@return 1 if the circles intersect, otherwise 0
        */
        int csrIntersectCircles(const CSR_Circle* pC1, const CSR_Circle* pC2);

        /**
        * Checks if a rect intersects a circle
        *@param pC - circle to check
        *@param pR - rect to check against
        *@return 1 if the circle intersects the rect, otherwise 0
        */
        int csrIntersectCircleRect(const CSR_Circle* pC, const CSR_Rect* pR);

        /**
        * Checks if a rect intersects another rect
        *@param pR1 - first rect to check
        *@param pR2 - second rect to check against
        *@return 1 if the rects intersect, otherwise 0
        */
        int csrIntersectRects(const CSR_Rect* pR1, const CSR_Rect* pR2);

        /**
        * Checks if a ray intersects a box
        *@param pR - ray
        *@param pB - box
        *@return 1 if the ray intersects the box, otherwise 0
        */
        int csrIntersectRayBox(const CSR_Ray3* pR, const CSR_Box* pB);

        /**
        * Checks if a box intersects another box
        *@param pB1 - first box to check
        *@param pB2 - second box to check against
        *@return 1 if the boxes intersect, otherwise 0
        */
        int csrIntersectBoxes(const CSR_Box* pB1, const CSR_Box* pB2);

        /**
        * Checks if a sphere intersects a polygon
        *@param pS - sphere
        *@param pP - polygon
        *@param pR - in case of intersection, the plane that can be used for the sliding
        *@return 1 if ray intersects polygon, otherwise 0
        */
        int csrIntersectSpherePolygon(const CSR_Sphere* pS, const CSR_Polygon3* pP, CSR_Plane* pR);

        /**
        * Checks if a sphere intersects a box
        *@param pS - sphere
        *@param pB - box
        *@return 1 if the sphere intersects the box, otherwise 0
        */
        int csrIntersectSphereBox(const CSR_Sphere* pS, const CSR_Box* pB);

        /**
        * Checks if a sphere intersects another sphere
        *@param pS1 - first sphere to check
        *@param pS2 - second sphere to check against
        *@return 1 if the spheres intersect, otherwise 0
        */
        int csrIntersectSpheres(const CSR_Sphere* pS1, const CSR_Sphere* pS2);

#ifdef __cplusplus
    }
#endif

//---------------------------------------------------------------------------
// Compiler
//---------------------------------------------------------------------------

// needed in mobile c compiler to link the .h file with the .c
#if defined(_OS_IOS_) || defined(_OS_ANDROID_) || defined(_OS_WINDOWS_)
    #include "CSR_Common.c"
#endif

#endif
