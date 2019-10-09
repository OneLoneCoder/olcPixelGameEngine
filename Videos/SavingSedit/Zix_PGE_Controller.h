#pragma once

#ifdef WIN32
#include <windows.h>
#include <xinput.h>

typedef DWORD(WINAPI XInputGetState_t)(DWORD dwUserIndex, XINPUT_STATE* pState);
static XInputGetState_t* XInputStateGet;

typedef DWORD(WINAPI XInputSetState_t)(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration);
static XInputSetState_t* XInputStateSet;
#endif

#define C_BUTTON_COUNT 14
enum CButton
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	START,
	BACK,
	A,
	B,
	X,
	Y,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	LEFT_THUMB,
	RIGHT_THUMB
};

struct CBState
{
	bool bPressed = false;
	bool bReleased = false;
	bool bHeld = false;
};

class ControllerManager
{
private:
	bool buttonState[C_BUTTON_COUNT];
	bool lastButtonState[C_BUTTON_COUNT];

	// Trigger values are in the range of 0 to 1, where 0 is fully
	// released and 1 is fully pressed.
	float triggerLeft = 0;
	float triggerRight = 0;

	// Stick values are in the range of -1 to 1. For X values, -1 is
	// all the way to the left while +1 is all the way to the right.
	float leftStickX = 0;
	float leftStickY = 0;
	float rightStickX = 0;
	float rightStickY = 0;

	// Whether or not the controller is plugged in.
	bool pluggedIn = true;

	bool vibrating = false;
	float vibrateTime = 0;
	float vibrateCounter = 0;

public:
	bool Initialize();
	void Update(float dt);

	void Vibrate(short amt, int timeMs);

	CBState GetButton(CButton button);

	float GetLeftTrigger() { return triggerLeft; }
	float GetRightTrigger() { return triggerRight; }

	float GetLeftStickX() { return leftStickX; }
	float GetLeftStickY() { return leftStickY; }

	float GetRightStickX() { return rightStickX; }
	float GetRightStickY() { return rightStickY; }

	bool IsVibrating() { return vibrating; }
	bool IsPluggedIn() { return pluggedIn; }

private:
	float NormalizeStickValue(short value);
};

bool ControllerManager::Initialize()
{
#ifdef WIN32
	// TODO: Should we check for version 9.1.0 if we fail to find 1.4?
	HMODULE lib = LoadLibraryA("xinput1_4.dll");

	if (!lib) return false;

	XInputStateGet = (XInputGetState_t*)GetProcAddress(lib, "XInputGetState");
	XInputStateSet = (XInputSetState_t*)GetProcAddress(lib, "XInputSetState");
#endif

	return true;
}

float ControllerManager::NormalizeStickValue(short value)
{
	// The value we are given is in the range -32768 to 32767 with some deadzone around zero.
	// We will assume all values in this dead zone to be a reading of zero (the stick is not moved).
	if (value > -7000 && value < 7000) return 0;

	// Otherwise, we are going to normalize the value.
	return ((value + 32768.0f) / (32768.0f + 32767.0f) * 2) - 1;
}

void ControllerManager::Vibrate(short amt, int timeMs)
{
	// If we are already vibrating, just ignore this, unless they say zero, in which case we will let them stop it.
	if (vibrating && amt != 0) return;

	// Only start the timer if we are actually vibrating.
	if (amt != 0)
	{
		vibrateTime = timeMs / 1000.0f;
		vibrating = true;
	}
#ifdef WIN32
	XINPUT_VIBRATION info =
	{
		amt,
		amt
	};
	XInputStateSet(0, &info);
#endif
}

CBState ControllerManager::GetButton(CButton button)
{
	return
	{
		!lastButtonState[button] && buttonState[button],
		lastButtonState[button] && !buttonState[button],
		lastButtonState[button] && buttonState[button]
	};
}

void ControllerManager::Update(float dt)
{
#ifdef WIN32
	if (vibrating)
	{
		vibrateCounter += dt;
		if (vibrateCounter >= vibrateTime)
		{
			XINPUT_VIBRATION info =
			{
				0, 0
			};
			XInputStateSet(0, &info);

			vibrating = false;
			vibrateCounter = 0;
			vibrateTime = 0;
		}
	}

	for (int i = 0; i < C_BUTTON_COUNT; i++)
	{
		lastButtonState[i] = buttonState[i];
	}

	XINPUT_STATE state;

	// Try and get the first controller. For now we will only support a single one.
	DWORD res = XInputStateGet(0, &state);

	// If the controller is plugged in, handle input.
	if (res == ERROR_SUCCESS)
	{
		XINPUT_GAMEPAD* pad = &state.Gamepad;

		buttonState[UP] = (pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
		buttonState[DOWN] = (pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
		buttonState[LEFT] = (pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
		buttonState[RIGHT] = (pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
		buttonState[START] = (pad->wButtons & XINPUT_GAMEPAD_START);
		buttonState[BACK] = (pad->wButtons & XINPUT_GAMEPAD_BACK);
		buttonState[LEFT_SHOULDER] = (pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
		buttonState[RIGHT_SHOULDER] = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
		buttonState[LEFT_THUMB] = (pad->wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
		buttonState[RIGHT_THUMB] = (pad->wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);
		buttonState[A] = (pad->wButtons & XINPUT_GAMEPAD_A);
		buttonState[B] = (pad->wButtons & XINPUT_GAMEPAD_B);
		buttonState[X] = (pad->wButtons & XINPUT_GAMEPAD_X);
		buttonState[Y] = (pad->wButtons & XINPUT_GAMEPAD_Y);

		triggerLeft = pad->bLeftTrigger / 255.0f;
		triggerRight = pad->bRightTrigger / 255.0f;
		leftStickX = NormalizeStickValue(pad->sThumbLX);
		leftStickY = NormalizeStickValue(pad->sThumbLY);
		rightStickX = NormalizeStickValue(pad->sThumbRX);
		rightStickY = NormalizeStickValue(pad->sThumbRY);

		if (!pluggedIn)
		{
			pluggedIn = true;
			// Send callback.
			// printf("Plugged in.\n");
		}
	}
	else
	{
		if (pluggedIn)
		{
			pluggedIn = false;
			// Send callback.
			// printf("Unplugged.\n");
		}
	}
#else
	for (int i = 0; i < C_BUTTON_COUNT; i++)
	{
		lastButtonState[i] = buttonState[i] = false;
	}
#endif
}