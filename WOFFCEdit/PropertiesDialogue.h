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

	void SetObjectData(std::vector<SceneObject>* SceneGraph, int Selected);
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
	int GetSelected() { return m_selected; };

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue

	std::vector<SceneObject>* m_sceneGraph;
	int m_selected;

	bool m_isActive;
	bool m_shouldUpdate;
	bool m_shouldCreate;

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
	
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);