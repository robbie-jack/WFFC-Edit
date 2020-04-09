#pragma once

#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"

#include "SceneObject.h"
#include "AIObject.h"
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
	bool UpdateAIObject(float dt);

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

	inline int GetCurrentObject() { return m_currentObject; };

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void End();		//kill the dialogue

	void UpdatePathComboBox();
	void UpdateNodeListBox();
	void UpdateObjectComboBox();

	std::vector<SceneObject>* m_sceneGraph;
	std::vector<int> m_aiObjects;
	std::vector<Path>* m_paths;

	int m_currentPath;
	int m_currentObject;

	bool m_isActive;
	bool m_shouldUpdate;
	bool m_playing;

	DECLARE_MESSAGE_MAP()
public:

	CListBox m_nodeListBox;
	CComboBox m_pathComboBox;
	CComboBox m_objectComboBox;

	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonAdd();

	afx_msg void OnCbnSelChangeComboPath();
	afx_msg void OnCbnEditChangeComboPath();
	afx_msg void OnCbnLoseFocusComboPath();

	afx_msg void OnLbnSelChangeListNode();
	afx_msg void OnCbnSelchangeComboObject();
	afx_msg void OnBnClickedButtonStartStop();
	afx_msg void OnBnClickedButtonReset();
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);
