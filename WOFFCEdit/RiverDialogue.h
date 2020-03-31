#pragma once

#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "River.h"

class RiverDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(RiverDialogue)

public:

	//RiverDialogue(CWnd* parent);		//Model
	RiverDialogue(CWnd* parent = NULL);	//Modeless
	virtual ~RiverDialogue();

	void SetObjectData(River* river);

	inline bool ShouldUpdate() { return m_shouldUpdate; m_shouldUpdate = false; };

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void End();		//kill the dialogue

	River* m_river;
	RiverSection* m_riverSection;

	bool m_shouldUpdate;

	DECLARE_MESSAGE_MAP()
public:

	CEdit m_editPointX1;
	CEdit m_editPointY1;
	CEdit m_editPointVal1;
	CEdit m_editPointX2;
	CEdit m_editPointY2;
	CEdit m_editPointVal2;
	CEdit m_editPointX3;
	CEdit m_editPointY3;
	CEdit m_editPointVal3;

	virtual BOOL OnInitDialog() override;
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();

	afx_msg void OnEnChangeEditPointx1();
	afx_msg void OnEnChangeEditPointy1();
	afx_msg void OnEnChangeEditPointx2();
	afx_msg void OnEnChangeEditPointy2();
	afx_msg void OnEnChangeEditPointx3();
	afx_msg void OnEnChangeEditPointy3();
	afx_msg void OnEnChangeEditPointval1();
	afx_msg void OnEnChangeEditPointval2();
	afx_msg void OnEnChangeEditPointval3();
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);