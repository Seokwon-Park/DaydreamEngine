#pragma once

#include "Event.h"

namespace Steins
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float _mouseX, float _mouseY)
			:mouseX(_mouseX), mouseY(_mouseY) {}
		inline Float32 GetX() const { return mouseX; }
		inline Float32 GetY() const { return mouseY; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		Float32 mouseX, mouseY;
	};
	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float _mouseXOffset, float _mouseYOffset)
			:mouseXOffset(_mouseXOffset), mouseYOffset(_mouseYOffset) {}
		inline Float32 GetXOffset() const { return mouseXOffset; }
		inline Float32 GetYOffset() const { return mouseYOffset; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		Float32 mouseXOffset, mouseYOffset;
	};
	class MouseButtonEvent : public Event
	{
	public:
		inline Int32 GetMouseButton() const { return mouseButton; }
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int _mouseButton)
			:mouseButton(_mouseButton) {}
		int mouseButton;
	};
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int _mouseButton)
			: MouseButtonEvent(_mouseButton) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mouseButton;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int _mouseButton)
			: MouseButtonEvent(_mouseButton) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mouseButton;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}