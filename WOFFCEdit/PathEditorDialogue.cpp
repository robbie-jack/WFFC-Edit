// PathEditorDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "PathEditorDialogue.h"

// PathEditorDialogue dialog

IMPLEMENT_DYNAMIC(PathEditorDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(PathEditorDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PathEditorDialogue::End)
	ON_BN_CLICKED(IDOK, &PathEditorDialogue::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, &PathEditorDialogue::OnCbnSelchangeCombo1)
	ON_LBN_SELCHANGE(IDC_LIST1, &PathEditorDialogue::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &PathEditorDialogue::OnBnClickedButtonAdd)
END_MESSAGE_MAP()

PathEditorDialogue::PathEditorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	m_isActive = false;
	m_shouldAddPath = false;
}

PathEditorDialogue::~PathEditorDialogue()
{
	m_isActive = false;
	m_shouldAddPath = false;
}

void PathEditorDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<Path>* Paths)
{
	m_sceneGraph = SceneGraph;
	m_paths = Paths;
}

void PathEditorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void PathEditorDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again.
	m_isActive = false;
	m_shouldAddPath = false;
}

BOOL PathEditorDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_isActive = true;
	m_shouldAddPath = false;

	return TRUE;
}

void PathEditorDialogue::PostNcDestroy()
{
}

void PathEditorDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

// PathEditorDialogue message handlers

void PathEditorDialogue::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}


void PathEditorDialogue::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
}


void PathEditorDialogue::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
}
