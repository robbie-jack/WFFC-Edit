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
END_MESSAGE_MAP()

PathEditorDialogue::PathEditorDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	m_isActive = false;
	m_playing = false;
	m_shouldUpdate = false;
	m_currentPath = -1;
	m_currentNode = -1;
	m_currentObject = -1;
}

PathEditorDialogue::~PathEditorDialogue()
{
	m_isActive = false;
	m_playing = false;
	m_shouldUpdate = false;
	m_currentPath = -1;
	m_currentNode = -1;
	m_currentObject = -1;
}

void PathEditorDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<Path>* Paths)
{
	m_sceneGraph = SceneGraph;
	m_paths = Paths;

	for (int i = 0; i < m_sceneGraph->size(); i++)
	{
		if (m_sceneGraph->at(i).AINode)
		{
			m_aiObjects.push_back(i);
		}
	}

	UpdatePathComboBox();
	UpdateObjectComboBox();
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
}

void PathEditorDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again.
	m_isActive = false;
	m_playing = false;
	m_shouldUpdate = false;
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
			std::wstring nodeEntry = std::to_wstring(nodes.at(i)->ID);
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
	//path.AddFirstSegment(nullptr, nullptr, nullptr, nullptr);
	m_paths->push_back(path);

	m_currentPath = m_paths->size() - 1;
	m_currentNode = -1;

	UpdatePathComboBox();
	UpdateNodeListBox();
}

void PathEditorDialogue::OnBnClickedButtonAdd()
{
	if (m_currentPath != -1)
	{
		/*m_paths->at(m_currentPath).AddNextSegment(nullptr, nullptr);
		m_currentSegment = m_paths->at(m_currentPath).GetSegments()->size() - 1;*/
		m_currentNode = -1;

		UpdateNodeListBox();
	}
}

void PathEditorDialogue::OnCbnSelChangeComboPath()
{
	m_currentPath = m_pathComboBox.GetCurSel();

	//UpdateSegmentComboBox();
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
	int index = m_nodeListBox.GetCurSel();
	CString currentNode;

	m_nodeListBox.GetText(index, currentNode);
}

void PathEditorDialogue::OnCbnSelchangeComboObject()
{
	int i = m_objectComboBox.GetCurSel();
	m_currentObject = m_aiObjects.at(i);
}


void PathEditorDialogue::OnBnClickedButtonStartStop()
{
	m_playing = !m_playing;

	if (m_playing)
		m_startstopButton.SetWindowTextW(L"Stop");
	else
		m_startstopButton.SetWindowTextW(L"Start");
}


void PathEditorDialogue::OnBnClickedButtonReset()
{
	m_paths->at(m_currentPath).ResetPath();
}