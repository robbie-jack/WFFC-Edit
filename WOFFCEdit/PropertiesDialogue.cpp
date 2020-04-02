#include "stdafx.h"
#include "PropertiesDialogue.h"

IMPLEMENT_DYNAMIC(PropertiesDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(PropertiesDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PropertiesDialogue::End)
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
END_MESSAGE_MAP()

PropertiesDialogue::PropertiesDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)		//constructor used in modal
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_sceneGraph = SceneGraph;
	m_isActive = false;
	m_shouldUpdate = false;
	m_shouldCreate = false;
	m_shouldDelete = false;
	m_selected = -1;

}

PropertiesDialogue::PropertiesDialogue(CWnd* pParent)			//constructor used in modeless
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_isActive = false;
	m_shouldUpdate = false;
	m_shouldCreate = false;
	m_shouldDelete = false;
	m_selected = -1;
}

PropertiesDialogue::~PropertiesDialogue()
{

}

void PropertiesDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, int Selected)
{
	m_sceneGraph = SceneGraph;

	if (m_selected != Selected)
	{
		m_selected = Selected;

		SceneObject object = m_sceneGraph->at(m_selected);

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
	}
}

void PropertiesDialogue::ClearData()
{
	std::wstring IDstring = L"";

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

	m_selected = -1;
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
	m_shouldDelete = false;
	m_selected = -1;
}

BOOL PropertiesDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_isActive = true;
	m_shouldUpdate = false;
	m_shouldCreate = false;
	m_shouldDelete = false;

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
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString posXText;
		m_editPosX.GetWindowTextW(posXText);

		object->posX = _ttoi(posXText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditPosy()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString posYText;
		m_editPosY.GetWindowTextW(posYText);

		object->posY = _ttoi(posYText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditPosz()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString posZText;
		m_editPosZ.GetWindowTextW(posZText);

		object->posZ = _ttoi(posZText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditRotx()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString rotXText;
		m_editRotX.GetWindowTextW(rotXText);

		object->rotX = _ttoi(rotXText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditRoty()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString rotYText;
		m_editRotY.GetWindowTextW(rotYText);

		object->rotY = _ttoi(rotYText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditRotz()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString rotZText;
		m_editRotZ.GetWindowTextW(rotZText);

		object->rotZ = _ttoi(rotZText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditScax()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString scaXText;
		m_editScaX.GetWindowTextW(scaXText);

		object->scaX = _ttoi(scaXText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditScay()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString scaYText;
		m_editScaY.GetWindowTextW(scaYText);

		object->scaY = _ttoi(scaYText);

		m_shouldUpdate = true;
	}
}

void PropertiesDialogue::OnEnChangeEditScaz()
{
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);

		CString scaZText;
		m_editScaZ.GetWindowTextW(scaZText);

		object->scaZ = _ttoi(scaZText);

		m_shouldUpdate = true;
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
	if (m_selected != -1)
	{
		SceneObject* object = &m_sceneGraph->at(m_selected);
		object->editor_wireframe = m_buttonWireframe.GetCheck();
	}
}


void PropertiesDialogue::OnBnClickedCreate()
{
	m_shouldCreate = true;
}


void PropertiesDialogue::OnBnClickedDelete()
{
	if (m_selected != -1)
	{
		m_shouldDelete = true;
	}
}
