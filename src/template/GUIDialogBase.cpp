#include "include/GUIDialogBase.h"
using namespace std;
using namespace ADDON;

CGUIDialogBase::CGUIDialogBase( std::string xmlFilename, bool ForceFallback, bool AsDialog, std::string DefaultSkin )
{
  m_window = GUI->Window_create(xmlFilename.c_str(), DefaultSkin.c_str(), ForceFallback, AsDialog);
	if(m_window)
	{
		m_window->m_cbhdl     = this;
		m_window->CBOnInit    = OnInitCB;
		m_window->CBOnFocus   = OnFocusCB;
		m_window->CBOnClick   = OnClickCB;
		m_window->CBOnAction  = OnActionCB;
	}
	else
	{
		XBMC->Log(LOG_ERROR, "Couldn't create window dialog %s !", xmlFilename.c_str());
	}
}

CGUIDialogBase::~CGUIDialogBase()
{
	if(m_window)
	{
		GUI->Window_destroy(m_window);
	}
}

bool CGUIDialogBase::OnInitCB(GUIHANDLE cbhdl)
{
  if (cbhdl)
  {
    CGUIDialogBase *dialog = static_cast<CGUIDialogBase*>(cbhdl);
    return dialog->OnInit();
  }

  return false;
}

bool CGUIDialogBase::OnClickCB(GUIHANDLE cbhdl, int controlId)
{
  if (cbhdl)
  {
    CGUIDialogBase *dialog = static_cast<CGUIDialogBase*>(cbhdl);
    return dialog->OnClick(controlId);
  }

  return false;
}

bool CGUIDialogBase::OnFocusCB(GUIHANDLE cbhdl, int controlId)
{
  if (cbhdl)
  {
    CGUIDialogBase *dialog = static_cast<CGUIDialogBase*>(cbhdl);
    return dialog->OnFocus(controlId);
  }

return false;
}

bool CGUIDialogBase::OnActionCB(GUIHANDLE cbhdl, int actionId)
{
  if (cbhdl)
  {
    CGUIDialogBase *dialog = static_cast<CGUIDialogBase*>(cbhdl);
    return dialog->OnAction(actionId);
  }

  return false;
}

bool CGUIDialogBase::Show()
{
	if (m_window)
	{
		return m_window->Show();
	}

	return false;
}

void CGUIDialogBase::Close()
{
	if (m_window)
	{
		m_window->Close();
	}
}

void CGUIDialogBase::DoModal()
{
	if (m_window)
	{
		m_window->DoModal();
	}
}