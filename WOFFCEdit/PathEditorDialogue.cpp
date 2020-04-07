// PathEditorDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "PathEditorDialogue.h"

// PathEditorDialogue dialog

IMPLEMENT_DYNAMIC(PathEditorDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(PathEditorDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PathEditorDialogue::End)
	ON_BN_CLICKED(IDOK, &PathEditorDialogue::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &PathEditorDialogue::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &PathEditorDialogue::OnBnClickedButtonCreate)
	ON_CBN_SELCHANGE(IDC_COMBO_PATH, &PathEditorDialogue::OnCbnSelchangeComboPath)
	ON_LBN_SELCHANGE(IDC_LIST_NODE, &PathEditorDialogue::OnLbnSelchangeListNode)
END_MESSAGE_MAP()

PathEditorDialogue::PathEditorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	m_isActive = false;
	m_shouldCreatePath = false;
}

PathEditorDialogue::~PathEditorDialogue()
{
	m_isActive = false;
	m_shouldCreatePath = false;
}

void PathEditorDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<Path>* Paths)
{
	m_sceneGraph = SceneGraph;
	m_paths = Paths;

	UpdatePathComboBox();
}

void PathEditorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NODE, m_nodeListBox);
	DDX_Control(pDX, IDC_COMBO_PATH, m_pathComboBox);
}

void PathEditorDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again.
	m_isActive = false;
	m_shouldCreatePath = false;
}

void PathEditorDialogue::UpdatePathComboBox()
{
	m_pathComboBox.ResetContent();

	int numPaths = m_paths->size();

	for (int i = 0; i < numPaths; i++)
	{
		std::wstring comboBoxEntry = m_paths->at(i).m_name;
		m_pathComboBox.AddString(comboBoxEntry.c_str());
	}
}

BOOL PathEditorDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_isActive = true;
	m_shouldCreatePath = false;

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


void PathEditorDialogue::OnBnClickedButtonCreate()
{
	m_shouldCreatePath = true;

	Path path;
	path.m_name = L"New Path";
	m_paths->push_back(path);

	UpdatePathComboBox();
}

void PathEditorDialogue::OnBnClickedButtonAdd()
{

}

void PathEditorDialogue::OnCbnSelchangeComboPath()
{
	// TODO: Add your control notification handler code here
}


void PathEditorDialogue::OnLbnSelchangeListNode()
{
	// TODO: Add your control notification handler code here
}
