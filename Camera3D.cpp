#include "Camera3D.h"

Camera3D::Camera3D()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	cLookX = 0.0f;
	cLookY = 0.0f;
	cLookZ = 1.0f;
}

Camera3D::~Camera3D()
{}

void Camera3D::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void Camera3D::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

void Camera3D::LookAt(float x, float y, float z)
{
	cLookX = x;
	cLookY = y;
	cLookZ = z;
}

DirectX::XMFLOAT3 Camera3D::GetPosition()
{
	return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 Camera3D::GetRotation()
{
	return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void Camera3D::Render()
{
	DirectX::XMFLOAT3 up, position, lookAt;
	DirectX::XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	DirectX::XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Load it into a XMVECTOR structure.
	upVector = XMLoadFloat3(&up);

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Load it into a XMVECTOR structure.
	positionVector = XMLoadFloat3(&position);

	// Setup where the camera is looking by default.
	lookAt.x = cLookX;
	lookAt.y = cLookY;
	lookAt.z = cLookZ;

	// Load it into a XMVECTOR structure.
	lookAtVector = XMLoadFloat3(&lookAt);

	lookAtVector = DirectX::XMVector3Normalize(lookAtVector);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	//pitch = m_rotationX * 0.0174532925f;
	//yaw = m_rotationY * 0.0174532925f;
	//roll = m_rotationZ * 0.0174532925f;

	pitch = m_rotationX;
	yaw = m_rotationY;
	roll = m_rotationZ;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = DirectX::XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = DirectX::XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}

void Camera3D::GetViewMatrix(DirectX::XMMATRIX& out)
{
	out = m_viewMatrix;
}

