#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "AIObject.h"
#include "InputCommands.h"

#include <vector>

enum class ObjectManipulationState
{
	Translate = 0,
	Rotate = 1,
	Scale = 2
};

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	std::vector<int>		getCurrentSelectionID();										//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND toolHandle, HWND renderHandle, int width, int height);			//Passes through tool/render handles and hieght and width and initialises DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	float	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);
	void	UpdateStates();
	void	ManipulateObjects(float dt);
	void	UpdateWindow(int width, int height);
	void	UpdateAllObjects();
	void	UpdateObject(int i);
	SceneObject*	CreateSceneObject();
	AIObject*		CreateAIObject();
	void	DrawPath(Path path);
	//void	GenerateRiver();

public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	std::vector<int> m_selectedObjects;						//ID of current Selection
	std::vector<Path> m_paths;

	ObjectManipulationState m_manipulationState;
	float m_translateSpeed;
	float m_rotateSpeed;
	float m_scaleSpeed;

	//River m_river;
	//AIObject* m_aiObject;
	//Path path;

private:	//methods
	void	onContentAdded();
	void	DeleteObjects();
	void	TranslateObjects(float dt, int selected);
	void	RotateObjects(float dt, int selected);
	void	ScaleObjects(float dt, int selected);
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	char	m_keyArray[256];
	sqlite3 *m_databaseConnection;	//sqldatabase handle

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
	int nextID;
};
