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
	ON_CBN_SELCHANGE(IDC_COMBO_PATH, &PathEditorDialogue::OnCbnSelChangeComboPath)
	ON_CBN_EDITCHANGE(IDC_COMBO_PATH, &PathEditorDialogue::OnCbnEditChangeComboPath)
	ON_CBN_KILLFOCUS(IDC_COMBO_PATH, &PathEditorDialogue::OnCbnLoseFocusComboPath)
	ON_LBN_SELCHANGE(IDC_LIST_NODE, &PathEditorDialogue::OnLbnSelChangeListNode)
END_MESSAGE_MAP()

PathEditorDialogue::PathEditorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	m_isActive = false;
	m_shouldCreatePath = false;
	m_currentPath = -1;
}

PathEditorDialogue::~PathEditorDialogue()
{
	m_isActive = false;
	m_shouldCreatePath = false;
	m_currentPath = -1;
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
	m_currentPath = -1;
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

	if (m_currentPath != -1)
	{
		m_pathComboBox.SetWindowTextW(m_paths->at(m_currentPath).m_name.c_str());
	}
}

void PathEditorDialogue::UpdateNodeListBox()
{
	m_nodeListBox.ResetContent();

	std::vector<PathSegment>* segments = m_paths->at(m_currentPath).GetSegments();

	int numSegments = segments->size();

	for (int i = 0; i < numSegments; i++)
	{
		PathSegment segment = segments->at(i);

		m_nodeListBox.AddString(std::to_wstring(segment.a->ID).c_str());
		m_nodeListBox.AddString(std::to_wstring(segment.b->ID).c_str());
		m_nodeListBox.AddString(std::to_wstring(segment.c->ID).c_str());
		m_nodeListBox.AddString(std::to_wstring(segment.d->ID).c_str());
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
	if (m_currentPath != -1)
	{
		//m_paths->at(m_currentPath).AddSegment();
	}
}

void PathEditorDialogue::OnCbnSelChangeComboPath()
{
	m_currentPath = m_pathComboBox.GetCurSel();

	UpdateNodeListBox();
}

void PathEditorDialogue::OnCbnEditChangeComboPath()
{
	if (m_currentPath != -1)
	{
		CString editText;
		m_pathComboBox.GetWindowTextW(editText);
		m_paths->at(m_currentPath).m_name = editText;
	}
}

void PathEditorDialogue::OnCbnLoseFocusComboPath()
{
	UpdatePathComboBox();
}

void PathEditorDialogue::OnLbnSelChangeListNode()
{
	// TODO: Add your control notification handler code here
}
