#pragma once

enum class KEY_TYPE
{
	UP = VK_UP,		   // �̵�
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,
	SPACE = VK_SPACE,  // ����

	X = 'X', 
	

	W = 'W',
	A = 'A',		   
	S = 'S',		   // ����
	D = 'D',

	Q = 'Q',		   // �ɷ� ������
	E = 'E',
	Z = 'Z',

	LBUTTON = VK_LBUTTON,
	RBUTTON = VK_RBUTTON,
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::END),
};

class InputManager
{
public:
	void Init(HWND hwnd);
	void Update();

	// ������ ���� ��
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// �� ó�� ������ ��
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// �� ó�� ������ ���� ��
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }
	
	const POINT& GetMousePos() { return _mousePos; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd;
	vector<KEY_STATE> _states;
	POINT _mousePos = {};
};
