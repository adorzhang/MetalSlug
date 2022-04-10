#pragma once
#include "System.h"
#include "KeyEventHandler.h"
#include <vector>

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

class CInputHandler : public CSystem
{
public:
	void SetHandleWindow(HWND hWnd) { this->hWnd = hWnd; }
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { this->keyHandler = handler; }

	void Initialize();
	void ProcessKeyboard();
	int IsKeyDown(int keyCode);

	int OnKeyDown(int KeyCode);

private:
	HWND hWnd;
	LPDIRECTINPUT8 di;										// The DirectInput object 
	LPDIRECTINPUTDEVICE8 didv;								// The keyboard device 

	LPKEYEVENTHANDLER keyHandler;

	std::vector<int> presses;

	BYTE keyStates[256];									// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data
};