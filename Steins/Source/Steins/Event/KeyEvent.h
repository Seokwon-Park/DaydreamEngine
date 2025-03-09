#pragma once
#include "Event.h"

namespace Steins 
{
	class KeyEvent : public Event
	{
	public:
		inline int32 GetKeyCode() const { return keyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int _keyCode)
			:keyCode(_keyCode) {}
		int keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int _keyCode, int _reapeatCount)
			: KeyEvent(_keyCode), repeatCount(_reapeatCount) {}
		inline int32 GetRepeatCount() const { return repeatCount; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int32 repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int _keyCode)
			: KeyEvent(_keyCode) {}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int _keyCode)
			: KeyEvent(_keyCode) {
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}

