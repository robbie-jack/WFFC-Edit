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
	ON_CBN_SELCHANGE(IDC_COMBO_OBJECT, &PathEditorDialogue::OnCbnSelchangeComboObject)
	ON_BN_CLICKED(IDC_BUTTON_STARTSTOP, &PathEditorDialogue::OnBnClickedButtonStartStop)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &PathEditorDialogue::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &PathEditorDialogue::OnBnClickedButtonDelete)
	ON_EN_CHANGE(IDC_EDIT_TENSION, &PathEditorDialogue::OnEnChangeEditTension)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, &PathEditorDialogue::OnEnChangeEditAlpha)
END_MESSAGE_MAP()

PathEditorDialogue::PathEditorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	m_isActive = false;
	m_playing = false;
	m_shouldUpdate = false;

	m_selected = -1;
	m_currentPath = -1;
	m_currentNode = -1;
	m_currentObject = -1;
}

PathEditorDialogue::~PathEditorDialogue()
{
	m_isActive = false;
	m_playing = false;
	m_shouldUpdate = false;

	m_selected = -1;
	m_currentPath = -1;
	m_currentNode = -1;
	m_currentObject = -1;
}

void PathEditorDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<Path>* Paths)
{
	m_sceneGraph = SceneGraph;
	m_paths = Paths;

	UpdateAIObjectsList();
	UpdatePathComboBox();
	UpdateObjectComboBox();
	UpdateAlphaTensionEdits();
}

void PathEditorDialogue::SetSelected(int Selected)
{
	if (m_selected != Selected)
	{
		m_selected = Selected;
	}
}

bool PathEditorDialogue::UpdateAIObject(float dt)
{
	if (m_currentObject != -1)
	{
		if (m_playing && !m_paths->at(m_currentPath).AtPathEnd())
		{
			Vector3 pathPosition = m_paths->at(m_currentPath).GetNextPoint(dt);
			m_sceneGraph->at(m_currentObject).posX = pathPosition.x;
			m_sceneGraph->at(m_currentObject).posY = pathPosition.y;
			m_sceneGraph->at(m_currentObject).posZ = pathPosition.z;

			return true;
		}
	}

	return false;
}

void PathEditorDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NODE, m_nodeListBox);
	DDX_Control(pDX, IDC_COMBO_PATH, m_pathComboBox);
	DDX_Control(pDX, IDC_COMBO_OBJECT, m_objectComboBox);
	DDX_Control(pDX, IDC_BUTTON_STARTSTOP, m_startstopButton);
	DDX_Control(pDX, IDC_EDIT_TENSION, m_editTension);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_editAlpha);
}

void PathEditorDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again.
	m_isActive = false;
	m_playing = false;
	m_shouldUpdate = false;

	m_selected = -1;
	m_currentPath = -1;
	m_currentNode = -1;
	m_currentObject = -1;
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

	if (m_currentPath != -1)
	{
		std::vector<SceneObject*> nodes = m_paths->at(m_currentPath).GetNodes();

		int numNodes = nodes.size();

		for (int i = 0; i < numNodes; i++)
		{
			std::wstring nodeEntry = L"Node: " + std::to_wstring(i) +  L" - ID: " + std::to_wstring(nodes.at(i)->ID);
			m_nodeListBox.AddString(nodeEntry.c_str());
		}
	}
}

void PathEditorDialogue::UpdateObjectComboBox()
{
	m_objectComboBox.ResetContent();

	int numObjects = m_aiObjects.size();

	for (int i = 0; i < numObjects; i++)
	{
		int j = m_aiObjects.at(i);
		std::wstring comboBoxEntry = L"ID: " + std::to_wstring(m_sceneGraph->at(j).ID);
		m_objectComboBox.AddString(comboBoxEntry.c_str());
	}

	if (m_currentObject != -1)
	{
		m_objectComboBox.SetWindowTextW(L"ID: " + m_sceneGraph->at(m_currentObject).ID);
	}
}

void PathEditorDialogue::UpdateAIObjectsList()
{
	std::vector<int> old_objects = m_aiObjects;
	m_aiObjects.clear();

	if (m_sceneGraph->size() > 0)
	{
		for (int i = 0; i < m_sceneGraph->size(); i++)
		{
			if (m_sceneGraph->at(i).AINode)
			{
				m_aiObjects.push_back(i);
			}
		}

		if (m_aiObjects != old_objects)
			UpdateObjectComboBox();
	}
}

void PathEditorDialogue::UpdateAlphaTensionEdits()
{
	m_editTension.Clear();
	m_editAlpha.Clear();

	if (m_currentPath != -1)
	{
		std::wstring TensionString = std::to_wstring(m_paths->at(m_currentPath).GetTension());
		std::wstring AlphaString = std::to_wstring(m_paths->at(m_currentPath).GetAlpha());

		m_editTension.SetWindowTextW(TensionString.c_str());
		m_editAlpha.SetWindowTextW(AlphaString.c_str());
	}
}

BOOL PathEditorDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_isActive = true;

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
	Path path;
	path.m_name = L"New Path";
	m_paths->push_back(path);

	m_currentPath = m_paths->size() - 1;
	m_currentNode = -1;

	m_playing = false;

	UpdatePathComboBox();
	UpdateNodeListBox();
	UpdateAlphaTensionEdits();
}

void PathEditorDialogue::OnBnClickedButtonAdd()
{
	if (m_currentPath != -1)
	{
		if (m_selected != -1 && m_sceneGraph->at(m_selected).path_node)
			m_paths->at(m_currentPath).AddNode(&m_sceneGraph->at(m_selected)); // Add Currently Selected Node to Path

		m_currentNode = -1;
		m_paths->at(m_currentPath).ResetPath();

		UpdateNodeListBox();
		UpdateAlphaTensionEdits();
	}
}

void PathEditorDialogue::OnCbnSelChangeComboPath()
{
	m_currentPath = m_pathComboBox.GetCurSel();

	UpdateNodeListBox();
	UpdateAlphaTensionEdits();
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
	int index = m_nodeListBox.GetCurSel();
	m_currentNode = index;
}

void PathEditorDialogue::OnCbnSelchangeComboObject()
{
	int index = m_objectComboBox.GetCurSel();
	m_currentObject = m_aiObjects.at(index);
}


void PathEditorDialogue::OnBnClickedButtonStartStop()
{
	if (m_currentPath != -1)
		if (m_paths->at(m_currentPath).GetNodes().size() >= 4)
			m_playing = !m_playing;
		else
			m_playing = false;

	if (m_playing)
		m_startstopButton.SetWindowTextW(L"Stop");
	else
		m_startstopButton.SetWindowTextW(L"Start");
}


void PathEditorDialogue::OnBnClickedButtonReset()
{
	m_paths->at(m_currentPath).ResetPath();
}

void PathEditorDialogue::OnBnClickedButtonDelete()
{
	if (m_currentPath != -1 && m_currentNode != -1)
	{
		m_paths->at(m_currentPath).DeleteNode(m_currentNode);

		UpdateNodeListBox();
	}
}

void PathEditorDialogue::OnEnChangeEditTension()
{
	if (m_currentPath != -1)
	{
		CString tensionText;
		m_editTension.GetWindowTextW(tensionText);

		m_paths->at(m_currentPath).SetTension(_ttof(tensionText));
		m_paths->at(m_currentPath).ResetPath();
	}
}

void PathEditorDialogue::OnEnChangeEditAlpha()
{
	if (m_currentPath != -1)
	{
		CString alphaText;
		m_editAlpha.GetWindowTextW(alphaText);

		m_paths->at(m_currentPath).SetAlpha(_ttof(alphaText));
		m_paths->at(m_currentPath).ResetPath();
	}
}