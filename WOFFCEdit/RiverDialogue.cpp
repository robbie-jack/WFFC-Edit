#include "stdafx.h"
#include "RiverDialogue.h"

IMPLEMENT_DYNAMIC(RiverDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(RiverDialogue, CDialogEx)
	ON_COMMAND(IDOK, &RiverDialogue::End)
	ON_BN_CLICKED(IDOK, &RiverDialogue::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_POINTX1, &RiverDialogue::OnEnChangeEditPointx1)
	ON_EN_CHANGE(IDC_EDIT_POINTY1, &RiverDialogue::OnEnChangeEditPointy1)
	ON_EN_CHANGE(IDC_EDIT_POINTX2, &RiverDialogue::OnEnChangeEditPointx2)
	ON_EN_CHANGE(IDC_EDIT_POINTY2, &RiverDialogue::OnEnChangeEditPointy2)
	ON_EN_CHANGE(IDC_EDIT_POINTX3, &RiverDialogue::OnEnChangeEditPointx3)
	ON_EN_CHANGE(IDC_EDIT_POINTY3, &RiverDialogue::OnEnChangeEditPointy3)
	ON_EN_CHANGE(IDC_EDIT_POINTVAL1, &RiverDialogue::OnEnChangeEditPointval1)
	ON_EN_CHANGE(IDC_EDIT_POINTVAL2, &RiverDialogue::OnEnChangeEditPointval2)
	ON_EN_CHANGE(IDC_EDIT_POINTVAL3, &RiverDialogue::OnEnChangeEditPointval3)
END_MESSAGE_MAP()

//RiverDialogue::RiverDialogue(CWnd* parent)
//{
//
//}

RiverDialogue::RiverDialogue(CWnd* pParent) : CDialogEx(IDD_DIALOG3, pParent)
{
	m_shouldUpdate = false;
}

RiverDialogue::~RiverDialogue()
{

}

void RiverDialogue::SetObjectData(River* river)
{
	m_river = river;

	m_riverSection = &river->GetSection(0);

	std::wstring PointX1String = std::to_wstring(m_riverSection->GetPoint(0).i);
	std::wstring PointY1String = std::to_wstring(m_riverSection->GetPoint(0).j);
	std::wstring PointVal1String = std::to_wstring(m_riverSection->GetPoint(0).value);

	std::wstring PointX2String = std::to_wstring(m_riverSection->GetPoint(1).i);
	std::wstring PointY2String = std::to_wstring(m_riverSection->GetPoint(1).j);
	std::wstring PointVal2String = std::to_wstring(m_riverSection->GetPoint(1).value);

	std::wstring PointX3String = std::to_wstring(m_riverSection->GetPoint(2).i);
	std::wstring PointY3String = std::to_wstring(m_riverSection->GetPoint(2).j);
	std::wstring PointVal3String = std::to_wstring(m_riverSection->GetPoint(2).value);

	m_editPointX1.SetWindowTextW(PointX1String.c_str());
	m_editPointY1.SetWindowTextW(PointY1String.c_str());
	m_editPointVal1.SetWindowTextW(PointVal1String.c_str());

	m_editPointX2.SetWindowTextW(PointX2String.c_str());
	m_editPointY2.SetWindowTextW(PointY2String.c_str());
	m_editPointVal2.SetWindowTextW(PointVal2String.c_str());

	m_editPointX3.SetWindowTextW(PointX3String.c_str());
	m_editPointY3.SetWindowTextW(PointY3String.c_str());
	m_editPointVal3.SetWindowTextW(PointVal3String.c_str());
}

void RiverDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_POINTX1, m_editPointX1);
	DDX_Control(pDX, IDC_EDIT_POINTY1, m_editPointY1);
	DDX_Control(pDX, IDC_EDIT_POINTX2, m_editPointX2);
	DDX_Control(pDX, IDC_EDIT_POINTY2, m_editPointY2);
	DDX_Control(pDX, IDC_EDIT_POINTX3, m_editPointX3);
	DDX_Control(pDX, IDC_EDIT_POINTY3, m_editPointY3);
	DDX_Control(pDX, IDC_EDIT_POINTVAL1, m_editPointVal1);
	DDX_Control(pDX, IDC_EDIT_POINTVAL2, m_editPointVal2);
	DDX_Control(pDX, IDC_EDIT_POINTVAL3, m_editPointVal3);
}

void RiverDialogue::End()
{
	DestroyWindow();	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again.
}

BOOL RiverDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void RiverDialogue::PostNcDestroy()
{

}

void RiverDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void RiverDialogue::OnEnChangeEditPointx1()
{
	CString text;
	m_editPointX1.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(0);
	point.i = _ttoi(text);

	m_riverSection->SetPoint(0, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointy1()
{
	CString text;
	m_editPointY1.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(0);
	point.j = _ttoi(text);

	m_riverSection->SetPoint(0, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointx2()
{
	CString text;
	m_editPointX2.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(1);
	point.i = _ttoi(text);

	m_riverSection->SetPoint(1, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointy2()
{
	CString text;
	m_editPointY2.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(1);
	point.j = _ttoi(text);

	m_riverSection->SetPoint(1, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointx3()
{
	CString text;
	m_editPointX3.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(2);
	point.i = _ttoi(text);

	m_riverSection->SetPoint(2, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointy3()
{
	CString text;
	m_editPointY3.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(2);
	point.j = _ttoi(text);

	m_riverSection->SetPoint(2, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointval1()
{
	CString text;
	m_editPointVal1.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(0);
	point.value = _ttoi(text);

	m_riverSection->SetPoint(0, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointval2()
{
	CString text;
	m_editPointVal2.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(1);
	point.value = _ttoi(text);

	m_riverSection->SetPoint(1, point);

	m_shouldUpdate = true;
}

void RiverDialogue::OnEnChangeEditPointval3()
{
	CString text;
	m_editPointVal3.GetWindowTextW(text);

	TerrainPoint point = m_riverSection->GetPoint(2);
	point.value = _ttoi(text);

	m_riverSection->SetPoint(2, point);

	m_shouldUpdate = true;
}