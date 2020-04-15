#pragma once

enum class KeyState
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
	KeyState translateForward;
	KeyState translateBackward;
	KeyState translateLeft;
	KeyState translateRight;
	KeyState translateUp;
	KeyState translateDown;
	KeyState switchToTranslate;
	KeyState switchToRotate;
	KeyState switchToScale;
	KeyState switch_Cam_Mode;
	KeyState mouse_RB;
	KeyState mouse_LB;
	int mouse_X;
	int mouse_Y;
	int mouse_X_Last;
	int mouse_Y_Last;
};
