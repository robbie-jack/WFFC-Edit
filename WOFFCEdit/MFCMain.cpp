#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_PROPERTIES, &MFCMain::MenuEditProperties)
	ON_COMMAND(ID_EDIT_PATHEDITOR, &MFCMain::ToolBarPathEditor)
	ON_COMMAND(ID_BUTTON40001, &MFCMain::ToolBarButtonSave)
	ON_COMMAND(ID_BUTTON40002, &MFCMain::ToolBarWireframe)
	ON_COMMAND(ID_BUTTON40003, &MFCMain::ToolBarNewObject)
	ON_COMMAND(ID_BUTTON40004, &MFCMain::MenuEditProperties)
	ON_COMMAND(ID_BUTTON40005, &MFCMain::ToolBarPathEditor)
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
	m_toolHandle = m_frame->GetSafeHwnd();
	m_renderHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_renderHandle, m_width, m_height);

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
			std::wstring statusString;

			switch (m_ToolSystem.m_manipulationState)
			{
			case ObjectManipulationState::Translate:
				statusString = L"Object State: Translate";
				break;
			case ObjectManipulationState::Rotate:
				statusString = L"Object State: Rotate";
				break;
			case ObjectManipulationState::Scale:
				statusString = L"Object State: Scale";
				break;
			}

			statusString += L", Selected Object: ";

			if (!IDs.empty())
			{
				//statusString += std::to_wstring(*IDs.begin());
				statusString += std::to_wstring(m_ToolSystem.m_sceneGraph[IDs[0]].ID);

				for (int i = 1; i < IDs.size(); i++)
				{
					statusString += (L", " + std::to_wstring(m_ToolSystem.m_sceneGraph[IDs[i]].ID));
				}
			}

			if (m_frame->IsResized())
			{
				m_frame->m_DirXView.GetClientRect(&WindowRECT);
				m_width = WindowRECT.Width();
				m_height = WindowRECT.Height();

				m_ToolSystem.UpdateWindow(m_width, m_height);
			}

			float dt = m_ToolSystem.Tick(&msg);

			UpdatePropertiesDialogue();
			UpdatePathEditorDialogue(dt);
			//UpdateRiverDialogue();

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

		m_ToolPropertiesDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph);
	}
}

//void MFCMain::ToolBarRiverGeneration()
//{
//	m_ToolRiverDialogue.Create(IDD_DIALOG3);
//	m_ToolRiverDialogue.ShowWindow(SW_SHOW);
//	m_ToolRiverDialogue.SetObjectData(&m_ToolSystem.m_river);
//}

void MFCMain::UpdatePropertiesDialogue()
{
	if (m_ToolPropertiesDialogue.IsActive())
	{
		if (m_ToolSystem.ObjectUpdated())
		{
			m_ToolPropertiesDialogue.SetChangeInCode();
		}

		m_ToolPropertiesDialogue.SetSelected(m_ToolSystem.m_selectedObjects);

		if (m_ToolPropertiesDialogue.ShouldUpdate())
		{
			int i = m_ToolPropertiesDialogue.GetFirstSelected();
			if (i != -1)
				m_ToolSystem.UpdateObject(i);
		}

		if (m_ToolPropertiesDialogue.ShouldCreate())
		{
			m_ToolSystem.CreateSceneObject();
			m_ToolPropertiesDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph);
			m_ToolPropertiesDialogue.SetSelected(m_ToolSystem.m_selectedObjects);
		}
	}
}

void MFCMain::UpdatePathEditorDialogue(float dt)
{
	if (m_ToolPathEditorDialogue.IsActive())
	{
		if (m_ToolPathEditorDialogue.UpdateAIObject(dt))
		{
			int i = m_ToolPathEditorDialogue.GetCurrentObject();
			if (i != -1)
				m_ToolSystem.UpdateObject(i);
		}

		m_ToolPathEditorDialogue.UpdateAIObjectsList();

		if (m_ToolSystem.m_selectedObjects.size() > 0)
			m_ToolPathEditorDialogue.SetSelected(m_ToolSystem.m_selectedObjects[0]);

		if (m_ToolPathEditorDialogue.GetCurrentPath() != -1)
		{
			m_ToolSystem.DrawPath(m_ToolSystem.m_paths[m_ToolPathEditorDialogue.GetCurrentPath()]);
		}
	}
}

//void MFCMain::UpdateRiverDialogue()
//{
//	if (m_ToolRiverDialogue.ShouldUpdate())
//	{
//		m_ToolSystem.GenerateRiver();
//	}
//}

void MFCMain::ToolBarButtonSave()
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
	m_ToolSystem.CreateSceneObject();
}

void MFCMain::ToolBarRefreshObjects()
{
	m_ToolSystem.UpdateAllObjects();
}

void MFCMain::ToolBarPathEditor()
{
	if (!m_ToolPathEditorDialogue.IsActive())
	{
		m_ToolPathEditorDialogue.Create(IDD_DIALOG4);
		m_ToolPathEditorDialogue.ShowWindow(SW_SHOW);

		m_ToolPathEditorDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.m_paths);

		if (m_ToolSystem.m_selectedObjects.size() > 0)
			m_ToolPathEditorDialogue.SetSelected(m_ToolSystem.m_selectedObjects[0]);
	}
}

MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}
