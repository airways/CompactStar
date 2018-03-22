/****************************************************************************
 * ==> CSR_VCLHelper -------------------------------------------------------*
 ****************************************************************************
 * Description : This module provides an helper class for the VCL           *
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

#ifndef CSR_VCLHelperH
#define CSR_VCLHelperH

// vcl
#include <Vcl.Controls.hpp>

/**
* Helper class for the VCL
*@author Jean-Milost Reymond
*/
class CSR_VCLHelper
{
    public:
        /**
        * Checks if a control is visible, considering also all his parents
        *@param pControl - control to check
        *@return true if the control is visible, otherwise false
        */
        static bool IsVisible(TControl* pControl);
};

#endif