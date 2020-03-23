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
	void SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<int> Selected);

	inline bool IsActive() { return m_isActive; };
	inline bool ShouldUpdate() { return m_shouldUpdate; m_shouldUpdate = false; };
	inline std::vector<int> GetSelected() { return m_selected; };

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	//afx_msg void Select();	//Item has been selected

	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int> m_selected;

	bool m_isActive;
	bool m_shouldUpdate;

	DECLARE_MESSAGE_MAP()
public:

	CStatic m_static;
	CEdit m_editPosX;
	CEdit m_editPosY;
	CEdit m_editPosZ;
	std::wstring IDstring;
	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditPosz();
	afx_msg void OnStnClickedTextId();
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);