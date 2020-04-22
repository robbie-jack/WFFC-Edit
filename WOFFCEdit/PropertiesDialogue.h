#pragma once

#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"

#include <vector>

class PropertiesDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(PropertiesDialogue)

public:

	PropertiesDialogue(CWnd* parent, std::vector<SceneObject>* SceneGraph); // Modal
	PropertiesDialogue(CWnd* parent = NULL); // Modeless
	virtual ~PropertiesDialogue();

	void SetObjectData(std::vector<SceneObject>* SceneGraph);
	void SetSelected(std::vector<int> Selected);
	void UpdateObjectData();
	void ClearData();

	inline bool IsActive() { return m_isActive; };

	inline bool ShouldUpdate() 
	{ 
		if (m_shouldUpdate)
		{
			m_shouldUpdate = false;
			return true;
		}
		else
			return false;
	};

	inline bool ShouldCreate() 
	{ 
		if (m_shouldCreate)
		{
			m_shouldCreate = false;
			return true;
		}
		else
			return false;
	};

	int GetFirstSelected() 
	{ 
		if (m_selected.size() > 0)
			return m_selected[0];
		else
			return -1;
	};
	std::vector<int> GetSelected() { return m_selected; };

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue

	bool CheckFocus();

	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int> m_selected;

	bool m_isActive;		// Is Properties Dialogue Active
	bool m_shouldUpdate;	// Should the currently selected objetc be updated
	bool m_shouldCreate;	// Should a new objetc be created
	bool m_objectUpdated;	// Was yhe current object updated outside the properties window

	DECLARE_MESSAGE_MAP()
public:

	CStatic m_static;

	CEdit m_editPosX;
	CEdit m_editPosY;
	CEdit m_editPosZ;

	CEdit m_editRotX;
	CEdit m_editRotY;
	CEdit m_editRotZ;

	CEdit m_editScaX;
	CEdit m_editScaY;
	CEdit m_editScaZ;

	CEdit m_editName;

	CButton m_buttonWireframe;
	CButton m_buttonAINode;
	CButton m_buttonPathNode;
	CButton m_buttonPathNodeStart;
	CButton m_buttonPathNodeEnd;

	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCreate();
	afx_msg void OnBnClickedDelete();

	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditPosz();

	afx_msg void OnEnChangeEditRotx();
	afx_msg void OnEnChangeEditRoty();
	afx_msg void OnEnChangeEditRotz();

	afx_msg void OnEnChangeEditScax();
	afx_msg void OnEnChangeEditScay();
	afx_msg void OnEnChangeEditScaz();
	afx_msg void OnEnChangeEditName();

	afx_msg void OnBnClickedCheckWireframe();
	
	afx_msg void OnBnClickedCheckAI();
	afx_msg void OnBnClickedCheckPath();
	afx_msg void OnBnClickedCheckPathStart();
	afx_msg void OnBnClickedCheckPathEnd();
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);