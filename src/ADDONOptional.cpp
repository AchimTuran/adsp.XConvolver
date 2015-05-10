/*
 *      Copyright (C) 2005-2014 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */


//! Example optional class method implementations.
/*!
 * \file ADDONOptional.cpp
 * ToDo: detailed description!
 */
#include "template/include/client.h"
#include "CPUFeatures.h"
#include <string>
using namespace std;
using namespace ADDON;

#include "ADDONOptional.h"

#include "Dialogs/GUIDialogXConvolverSettings.h"

#include "filterManager.h"

#include "LibXConvolverCore/include/LXC_Core.h"

// Helper functions
string GetAddonPath();
extern LXC_OPTIMIZATION_MODULE g_LXC_optModule;

CADDONOptional::CADDONOptional()
{
}

CADDONOptional::~CADDONOptional()
{
}


ADDON_STATUS CADDONOptional::SetSetting(std::string SettingName, const void *SettingValue)
{
	return ADDON_STATUS_OK;
}

unsigned int CADDONOptional::GetSettings(ADDON_StructSetting ***sSet)
{
	return 0;
}

void CADDONOptional::Destroy()
{
  LXC_Core_close();
}

void CADDONOptional::Stop()
{
}

void CADDONOptional::FreeSettings()
{
}

void CADDONOptional::Announce(std::string Flag, std::string Sender, std::string Message, const void *Data)
{
}

void CADDONOptional::ReadSettings(void)
{
}

AE_DSP_ERROR CADDONOptional::CallMenuHook(const AE_DSP_MENUHOOK& Menuhook, const AE_DSP_MENUHOOK_DATA &Item)
{
	switch(Menuhook.iHookId)
	{
    case ID_MENU_XCONVOLVER_SETTINGS:
		  {
        CGUIDialogXConvolverSettings dialog(4);
        dialog.DoModal();
		  }
    break;

    case ID_MENU_XCONVOLVER_INFORMATION:
    {
      CGUIDialogXConvolverSettings dialog(6);
      dialog.DoModal();
    }
    break;

    case ID_MENU_XCONVOLVER_FILTER_MANAGER:
    {
      CGUIDialogXConvolverSettings dialog(0);
      dialog.DoModal();
    }
    break;

    case ID_MENU_XCONVOLVER_CHANNEL_MIXER:
    {
      CGUIDialogXConvolverSettings dialog(3);
      dialog.DoModal();
    }
    break;

	  default:
		  XBMC->Log(LOG_ERROR, "called unknown menu hook!" );
		  return AE_DSP_ERROR_FAILED;
	  break;
	};

	return AE_DSP_ERROR_NO_ERROR;
}

bool CADDONOptional::OptionalInit()
{
  // create filter manager
  LXC_ERROR_CODE err = LXC_Core_init(GetAddonPath().c_str());
  if (err != LXC_NO_ERR)
  {
    // ToDo: show some error message
    return false;
  }

  CFilterManager *filterManager = CFilterManager::Get();
  if (!filterManager)
  {
    XBMC->Log(LOG_ERROR, "Could not create filter manager! Not enough free memory?");
    return false;
  }

  CFilterManager::Get()->LoadFilters();

  unsigned int cpuFeatures = XBMC->GetCPUFeatures();

  if (cpuFeatures & CPU_FEATURE_SSE3)
  {
    g_LXC_optModule = LXC_OPT_SSE3;
    XBMC->Log(LOG_INFO, "XConvolver will use SSE3 optimization for convolution.");
  }
  else
  {
    g_LXC_optModule = LXC_OPT_NATIVE;
    XBMC->Log(LOG_INFO, "XConvolver will use Native optimization for convolution.");
  }

  // check if filter scrapper addon is available
  if (XBMC->HasAddon("script.filter-scrapper"))
  {
    int id = XBMC->ExecutePythonScript("script.filter-scrapper");
    while (id)
    {
      Sleep(100);
      if (!XBMC->IsPythonScriptRunning(id))
      {
        break;
      }
    }
  }

  // now we register menu hooks
	AE_DSP_MENUHOOK hook;

  // register menu hook for XConvolver information
  hook.iHookId            = ID_MENU_XCONVOLVER_INFORMATION;
  hook.category           = AE_DSP_MENUHOOK_INFORMATION;
  hook.iLocalizedStringId = 30000;
  hook.iRelevantModeId    = POST_PROCESS_CONVOLVER_MODE_ID;
  hook.bNeedPlayback      = false;
  ADSP->AddMenuHook(&hook);

  // register menu hook for XConvolver settings
  hook.iHookId            = ID_MENU_XCONVOLVER_SETTINGS;
  hook.category           = AE_DSP_MENUHOOK_SETTING;
  hook.iLocalizedStringId = 30000;
  hook.iRelevantModeId    = POST_PROCESS_CONVOLVER_MODE_ID;
  hook.bNeedPlayback      = false;
  ADSP->AddMenuHook(&hook);

  // register menu hook for XConvolver information
  hook.iHookId            = ID_MENU_XCONVOLVER_INFORMATION;
  hook.category           = AE_DSP_MENUHOOK_INFORMATION;
  hook.iLocalizedStringId = 30000;
  hook.iRelevantModeId    = POST_PROCESS_CHANNEL_MIXER_MODE_ID;
  hook.bNeedPlayback      = false;
  ADSP->AddMenuHook(&hook);

  // register menu hook for XConvolver settings
  hook.iHookId            = ID_MENU_XCONVOLVER_SETTINGS;
  hook.category           = AE_DSP_MENUHOOK_SETTING;
  hook.iLocalizedStringId = 30000;
  hook.iRelevantModeId    = POST_PROCESS_CHANNEL_MIXER_MODE_ID;
  hook.bNeedPlayback      = false;
  ADSP->AddMenuHook(&hook);

  // register menu hook for Convolver settings
  hook.iHookId            = ID_MENU_XCONVOLVER_FILTER_MANAGER;
  hook.category           = AE_DSP_MENUHOOK_POST_PROCESS;
  hook.iLocalizedStringId = 30000;
  hook.iRelevantModeId    = POST_PROCESS_CONVOLVER_MODE_ID;
  hook.bNeedPlayback      = false;
  ADSP->AddMenuHook(&hook);

  // register menu hook for XConvolver settings
  hook.iHookId            = ID_MENU_XCONVOLVER_CHANNEL_MIXER;
  hook.category           = AE_DSP_MENUHOOK_POST_PROCESS;
  hook.iLocalizedStringId = 30000;
  hook.iRelevantModeId    = POST_PROCESS_CHANNEL_MIXER_MODE_ID;
  hook.bNeedPlayback      = false;
  ADSP->AddMenuHook(&hook);
	return true;
}

string GetAddonPath()
{
  string addonPath = g_strAddonPath;
  if (!(addonPath.at(addonPath.size() - 1) == '\\' ||
    addonPath.at(addonPath.size() - 1) == '/'))
  {
#ifdef TARGET_WINDOWS
    addonPath += "\\";
#else
    addonPath += "/";
#endif
  }

  return addonPath;
}