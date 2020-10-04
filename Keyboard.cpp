#include "Keyboard.h"

KeyboardClient::KeyboardClient(KeyboardServer& kServer)
	: server(kServer)
{}

bool KeyboardClient::KeyIsPressed(unsigned char keycode) const
{
	return server.keystates[keycode];
}

unsigned char KeyboardClient::ReadKey()
{
	if (server.cKeybuffer.size() > 0)
	{
		unsigned char keycode = server.cKeybuffer.front();
		server.cKeybuffer.pop();
		return keycode;
	}
	else
	{
		return 0;
	}
}

unsigned char KeyboardClient::PeekKey() const
{
	if (server.cKeybuffer.size() > 0)
	{
		return server.cKeybuffer.front();
	}
	else
	{
		return 0;
	}
}

unsigned char KeyboardClient::ReadChar()
{
	if (server.cCharbuffer.size() > 0)
	{
		unsigned char charcode = server.cCharbuffer.front();
		server.cCharbuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

unsigned char KeyboardClient::PeekChar() const
{
	if (server.cCharbuffer.size() > 0)
	{
		return server.cCharbuffer.front();
	}
	else
	{
		return 0;
	}
}

void KeyboardClient::FlushKeyBuffer()
{
	while (!server.cKeybuffer.empty())
	{
		server.cKeybuffer.pop();
	}
}

void KeyboardClient::FlushCharBuffer()
{
	while (!server.cCharbuffer.empty())
	{
		server.cCharbuffer.pop();
	}
}

void KeyboardClient::FlushBuffers()
{
	FlushKeyBuffer();
	FlushCharBuffer();
}

KeyboardServer::KeyboardServer()
{
	for (int x = 0; x < cNKeys; x++)
	{
		keystates[x] = false;
	}
}

void KeyboardServer::OnKeyPressed(unsigned char keycode)
{
	keystates[keycode] = true;

	cKeybuffer.push(keycode);
	if (cKeybuffer.size() > cBufferSize)
	{
		cKeybuffer.pop();
	}
}

void KeyboardServer::OnKeyReleased(unsigned char keycode)
{
	keystates[keycode] = false;
}

void KeyboardServer::OnChar(unsigned char character)
{
	cCharbuffer.push(character);
	if (cCharbuffer.size() > cBufferSize)
	{
		cCharbuffer.pop();
	}
}

