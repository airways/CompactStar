/****************************************************************************
 * ==> CompactStar_GroundCollision -----------------------------------------*
 ****************************************************************************
 * Description : Ground collision tool main entry point                     *
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
#pragma package(smart_init) // madExcept
#pragma link "madExcept"
#pragma link "madLinkDisAsm"
#pragma link "madListHardware"
#pragma link "madListProcesses"
#pragma link "madListModules"

// vcl
#include <tchar.h>

//---------------------------------------------------------------------------
USEFORM("Main.cpp", MainForm);
USEFORM("TSoundSelection.cpp", SoundSelection);
USEFORM("TLandscapeSelection.cpp", LandscapeSelection);
USEFORM("TVector3Frame.cpp", Vector3Frame); /* TFrame: File Type */
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
        Application->Initialize();
        Application->MainFormOnTaskBar = true;
        Application->CreateForm(__classid(TMainForm), &MainForm);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    catch (...)
    {
        try
        {
            throw Exception("");
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
    }

    return 0;
}
//---------------------------------------------------------------------------
