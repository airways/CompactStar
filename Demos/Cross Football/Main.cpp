/****************************************************************************
 * ==> Main ----------------------------------------------------------------*
 ****************************************************************************
 * Description : Cross football game main form                              *
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

#include <vcl.h>
#pragma hdrstop
#include "Main.h"

// vcl
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>

// std
#include <memory>
#include <sstream>

// compactStar engine
#include "CSR_Common.h"
#include "CSR_Geometry.h"
#include "CSR_Scene.h"

// classes
#include "CSR_OpenGLHelper.h"
#include "CSR_ShaderHelper.h"

#pragma package(smart_init)
#ifdef __llvm__
    #pragma link "glewSL.a"
#else
    #pragma link "glewSL.lib"
#endif
#pragma link "openAL32E.lib"
#pragma resource "*.dfm"

// resource files to load
#define LANDSCAPE_TEXTURE_FILE "\\football_ground.png"
#define BALL_TEXTURE_FILE      "\\soccer_ball.png"
#define LANDSCAPE_DATA_FILE    "\\level.bmp"
#define SKYBOX_LEFT            "\\left.png"
#define SKYBOX_TOP             "\\top.png"
#define SKYBOX_RIGHT           "\\right.png"
#define SKYBOX_BOTTOM          "\\bottom.png"
#define SKYBOX_FRONT           "\\front.png"
#define SKYBOX_BACK            "\\back.png"
#define PLAYER_STEP_SOUND_FILE "\\running_sound.wav"

//---------------------------------------------------------------------------
// Global defines
//---------------------------------------------------------------------------
#define M_ShootEnergyFactor 23.5f // energy factor for the shoot
//---------------------------------------------------------------------------
// TMainForm
//---------------------------------------------------------------------------
TMainForm* MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* pOwner) :
    TForm(pOwner),
    m_hDC(NULL),
    m_hRC(NULL),
    m_pOpenALDevice(NULL),
    m_pOpenALContext(NULL),
    m_pScene(NULL),
    m_pShader(NULL),
    m_pSkyboxShader(NULL),
    m_pLandscapeKey(NULL),
    m_FrameCount(0),
    m_PrevOrigin(0),
    m_Angle(M_PI / -4.0f),
    m_RollAngle(0.0f),
    m_BallDirAngle(0.0f),
    m_BallOffset(0.0f),
    m_PosVelocity(0.0f),
    m_DirVelocity(0.0f),
    m_StepTime(0.0f),
    m_StepInterval(300.0f),
    m_FPS(0.0),
    m_StartTime(0),
    m_PreviousTime(0),
    m_Initialized(false)
{
    // build the model dir from the application exe
    UnicodeString sceneDir = ::ExtractFilePath(Application->ExeName);
                  sceneDir = ::ExtractFilePath(::ExcludeTrailingPathDelimiter(sceneDir));
                  sceneDir = ::ExtractFilePath(::ExcludeTrailingPathDelimiter(sceneDir));
                  sceneDir = ::ExcludeTrailingPathDelimiter(sceneDir) + L"\\Resources";
                m_SceneDir =   AnsiString(sceneDir).c_str();

    // initialize OpenAL
    csrSoundInitializeOpenAL(&m_pOpenALDevice, &m_pOpenALContext);

    // enable OpenGL
    CSR_OpenGLHelper::EnableOpenGL(Handle, &m_hDC, &m_hRC);

    // stop GLEW crashing on OSX :-/
    glewExperimental = GL_TRUE;

    // initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        // shutdown OpenGL
        CSR_OpenGLHelper::DisableOpenGL(Handle, m_hDC, m_hRC);

        // close the app
        Application->Terminate();
    }
}
//---------------------------------------------------------------------------
__fastcall TMainForm::~TMainForm()
{
    // delete the scene completely
    DeleteScene();

    // shutdown OpenGL
    CSR_OpenGLHelper::DisableOpenGL(Handle, m_hDC, m_hRC);

    // release OpenAL interface
    csrSoundReleaseOpenAL(m_pOpenALDevice, m_pOpenALContext);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject* pSender)
{
    // initialize the scene
    InitScene(ClientWidth, ClientHeight);

    // initialize the timer
    m_StartTime    = ::GetTickCount();
    m_PreviousTime = ::GetTickCount();

    // listen the application idle
    Application->OnIdle = OnIdle;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject* pSender)
{
    if (!m_Initialized)
        return;

    // update the viewport
    CSR_OpenGLHelper::CreateViewport(ClientWidth,
                                     ClientHeight,
                                     0.001f,
                                     1000.0f,
                                     m_pShader,
                                     m_pScene->m_ProjectionMatrix);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::aeEventsMessage(tagMSG& msg, bool& handled)
{
    m_PosVelocity = 0.0f;
    m_DirVelocity = 0.0f;

    switch (msg.message)
    {
        case WM_KEYDOWN:
            switch (msg.wParam)
            {
                case VK_LEFT:  m_DirVelocity = -1.0f; handled = true; break;
                case VK_RIGHT: m_DirVelocity =  1.0f; handled = true; break;
                case VK_UP:    m_PosVelocity =  1.0f; handled = true; break;
                case VK_DOWN:  m_PosVelocity = -1.0f; handled = true; break;

                case VK_SPACE:
                {
                    // get the player position as a circle (i.e. ignore the y axis)
                    CSR_Circle playerCircle;
                    playerCircle.m_Center.m_X = m_ViewSphere.m_Center.m_X;
                    playerCircle.m_Center.m_Y = m_ViewSphere.m_Center.m_Z;
                    playerCircle.m_Radius     = m_ViewSphere.m_Radius + 0.15f;

                    // get the ball position as a circle (i.e. ignore the y axis)
                    CSR_Circle ballCircle;
                    ballCircle.m_Center.m_X = m_Ball.m_Geometry.m_Center.m_X;
                    ballCircle.m_Center.m_Y = m_Ball.m_Geometry.m_Center.m_Z;
                    ballCircle.m_Radius     = m_Ball.m_Geometry.m_Radius;

                    CSR_Figure2 f1;
                    f1.m_Type    = CSR_F2_Circle;
                    f1.m_pFigure = &playerCircle;

                    CSR_Figure2 f2;
                    f2.m_Type    = CSR_F2_Circle;
                    f2.m_pFigure = &ballCircle;

                    // check if the player is closer enough to the ball to shoot it
                    if (csrIntersect2(&f1, &f2, 0, 0))
                    {
                        // calculate the direction and intensity of the shoot
                        CSR_Vector2 distance;
                        csrVec2Sub(&ballCircle.m_Center, &playerCircle.m_Center, &distance);

                        // shoot the ball
                        m_Ball.m_Body.m_Velocity.m_X = M_ShootEnergyFactor * distance.m_X;
                        m_Ball.m_Body.m_Velocity.m_Y = 0.0f;
                        m_Ball.m_Body.m_Velocity.m_Z = M_ShootEnergyFactor * distance.m_Y;
                    }

                    break;
                }

                case 'R':
                    // reset the simulation
                    csrMat4Identity(&m_Ball.m_Matrix);
                    m_Ball.m_Geometry.m_Center.m_X = 0.0f;
                    m_Ball.m_Geometry.m_Center.m_Y = 0.0f;
                    m_Ball.m_Geometry.m_Center.m_Z = 0.0f;
                    m_Ball.m_Body.m_Velocity.m_X   = 0.0f;
                    m_Ball.m_Body.m_Velocity.m_Y   = 0.0f;
                    m_Ball.m_Body.m_Velocity.m_Z   = 0.0f;
                    m_BallDirAngle                 = 0.0f;
                    m_RollAngle                    = 0.0f;
                    break;
            }

            return;
    }
}
//------------------------------------------------------------------------------
CSR_Shader* TMainForm::OnGetShaderCallback(const void* pModel, CSR_EModelType type)
{
    TMainForm* pMainForm = static_cast<TMainForm*>(Application->MainForm);

    if (!pMainForm)
        return 0;

    return pMainForm->OnGetShader(pModel, type);
}
//---------------------------------------------------------------------------
void TMainForm::OnSceneBeginCallback(const CSR_Scene* pScene, const CSR_SceneContext* pContext)
{
    TMainForm* pMainForm = static_cast<TMainForm*>(Application->MainForm);

    if (!pMainForm)
        return;

    pMainForm->OnSceneBegin(pScene, pContext);
}
//---------------------------------------------------------------------------
void TMainForm::OnSceneEndCallback(const CSR_Scene* pScene, const CSR_SceneContext* pContext)
{
    TMainForm* pMainForm = static_cast<TMainForm*>(Application->MainForm);

    if (!pMainForm)
        return;

    pMainForm->OnSceneEnd(pScene, pContext);
}
//---------------------------------------------------------------------------
GLuint TMainForm::LoadTexture(const std::string& fileName) const
{
    try
    {
        // load texture in a picture
        std::auto_ptr<TPicture> pPicture(new TPicture());
        pPicture->LoadFromFile(fileName.c_str());

        // convert it to a bitmap
        std::auto_ptr<TBitmap> pTexture(new TBitmap());
        pTexture->Assign(pPicture->Graphic);

        int pixelSize;

        // search for bitmap pixel format
        switch (pTexture->PixelFormat)
        {
            case pf24bit: pixelSize = 3; break;
            case pf32bit: pixelSize = 4; break;
            default:                     return M_CSR_Error_Code;
        }

        GLuint           textureID    = M_CSR_Error_Code;
        CSR_PixelBuffer* pPixelBuffer = csrPixelBufferCreate();

        try
        {
            // configure the pixel buffer
            pPixelBuffer->m_PixelType    = CSR_PT_BGR;
            pPixelBuffer->m_ImageType    = CSR_IT_Raw;
            pPixelBuffer->m_Width        = pTexture->Width;
            pPixelBuffer->m_Height       = pTexture->Height;
            pPixelBuffer->m_BytePerPixel = pixelSize;
            pPixelBuffer->m_DataLength   = pTexture->Width * pTexture->Height * pixelSize;

            // reserve memory for the pixel array. NOTE use malloc and not new here to be conform
            // with the c standards (otherwise CodeGuard will not be happy)
            pPixelBuffer->m_pData = malloc(pPixelBuffer->m_DataLength);

            TRGBTriple* pLineRGB;
            TRGBQuad*   pLineRGBA;

            // iterate through lines to copy
            for (int y = 0; y < pTexture->Height; ++y)
            {
                // get the next pixel line from bitmap
                if (pixelSize == 3)
                    pLineRGB  = static_cast<TRGBTriple*>(pTexture->ScanLine[y]);
                else
                    pLineRGBA = static_cast<TRGBQuad*>(pTexture->ScanLine[y]);

                // calculate the start y position
                const int yPos = y * pTexture->Width * pixelSize;

                // iterate through pixels to copy
                for (int x = 0; x < pTexture->Width; ++x)
                {
                    // copy to pixel array and take the opportunity to swap the pixel RGB values
                    if (pixelSize == 3)
                    {
                        ((unsigned char*)pPixelBuffer->m_pData)[yPos + (x * 3)]     = pLineRGB[x].rgbtRed;
                        ((unsigned char*)pPixelBuffer->m_pData)[yPos + (x * 3) + 1] = pLineRGB[x].rgbtGreen;
                        ((unsigned char*)pPixelBuffer->m_pData)[yPos + (x * 3) + 2] = pLineRGB[x].rgbtBlue;
                    }
                    else
                    {
                        ((unsigned char*)pPixelBuffer->m_pData)[yPos + (x * 4)]     = pLineRGBA[x].rgbRed;
                        ((unsigned char*)pPixelBuffer->m_pData)[yPos + (x * 4) + 1] = pLineRGBA[x].rgbGreen;
                        ((unsigned char*)pPixelBuffer->m_pData)[yPos + (x * 4) + 2] = pLineRGBA[x].rgbBlue;
                        ((unsigned char*)pPixelBuffer->m_pData)[yPos + (x * 4) + 3] = pLineRGBA[x].rgbReserved;
                    }
                }
            }

            // load the texture on the GPU
            textureID = csrTextureFromPixelBuffer(pPixelBuffer);
        }
        __finally
        {
            csrPixelBufferRelease(pPixelBuffer);
        }

        return textureID;
    }
    catch (...)
    {
        return M_CSR_Error_Code;
    }
}
//---------------------------------------------------------------------------
GLuint TMainForm::LoadCubemap(const IFileNames fileNames) const
{
    try
    {
        GLuint textureID = M_CSR_Error_Code;

        // create new OpenGL texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        const std::size_t fileNameCount = fileNames.size();

        // iterate through the cubemap texture files to load
        for (std::size_t i = 0; i < fileNameCount; i++)
        {
            // load texture in a picture
            std::auto_ptr<TPicture> pPicture(new TPicture());
            pPicture->LoadFromFile(fileNames[i].c_str());

            // convert it to a bitmap
            std::auto_ptr<TBitmap> pTexture(new TBitmap());
            pTexture->Assign(pPicture->Graphic);

            int pixelSize;

            // search for bitmap pixel format
            switch (pTexture->PixelFormat)
            {
                case pf24bit: pixelSize = 3; break;
                case pf32bit: pixelSize = 4; break;
                default:                     return M_CSR_Error_Code;
            }

            CSR_PixelBuffer* pPixelBuffer = csrPixelBufferCreate();

            try
            {
                // configure the pixel buffer
                pPixelBuffer->m_PixelType    = CSR_PT_BGR;
                pPixelBuffer->m_ImageType    = CSR_IT_Raw;
                pPixelBuffer->m_Width        = pTexture->Width;
                pPixelBuffer->m_Height       = pTexture->Height;
                pPixelBuffer->m_BytePerPixel = pixelSize;
                pPixelBuffer->m_DataLength   = pTexture->Width * pTexture->Height * pixelSize;

                // reserve memory for the pixel array. NOTE use malloc and not new here to be conform
                // with the c standards (otherwise CodeGuard will not be happy)
                pPixelBuffer->m_pData = malloc(pPixelBuffer->m_DataLength);

                TRGBTriple* pLineRGB;
                TRGBQuad*   pLineRGBA;

                // iterate through lines to copy
                for (int y = 0; y < pTexture->Height; ++y)
                {
                    // get the next pixel line from bitmap
                    if (pixelSize == 3)
                        pLineRGB  = static_cast<TRGBTriple*>(pTexture->ScanLine[y]);
                    else
                        pLineRGBA = static_cast<TRGBQuad*>(pTexture->ScanLine[y]);

                    // calculate the start y position
                    const int yPos = y * pTexture->Width * pixelSize;

                    // iterate through pixels to copy
                    for (int x = 0; x < pTexture->Width; ++x)
                    {
                        // copy to pixel array
                        if (pixelSize == 3)
                        {
                            const std::size_t offset = yPos + (((pTexture->Width - 1) - x) * 3);

                            ((unsigned char*)pPixelBuffer->m_pData)[offset]     = pLineRGB[x].rgbtRed;
                            ((unsigned char*)pPixelBuffer->m_pData)[offset + 1] = pLineRGB[x].rgbtGreen;
                            ((unsigned char*)pPixelBuffer->m_pData)[offset + 2] = pLineRGB[x].rgbtBlue;
                        }
                        else
                        {
                            const std::size_t offset = yPos + (((pTexture->Width - 1) - x) * 4);

                            ((unsigned char*)pPixelBuffer->m_pData)[offset]     = pLineRGBA[x].rgbRed;
                            ((unsigned char*)pPixelBuffer->m_pData)[offset + 1] = pLineRGBA[x].rgbGreen;
                            ((unsigned char*)pPixelBuffer->m_pData)[offset + 2] = pLineRGBA[x].rgbBlue;
                            ((unsigned char*)pPixelBuffer->m_pData)[offset + 3] = pLineRGBA[x].rgbReserved;
                        }
                    }
                }

                // load the texture on the GPU
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_RGB,
                             pPixelBuffer->m_Width,
                             pPixelBuffer->m_Height,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             pPixelBuffer->m_pData);
            }
            __finally
            {
                csrPixelBufferRelease(pPixelBuffer);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE);

        return textureID;
    }
    catch (...)
    {
        return M_CSR_Error_Code;
    }
}
//------------------------------------------------------------------------------
void TMainForm::InitScene(int w, int h)
{
    // create the default scene
    m_pScene = csrSceneCreate();

    // create the scene context
    csrSceneContextInit(&m_SceneContext);
    m_SceneContext.m_fOnSceneBegin = OnSceneBeginCallback;
    m_SceneContext.m_fOnSceneEnd   = OnSceneEndCallback;
    m_SceneContext.m_fOnGetShader  = OnGetShaderCallback;

    // configure the scene background color
    m_pScene->m_Color.m_R = 0.45f;
    m_pScene->m_Color.m_G = 0.8f;
    m_pScene->m_Color.m_B = 1.0f;
    m_pScene->m_Color.m_A = 1.0f;

    // configure the scene ground direction
    m_pScene->m_GroundDir.m_X =  0.0f;
    m_pScene->m_GroundDir.m_Y = -1.0f;
    m_pScene->m_GroundDir.m_Z =  0.0f;

    // configure the scene view matrix
    csrMat4Identity(&m_pScene->m_ViewMatrix);

    // set the viewpoint bounding sphere default position
    m_ViewSphere.m_Center.m_X = 3.08f;
    m_ViewSphere.m_Center.m_Y = 0.0f;
    m_ViewSphere.m_Center.m_Z = 3.08f;
    m_ViewSphere.m_Radius     = 0.1f;

    // set the ball bounding sphere default position
    m_Ball.m_Geometry.m_Center.m_X = 0.0f;
    m_Ball.m_Geometry.m_Center.m_Y = 0.0f;
    m_Ball.m_Geometry.m_Center.m_Z = 0.0f;
    m_Ball.m_Geometry.m_Radius     = 0.025f;
    csrBodyInit(&m_Ball.m_Body);

    // configure the friction force
    m_FrictionForce.m_X = 0.1f;
    m_FrictionForce.m_Y = 0.1f;
    m_FrictionForce.m_Z = 0.1f;

    const std::string vsTextured = CSR_ShaderHelper::GetVertexShader(CSR_ShaderHelper::IE_ST_Texture);
    const std::string fsTextured = CSR_ShaderHelper::GetFragmentShader(CSR_ShaderHelper::IE_ST_Texture);

    // load the shader
    m_pShader = csrShaderLoadFromStr(vsTextured.c_str(),
                                     vsTextured.length(),
                                     fsTextured.c_str(),
                                     fsTextured.length(),
                                     0,
                                     0);

    // succeeded?
    if (!m_pShader)
    {
        Close();
        return;
    }

    // get shader attributes
    m_pShader->m_VertexSlot   = glGetAttribLocation(m_pShader->m_ProgramID,  "csr_aVertices");
    m_pShader->m_ColorSlot    = glGetAttribLocation(m_pShader->m_ProgramID,  "csr_aColor");
    m_pShader->m_TexCoordSlot = glGetAttribLocation(m_pShader->m_ProgramID,  "csr_aTexCoord");
    m_pShader->m_TextureSlot  = glGetUniformLocation(m_pShader->m_ProgramID, "csr_sTexture");

    // configure OpenGL depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRangef(0.0f, 1.0f);

    // load the landscape
    if (!LoadLandscapeFromBitmap((m_SceneDir + LANDSCAPE_DATA_FILE).c_str()))
    {
        Close();
        return;
    }

    // get back the scene item containing the model
    CSR_SceneItem* pItem = csrSceneGetItem(m_pScene, m_pLandscapeKey);

    // found it?
    if (!pItem)
    {
        Close();
        return;
    }

    // load landscape texture
    ((CSR_Model*)(pItem->m_pModel))->m_pMesh[0].m_Shader.m_TextureID = LoadTexture(m_SceneDir + LANDSCAPE_TEXTURE_FILE);

    CSR_VertexFormat vertexFormat;
    vertexFormat.m_HasNormal         = 0;
    vertexFormat.m_HasPerVertexColor = 1;
    vertexFormat.m_HasTexCoords      = 1;

    CSR_Material material;
    material.m_Color       = 0xFFFFFFFF;
    material.m_Transparent = 0;
    material.m_Wireframe   = 0;

    // create the ball
    CSR_Mesh* pMesh = csrShapeCreateSphere(m_Ball.m_Geometry.m_Radius,
                                           20,
                                           20,
                                          &vertexFormat,
                                           0,
                                          &material,
                                           0);

    // load ball texture
    pMesh->m_Shader.m_TextureID = LoadTexture(m_SceneDir + BALL_TEXTURE_FILE);

    // add the mesh to the scene
    CSR_SceneItem* pSceneItem = csrSceneAddMesh(m_pScene, pMesh, 0, 1);
    csrSceneAddModelMatrix(m_pScene, pMesh, &m_Ball.m_Matrix);

    // configure the ball particle
    m_Ball.m_pKey        = pSceneItem->m_pModel;
    m_Ball.m_Body.m_Mass = 0.3f;

    const std::string vsSkybox = CSR_ShaderHelper::GetVertexShader(CSR_ShaderHelper::IE_ST_Skybox);
    const std::string fsSkybox = CSR_ShaderHelper::GetFragmentShader(CSR_ShaderHelper::IE_ST_Skybox);

    // load the skybox shader
    m_pSkyboxShader = csrShaderLoadFromStr(vsSkybox.c_str(),
                                           vsSkybox.length(),
                                           fsSkybox.c_str(),
                                           fsSkybox.length(),
                                           0,
                                           0);

    // succeeded?
    if (!m_pSkyboxShader)
    {
        Close();
        return;
    }

    // enable the shader program
    csrShaderEnable(m_pSkyboxShader);

    // get shader attributes
    m_pSkyboxShader->m_VertexSlot  = glGetAttribLocation (m_pSkyboxShader->m_ProgramID, "csr_aVertices");
    m_pSkyboxShader->m_CubemapSlot = glGetUniformLocation(m_pSkyboxShader->m_ProgramID, "csr_sCubemap");

    // create the skybox
    m_pScene->m_pSkybox = csrSkyboxCreate(1.0f, 1.0f, 1.0f);

    // succeeded?
    if (!m_pScene->m_pSkybox)
    {
        Close();
        return;
    }

    IFileNames cubemapFileNames;
    cubemapFileNames.push_back(m_SceneDir + SKYBOX_RIGHT);
    cubemapFileNames.push_back(m_SceneDir + SKYBOX_LEFT);
    cubemapFileNames.push_back(m_SceneDir + SKYBOX_TOP);
    cubemapFileNames.push_back(m_SceneDir + SKYBOX_BOTTOM);
    cubemapFileNames.push_back(m_SceneDir + SKYBOX_FRONT);
    cubemapFileNames.push_back(m_SceneDir + SKYBOX_BACK);

    // load the cubemap texture
    m_pScene->m_pSkybox->m_Shader.m_CubeMapID = LoadCubemap(cubemapFileNames);

    // load step sound file
    m_pSound = csrSoundOpenWavFile(m_pOpenALDevice, m_pOpenALContext, (m_SceneDir + PLAYER_STEP_SOUND_FILE).c_str());

    m_Initialized = true;
}
//------------------------------------------------------------------------------
void TMainForm::DeleteScene()
{
    m_Initialized = false;

    // release the shaders
    csrShaderRelease(m_pShader);
    csrShaderRelease(m_pSkyboxShader);

    // release the scene
    csrSceneRelease(m_pScene);

    // stop running step sound, if needed
    csrSoundStop(m_pSound);

    // release OpenAL interface
    csrSoundRelease(m_pSound);
}
//------------------------------------------------------------------------------
void TMainForm::UpdateScene(float elapsedTime)
{
    ApplyPhysics(elapsedTime);

    // is player rotating?
    if (m_DirVelocity)
    {
        // calculate the player direction
        m_Angle += m_DirVelocity * elapsedTime;

        // validate and apply it
        if (m_Angle > M_PI * 2.0f)
            m_Angle -= M_PI * 2.0f;
        else
        if (m_Angle < 0.0f)
            m_Angle += M_PI * 2.0f;
    }

    // keep the old position to revert it in case of error
    const CSR_Vector3 oldPos = m_ViewSphere.m_Center;

    // is player moving?
    if (m_PosVelocity)
    {
        // calculate the next position
        m_ViewSphere.m_Center.m_X += m_PosVelocity * sinf(m_Angle) * elapsedTime;
        m_ViewSphere.m_Center.m_Z -= m_PosVelocity * cosf(m_Angle) * elapsedTime;

        // calculate next time where the step sound should be played
        m_StepTime += (elapsedTime * 1000.0f);

        // do play the sound?
        if (m_StepTime > m_StepInterval)
        {
            csrSoundStop(m_pSound);
            csrSoundPlay(m_pSound);
            m_StepTime = 0.0f;
        }
    }

    // enable the shader program
    csrShaderEnable(m_pShader);

    CSR_Plane groundPlane;

    // calculate the ground position and check if next position is valid
    if (!ApplyGroundCollision(&m_ViewSphere, m_Angle, &m_pScene->m_ViewMatrix, &groundPlane))
    {
        // invalid next position, get the scene item (just one for this scene)
        const CSR_SceneItem* pItem = csrSceneGetItem(m_pScene, m_pLandscapeKey);

        // found it?
        if (pItem)
        {
            // check if the x position is out of bounds, and correct it if yes
            if (m_ViewSphere.m_Center.m_X <= pItem->m_pAABBTree->m_pBox->m_Min.m_X ||
                m_ViewSphere.m_Center.m_X >= pItem->m_pAABBTree->m_pBox->m_Max.m_X)
                m_ViewSphere.m_Center.m_X = oldPos.m_X;

            // do the same thing with the z position. Doing that separately for each axis will make
            // the point of view to slide against the landscape border (this is possible because the
            // landscape is axis-aligned)
            if (m_ViewSphere.m_Center.m_Z <= pItem->m_pAABBTree->m_pBox->m_Min.m_Z ||
                m_ViewSphere.m_Center.m_Z >= pItem->m_pAABBTree->m_pBox->m_Max.m_Z)
                m_ViewSphere.m_Center.m_Z = oldPos.m_Z;
        }
        else
            // failed to get the scene item, just revert the position
            m_ViewSphere.m_Center = oldPos;

        // recalculate the ground value (this time the collision result isn't tested, because the
        // previous position is always considered as valid)
        ApplyGroundCollision(&m_ViewSphere, m_Angle, &m_pScene->m_ViewMatrix, &groundPlane);
    }
}
//------------------------------------------------------------------------------
void TMainForm::DrawScene()
{
    // draw the scene
    csrSceneDraw(m_pScene, &m_SceneContext);
}
//---------------------------------------------------------------------------
int TMainForm::LoadLandscapeFromBitmap(const char* fileName)
{
    CSR_Material      material;
    CSR_VertexCulling vc;
    CSR_VertexFormat  vf;
    CSR_Model*        pModel;
    CSR_PixelBuffer*  pBitmap;
    CSR_SceneItem*    pSceneItem;

    material.m_Color       = 0xFFFFFFFF;
    material.m_Transparent = 0;
    material.m_Wireframe   = 0;

    vc.m_Type = CSR_CT_None;
    vc.m_Face = CSR_CF_CW;

    vf.m_HasNormal         = 0;
    vf.m_HasTexCoords      = 1;
    vf.m_HasPerVertexColor = 1;

    // create a model to contain the landscape
    pModel = csrModelCreate();

    // succeeded?
    if (!pModel)
        return 0;

    // load a default grayscale bitmap from which a landscape will be generated
    pBitmap = csrPixelBufferFromBitmapFile(fileName);

    // succeeded?
    if (!pBitmap)
    {
        csrModelRelease(pModel);
        return 0;
    }

    // load the landscape mesh from the grayscale bitmap
    pModel->m_pMesh     = csrLandscapeCreate(pBitmap, 3.0f, 0.2f, &vf, &vc, &material, 0);
    pModel->m_MeshCount = 1;

    csrPixelBufferRelease(pBitmap);

    csrMat4Identity(&m_LandscapeMatrix);

    // add the model to the scene
    pSceneItem = csrSceneAddModel(m_pScene, pModel, 0, 1);
    csrSceneAddModelMatrix(m_pScene, pModel, &m_LandscapeMatrix);

    // succeeded?
    if (pSceneItem)
        pSceneItem->m_CollisionType = CSR_CO_Ground;

    // keep the key
    m_pLandscapeKey = pModel;

    return 1;
}
//---------------------------------------------------------------------------
int TMainForm::ApplyGroundCollision(const CSR_Sphere*  pBoundingSphere,
                                          float        dir,
                                          CSR_Matrix4* pMatrix,
                                          CSR_Plane*   pGroundPlane) const
{
    if (!m_pScene)
        return 0;

    // validate the inputs
    if (!pBoundingSphere || !pMatrix)
        return 0;

    CSR_CollisionInput collisionInput;
    csrCollisionInputInit(&collisionInput);
    collisionInput.m_BoundingSphere.m_Radius = pBoundingSphere->m_Radius;

    CSR_Camera camera;

    // calculate the camera position in the 3d world, without the ground value
    camera.m_Position.m_X = -pBoundingSphere->m_Center.m_X;
    camera.m_Position.m_Y =  0.0f;
    camera.m_Position.m_Z = -pBoundingSphere->m_Center.m_Z;
    camera.m_xAngle       =  0.0f;
    camera.m_yAngle       =  dir;
    camera.m_zAngle       =  0.0f;
    camera.m_Factor.m_X   =  1.0f;
    camera.m_Factor.m_Y   =  1.0f;
    camera.m_Factor.m_Z   =  1.0f;
    camera.m_MatCombType  =  IE_CT_Translate_Scale_Rotate;

    // get the view matrix matching with the camera
    csrSceneCameraToMatrix(&camera, pMatrix);

    CSR_Vector3 modelCenter;

    // get the model center
    modelCenter.m_X = 0.0f;
    modelCenter.m_Y = 0.0f;
    modelCenter.m_Z = 0.0f;

    CSR_Matrix4 invertMatrix;
    float       determinant;

    // calculate the current camera position above the landscape
    csrMat4Inverse(pMatrix, &invertMatrix, &determinant);
    csrMat4Transform(&invertMatrix, &modelCenter, &collisionInput.m_BoundingSphere.m_Center);
    collisionInput.m_CheckPos = collisionInput.m_BoundingSphere.m_Center;

    CSR_CollisionOutput collisionOutput;

    // calculate the collisions in the whole scene
    csrSceneDetectCollision(m_pScene, &collisionInput, &collisionOutput, 0);

    // update the ground position directly inside the matrix (this is where the final value is required)
    pMatrix->m_Table[3][1] = -collisionOutput.m_GroundPos;

    // get the resulting plane
    *pGroundPlane = collisionOutput.m_GroundPlane;

    if (collisionOutput.m_Collision & CSR_CO_Ground)
        return 1;

    return 0;
}
//---------------------------------------------------------------------------
void TMainForm::ApplyPhysics(float elapsedTime)
{
    float       gravity;
    float       thetaX;
    float       thetaZ;
    CSR_Plane   groundPlane;
    CSR_Vector3 planeNormal;
    CSR_Vector3 acceleration;
    CSR_Vector3 prevCenter;

    // apply the ground collision on the current position and get the ground polygon
    ApplyGroundCollision(&m_Ball.m_Geometry, 0.0f, &m_Ball.m_Matrix, &groundPlane);

    // get the normal of the plane
    planeNormal.m_X = groundPlane.m_A;
    planeNormal.m_Y = groundPlane.m_B;
    planeNormal.m_Z = groundPlane.m_C;

    // calculate the next ball roll position
    csrPhysicsRoll(&planeNormal, m_Ball.m_Body.m_Mass, 0.0025f, elapsedTime, &m_Ball.m_Body.m_Velocity);

    // keep the previous ball position
    prevCenter = m_Ball.m_Geometry.m_Center;

    // calculate the new position ( using the formula pos = pos + (v * dt))
    m_Ball.m_Geometry.m_Center.m_X += m_Ball.m_Body.m_Velocity.m_X * elapsedTime;
    m_Ball.m_Geometry.m_Center.m_Y += m_Ball.m_Body.m_Velocity.m_Y * elapsedTime;
    m_Ball.m_Geometry.m_Center.m_Z += m_Ball.m_Body.m_Velocity.m_Z * elapsedTime;

    // check if the new position is valid
    if (!ApplyGroundCollision(&m_Ball.m_Geometry, 0.0f, &m_Ball.m_Matrix, &groundPlane))
    {
        // do perform a rebound on the x or z axis?
        const bool xRebound = m_Ball.m_Geometry.m_Center.m_X <= -3.08f || m_Ball.m_Geometry.m_Center.m_X >= 3.08f;
        const bool zRebound = m_Ball.m_Geometry.m_Center.m_Z <= -3.08f || m_Ball.m_Geometry.m_Center.m_Z >= 3.08f;

        // reset the ball to the previous position
        m_Ball.m_Geometry.m_Center = prevCenter;

        // do perform a rebound on the x axis?
        if (xRebound)
            m_Ball.m_Body.m_Velocity.m_X = -m_Ball.m_Body.m_Velocity.m_X;

        // do perform a rebound on the z axis?
        if (zRebound)
            m_Ball.m_Body.m_Velocity.m_Z = -m_Ball.m_Body.m_Velocity.m_Z;
    }
    else
    {
        float       distance;
        float       ballDirAngle;
        CSR_Matrix4 rxMatrix;
        CSR_Matrix4 ryMatrix;
        CSR_Matrix4 rzMatrix;
        CSR_Matrix4 rMatrix;
        CSR_Matrix4 ballMatrix;
        CSR_Vector3 rollDistance;
        CSR_Vector3 rollDir;
        CSR_Vector3 axis;

        // calculate the rolling angle (depends on the distance the ball moved)
        csrVec3Sub(&m_Ball.m_Geometry.m_Center, &prevCenter, &rollDistance);
        csrVec3Length(&rollDistance, &distance);
        m_RollAngle = std::fmod(m_RollAngle + (distance * 10.0f), M_PI * 2.0f);

        axis.m_X = 1.0f;
        axis.m_Y = 0.0f;
        axis.m_Z = 0.0f;

        // the ball moved since the last frame?
        if (distance)
        {
            // calculate the new ball direction angle
            csrVec3Normalize(&rollDistance, &rollDir);
            csrVec3Dot(&rollDir, &axis, &m_BallDirAngle);
        }

        // calculate the rotation matrix on the x axis
        csrMat4Rotate(m_RollAngle, &axis, &rxMatrix);

        axis.m_X = 0.0f;
        axis.m_Y = 1.0f;
        axis.m_Z = 0.0f;

        // ball moved on the z axis since the last frame?
        if (rollDistance.m_Z)
            // calculate the offset to apply to the ball direction
            m_BallOffset = (rollDistance.m_Z > 0.0f ? 1.0f : -1.0f);

        // calculate the rotation matrix on the y axis
        csrMat4Rotate((M_PI * 2.0f) - ((std::acos(m_BallDirAngle) * m_BallOffset) - (M_PI / 2.0f)),
                      &axis,
                      &ryMatrix);

        // build the final matrix
        csrMat4Multiply(&rxMatrix, &ryMatrix, &ballMatrix);

        // replace the ball in the model coordinate system (do that directly on the matrix)
        ballMatrix.m_Table[3][0] = -m_Ball.m_Matrix.m_Table[3][0];
        ballMatrix.m_Table[3][1] = -m_Ball.m_Matrix.m_Table[3][1];
        ballMatrix.m_Table[3][2] = -m_Ball.m_Matrix.m_Table[3][2];

        m_Ball.m_Matrix = ballMatrix;
    }
}
//---------------------------------------------------------------------------
void TMainForm::OnDrawScene(bool resize)
{
    // do draw the scene for a resize?
    if (resize)
    {
        if (!m_Initialized)
            return;

        // just process a minimal draw
        UpdateScene(0.0);
        DrawScene();

        ::SwapBuffers(m_hDC);
        return;
    }

    // calculate time interval
    const unsigned __int64 now            = ::GetTickCount();
    const double           elapsedTime    = (now - m_PreviousTime) / 1000.0;
                           m_PreviousTime =  now;

    if (!m_Initialized)
        return;

    ++m_FrameCount;

    // calculate the FPS
    if (m_FrameCount >= 100)
    {
        const double      smoothing = 0.1;
        const std::size_t fpsTime   = now > m_StartTime ? now - m_StartTime : 1;
        const std::size_t newFPS    = (m_FrameCount * 100) / fpsTime;
        m_StartTime                 = ::GetTickCount();
        m_FrameCount                = 0;
        m_FPS                       = (newFPS * smoothing) + (m_FPS * (1.0 - smoothing));
    }

    // update and draw the scene
    UpdateScene(elapsedTime);
    DrawScene();

    ::SwapBuffers(m_hDC);
}
//---------------------------------------------------------------------------
CSR_Shader* TMainForm::OnGetShader(const void* pModel, CSR_EModelType type)
{
    if (pModel == m_pScene->m_pSkybox)
        return m_pSkyboxShader;

    return m_pShader;
}
//---------------------------------------------------------------------------
void TMainForm::OnSceneBegin(const CSR_Scene* pScene, const CSR_SceneContext* pContext)
{
    csrDrawBegin(&pScene->m_Color);
}
//---------------------------------------------------------------------------
void TMainForm::OnSceneEnd(const CSR_Scene* pScene, const CSR_SceneContext* pContext)
{
    csrDrawEnd();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OnIdle(TObject* pSender, bool& done)
{
    done = false;
    OnDrawScene(false);
}
//---------------------------------------------------------------------------