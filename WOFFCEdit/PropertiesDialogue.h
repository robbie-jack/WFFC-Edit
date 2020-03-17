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
	void SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<int>* Selected);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue
	//afx_msg void Select();	//Item has been selected

	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int>* m_selected;

	DECLARE_MESSAGE_MAP()
public:

	CStatic m_static;
	virtual BOOL OnInitDialog() override;
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);