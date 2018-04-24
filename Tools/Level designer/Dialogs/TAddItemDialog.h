/*****************************************************************************
 * ==> TAddItemDialog -------------------------------------------------------*
 *****************************************************************************
 * Description : This module contains a toolbox dialog to add a scene item   *
 * Developer   : Jean-Milost Reymond                                         *
 * Copyright   : 2015 - 2018, this file is part of the Minimal API. You are  *
 *               free to copy or redistribute this file, modify it, or use   *
 *               it for your own projects, commercial or not. This file is   *
 *               provided "as is", without ANY WARRANTY OF ANY KIND          *
 *****************************************************************************/

#ifndef TAddItemDialogH
#define TAddItemDialogH

// vcl
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.CheckLst.hpp>

// frames
#include "TTextureSelectionFrame.h"
#include "TVertexColorFrame.h"
#include "TFileFrame.h"

// compactStar engine
#include "CSR_Collision.h"
#include "CSR_Model.h"

/**
* Add an item dialog box
*@author Jean-Milost Reymond
*/
class TAddItemDialog : public TForm
{
    __published:
        TPageControl *pcWizard;
        TTabSheet *tsSelectItem;
        TTabSheet *tsConfig;
        TBevel *blSelectItemSeparator;
        TPanel *paNavigation;
        TButton *btOK;
        TButton *btCancel;
        TLabel *laSelectItemTitle;
        TSpeedButton *btSelectItemAddSurface;
        TSpeedButton *btSelectItemAddBox;
        TSpeedButton *btSelectItemAddSphere;
        TSpeedButton *btSelectItemAddCylinder;
        TSpeedButton *btSelectItemAddDisk;
        TSpeedButton *btSelectItemAddSpiral;
        TSpeedButton *btSelectItemAddRing;
        TSpeedButton *btSelectItemAddModel;
        TPanel *paMain;
        TLabel *laConfigTitle;
        TBevel *blConfigSeparator;
        TButton *btBack;
        TTextureSelectionFrame *tsfConfigTexture;
        TLabel *laConfigTextureTitle;
        TLabel *laConfigBumpTitle;
        TTextureSelectionFrame *tsfConfigBump;
        TCheckListBox *clConfigOptions;
        TLabel *laConfigOptions;
        TBevel *blConfigSeparator2;
        TBevel *blConfigSeparator3;
        TBevel *blConfigSeparator4;
        TTabSheet *tsModel;
        TButton *btNext;
        TVertexColorFrame *vcfConfigVertexColor;
        TFileFrame *ffModelFile;
        TBevel *blModelSeparator1;
        TLabel *laModelTitle;
        TLabel *laModelFileNameCaption;
        TImage *imScreenshot;
        TPanel *paModelScreenshot;
        TLabel *laModelScreenshot;
        TPanel *paModelScreenshotLeft;

        void __fastcall FormShow(TObject* pSender);
        void __fastcall btCancelClick(TObject* pSender);
        void __fastcall btBackClick(TObject* pSender);
        void __fastcall OnNextClick(TObject* pSender);
        void __fastcall btOKClick(TObject* pSender);
        void __fastcall OnSelectItemButtonClick(TObject* pSender);

    public:
        /**
        * Model types
        */
        enum IEModelType
        {
            IE_MT_Unknown = 0,
            IE_MT_Surface,
            IE_MT_Box,
            IE_MT_Sphere,
            IE_MT_Cylinder,
            IE_MT_Disk,
            IE_MT_Ring,
            IE_MT_Spiral,
            IE_MT_Model
        };

        /**
        * Constructor
        *@aram pOwner - form owner
        */
        __fastcall TAddItemDialog(TComponent* pOwner);

        virtual __fastcall ~TAddItemDialog();

        /**
        * Gets the selected model type to create
        *@return the model type
        */
        IEModelType GetModelType() const;

    private:
        IEModelType m_ModelType;

        /**
        * Checks if a model file exists
        *@return true if the model file exists, otherwise false
        */
        bool ModelFileExists() const;

        /**
        * Draws the currently selected model in a bitmal
        *@param fileName - model file name to load in a bitmap
        *@param width - bitmap width
        *@param height - bitmap height
        *@param viewMatrix - the view matrix to use
        *@param[in, out] pBitmap - bitmap which will contain the drawn model
        */
        void DrawModelToBitmap(const std::string& fileName,
                                     int          width,
                                     int          height,
                                     CSR_Matrix4  viewMatrix,
                                     TBitmap*     pBitmap);

        /**
        * Called when a file was selected
        *@param pSender - event sender
        *@param fileName - newly selected file name
        */
        void __fastcall OnFileSelected(TObject* pSender, const std::wstring& fileName);
};
extern PACKAGE TAddItemDialog* AddItemDialog;
#endif