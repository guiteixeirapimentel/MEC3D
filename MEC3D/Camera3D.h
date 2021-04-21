#pragma once

#include <DirectXMath.h>

class Camera3D
{
public:
	Camera3D();
	~Camera3D();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void LookAt(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	float cLookX, cLookY, cLookZ;
	DirectX::XMMATRIX m_viewMatrix;
};
