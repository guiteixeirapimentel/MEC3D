#pragma once
#include <Windows.h>
#include <queue>

class KeyboardServer;

class KeyboardClient
{
public:
	KeyboardClient(KeyboardServer& kServer);
	bool KeyIsPressed(unsigned char keycode) const;
	unsigned char ReadKey();
	unsigned char PeekKey() const;
	unsigned char ReadChar();
	unsigned char PeekChar() const;
	void FlushKeyBuffer();
	void FlushCharBuffer();
	void FlushBuffers();
private:
	KeyboardServer& server;
};

class KeyboardServer
{
	friend KeyboardClient;
public:
	KeyboardServer();
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(unsigned char character);
private:
	static constexpr int cNKeys = 256;
	static constexpr int cBufferSize = 4;
	bool keystates[cNKeys];
	std::queue<unsigned char> cKeybuffer;
	std::queue<unsigned char> cCharbuffer;
};