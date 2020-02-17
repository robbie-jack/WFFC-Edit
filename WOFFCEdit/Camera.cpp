#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(float camMoveSpeed, float camRotRate, float camSensitivity, Vector3 camPosition, Vector3 camOrientation, Vector3 camLookAt, Vector3 camLookDirection, Vector3 camForward, Vector3 camRight, Vector3 camUp)
{
	// Initialise Camera Variables
	m_camMoveSpeed = camMoveSpeed;
	m_camRotRate = camRotRate;
	m_camSensitivity = camSensitivity;

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
	//TODO  any more complex than this, and the camera should be abstracted out to somewhere else
	//camera motion is on a plane, so kill the 7 component of the look direction
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

	// Mouse Rotation Input
	if (InputCommands.mouseRight)
	{
		int dx = InputCommands.mouseX - InputCommands.mouseXLast;
		int dy = InputCommands.mouseY - InputCommands.mouseYLast;

		m_camOrientation.y += dx * m_camSensitivity;
		m_camOrientation.x -= dy * m_camSensitivity;
	}

	// Keyboard Rotation Input
	if (InputCommands.rotLeft)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (InputCommands.rotRight)
	{
		m_camOrientation.y += m_camRotRate;
	}
	if (InputCommands.rotDown && m_camOrientation.x > -90)
	{
		m_camOrientation.x -= m_camRotRate;
	}
	if (InputCommands.rotUp && m_camOrientation.x < 90)
	{
		m_camOrientation.x += m_camRotRate;
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
	if (InputCommands.forward)
	{
		m_camPosition += m_camLookDirection * m_camMoveSpeed;
	}
	if (InputCommands.back)
	{
		m_camPosition -= m_camLookDirection * m_camMoveSpeed;
	}
	if (InputCommands.right)
	{
		m_camPosition += m_camRight * m_camMoveSpeed;
	}
	if (InputCommands.left)
	{
		m_camPosition -= m_camRight * m_camMoveSpeed;
	}
	if (InputCommands.up)
	{
		m_camPosition += m_camUp * m_camMoveSpeed;
	}
	if (InputCommands.down)
	{
		m_camPosition -= m_camUp * m_camMoveSpeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
}
