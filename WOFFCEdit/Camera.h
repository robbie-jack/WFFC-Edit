#pragma once

#include "pch.h"
#include "InputCommands.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

enum CameraMode
{
	FREE = 0, // Camera is free to move and look around
	ORBIT = 1 // Camera View is locked to object and will orbit arount it when moving
};

class Camera
{
public:
	Camera(	CameraMode camMode = FREE,
			float camMoveSpeed = 0.50f,
			float camRotRate = 3.0f,
			float camSensitivity = 0.5f,
			DirectX::SimpleMath::Vector3 camPosition = DirectX::SimpleMath::Vector3(0.0f, 3.7f, -3.5f),
			DirectX::SimpleMath::Vector3 camOrientation = DirectX::SimpleMath::Vector3(0, 0, 0),
			DirectX::SimpleMath::Vector3 camLookAt = DirectX::SimpleMath::Vector3(0, 0, 0),
			DirectX::SimpleMath::Vector3 camLookDirection = DirectX::SimpleMath::Vector3(0, 0, 0),
			DirectX::SimpleMath::Vector3 camForward = DirectX::SimpleMath::Vector3(0, 0, -1),
			DirectX::SimpleMath::Vector3 camRight = DirectX::SimpleMath::Vector3 (1, 0, 0),
			DirectX::SimpleMath::Vector3 camUp = DirectX::SimpleMath::Vector3(0, 1, 0));

	~Camera();
	void Update(InputCommands InputCommands);

	inline DirectX::SimpleMath::Matrix GetCamToWorldMatrix()
	{
		DirectX::SimpleMath::Matrix cam_to_world = DirectX::SimpleMath::Matrix::Identity;

		DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f };

		//convert euler angles into a quaternion for the rotation of the object
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_camOrientation.y *3.1415 / 180,
			m_camOrientation.x *3.1415 / 180,
			m_camOrientation.z *3.1415 / 180);

		cam_to_world = XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, m_camPosition);

		return cam_to_world;
	}

	inline DirectX::SimpleMath::Matrix GetWorldToCamMatrix()
	{
		DirectX::SimpleMath::Matrix world_to_cam = DirectX::SimpleMath::Matrix::Identity;

		DirectX::SimpleMath::Vector3 scale = { 1.0f, 1.0f, 1.0f };

		//convert euler angles into a quaternion for the rotation of the object
		XMVECTOR inv_rotate = Quaternion::CreateFromYawPitchRoll(-m_camOrientation.y *3.1415 / 180,
			-m_camOrientation.x *3.1415 / 180,
			-m_camOrientation.z *3.1415 / 180);

		DirectX::SimpleMath::Vector3 inv_position = m_camPosition * DirectX::SimpleMath::Vector3(-1.0f, -1.0f, -1.0f);

		world_to_cam = XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, inv_rotate, inv_position);

		return world_to_cam;
	}

	inline DirectX::SimpleMath::Vector3 GetCamPosition() { return m_camPosition; };
	inline DirectX::SimpleMath::Vector3 GetCamOrientation() { return m_camOrientation; };

	inline DirectX::SimpleMath::Vector3 GetCamLookAt() { return m_camLookAt; };
	inline void SetCamLookAt(DirectX::SimpleMath::Vector3 lookAt) { m_camLookAt = lookAt; };

	inline DirectX::SimpleMath::Vector3 GetCamLookDirection() { return m_camLookDirection; };
	inline DirectX::SimpleMath::Vector3 GetCamForwardVector() { return m_camForward; };
	inline DirectX::SimpleMath::Vector3 GetCamRightVector() { return m_camRight; };
	inline DirectX::SimpleMath::Vector3 GetCamUpVector() { return m_camUp; };
	inline float GetCamRotationRate() { return m_camRotRate; };
	inline CameraMode GetCamMode() { return m_camMode; };
	inline void SetCamMode(CameraMode mode) { m_camMode = mode; };

private:

	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camForward;
	DirectX::SimpleMath::Vector3		m_camRight;
	DirectX::SimpleMath::Vector3		m_camUp;

	float								m_camRotRate;
	float								m_camMoveSpeed;
	float								m_camSensitivity;
	float								m_camDistance;

	CameraMode							m_camMode;

	void FreeCamUpdate(InputCommands InputCommands);
	void OrbitCamUpdate(InputCommands InputCommands);
};

