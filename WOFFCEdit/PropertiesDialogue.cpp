#include "stdafx.h"
#include "PropertiesDialogue.h"

IMPLEMENT_DYNAMIC(PropertiesDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(PropertiesDialogue, CDialogEx)
	ON_COMMAND(IDOK, &PropertiesDialogue::End)
	ON_BN_CLICKED(IDOK, &PropertiesDialogue::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_POSY, &PropertiesDialogue::OnEnChangeEditPosy)
	ON_EN_CHANGE(IDC_EDIT_POSX, &PropertiesDialogue::OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_POSZ, &PropertiesDialogue::OnEnChangeEditPosz)
END_MESSAGE_MAP()

PropertiesDialogue::PropertiesDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph)		//constructor used in modal
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_sceneGraph = SceneGraph;
	m_isActive = false;
	m_shouldUpdate = false;

}

PropertiesDialogue::PropertiesDialogue(CWnd* pParent)			//constructor used in modeless
	: CDialogEx(IDD_DIALOG2, pParent)
{
	m_isActive = false;
	m_shouldUpdate = false;
}

PropertiesDialogue::~PropertiesDialogue()
{

}

void PropertiesDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<int> Selected)
{
	m_sceneGraph = SceneGraph;

	if (m_selected.size() == 0 || Selected[0] != m_selected[0])
	{
		m_selected = Selected;

		SceneObject object = m_sceneGraph->at(m_selected[0]);

		std::wstring IDstring = L"ID:" + std::to_wstring(object.ID);
		std::wstring PosXstring = std::to_wstring(object.posX);
		std::wstring PosYstring = std::to_wstring(object.posY);
		std::wstring PosZstring = std::to_wstring(object.posZ);

		m_static.SetWindowTextW(IDstring.c_str());

		m_editPosX.SetWindowTextW(PosXstring.c_str());
		m_editPosY.SetWindowTextW(PosYstring.c_str());
		m_editPosZ.SetWindowTextW(PosZstring.c_str());
	}
}

void PropertiesDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_ID, m_static);
	DDX_Control(pDX, IDC_EDIT_POSX, m_editPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_editPosY);
	DDX_Control(pDX, IDC_EDIT_POSZ, m_editPosZ);
}

void PropertiesDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again.
	m_isActive = false;
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

	m_isActive = true;

	return TRUE;
}

void PropertiesDialogue::PostNcDestroy()
{
}




// SelectDialogue message handlers callback   - We only need this if the dailogue is being setup-with createDialogue().  We are doing
//it manually so its better to use the messagemap
/*INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		//	EndDialog(hwndDlg, wParam);
			DestroyWindow(hwndDlg);
			return TRUE;


		case IDCANCEL:
			EndDialog(hwndDlg, wParam);
			return TRUE;
			break;
		}
	}

	return INT_PTR();
}*/


void PropertiesDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void PropertiesDialogue::OnEnChangeEditPosx()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int i = m_selected[0];
	SceneObject* object = &m_sceneGraph->at(i);

	CString posXText;
	m_editPosX.GetWindowTextW(posXText);

	object->posX = _ttoi(posXText);

	m_shouldUpdate = true;
}

void PropertiesDialogue::OnEnChangeEditPosy()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int i = m_selected[0];
	SceneObject* object = &m_sceneGraph->at(i);

	CString posYText;
	m_editPosY.GetWindowTextW(posYText);

	object->posY = _ttoi(posYText);

	m_shouldUpdate = true;
}

void PropertiesDialogue::OnEnChangeEditPosz()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int i = m_selected[0];
	SceneObject* object = &m_sceneGraph->at(i);

	CString posZText;
	m_editPosZ.GetWindowTextW(posZText);

	object->posZ = _ttoi(posZText);

	m_shouldUpdate = true;
}
