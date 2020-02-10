#pragma once

#include "pch.h"
#include "InputCommands.h"

class Camera
{
public:
	Camera(	float camMoveSpeed = 0.30f,
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

	inline DirectX::SimpleMath::Vector3 GetCamPosition() { return m_camPosition; };
	inline DirectX::SimpleMath::Vector3 GetCamOrientation() { return m_camOrientation; };
	inline DirectX::SimpleMath::Vector3 GetCamLookAt() { return m_camLookAt; };
	inline DirectX::SimpleMath::Vector3 GetCamLookDirection() { return m_camLookDirection; };
	inline DirectX::SimpleMath::Vector3 GetCamForwardVector() { return m_camForward; };
	inline DirectX::SimpleMath::Vector3 GetCamRightVector() { return m_camRight; };
	inline DirectX::SimpleMath::Vector3 GetCamUpVector() { return m_camUp; };
	inline float GetCamRotationRate() { return m_camRotRate; };

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
};

