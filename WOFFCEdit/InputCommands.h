#pragma once

enum KeyState
{
	Pressed = 0,
	Down = 1,
	Released = 2,
	Up = 3
};

struct InputCommands
{
	KeyState forward;
	KeyState back;
	KeyState right;
	KeyState left;
	KeyState up;
	KeyState down;
	KeyState shift;
	KeyState switch_Cam_Mode;
	KeyState mouse_RB;
	KeyState mouse_LB;
	int mouse_X;
	int mouse_Y;
	int mouse_X_Last;
	int mouse_Y_Last;
};
