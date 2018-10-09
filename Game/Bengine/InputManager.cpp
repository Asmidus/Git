#include "stdafx.h"
#include "InputManager.h"

namespace Bengine {
	InputManager::InputManager() : _mouseCoords(0.0f) {}


	InputManager::~InputManager() {}

	void Bengine::InputManager::pressKey(unsigned int keyID) {
		_keyMap[keyID] = true;
	}

	void Bengine::InputManager::releaseKey(unsigned int keyID) {
		_keyMap[keyID] = false;
	}

	bool Bengine::InputManager::isKeyPressed(unsigned int keyID) {
		auto it = _keyMap.find(keyID);
		if (it != _keyMap.end()) {
			return it->second;
		}
		return false;
		//return _keyMap[keyID];
	}
	void InputManager::setMouseCoords(float x, float y) {
		_mouseCoords.x = x;
		_mouseCoords.y = y;
	}
}