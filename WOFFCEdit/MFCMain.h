#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "PropertiesDialogue.h"
#include "RiverDialogue.h"
#include "PathEditorDialogue.h"

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	PropertiesDialogue m_ToolPropertiesDialogue;
	PathEditorDialogue m_ToolPathEditorDialogue;
	//RiverDialogue m_ToolRiverDialogue;

	int m_width;		
	int m_height;

	void UpdatePropertiesDialogue();
	void UpdatePathEditorDialogue();
	//void UpdateRiverDialogue();
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg void MenuEditProperties();
	afx_msg	void ToolBarButton1();
	afx_msg void ToolBarWireframe();
	afx_msg void ToolBarNewObject();
	afx_msg void ToolBarRefreshObjects();
	afx_msg void ToolBarPathEditor();
	//afx_msg void ToolBarRiverGeneration();

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
