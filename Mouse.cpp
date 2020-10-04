#include "Mouse.h"

MouseClient::MouseClient(MouseServer& server)
	: server(server)
{}
int MouseClient::GetMouseX() const
{
	return server.x;
}
int MouseClient::GetMouseY() const
{
	return server.y;
}
bool MouseClient::LeftIsPressed() const
{
	return server.leftIsPressed;
}
bool MouseClient::RightIsPressed() const
{
	return server.rightIsPressed;
}
bool MouseClient::WheelIsPressed() const
{
	return server.wheelIsPressed;
}
bool MouseClient::IsInWindow() const
{
	return server.isInWindow;
}
MouseEvent MouseClient::ReadMouse()
{
	if (server.buffer.size() > 0)
	{
		MouseEvent e = server.buffer.front();
		server.buffer.pop();
		return e;
	}
	else
	{
		return MouseEvent(MouseEvent::Invalid, 0, 0);
	}
}
bool MouseClient::MouseEmpty() const
{
	return server.buffer.empty();
}


MouseServer::MouseServer()
	: isInWindow(false),
	leftIsPressed(false),
	rightIsPressed(false),
	x(-1),
	y(-1)
{}
void MouseServer::OnMouseMove(int x, int y)
{
	this->x = x;
	this->y = y;

	buffer.push(MouseEvent(MouseEvent::Move, x, y));
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
void MouseServer::OnMouseLeave()
{
	isInWindow = false;
}
void MouseServer::OnMouseEnter()
{
	isInWindow = true;
}
void MouseServer::OnLeftPressed(int x, int y)
{
	leftIsPressed = true;

	buffer.push(MouseEvent(MouseEvent::LPress, x, y));
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
void MouseServer::OnLeftReleased(int x, int y)
{
	leftIsPressed = false;

	buffer.push(MouseEvent(MouseEvent::LRelease, x, y));
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
void MouseServer::OnRightPressed(int x, int y)
{
	rightIsPressed = true;

	buffer.push(MouseEvent(MouseEvent::RPress, x, y));
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
void MouseServer::OnRightReleased(int x, int y)
{
	rightIsPressed = false;

	buffer.push(MouseEvent(MouseEvent::RRelease, x, y));
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
void MouseServer::OnWheelUp(int x, int y)
{
	buffer.push(MouseEvent(MouseEvent::WheelUp, x, y));
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void MouseServer::OnWheelPress()
{
	wheelIsPressed = true;
}

void MouseServer::FlushBuffers()
{
	while (!buffer.empty())
	{
		buffer.pop();
	}
}

void MouseServer::OnWheelRelease()
{
	wheelIsPressed = false;
}
void MouseServer::OnWheelDown(int x, int y)
{
	buffer.push(MouseEvent(MouseEvent::WheelDown, x, y));
	if (buffer.size() > bufferSize)
	{
		buffer.pop();
	}

}
bool MouseServer::IsInWindow() const
{
	return isInWindow;
}

void MouseClient::FlushBuffers()
{
	server.FlushBuffers();
}