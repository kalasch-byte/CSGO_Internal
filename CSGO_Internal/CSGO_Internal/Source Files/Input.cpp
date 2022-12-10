#include "../Header Files/Input.hpp"
#include <string>
#include <Windows.h>

CInputManager g_input;

const char* const keyNames[] =
{
	"unknown",
	"mouse1",
	"mouse2",
	"cancel",
	"mouse3",
	"mouse4",
	"mouse5",
	"unknown",
	"backspace",
	"Tab",
	"unknown",
	"unknown",
	"clear",
	"return",
	"unknown",
	"unknown",
	"shift",
	"control",
	"alt",
	"pause",
	"caps lock",
	"kana",
	"unknown",
	"junja",
	"final",
	"kanji",
	"unknown",
	"escape",
	"convert",
	"nonconvert",
	"accept",
	"modechange",
	"space",
	"prior",
	"next",
	"end",
	"home",
	"left",
	"up",
	"right",
	"down",
	"select",
	"print",
	"execute",
	"snapshot",
	"ins",
	"delete",
	"help",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"lwin",
	"rwin",
	"apps",
	"unknown",
	"unknown",
	"numpad0",
	"numpad1",
	"numpad2",
	"numpad3",
	"numpad4",
	"numpad5",
	"numpad6",
	"numpad7",
	"numpad8",
	"numpad9",
	"multiply",
	"add",
	"separator",
	"subtract",
	"decimal",
	"divide",
	"f1",
	"f2",
	"f3",
	"f4",
	"f5",
	"f6",
	"f7",
	"f8",
	"f9",
	"f10",
	"f11",
	"f12",
	"f13",
	"f14",
	"f15",
	"f16",
	"f17",
	"f18",
	"f19",
	"f20",
	"f21",
	"f22",
	"f23",
	"f24",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"numlock",
	"scroll",
	"oem_nec_equal",
	"oem_fj_masshou",
	"oem_fj_touroku",
	"oem_fj_loya",
	"oem_fj_roya",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"unknown",
	"lshift",
	"rshift",
	"lcontrol",
	"rcontrol",
	"lmenu",
	"rmenu",
};

void CInputManager::CaptureMouseMove(long lparam)
{
	m_iMousePos[0] = LOWORD(lparam);
	m_iMousePos[1] = HIWORD(lparam);
}

bool CInputManager::RegisterKeyPress(VirtualKeyEvents_t keyEvent, VirtualKeys_t key)
{
	switch (keyEvent)
	{
		case KEYDOWN:
		{
			if (IsValidKey(key))
				m_bPressedKeys[key] = true;

			return true;
		}

		case KEYUP:
		{
			if (IsValidKey(key))
				m_bPressedKeys[key] = false;

			return true;
		}

		case SYSKEYDOWN:
		{
			if (key == KEYS_ALT)
				m_bPressedKeys[key] = true;

			break;
		}

		case SYSKEYUP:
		{
			if (key == KEYS_ALT)
				m_bPressedKeys[key] = false;

			break;
		}

		case LBUTTONDOWN:
			m_bPressedKeys[KEYS_MOUSE1] = true;
			return true;

		case LBUTTONUP:
			m_bPressedKeys[KEYS_MOUSE1] = false;
			return true;

		case RBUTTONDOWN:
			m_bPressedKeys[KEYS_MOUSE2] = true;
			return true;

		case RBUTTONUP:
			m_bPressedKeys[KEYS_MOUSE2] = false;
			return true;

		case MBUTTONDOWN:
			m_bPressedKeys[KEYS_MOUSE3] = true;
			return true;

		case MBUTTONUP:
			m_bPressedKeys[KEYS_MOUSE3] = false;
			return true;

		case XBUTTONDOWN:
		{
			bool pressed_xbutton = static_cast<bool>(HIWORD(key) - 1); // Should result in mouse4 as false, and mouse5 as true.
			m_bPressedKeys[pressed_xbutton ? KEYS_MOUSE5 : KEYS_MOUSE4] = true;
			return true;
		}

		case XBUTTONUP:
		{
			bool pressed_xbutton = static_cast<bool>(HIWORD(key) - 1); // Should result in mouse4 as false, and mouse5 as true.
			m_bPressedKeys[pressed_xbutton ? KEYS_MOUSE5 : KEYS_MOUSE4] = false;
			return true;
		}

		case MOUSEWHEEL:
		{
			short scroll_input = (short)HIWORD(key);
			m_iScrollWheelState = scroll_input > 0 ? 1 : -1;
			return true;
		}
	}

	return keyEvent == 0x200 || keyEvent == 0x203 || keyEvent == 0x206 || keyEvent == 0x209; // Gotta block WM_MOUSEFIST | WM_LBUTTONDBLCLK | WM_RBUTTONDBLCLK | WM_MBUTTONDBLCLK
}

bool CInputManager::IsKeyPressed(int key)
{
	auto k = static_cast<VirtualKeys_t>(key);
	return IsValidKey(k) && m_bPressedKeys[k];
}

const char* CInputManager::GetKeyName(VirtualKeys_t key)
{
	if (!IsValidKey(key))
		return keyNames[KEYS_NONE];

	return keyNames[key];
}

VirtualKeys_t CInputManager::IsAnyKeyPressed()
{
	// uint16_t?
	for (size_t i{}; i < KEYS_MAX; ++i)
	{
		if (m_bPressedKeys[i])
			return VirtualKeys_t(i);
	}

	return KEYS_NONE;
}

int CInputManager::GetScrollState()
{
	// uint16_t?
	int current_state = m_iScrollWheelState;
	m_iScrollWheelState = 0;

	return current_state;
}

char CInputManager::GetPressedChar(VirtualKeys_t* out)
{
	// uint16_t?
	size_t pressedCharacter{};
	for (size_t i{}; i < KEYS_MAX; ++i)
	{
		if (IsKeyPressed(VirtualKeys_t(i)))
		{
			if ((i >= KEYS_A && i <= KEYS_Z) || (i >= KEYS_N0 && i <= KEYS_N9))
				pressedCharacter = i;
		}
	}

	if (pressedCharacter)
	{
		if (out)
			*out = VirtualKeys_t(pressedCharacter);

		if (IsKeyPressed(KEYS_SHIFT))
		{
			if (pressedCharacter >= KEYS_A && pressedCharacter <= KEYS_Z)
				return char(pressedCharacter);

			// Shift it to symbols
			if (pressedCharacter >= KEYS_N0 && pressedCharacter <= KEYS_N9)
			{
				switch (pressedCharacter)
				{
					case KEYS_N0:
						return ')';
					case KEYS_N1:
						return '!';
					case KEYS_N2:
						return '@';
					case KEYS_N3:
						return '#';
					case KEYS_N4:
						return '$';
					case KEYS_N5:
						return '%';
					case KEYS_N6:
						return '^';
					case KEYS_N7:
						return '&';
					case KEYS_N8:
						return '*';
					case KEYS_N9:
						return '(';
				}
			}
		}
		else
		{
			if (pressedCharacter >= KEYS_A && pressedCharacter <= KEYS_Z)
				return char(tolower(pressedCharacter));

			if (pressedCharacter >= KEYS_N0 && pressedCharacter <= KEYS_N9)
				return char(pressedCharacter);
		}
	}
	else if (IsKeyPressed(KEYS_SPACE))
	{
		if (out)
			*out = KEYS_SPACE;

		return ' ';
	}
	else if (IsKeyPressed(KEYS_BACK))
	{
		if (out)
			*out = KEYS_BACK;

		return 0;
	}

	if (out)
		*out = KEYS_NONE;

	return 0;
}
