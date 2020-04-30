#include "ToolMain.h"
#include "resource.h"
#include "River.h"

#include <vector>
#include <sstream>

//
//ToolMain Class
ToolMain::ToolMain()
{

	m_currentChunk = 0;		//default value
	//m_selectedObjects.push_back(0);	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;

	//zero input commands
	m_toolInputCommands.forward				= KeyState::Up;
	m_toolInputCommands.back				= KeyState::Up;
	m_toolInputCommands.left				= KeyState::Up;
	m_toolInputCommands.right				= KeyState::Up;
	m_toolInputCommands.up					= KeyState::Up;
	m_toolInputCommands.down				= KeyState::Up;
	m_toolInputCommands.shift				= KeyState::Up;
	m_toolInputCommands.translateForward	= KeyState::Up;
	m_toolInputCommands.translateBackward	= KeyState::Up;
	m_toolInputCommands.translateLeft		= KeyState::Up;
	m_toolInputCommands.translateRight		= KeyState::Up;
	m_toolInputCommands.translateUp			= KeyState::Up;
	m_toolInputCommands.translateDown		= KeyState::Up;
	m_toolInputCommands.switch_Cam_Mode		= KeyState::Up;
	m_toolInputCommands.mouse_RB			= KeyState::Up;
	m_toolInputCommands.mouse_LB			= KeyState::Up;
	m_toolInputCommands.mouse_X				= 0;
	m_toolInputCommands.mouse_Y				= 0;
	m_toolInputCommands.mouse_X_Last		= 0;
	m_toolInputCommands.mouse_Y_Last		= 0;

	m_manipulationState = ObjectManipulationState::Translate;

	m_translateSpeed = 5.0f;
	m_rotateSpeed = 30.0f;
	m_scaleSpeed = 5.0f;

	m_objectUpdated = false;
}


ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}


std::vector<int> ToolMain::getCurrentSelectionID()
{
	return m_selectedObjects;
}

void ToolMain::onActionInitialise(HWND toolHandle, HWND renderHandle, int width, int height)
{
	m_toolHandle = toolHandle;

	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	m_d3dRenderer.Initialize(renderHandle, m_width, m_height);

	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}

	onActionLoad();

	Path path;
	path.m_name = L"Default Path";
	path.AddNode(&m_sceneGraph[17]);
	path.AddNode(&m_sceneGraph[18]);
	path.AddNode(&m_sceneGraph[19]);
	path.AddNode(&m_sceneGraph[20]);
	m_paths.push_back(path);
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);
	
		newSceneObject.is_deleted = false;

		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	nextID = m_sceneGraph.back().ID + 1;

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);


	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);


	//Process REsults into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_sceneGraph.at(i).ID << ","
			<< m_sceneGraph.at(i).chunk_ID  << ","
			<< "'" << m_sceneGraph.at(i).model_path <<"'" << ","
			<< "'" << m_sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_sceneGraph.at(i).posX << ","
			<< m_sceneGraph.at(i).posY << ","
			<< m_sceneGraph.at(i).posZ << ","
			<< m_sceneGraph.at(i).rotX << ","
			<< m_sceneGraph.at(i).rotY << ","
			<< m_sceneGraph.at(i).rotZ << ","
			<< m_sceneGraph.at(i).scaX << ","
			<< m_sceneGraph.at(i).scaY << ","
			<< m_sceneGraph.at(i).scaZ << ","
			<< m_sceneGraph.at(i).render << ","
			<< m_sceneGraph.at(i).collision << ","
			<< "'" << m_sceneGraph.at(i).collision_mesh << "'" << ","
			<< m_sceneGraph.at(i).collectable << ","
			<< m_sceneGraph.at(i).destructable << ","
			<< m_sceneGraph.at(i).health_amount << ","
			<< m_sceneGraph.at(i).editor_render << ","
			<< m_sceneGraph.at(i).editor_texture_vis << ","
			<< m_sceneGraph.at(i).editor_normals_vis << ","
			<< m_sceneGraph.at(i).editor_collision_vis << ","
			<< m_sceneGraph.at(i).editor_pivot_vis << ","
			<< m_sceneGraph.at(i).pivotX << ","
			<< m_sceneGraph.at(i).pivotY << ","
			<< m_sceneGraph.at(i).pivotZ << ","
			<< m_sceneGraph.at(i).snapToGround << ","
			<< m_sceneGraph.at(i).AINode << ","
			<< "'" << m_sceneGraph.at(i).audio_path << "'" << ","
			<< m_sceneGraph.at(i).volume << ","
			<< m_sceneGraph.at(i).pitch << ","
			<< m_sceneGraph.at(i).pan << ","
			<< m_sceneGraph.at(i).one_shot << ","
			<< m_sceneGraph.at(i).play_on_init << ","
			<< m_sceneGraph.at(i).play_in_editor << ","
			<< m_sceneGraph.at(i).min_dist << ","
			<< m_sceneGraph.at(i).max_dist << ","
			<< m_sceneGraph.at(i).camera << ","
			<< m_sceneGraph.at(i).path_node << ","
			<< m_sceneGraph.at(i).path_node_start << ","
			<< m_sceneGraph.at(i).path_node_end << ","
			<< m_sceneGraph.at(i).parent_id << ","
			<< m_sceneGraph.at(i).editor_wireframe << ","
			<< "'" << m_sceneGraph.at(i).name << "'" << ","

			<< m_sceneGraph.at(i).light_type << ","
			<< m_sceneGraph.at(i).light_diffuse_r << ","
			<< m_sceneGraph.at(i).light_diffuse_g << ","
			<< m_sceneGraph.at(i).light_diffuse_b << ","
			<< m_sceneGraph.at(i).light_specular_r << ","
			<< m_sceneGraph.at(i).light_specular_g << ","
			<< m_sceneGraph.at(i).light_specular_b << ","
			<< m_sceneGraph.at(i).light_spot_cutoff << ","
			<< m_sceneGraph.at(i).light_constant << ","
			<< m_sceneGraph.at(i).light_linear << ","
			<< m_sceneGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

float ToolMain::Tick(MSG *msg)
{
	//do we have a selection
	//do we have a mode
	//are we clicking / dragging /releasing
	//has something changed
		//update Scenegraph
		//add to scenegraph
		//resend scenegraph to Direct X renderer

	if (m_toolHandle == GetForegroundWindow() && m_toolInputCommands.mouse_LB == KeyState::Pressed)
	{
		m_selectedObjects = m_d3dRenderer.MousePicking(m_selectedObjects);
	}

	//Renderer Update Call
	float dt = m_d3dRenderer.Tick(&m_toolInputCommands);
	ManipulateObjects(dt);

	DeleteObjects();

	UpdateStates();

	return dt;
}

void ToolMain::UpdateInput(MSG * msg)
{
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
	case WM_KEYDOWN:
		m_keyArray[msg->wParam] = true;
		break;

	case WM_KEYUP:
		m_keyArray[msg->wParam] = false;
		break;

	case WM_MOUSEMOVE:
		m_toolInputCommands.mouse_X_Last	= m_toolInputCommands.mouse_X;
		m_toolInputCommands.mouse_Y_Last	= m_toolInputCommands.mouse_Y;
		m_toolInputCommands.mouse_X		= GET_X_LPARAM(msg->lParam);
		m_toolInputCommands.mouse_Y		= GET_Y_LPARAM(msg->lParam);
		
		break;

	case WM_LBUTTONDOWN:	//mouse button down,  you will probably need to check when its up too
		//set some flag for the mouse button in inputcommands
		m_toolInputCommands.mouse_LB = KeyState::Pressed;
		
		break;

	case WM_LBUTTONUP:
		m_toolInputCommands.mouse_LB	= KeyState::Up;
		break;

	case WM_RBUTTONDOWN:
		m_toolInputCommands.mouse_RB = KeyState::Pressed;
		break;

	case WM_RBUTTONUP:
		m_toolInputCommands.mouse_RB	= KeyState::Up;
		break;

	case WM_SIZING:
		break;

	case WM_SIZE:

		// When window is finished resizing, get new width/height
		int width = LOWORD(msg->lParam);
		int height = HIWORD(msg->lParam);

		//m_d3dRenderer.OnWindowSizeChanged(width, height);
		//m_d3dRenderer.UpdateWindow(m_toolHandle, width, height);

		break;
	}
	//here we update all the actual app functionality that we want.  This information will either be used int toolmain, or sent down to the renderer (Camera movement etc
	//WASDQE movement
	if (m_keyArray['W'])
	{
		m_toolInputCommands.forward = KeyState::Down;
	}
	else m_toolInputCommands.forward = KeyState::Up;
	
	if (m_keyArray['S'])
	{
		m_toolInputCommands.back = KeyState::Down;
	}
	else m_toolInputCommands.back = KeyState::Up;

	if (m_keyArray['A'])
	{
		m_toolInputCommands.left = KeyState::Down;
	}
	else m_toolInputCommands.left = KeyState::Up;

	if (m_keyArray['D'])
	{
		m_toolInputCommands.right = KeyState::Down;
	}
	else m_toolInputCommands.right = KeyState::Up;

	if (m_keyArray['Q'])
	{
		m_toolInputCommands.down = KeyState::Down;
	}
	else m_toolInputCommands.down = KeyState::Up;

	if (m_keyArray['E'])
	{
		m_toolInputCommands.up = KeyState::Down;
	}
	else m_toolInputCommands.up = KeyState::Up;

	// Switch Camera Mode
	if (m_keyArray['C'])
	{
		m_toolInputCommands.switch_Cam_Mode = KeyState::Pressed;
	}
	else m_toolInputCommands.switch_Cam_Mode = KeyState::Up;

	// Object Translation/Rotation/Scale
	if (m_keyArray['I'])
	{
		m_toolInputCommands.translateForward = KeyState::Down;
	}
	else m_toolInputCommands.translateForward = KeyState::Up;

	if (m_keyArray['K'])
	{
		m_toolInputCommands.translateBackward = KeyState::Down;
	}
	else m_toolInputCommands.translateBackward = KeyState::Up;

	if (m_keyArray['J'])
	{
		m_toolInputCommands.translateLeft = KeyState::Down;
	}
	else m_toolInputCommands.translateLeft = KeyState::Up;

	if (m_keyArray['L'])
	{
		m_toolInputCommands.translateRight = KeyState::Down;
	}
	else m_toolInputCommands.translateRight = KeyState::Up;

	if (m_keyArray['U'])
	{
		m_toolInputCommands.translateUp = KeyState::Down;
	}
	else m_toolInputCommands.translateUp = KeyState::Up;

	if (m_keyArray['O'])
	{
		m_toolInputCommands.translateDown = KeyState::Down;
	}
	else m_toolInputCommands.translateDown = KeyState::Up;

	// Switch Object Manipulation State
	if (m_keyArray['B'])
	{
		m_toolInputCommands.switchToTranslate = KeyState::Pressed;
	}
	else m_toolInputCommands.switchToTranslate = KeyState::Up;

	if (m_keyArray['N'])
	{
		m_toolInputCommands.switchToRotate = KeyState::Pressed;
	}
	else m_toolInputCommands.switchToRotate = KeyState::Up;

	if (m_keyArray['M'])
	{
		m_toolInputCommands.switchToScale = KeyState::Pressed;
	}
	else m_toolInputCommands.switchToScale = KeyState::Up;

	if (m_keyArray[VK_SHIFT])
	{
		m_toolInputCommands.shift = KeyState::Down;
	}
	else m_toolInputCommands.shift = KeyState::Up;
}

void ToolMain::UpdateStates()
{
	if (m_toolInputCommands.mouse_LB == KeyState::Pressed)
		m_toolInputCommands.mouse_LB = KeyState::Down;

	if (m_toolInputCommands.mouse_RB == KeyState::Pressed)
		m_toolInputCommands.mouse_RB = KeyState::Down;

	if (m_toolInputCommands.switch_Cam_Mode == KeyState::Pressed)
		m_toolInputCommands.switch_Cam_Mode = KeyState::Down;
}

void ToolMain::ManipulateObjects(float dt)
{
	// Switch Object Manipulation Mode
	if (m_toolInputCommands.switchToTranslate == KeyState::Pressed)
	{
		m_manipulationState = ObjectManipulationState::Translate;
	}

	if (m_toolInputCommands.switchToRotate == KeyState::Pressed)
	{
		m_manipulationState = ObjectManipulationState::Rotate;
	}

	if (m_toolInputCommands.switchToScale == KeyState::Pressed)
	{
		m_manipulationState = ObjectManipulationState::Scale;
	}

	// Update Selcted Objects Translation/Rotation/Scale
	if (m_selectedObjects.size()  > 0)
	{
		int selectedSize = m_selectedObjects.size();

		for (int i = 0; i < selectedSize; i++)
		{
			switch (m_manipulationState)
			{
			case ObjectManipulationState::Translate:
				TranslateObjects(dt, m_selectedObjects[i]);
				break;
			case ObjectManipulationState::Rotate:
				RotateObjects(dt, m_selectedObjects[i]);
				break;
			case ObjectManipulationState::Scale:
				ScaleObjects(dt, m_selectedObjects[i]);
				break;
			}

			UpdateObject(m_selectedObjects[i]);
		}
	}
}

void ToolMain::TranslateObjects(float dt, int selected)
{
	if (m_toolInputCommands.translateForward == KeyState::Down)
	{
		m_sceneGraph.at(selected).posZ += m_translateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateBackward == KeyState::Down)
	{
		m_sceneGraph.at(selected).posZ -= m_translateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateLeft == KeyState::Down)
	{
		m_sceneGraph.at(selected).posX += m_translateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateRight == KeyState::Down)
	{
		m_sceneGraph.at(selected).posX -= m_translateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateUp == KeyState::Down)
	{
		m_sceneGraph.at(selected).posY += m_translateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateDown == KeyState::Down)
	{
		m_sceneGraph.at(selected).posY -= m_translateSpeed * dt;
		m_objectUpdated = true;
	}
}

void ToolMain::RotateObjects(float dt, int selected)
{
	if (m_toolInputCommands.translateForward == KeyState::Down)
	{
		m_sceneGraph.at(selected).rotX += m_rotateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateBackward == KeyState::Down)
	{
		m_sceneGraph.at(selected).rotX -= m_rotateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateLeft == KeyState::Down)
	{
		m_sceneGraph.at(selected).rotZ -= m_rotateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateRight == KeyState::Down)
	{
		m_sceneGraph.at(selected).rotZ += m_rotateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateUp == KeyState::Down)
	{
		m_sceneGraph.at(selected).rotY += m_rotateSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateDown == KeyState::Down)
	{
		m_sceneGraph.at(selected).rotY -= m_rotateSpeed * dt;
		m_objectUpdated = true;
	}
}

void ToolMain::ScaleObjects(float dt, int selected)
{
	if (m_toolInputCommands.translateForward == KeyState::Down)
	{
		m_sceneGraph.at(selected).scaZ += m_scaleSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateBackward == KeyState::Down)
	{
		m_sceneGraph.at(selected).scaZ -= m_scaleSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateLeft == KeyState::Down)
	{
		m_sceneGraph.at(selected).scaX -= m_scaleSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateRight == KeyState::Down)
	{
		m_sceneGraph.at(selected).scaX += m_scaleSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateUp == KeyState::Down)
	{
		m_sceneGraph.at(selected).scaY += m_scaleSpeed * dt;
		m_objectUpdated = true;
	}

	if (m_toolInputCommands.translateDown == KeyState::Down)
	{
		m_sceneGraph.at(selected).scaY -= m_scaleSpeed * dt;
		m_objectUpdated = true;
	}
}

void ToolMain::UpdateWindow(int width, int height)
{
	m_d3dRenderer.OnWindowSizeChanged(width, height);
}

void ToolMain::UpdateAllObjects()
{
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
}

void ToolMain::UpdateObject(int i)
{
	m_d3dRenderer.UpdateDisplayObject(i, &m_sceneGraph[i]);
}

SceneObject* ToolMain::CreateSceneObject()
{
	SceneObject newSceneObject;

	// New Object ID is number of objects + 1
	newSceneObject.ID = nextID;
	newSceneObject.model_path = "database/data/placeholder.cmo";
	newSceneObject.tex_diffuse_path = "database/data/placeholder.dds";
	newSceneObject.posX = 10.0f;
	newSceneObject.posZ = 10.0f;
	newSceneObject.scaX = 1.0f;
	newSceneObject.scaY = 1.0f;
	newSceneObject.scaZ = 1.0f;
	newSceneObject.is_deleted = false;

	// Push new object to scene graph and add to renderer display list
	m_sceneGraph.push_back(newSceneObject);
	m_d3dRenderer.AddDisplayObject(&m_sceneGraph.back());

	m_selectedObjects.clear();
	m_selectedObjects.push_back(m_sceneGraph.size() - 1);

	nextID++;

	return &m_sceneGraph.back();
}

AIObject* ToolMain::CreateAIObject()
{
	AIObject newAIObject;

	// New Object ID is number of objects + 1
	newAIObject.ID = nextID;
	newAIObject.model_path = "database/data/placeholder.cmo";
	newAIObject.tex_diffuse_path = "database/data/placeholder.dds";
	newAIObject.posX = 10.0f;
	newAIObject.posZ = 10.0f;
	newAIObject.scaX = 1.0f;
	newAIObject.scaY = 1.0f;
	newAIObject.scaZ = 1.0f;
	newAIObject.is_deleted = false;
	newAIObject.AINode = true;

	// Push new object to scene graph and add to renderer display list
	m_sceneGraph.push_back(newAIObject);
	m_d3dRenderer.AddDisplayObject(&m_sceneGraph.back());

	m_selectedObjects.clear();
	m_selectedObjects.push_back(m_sceneGraph.size() - 1);

	nextID++;

	return (AIObject*)&m_sceneGraph.back();
}

void ToolMain::DeleteObjects()
{
	bool object_was_deleted = false;

	for (int i = 0; i < m_sceneGraph.size(); i++)
	{
		if (m_sceneGraph[i].is_deleted)
		{
			m_sceneGraph.erase(m_sceneGraph.begin() + i);
			m_d3dRenderer.RemoveDisplayObject(i);
			object_was_deleted = true;
		}
	}

	if (object_was_deleted)
	{
		m_selectedObjects.clear();
		//m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	}
}

void ToolMain::DrawPath(Path path)
{
	m_d3dRenderer.DrawPath(path);
}

//void ToolMain::GenerateRiver()
//{
//	m_d3dRenderer.BuildRiver(m_river.GetSection(0));
//}