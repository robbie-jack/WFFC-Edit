#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_PROPERTIES, &MFCMain::MenuEditProperties)
	ON_COMMAND(ID_BUTTON40001, &MFCMain::ToolBarButton1)
	ON_COMMAND(ID_BUTTON40002, &MFCMain::ToolBarWireframe)
	ON_COMMAND(ID_BUTTON40003, &MFCMain::ToolBarNewObject)
	ON_COMMAND(ID_BUTTON40004, &MFCMain::MenuEditProperties)
	ON_COMMAND(ID_BUTTON40005, &MFCMain::ToolBarRiverGeneration)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{	
			std::vector<int> IDs = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString = L"Selected Object: ";

			if (!IDs.empty())
			{
				//statusString += std::to_wstring(*IDs.begin());
				statusString += std::to_wstring(m_ToolSystem.m_sceneGraph[IDs[0]].ID);

				for (int i = 1; i < IDs.size(); i++)
				{
					statusString += (L", " + std::to_wstring(m_ToolSystem.m_sceneGraph[IDs[i]].ID));
				}
			}

			m_ToolSystem.Tick(&msg);

			UpdatePropertiesDialogue();
			UpdateRiverDialogue();

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);
		}
	}

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless

	if (m_ToolSystem.m_selectedObjects.size() > 0)
		m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_selectedObjects[0]);
}

void MFCMain::MenuEditProperties()
{
	if (!m_ToolPropertiesDialogue.IsActive())
	{
		m_ToolPropertiesDialogue.Create(IDD_DIALOG2);
		m_ToolPropertiesDialogue.ShowWindow(SW_SHOW);

		if (m_ToolSystem.m_selectedObjects.size() > 0)
			m_ToolPropertiesDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, m_ToolSystem.m_selectedObjects[0]);
	}
}

void MFCMain::ToolBarRiverGeneration()
{
	m_ToolRiverDialogue.Create(IDD_DIALOG3);
	m_ToolRiverDialogue.ShowWindow(SW_SHOW);
	m_ToolRiverDialogue.SetObjectData(&m_ToolSystem.m_river);
}

void MFCMain::UpdatePropertiesDialogue()
{
	if (m_ToolPropertiesDialogue.IsActive())
	{
		if (m_ToolSystem.m_selectedObjects.size() > 0)
			m_ToolPropertiesDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, m_ToolSystem.m_selectedObjects[0]);

		if (m_ToolPropertiesDialogue.ShouldUpdate())
		{
			int i = m_ToolPropertiesDialogue.GetSelected();
			if (i != -1)
				m_ToolSystem.UpdateObject(i);
		}
	}
}

void MFCMain::UpdateRiverDialogue()
{
	if (m_ToolRiverDialogue.ShouldUpdate())
	{
		m_ToolSystem.GenerateRiver();
	}
}

void MFCMain::ToolBarButton1()
{
	m_ToolSystem.onActionSave();
}

void MFCMain::ToolBarWireframe()
{
	for (auto &sceneObject : m_ToolSystem.m_sceneGraph)
	{
		sceneObject.editor_wireframe = !sceneObject.editor_wireframe;
	}

	m_ToolSystem.UpdateAllObjects();
}

void MFCMain::ToolBarNewObject()
{
	m_ToolSystem.CreateObject();
}

void MFCMain::ToolBarRefreshObjects()
{
	m_ToolSystem.UpdateAllObjects();
}

MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}
