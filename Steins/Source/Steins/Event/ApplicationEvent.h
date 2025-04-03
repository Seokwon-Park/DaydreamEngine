#pragma once

#include "Event.h"

namespace Steins
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(UInt32 _width, UInt32 _height)
			: width(_width), height(_height) {}
		inline UInt32 GetWidth() const { return width; }
		inline UInt32 GetHeight() const { return height; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		UInt32 width, height;
	};
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}
		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}

