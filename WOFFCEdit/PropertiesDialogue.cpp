#include "stdafx.h"
#include "PropertiesDialogue.h"

IMPLEMENT_DYNAMIC(PropertiesDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(PropertiesDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PropertiesDialogue::End)
	ON_COMMAND(IDCANCEL, &PropertiesDialogue::End)
	ON_BN_CLICKED(IDOK, &PropertiesDialogue::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_POSY, &PropertiesDialogue::OnEnChangeEditPosy)
	ON_EN_CHANGE(IDC_EDIT_POSX, &PropertiesDialogue::OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_POSZ, &PropertiesDialogue::OnEnChangeEditPosz)
	ON_EN_CHANGE(IDC_EDIT_SCAX, &PropertiesDialogue::OnEnChangeEditScax)
	ON_EN_CHANGE(IDC_EDIT_ROTX, &PropertiesDialogue::OnEnChangeEditRotx)
	ON_EN_CHANGE(IDC_EDIT_ROTY, &PropertiesDialogue::OnEnChangeEditRoty)
	ON_EN_CHANGE(IDC_EDIT_ROTZ, &PropertiesDialogue::OnEnChangeEditRotz)
	ON_EN_CHANGE(IDC_EDIT_SCAY, &PropertiesDialogue::OnEnChangeEditScay)
	ON_EN_CHANGE(IDC_EDIT_SCAZ, &PropertiesDialogue::OnEnChangeEditScaz)
	ON_EN_CHANGE(IDC_EDIT_NAME, &PropertiesDialogue::OnEnChangeEditName)
	ON_BN_CLICKED(IDC_CHECK_WIREFRAME, &PropertiesDialogue::OnBnClickedCheckWireframe)
	ON_BN_CLICKED(IDC_CREATE, &PropertiesDialogue::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_DELETE, &PropertiesDialogue::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_CHECK_AI, &PropertiesDialogue::OnBnClickedCheckAI)
	ON_BN_CLICKED(IDC_CHECK_PATH, &PropertiesDialogue::OnBnClickedCheckPath)
	ON_BN_CLICKED(IDC_CHECK_PATH_START, &PropertiesDialogue::OnBnClickedCheckPathStart)
	ON_BN_CLICKED(IDC_CHECK_PATH_END, &PropertiesDialogue::OnBnClickedCheckPathEnd)
END_MESSAGE_MAP()

PropertiesDialogue::PropertiesDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)		//constructor used in modal
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_sceneGraph = SceneGraph;
	m_isActive = false;
	m_shouldUpdate = false;
	m_shouldCreate = false;
	m_objectUpdated = false;
	m_changeInCode = false;
	m_selected.clear();
}

PropertiesDialogue::PropertiesDialogue(CWnd* pParent)			//constructor used in modeless
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_isActive = false;
	m_shouldUpdate = false;
	m_shouldCreate = false;
	m_objectUpdated = false;
	m_changeInCode = false;
	m_selected.clear();
}

PropertiesDialogue::~PropertiesDialogue()
{

}

void PropertiesDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph)
{
	m_sceneGraph = SceneGraph;
}

void PropertiesDialogue::SetSelected(std::vector<int> Selected)
{
	if (m_selected != Selected)
	{
		std::vector<int> old_selected = m_selected;
		m_selected = Selected;

		if (m_selected.size() > 0 && old_selected.size() > 0)
		{
			if (old_selected[0] != m_selected[0])
			{
				m_changeInCode = true;
			}
		}
		else if (m_selected.size() > 0 && old_selected.size() == 0)
		{
			m_changeInCode = true;
		}
		else
		{
			ClearData();
		}
	}

	UpdateObjectData();
}

void PropertiesDialogue::UpdateObjectData()
{
	if (m_sceneGraph != nullptr && m_selected.size() > 0)
	{
		if (m_changeInCode)
		{
			SceneObject object = m_sceneGraph->at(m_selected[0]);

			std::wstring IDstring = L"ID:" + std::to_wstring(object.ID);

			std::wstring PosXstring = std::to_wstring(object.posX);
			std::wstring PosYstring = std::to_wstring(object.posY);
			std::wstring PosZstring = std::to_wstring(object.posZ);

			std::wstring RotXstring = std::to_wstring(object.rotX);
			std::wstring RotYstring = std::to_wstring(object.rotY);
			std::wstring RotZstring = std::to_wstring(object.rotZ);

			std::wstring ScaXstring = std::to_wstring(object.scaX);
			std::wstring ScaYstring = std::to_wstring(object.scaY);
			std::wstring ScaZstring = std::to_wstring(object.scaZ);

			m_static.SetWindowTextW(IDstring.c_str());

			//m_editName.SetWindowTextW(std::object.name);

			m_editPosX.SetWindowTextW(PosXstring.c_str());
			m_editPosY.SetWindowTextW(PosYstring.c_str());
			m_editPosZ.SetWindowTextW(PosZstring.c_str());

			m_editRotX.SetWindowTextW(RotXstring.c_str());
			m_editRotY.SetWindowTextW(RotYstring.c_str());
			m_editRotZ.SetWindowTextW(RotZstring.c_str());

			m_editScaX.SetWindowTextW(ScaXstring.c_str());
			m_editScaY.SetWindowTextW(ScaYstring.c_str());
			m_editScaZ.SetWindowTextW(ScaZstring.c_str());

			m_buttonWireframe.SetCheck(object.editor_wireframe);
			m_buttonAINode.SetCheck(object.AINode);
			m_buttonPathNode.SetCheck(object.path_node);
			m_buttonPathNodeStart.SetCheck(object.path_node_start);
			m_buttonPathNodeEnd.SetCheck(object.path_node_end);

			m_changeInCode = false;
		}
	}
}

void PropertiesDialogue::ClearData()
{
	if (m_selected.size() == 0)
	{
		std::wstring IDstring = L"ID:";

		std::wstring PosXstring = L"";
		std::wstring PosYstring = L"";
		std::wstring PosZstring = L"";

		std::wstring RotXstring = L"";
		std::wstring RotYstring = L"";
		std::wstring RotZstring = L"";

		std::wstring ScaXstring = L"";
		std::wstring ScaYstring = L"";
		std::wstring ScaZstring = L"";

		m_static.SetWindowTextW(IDstring.c_str());

		//m_editName.SetWindowTextW(std::object.name);

		m_editPosX.SetWindowTextW(PosXstring.c_str());
		m_editPosY.SetWindowTextW(PosYstring.c_str());
		m_editPosZ.SetWindowTextW(PosZstring.c_str());

		m_editRotX.SetWindowTextW(RotXstring.c_str());
		m_editRotY.SetWindowTextW(RotYstring.c_str());
		m_editRotZ.SetWindowTextW(RotZstring.c_str());

		m_editScaX.SetWindowTextW(ScaXstring.c_str());
		m_editScaY.SetWindowTextW(ScaYstring.c_str());
		m_editScaZ.SetWindowTextW(ScaZstring.c_str());

		m_buttonWireframe.SetCheck(false);
		m_buttonAINode.SetCheck(false);
		m_buttonPathNode.SetCheck(false);
		m_buttonPathNodeStart.SetCheck(false);
		m_buttonPathNodeEnd.SetCheck(false);

		m_selected.clear();
	}
}

void PropertiesDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_TEXT_ID, m_static);

	DDX_Control(pDX, IDC_EDIT_POSX, m_editPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_editPosY);
	DDX_Control(pDX, IDC_EDIT_POSZ, m_editPosZ);

	DDX_Control(pDX, IDC_EDIT_ROTX, m_editRotX);
	DDX_Control(pDX, IDC_EDIT_ROTY, m_editRotY);
	DDX_Control(pDX, IDC_EDIT_ROTZ, m_editRotZ);

	DDX_Control(pDX, IDC_EDIT_SCAX, m_editScaX);
	DDX_Control(pDX, IDC_EDIT_SCAY, m_editScaY);
	DDX_Control(pDX, IDC_EDIT_SCAZ, m_editScaZ);

	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);

	DDX_Control(pDX, IDC_CHECK_WIREFRAME, m_buttonWireframe);
	DDX_Control(pDX, IDC_CHECK_AI, m_buttonAINode);
	DDX_Control(pDX, IDC_CHECK_PATH, m_buttonPathNode);
	DDX_Control(pDX, IDC_CHECK_PATH_START, m_buttonPathNodeStart);
	DDX_Control(pDX, IDC_CHECK_PATH_END, m_buttonPathNodeEnd);
}

void PropertiesDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again.
	m_isActive = false;
	m_shouldUpdate = false;
	m_shouldCreate = false;
	m_changeInCode = false;
	m_selected.clear();
}

BOOL PropertiesDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_isActive = true;
	m_shouldUpdate = false;
	m_shouldCreate = false;
	m_changeInCode = false;
	m_selected.clear();

	return TRUE;
}

void PropertiesDialogue::PostNcDestroy()
{
}

void PropertiesDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void PropertiesDialogue::OnEnChangeEditPosx()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString posXText;
				m_editPosX.GetWindowTextW(posXText);

				object->posX = _ttof(posXText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditPosy()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString posYText;
				m_editPosY.GetWindowTextW(posYText);

				object->posY = _ttof(posYText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditPosz()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString posZText;
				m_editPosZ.GetWindowTextW(posZText);

				object->posZ = _ttof(posZText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditRotx()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString rotXText;
				m_editRotX.GetWindowTextW(rotXText);

				object->rotX = _ttof(rotXText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditRoty()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString rotYText;
				m_editRotY.GetWindowTextW(rotYText);

				object->rotY = _ttof(rotYText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditRotz()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString rotZText;
				m_editRotZ.GetWindowTextW(rotZText);

				object->rotZ = _ttof(rotZText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditScax()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString scaXText;
				m_editScaX.GetWindowTextW(scaXText);

				object->scaX = _ttof(scaXText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditScay()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString scaYText;
				m_editScaY.GetWindowTextW(scaYText);

				object->scaY = _ttof(scaYText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditScaz()
{
	if (!m_changeInCode)
	{
		if (m_selected.size() > 0)
		{
			for (int i = 0; i < m_selected.size(); i++)
			{
				SceneObject* object = &m_sceneGraph->at(m_selected[i]);

				CString scaZText;
				m_editScaZ.GetWindowTextW(scaZText);

				object->scaZ = _ttof(scaZText);
			}

			m_shouldUpdate = true;
		}
	}
}

void PropertiesDialogue::OnEnChangeEditName()
{
	/*if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString nameText;
		m_editName.GetWindowTextW(nameText);

		object->name = _ttoi(nameText);

		m_shouldUpdate = true;
	}*/
}


void PropertiesDialogue::OnBnClickedCheckWireframe()
{
	if (m_selected.size() > 0)
	{
		for (int i = 0; i < m_selected.size(); i++)
		{
			SceneObject* object = &m_sceneGraph->at(m_selected[i]);
			object->editor_wireframe = m_buttonWireframe.GetCheck();
		}

		m_shouldUpdate = true;
	}
}


void PropertiesDialogue::OnBnClickedCreate()
{
	m_shouldCreate = true;
}


void PropertiesDialogue::OnBnClickedDelete()
{
	if (m_selected.size() > 0)
	{
		for (int i = 0; i < m_selected.size(); i++)
		{
			SceneObject* object = &m_sceneGraph->at(m_selected[i]);
			object->is_deleted = true;
		}

		m_selected.clear();
		ClearData();
	}
}


void PropertiesDialogue::OnBnClickedCheckAI()
{
	if (m_selected.size() > 0)
	{
		for (int i = 0; i < m_selected.size(); i++)
		{
			SceneObject* object = &m_sceneGraph->at(m_selected[i]);
			object->AINode = m_buttonAINode.GetCheck();
		}

		m_shouldUpdate = true;
	}
}


void PropertiesDialogue::OnBnClickedCheckPath()
{
	if (m_selected.size() > 0)
	{
		for (int i = 0; i < m_selected.size(); i++)
		{
			SceneObject* object = &m_sceneGraph->at(m_selected[i]);
			bool path_node = m_buttonPathNode.GetCheck();

			// If Object is unmarked as path node it shouldn't be path node start/end either
			if (path_node == false)
			{
				object->path_node_start = path_node;
				object->path_node_end = path_node;

				m_buttonPathNodeStart.SetCheck(path_node);
				m_buttonPathNodeEnd.SetCheck(path_node);
			}

			object->path_node = path_node;
		}

		m_shouldUpdate = true;
	}
}


void PropertiesDialogue::OnBnClickedCheckPathStart()
{
	if (m_selected.size() > 0)
	{
		for (int i = 0; i < m_selected.size(); i++)
		{
			SceneObject* object = &m_sceneGraph->at(m_selected[i]);
			bool path_node_start = m_buttonPathNodeStart.GetCheck();

			// If Object is marked as a Path Node Start it should also be a Path Node
			if (path_node_start == true)
			{
				if (object->path_node == false)
				{
					object->path_node = true;
					m_buttonPathNode.SetCheck(object->path_node);
				}
			}


			object->path_node_start = path_node_start;
		}

		m_shouldUpdate = true;
	}
}


void PropertiesDialogue::OnBnClickedCheckPathEnd()
{
	if (m_selected.size() > 0)
	{
		for (int i = 0; i < m_selected.size(); i++)
		{
			SceneObject* object = &m_sceneGraph->at(m_selected[0]);
			bool path_node_end = m_buttonPathNodeEnd.GetCheck();

			// If Object is marked as a Path Node End it should also be a Path Node
			if (path_node_end == true)
			{
				if (object->path_node == false)
				{
					object->path_node = true;
					m_buttonPathNode.SetCheck(object->path_node);
				}
			}

			object->path_node_end = path_node_end;
		}

		m_shouldUpdate = true;
	}
}
