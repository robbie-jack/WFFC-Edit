#pragma once

#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"

#include "SceneObject.h"
#include "Path.h"

#include <vector>

// PathEditorDialogue dialog

class PathEditorDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(PathEditorDialogue)

public:
	PathEditorDialogue(CWnd* pParent = NULL);   // standard constructor
	virtual ~PathEditorDialogue();

	void SetObjectData(std::vector<SceneObject>* SceneGraph, std::vector<Path>* paths);

	inline bool IsActive() { return m_isActive; };

	inline bool ShouldAddPath()
	{
		if (m_shouldAddPath)
		{
			m_shouldAddPath = false;
			return true;
		}
		else
			return false;
	};

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void End();		//kill the dialogue

	std::vector<SceneObject>* m_sceneGraph;
	std::vector<Path>* m_paths;

	bool m_isActive;
	bool m_shouldAddPath;

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedButtonAdd();
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
