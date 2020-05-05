#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(CameraMode camMode, float camMoveSpeed, float camRotRate, float camSensitivity, Vector3 camPosition, Vector3 camOrientation, Vector3 camLookAt, Vector3 camLookDirection, Vector3 camForward, Vector3 camRight, Vector3 camUp)
{
	// Initialise Camera Variables
	m_camMode = camMode;

	m_camMoveSpeed = camMoveSpeed;
	m_camRotRate = camRotRate;
	m_camSensitivity = camSensitivity;
	m_camDistance = 10.0f;

	m_camPosition = camPosition;
	m_camOrientation = camOrientation;
	m_camLookAt = camLookAt;
	m_camLookDirection = camLookDirection;

	m_camForward = camForward;
	m_camRight = camRight;
	m_camUp = camUp;
}

Camera::~Camera()
{
}

void Camera::Update(InputCommands InputCommands)
{
	switch (m_camMode)
	{
	case CameraMode::FREE:
		FreeCamUpdate(InputCommands);
		break;

	case CameraMode::ORBIT:
		OrbitCamUpdate(InputCommands);
		break;
	}
}

void Camera::FreeCamUpdate(InputCommands InputCommands)
{
	// Mouse Rotation Input
	if (InputCommands.mouse_RB == KeyState::Down)
	{
		int dx = InputCommands.mouse_Y - InputCommands.mouse_Y_Last;
		int dy = InputCommands.mouse_X - InputCommands.mouse_X_Last;

		m_camOrientation.y += dy * m_camSensitivity;
		m_camOrientation.x -= dx * m_camSensitivity;
	}

	// Max/Min Camera Pitch
	if (m_camOrientation.x < -90)
	{
		m_camOrientation.x = -90;
	}

	if (m_camOrientation.x > 90)
	{
		m_camOrientation.x = 90;
	}

	if (m_camOrientation.y > 360)
	{
		m_camOrientation.y = 0;
	}

	if (m_camOrientation.y < 0)
	{
		m_camOrientation.y = 360;
	}

	//create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = cos((m_camOrientation.x) * 3.1415 / 180) * cos((m_camOrientation.y) * 3.1415 / 180);
	m_camLookDirection.y = sin((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.x) * 3.1415 / 180) * sin((m_camOrientation.y) * 3.1415 / 180);

	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(m_camUp, m_camRight);

	//process input and update stuff
	if (InputCommands.forward == KeyState::Down)
	{
		m_camPosition += m_camLookDirection * m_camMoveSpeed;
	}
	if (InputCommands.back == KeyState::Down)
	{
		m_camPosition -= m_camLookDirection * m_camMoveSpeed;
	}
	if (InputCommands.right == KeyState::Down)
	{
		m_camPosition += m_camRight * m_camMoveSpeed;
	}
	if (InputCommands.left == KeyState::Down)
	{
		m_camPosition -= m_camRight * m_camMoveSpeed;
	}
	if (InputCommands.up == KeyState::Down)
	{
		m_camPosition += m_camUp * m_camMoveSpeed;
	}
	if (InputCommands.down == KeyState::Down)
	{
		m_camPosition -= m_camUp * m_camMoveSpeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
}

void Camera::OrbitCamUpdate(InputCommands InputCommands)
{
	//process input and update stuff
	if (InputCommands.forward == KeyState::Down)
	{
		m_camDistance -= m_camMoveSpeed;
	}
	if (InputCommands.back == KeyState::Down)
	{
		m_camDistance += m_camMoveSpeed;
	}

	if (m_camDistance < 0.1f)
		m_camDistance = 0.1f;

	if (InputCommands.up == KeyState::Down)
	{
		m_camOrientation.x -= m_camRotRate;
	}
	if (InputCommands.down == KeyState::Down)
	{
		m_camOrientation.x += m_camRotRate;
	}
	if (InputCommands.right == KeyState::Down)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (InputCommands.left == KeyState::Down)
	{
		m_camOrientation.y += m_camRotRate;
	}

	// Max/Min Camera Pitch
	if (m_camOrientation.x < -90)
	{
		m_camOrientation.x = -90;
	}

	if (m_camOrientation.x > 90)
	{
		m_camOrientation.x = 90;
	}

	if (m_camOrientation.y > 360)
	{
		m_camOrientation.y = 0;
	}

	if (m_camOrientation.y < 0)
	{
		m_camOrientation.y = 360;
	}

	//create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = cos((m_camOrientation.x) * 3.1415 / 180) * cos((m_camOrientation.y) * 3.1415 / 180);
	m_camLookDirection.y = sin((m_camOrientation.x) * 3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.x) * 3.1415 / 180) * sin((m_camOrientation.y) * 3.1415 / 180);

	m_camLookDirection.Normalize();

	m_camPosition = m_camLookAt - (m_camLookDirection * m_camDistance);
}