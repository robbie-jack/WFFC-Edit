//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "DisplayObject.h"
#include <string>
#include <iostream>

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game()

{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
	m_displayList.clear();
	
	//initial Settings
	//modes
	m_grid = false;
	m_drawPath = false;
}

Game::~Game()
{

#ifdef DXTK_AUDIO
    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }
#endif
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_gamePad = std::make_unique<GamePad>();

    m_keyboard = std::make_unique<Keyboard>();

    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

	m_cam = std::make_unique<Camera>();

    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	GetClientRect(window, &m_ScreenDimensions);

	m_windowHandle = window;

#ifdef DXTK_AUDIO
    // Create DirectXTK for Audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine = std::make_unique<AudioEngine>(eflags);

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_waveBank = std::make_unique<WaveBank>(m_audEngine.get(), L"adpcmdroid.xwb");

    m_soundEffect = std::make_unique<SoundEffect>(m_audEngine.get(), L"MusicMono_adpcm.wav");
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

    m_effect1->Play(true);
    m_effect2->Play();
#endif
}

void Game::SetGridState(bool state)
{
	m_grid = state;
}

#pragma region Frame Update
// Executes the basic game loop.
float Game::Tick(InputCommands *Input)
{
	float dt;

	//copy over the input commands so we have a local version to use elsewhere.
	m_InputCommands = *Input;
    m_timer.Tick([&]()
    {
        dt = Update(m_timer);
    });

#ifdef DXTK_AUDIO
    // Only update audio engine once per frame
    if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
#endif

    Render();

	return dt;
}

// Updates the world.
float Game::Update(DX::StepTimer const& timer)
{
	float dt = float(timer.GetElapsedSeconds());

	// Switch Camera Mode
	if (m_InputCommands.switch_Cam_Mode == KeyState::Pressed)
	{
		switch (m_cam->GetCamMode())
		{
		case CameraMode::FREE:
			if (!m_currentIDs.empty())
			{
				m_cam->SetCamMode(CameraMode::ORBIT);

				//for (auto display_object : m_displayList)
				//{
				//	if (display_object.m_ID == m_currentIDs[0])
				//	{
				//		m_cam->SetCamLookAt(display_object.m_position);

				//		//std::cout << "Object Position X: " << display_object.m_position.x << " Y: " << display_object.m_position.y << " Z: " << display_object.m_position.z << std::endl;
				//		//std::cout << "Camera Look At X: " << m_cam->GetCamLookAt().x << " Y: " << m_cam->GetCamLookAt().y << " Z: " << m_cam->GetCamLookAt().z << std::endl;
				//	}
				//}

				m_cam->SetCamLookAt(m_displayList[m_currentIDs[0]].m_position);
			}

			break;
		case CameraMode::ORBIT:
			m_cam->SetCamMode(CameraMode::FREE);
			break;
		}
	}

	switch (m_cam->GetCamMode())
	{
	case CameraMode::ORBIT:
		m_cam->SetCamLookAt(m_displayList[m_currentIDs[0]].m_position);
		break;
	case CameraMode::FREE:

		if (m_InputCommands.focus == KeyState::Pressed)
		{
			m_cam->SetCamLookAt(m_displayList[m_currentIDs[0]].m_position);
		}

		break;
	}

	// Update Camera
	m_cam->Update(m_InputCommands);

	//apply camera vectors
	m_view = Matrix::CreateLookAt(m_cam->GetCamPosition(), m_cam->GetCamLookAt(), m_cam->GetCamUpVector());

    m_batchEffect->SetView(m_view);
    m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainEffect->SetView(m_view);
	m_displayChunk.m_terrainEffect->SetWorld(Matrix::Identity);

#ifdef DXTK_AUDIO
    m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
    if (m_audioTimerAcc < 0)
    {
        if (m_retryDefault)
        {
            m_retryDefault = false;
            if (m_audEngine->Reset())
            if (m_audEngine->Reset()
            {
                // Restart looping audio
                m_effect1->Play(true);
            }
        }
        else
        {
            m_audioTimerAcc = 4.f;

            m_waveBank->Play(m_audioEvent++);

            if (m_audioEvent >= 11)
                m_audioEvent = 0;
        }
    }
#endif

	return dt;
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

	if (m_grid)
	{
		// Draw procedurally generated dynamic grid
		const XMVECTORF32 xaxis = { 512.f, 0.f, 0.f };
		const XMVECTORF32 yaxis = { 0.f, 0.f, 512.f };
		DrawGrid(xaxis, yaxis, g_XMZero, 512, 512, Colors::Gray);
	}

	//RENDER OBJECTS FROM SCENEGRAPH
	int numRenderObjects = m_displayList.size();
	for (int i = 0; i < numRenderObjects; i++)
	{
		m_deviceResources->PIXBeginEvent(L"Draw model");
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x, m_displayList[i].m_scale.y, m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x, m_displayList[i].m_position.y, m_displayList[i].m_position.z };

		//convert degrees into radians for rotation matrix
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
															m_displayList[i].m_orientation.x *3.1415 / 180,
															m_displayList[i].m_orientation.z *3.1415 / 180);

		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		m_displayList[i].m_model->Draw(context, *m_states, local, m_view, m_projection, m_displayList[i].m_wireframe);	//last variable in draw,  make TRUE for wireframe

		m_deviceResources->PIXEndEvent();
	}
    m_deviceResources->PIXEndEvent();

	//RENDER TERRAIN
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault(),0);
	context->RSSetState(m_states->CullNone());
	//context->RSSetState(m_states->Wireframe());		//uncomment for wireframe

	//Render the batch,  This is handled in the Display chunk becuase it has the potential to get complex
	m_displayChunk.RenderBatch(m_deviceResources);

	if (m_drawPath)
	{
		RenderPath(m_path);
	}

	//CAMERA POSITION ON HUD
	/*m_sprites->Begin();
	WCHAR   Buffer[256];
	std::wstring var = L"Cam X: " + std::to_wstring(m_cam->GetCamPosition().x) + L" Cam Y: " + std::to_wstring(m_cam->GetCamPosition().y) + L" Cam Z: " + std::to_wstring(m_cam->GetCamPosition().z) +
		L" CamRot X: " + std::to_wstring(m_cam->GetCamOrientation().x) + L" CamRot Y: " + std::to_wstring(m_cam->GetCamOrientation().y);
	m_font->DrawString(m_sprites.get(), var.c_str(), XMFLOAT2(20, 10), Colors::Yellow);
	m_sprites->End();*/

    m_deviceResources->Present();
}

void Game::RenderPath(Path path)
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_batchEffect->Apply(context);

	context->IASetInputLayout(m_batchInputLayout.Get());

	// Render AI Paths
	m_batch->Begin();

	// Draw Straight lines between path nodes
	if (path.GetNodes().size() >= 2)
	{
		for (int i = 0; i < path.GetNodes().size() - 1; i++)
		{
			Vector3 pos1 = Vector3(path.GetNode(i)->posX, path.GetNode(i)->posY, path.GetNode(i)->posZ);
			Vector3 pos2 = Vector3(path.GetNode(i + 1)->posX, path.GetNode(i + 1)->posY, path.GetNode(i + 1)->posZ);

			VertexPositionColor v1(pos1, Colors::Red);
			VertexPositionColor v2(pos2, Colors::Red);

			m_batch->DrawLine(v1, v2);
		}
	}

	// Draw path AI node will follow
	if (path.GetNodes().size() >= 4)
	{
		float t = 0.1f;
		Vector3 lastPos = path.GetNextPoint(t);

		// Draw Each Segment of Path
		while (!path.AtPathEnd())
		{
			Vector3 pos1 = lastPos;
			Vector3 pos2 = path.GetNextPoint(t);

			VertexPositionColor v1(pos1, Colors::Green);
			VertexPositionColor v2(pos2, Colors::Green);

			m_batch->DrawLine(v1, v2);

			lastPos = pos2;
		}

		path.ResetPath();
	}

	m_drawPath = false;

	m_batch->End();
}

void Game::DrawPath(Path path)
{
	m_path = path;
	m_drawPath = true;
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetBackBufferRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}

std::vector<int> Game::MousePicking(std::vector<int> currentIDs)
{
	int selectedID = -1;
	float pickedDistance = 0;
	float closestDistance = -1;

	//setup near and far planes of frustum with mouse X and mouse y passed down from Toolmain. 
	//they may look the same but note, the difference in Z
	const XMVECTOR nearSource = XMVectorSet(m_InputCommands.mouse_X, m_InputCommands.mouse_Y, 0.0f, 1.0f);
	const XMVECTOR farSource = XMVectorSet(m_InputCommands.mouse_X, m_InputCommands.mouse_Y, 1.0f, 1.0f);

	m_deviceResources->GetOutputSize();

	//Loop through entire display list of objects and pick with each in turn. 
	for (int i = 0; i < m_displayList.size(); i++)
	{
		//Get the scale factor and translation of the object
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x,		m_displayList[i].m_scale.y,		m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x,	m_displayList[i].m_position.y,	m_displayList[i].m_position.z };

		//convert euler angles into a quaternion for the rotation of the object
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
			m_displayList[i].m_orientation.x *3.1415 / 180,
			m_displayList[i].m_orientation.z *3.1415 / 180);

		//create set the matrix of the selected object in the world based on the translation, scale and rotation.
		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		//Unproject the points on the near and far plane, with respect to the matrix we just created.
		XMVECTOR nearPoint = XMVector3Unproject(nearSource, 0.0f, 0.0f, m_ScreenDimensions.right, m_ScreenDimensions.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);
		XMVECTOR farPoint = XMVector3Unproject(farSource, 0.0f, 0.0f, m_ScreenDimensions.right, m_ScreenDimensions.bottom, m_deviceResources->GetScreenViewport().MinDepth, m_deviceResources->GetScreenViewport().MaxDepth, m_projection, m_view, local);

		//turn the transformed points into our picking vector. 
		XMVECTOR pickingVector = farPoint - nearPoint;
		pickingVector = XMVector3Normalize(pickingVector);

		//loop through mesh list for object
		for (int y = 0; y < m_displayList[i].m_model.get()->meshes.size(); y++)
		{
			//checking for ray intersection
			if (m_displayList[i].m_model.get()->meshes[y]->boundingBox.Intersects(nearPoint, pickingVector, pickedDistance))
			{
				if (closestDistance == -1)
				{
					//selectedID = m_displayList[i].m_ID;
					selectedID = i;
					closestDistance = pickedDistance;
				}

				// Only select if object is closer than previous object
				if (pickedDistance < closestDistance)
				{
					//selectedID = m_displayList[i].m_ID;
					selectedID = i;
					closestDistance = pickedDistance;
				}
			}
		}
	}

	std::vector<int>::iterator currentID;
	bool isSelected = false; // Boolean for checking is objetc is already selected

	// Check if picked object is already selected
	for (currentID = currentIDs.begin(); currentID < currentIDs.end(); currentID++)
	{
		if (selectedID == *currentID)
		{
			isSelected = true;
			break;
		}
	}

	if (selectedID != -1)
	{
		if (m_InputCommands.shift == KeyState::Down)
		{
			// If object is selected remove it, else add it
			if (isSelected)
			{
				currentIDs.erase(currentID);
			}
			else
			{
				currentIDs.push_back(selectedID);
			}
		}
		else
		{
			// If picked object is only one selected, deselect, else select object
			if (isSelected && currentIDs.size() == 1)
			{
				currentIDs.clear();
			}
			else
			{
				currentIDs.clear();
				currentIDs.push_back(selectedID);
			}
		}
	}
	else
		currentIDs.clear();

	//if we got a hit.  return it.
	m_currentIDs = currentIDs;

	return currentIDs;
}

void XM_CALLCONV Game::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
    m_deviceResources->PIXBeginEvent(L"Draw grid");

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthNone(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);

    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    xdivs = std::max<size_t>(1, xdivs);
    ydivs = std::max<size_t>(1, ydivs);

    for (size_t i = 0; i <= xdivs; ++i)
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    for (size_t i = 0; i <= ydivs; i++)
    {
        float fPercent = float(i) / float(ydivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    m_batch->End();

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
#ifdef DXTK_AUDIO
    m_audEngine->Suspend();
#endif
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

#ifdef DXTK_AUDIO
    m_audEngine->Resume();
#endif
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

	m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	GetClientRect(m_windowHandle, &m_ScreenDimensions);
}

void Game::BuildDisplayList(std::vector<SceneObject> * SceneGraph)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	if (!m_displayList.empty())		//is the vector empty
	{
		m_displayList.clear();		//if not, empty it
	}

	//for every item in the scenegraph
	int numObjects = SceneGraph->size();
	for (int i = 0; i < numObjects; i++)
	{
		
		//create a temp display object that we will populate then append to the display list.
		DisplayObject newDisplayObject;
		
		//load model
		std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(SceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

		//if texture fails.  load error default
		if (rs)
		{
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
		}

		//apply new texture to models effect
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		{	
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);			
			}
		});

		//set id
		newDisplayObject.m_ID = SceneGraph->at(i).ID;

		//set position
		newDisplayObject.m_position.x = SceneGraph->at(i).posX;
		newDisplayObject.m_position.y = SceneGraph->at(i).posY;
		newDisplayObject.m_position.z = SceneGraph->at(i).posZ;
		
		//setorientation
		newDisplayObject.m_orientation.x = SceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = SceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = SceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = SceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = SceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = SceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render		= SceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe	= SceneGraph->at(i).editor_wireframe;

		newDisplayObject.m_light_type		= SceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r	= SceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g	= SceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b	= SceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_specular_r = SceneGraph->at(i).light_specular_r;
		newDisplayObject.m_light_specular_g = SceneGraph->at(i).light_specular_g;
		newDisplayObject.m_light_specular_b = SceneGraph->at(i).light_specular_b;
		newDisplayObject.m_light_spot_cutoff = SceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant	= SceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear		= SceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic	= SceneGraph->at(i).light_quadratic;
		
		m_displayList.push_back(newDisplayObject);
		
	}
}

void Game::UpdateDisplayObject(int i, SceneObject* SceneObject)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	//load model
	std::wstring modelwstr = StringToWCHART(SceneObject->model_path);							//convect string to Wchar
	m_displayList[i].m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

	//Load Texture
	std::wstring texturewstr = StringToWCHART(SceneObject->tex_diffuse_path);								//convect string to Wchar
	HRESULT rs;
	rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &m_displayList[i].m_texture_diffuse);	//load tex into Shader resource

	//if texture fails.  load error default
	if (rs)
	{
		CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &m_displayList[i].m_texture_diffuse);	//load tex into Shader resource
	}

	//apply new texture to models effect
	m_displayList[i].m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
	{
		auto lights = dynamic_cast<BasicEffect*>(effect);
		if (lights)
		{
			lights->SetTexture(m_displayList[i].m_texture_diffuse);
		}
	});

	//set id
	m_displayList[i].m_ID = SceneObject->ID;

	//set position
	m_displayList[i].m_position.x = SceneObject->posX;
	m_displayList[i].m_position.y = SceneObject->posY;
	m_displayList[i].m_position.z = SceneObject->posZ;

	//setorientation
	m_displayList[i].m_orientation.x = SceneObject->rotX;
	m_displayList[i].m_orientation.y = SceneObject->rotY;
	m_displayList[i].m_orientation.z = SceneObject->rotZ;

	//set scale
	m_displayList[i].m_scale.x = SceneObject->scaX;
	m_displayList[i].m_scale.y = SceneObject->scaY;
	m_displayList[i].m_scale.z = SceneObject->scaZ;

	//set wireframe / render flags
	m_displayList[i].m_render = SceneObject->editor_render;
	m_displayList[i].m_wireframe = SceneObject->editor_wireframe;

	m_displayList[i].m_light_type = SceneObject->light_type;
	m_displayList[i].m_light_diffuse_r = SceneObject->light_diffuse_r;
	m_displayList[i].m_light_diffuse_g = SceneObject->light_diffuse_g;
	m_displayList[i].m_light_diffuse_b = SceneObject->light_diffuse_b;
	m_displayList[i].m_light_specular_r = SceneObject->light_specular_r;
	m_displayList[i].m_light_specular_g = SceneObject->light_specular_g;
	m_displayList[i].m_light_specular_b = SceneObject->light_specular_b;
	m_displayList[i].m_light_spot_cutoff = SceneObject->light_spot_cutoff;
	m_displayList[i].m_light_constant = SceneObject->light_constant;
	m_displayList[i].m_light_linear = SceneObject->light_linear;
	m_displayList[i].m_light_quadratic = SceneObject->light_quadratic;
}

void Game::AddDisplayObject(SceneObject* SceneObject)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	//create a temp display object that we will populate then append to the display list.
	DisplayObject newDisplayObject;

	//load model
	std::wstring modelwstr = StringToWCHART(SceneObject->model_path);							//convect string to Wchar
	newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

	//Load Texture
	std::wstring texturewstr = StringToWCHART(SceneObject->tex_diffuse_path);								//convect string to Wchar
	HRESULT rs;
	rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

	//if texture fails.  load error default
	if (rs)
	{
		CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
	}

	//apply new texture to models effect
	newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		{
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);
			}
		});

	//set id
	newDisplayObject.m_ID = SceneObject->ID;

	//set position
	newDisplayObject.m_position.x = SceneObject->posX;
	newDisplayObject.m_position.y = SceneObject->posY;
	newDisplayObject.m_position.z = SceneObject->posZ;

	//setorientation
	newDisplayObject.m_orientation.x = SceneObject->rotX;
	newDisplayObject.m_orientation.y = SceneObject->rotY;
	newDisplayObject.m_orientation.z = SceneObject->rotZ;

	//set scale
	newDisplayObject.m_scale.x = SceneObject->scaX;
	newDisplayObject.m_scale.y = SceneObject->scaY;
	newDisplayObject.m_scale.z = SceneObject->scaZ;

	//set wireframe / render flags
	newDisplayObject.m_render = SceneObject->editor_render;
	newDisplayObject.m_wireframe = SceneObject->editor_wireframe;

	newDisplayObject.m_light_type = SceneObject->light_type;
	newDisplayObject.m_light_diffuse_r = SceneObject->light_diffuse_r;
	newDisplayObject.m_light_diffuse_g = SceneObject->light_diffuse_g;
	newDisplayObject.m_light_diffuse_b = SceneObject->light_diffuse_b;
	newDisplayObject.m_light_specular_r = SceneObject->light_specular_r;
	newDisplayObject.m_light_specular_g = SceneObject->light_specular_g;
	newDisplayObject.m_light_specular_b = SceneObject->light_specular_b;
	newDisplayObject.m_light_spot_cutoff = SceneObject->light_spot_cutoff;
	newDisplayObject.m_light_constant = SceneObject->light_constant;
	newDisplayObject.m_light_linear = SceneObject->light_linear;
	newDisplayObject.m_light_quadratic = SceneObject->light_quadratic;

	m_displayList.push_back(newDisplayObject);
}

void Game::RemoveDisplayObject(int i)
{
	m_displayList.erase(m_displayList.begin() + i);
}

void Game::BuildDisplayChunk(ChunkObject * SceneChunk)
{
	//populate our local DISPLAYCHUNK with all the chunk info we need from the object stored in toolmain
	//which, to be honest, is almost all of it. Its mostly rendering related info so...
	m_displayChunk.PopulateChunkData(SceneChunk);		//migrate chunk data
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_terrainEffect->SetProjection(m_projection);
	m_displayChunk.InitialiseBatch();
}

void Game::SaveDisplayChunk(ChunkObject * SceneChunk)
{
	m_displayChunk.SaveHeightMap();			//save heightmap to file.
}

void Game::BuildRiver(RiverSection river)
{
	m_displayChunk.GenerateRiver(river);
}

#ifdef DXTK_AUDIO
void Game::NewAudioDevice()
{
    if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
}
#endif


#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"database/data/"); //fx Factory will look in the database directory
	m_fxFactory->SetSharing(false);	//we must set this to false otherwise it will share effects based on the initial tex loaded (When the model loads) rather than what we will change them to.

    m_sprites = std::make_unique<SpriteBatch>(context);

    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

    m_batchEffect = std::make_unique<BasicEffect>(device);
    m_batchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        DX::ThrowIfFailed(
            device->CreateInputLayout(VertexPositionColor::InputElements,
                VertexPositionColor::InputElementCount,
                shaderByteCode, byteCodeLength,
                m_batchInputLayout.ReleaseAndGetAddressOf())
        );
    }

    m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

//    m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

    // SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
    m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);
	

    // Load textures
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
    );

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
    );

}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    float fovAngleY = 70.0f * XM_PI / 180.0f;

    // This is a simple example of change that can be made when the app is in
    // portrait or snapped view.
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    m_projection = Matrix::CreatePerspectiveFieldOfView(
        fovAngleY,
        aspectRatio,
        0.01f,
        1000.0f
    );

    m_batchEffect->SetProjection(m_projection);
	
}

void Game::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_batch.reset();
    m_batchEffect.reset();
    m_font.reset();
    m_shape.reset();
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

std::wstring StringToWCHART(std::string s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
