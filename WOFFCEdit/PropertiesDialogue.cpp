#include "stdafx.h"
#include "PropertiesDialogue.h"

IMPLEMENT_DYNAMIC(PropertiesDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(PropertiesDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PropertiesDialogue::End)
END_MESSAGE_MAP()

PropertiesDialogue::PropertiesDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)		//constructor used in modal
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_sceneGraph = SceneGraph;
}

PropertiesDialogue::PropertiesDialogue(CWnd* pParent)			//constructor used in modeless
	: CDialogEx(IDD_DIALOG2, pParent)
{
}

PropertiesDialogue::~PropertiesDialogue()
{

}

void PropertiesDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<int>* Selected)
{
	m_sceneGraph = SceneGraph;
	m_selected = Selected;

	std::wstring IDstring = L"ID: " + std::to_wstring(m_sceneGraph->at(m_selected->at(0) - 1).ID);
	m_static.SetDlgItemTextW(1, IDstring.c_str());
}

void PropertiesDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC, m_static);
}

void PropertiesDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

//void PropertiesDialogue::Select()
//{
//	int index = m_listBox.GetCurSel();
//	CString currentSelectionValue;
//
//	m_listBox.GetText(index, currentSelectionValue);
//
//	*m_currentSelection = _ttoi(currentSelectionValue);
//
//}

BOOL PropertiesDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}