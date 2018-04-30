/*****************************************************************************
 * ==> TScreenshotFrame -----------------------------------------------------*
 *****************************************************************************
 * Description : This module contains a frame that allows the user to create *
 *               a screenshot from a model                                   *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef TScreenshotFrameH
#define TScreenshotFrameH

// vcl
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.AppEvnts.hpp>

// compactStar engine
#include "CSR_Collision.h"
#include "CSR_Model.h"
#include "CSR_Scene.h"

// classes
#include "CSR_OpenGLHelper.h"

/**
* Frame allowing to create a screenshot from a model
*@author Jean-Milost Reymond
*/
class TScreenshotFrame : public TFrame
{
    __published:
        TPanel *paBackground;
        TPanel *paLeft;
        TImage *imScreenshot;
        TPanel *paRight;
        TPanel *paColor;
        TLabel *laColorCaption;
        TPanel *paColorValue;
        TColorDialog *cdColor;
        TPanel *paCameraType;
        TRadioButton *rbArcball;
        TRadioButton *rbFirstViewPerson;
        TBevel *blSeparator;
        TLabel *laCameraCaption;
        TPanel *paCameraCaption;
        TPanel *paCameraUp;
        TPanel *paCameraDown;
        TSpeedButton *btCameraLeft;
        TSpeedButton *btCameraUp;
        TSpeedButton *btCameraBack;
        TSpeedButton *btCameraRight;
        TSpeedButton *btCameraDown;
        TSpeedButton *btCameraFront;
        TLabel *laCameraX;
        TLabel *laCameraY;
        TLabel *laCameraZ;
        TPanel *paCamera;

        void __fastcall FrameResize(TObject* pSender);
        void __fastcall btCameraLeftClick(TObject* pSender);
        void __fastcall btCameraRightClick(TObject* pSender);
        void __fastcall btCameraUpClick(TObject* pSender);
        void __fastcall btCameraDownClick(TObject* pSender);
        void __fastcall btCameraBackClick(TObject* pSender);
        void __fastcall btCameraFrontClick(TObject* pSender);
        void __fastcall rbArcballClick(TObject* pSender);
        void __fastcall rbFirstViewPersonClick(TObject* pSender);
        void __fastcall paColorValueClick(TObject* pSender);

    public:
        /**
        * Constructor
        *@param pOwner - frame owner
        */
        __fastcall TScreenshotFrame(TComponent* pOwner);

        __fastcall ~TScreenshotFrame();

        /**
        * Loads the model for which the screenshot should be created
        *@param fileName - model file name to load
        *@return true on success, otherwise false
        */
        bool LoadModel(const std::string& fileName);

        /**
        * Gets a screenshot of the model belonging to the item
        *@param pBitmap - bitmap in which the screenshot should be drawn
        *@return true on success, otherwise false
        */
        bool GetScreenshot(TBitmap* pBitmap) const;

    private:
        CSR_OpenGLHelper::IContext m_SceneContext;
        TForm*                     m_pSceneOverlayForm;
        CSR_Shader*                m_pSceneShader;
        CSR_Color                  m_SceneColor;
        CSR_MDL*                   m_pMDL;
        CSR_Model*                 m_pModel;
        CSR_AABBNode*              m_pAABBTree;
        CSR_ArcBall                m_ArcBall;
        CSR_Camera                 m_Camera;
        CSR_Matrix4                m_ModelMatrix;
        int                        m_AntialiasingFactor;
        float                      m_Offset;

        /**
        * Creates an offscreen scene in which the model can be drawn to be exported as a screenshot
        *@param fileName - model file name to export as a screenshot
        *@param width - screenshot width
        *@param height - screenshot height
        *@return true on success, otherwise false
        */
        bool CreateScene(const std::string& fileName, int width, int height);

        /**
        * Releases a previously created screenshot scene
        */
        void ReleaseScene();

        /**
        * Draws a screenshot scene in a bitmap
        *@param pBitmap - bitmap in which the screenshot should be drawn
        *@return true on success, otherwise false
        */
        bool DrawScene(TBitmap* pBitmap) const;

        /**
        * Draws a screenshot scene and show it in the preview image
        *@return true on success, otherwise false
        */
        bool DrawScene() const;
};
extern PACKAGE TScreenshotFrame* ScreenshotFrame;
#endif
