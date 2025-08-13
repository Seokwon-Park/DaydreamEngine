#pragma once

#include "Daydream/Graphics/Resources/Light.h"

namespace Daydream::UI
{
	void DrawDragFloat(const String& _label, Float32& _value, Float32 _resetValue = 0.0f, Float32 _columnWidth = 100.0f);

	void DrawAxisControl(const String& _label, Float32* _value, Float32 _speed = 0.1f, Float32 _minValue = 0.0f, Float32 _maxValue = 0.0f, Float32 _resetValue = 0.0f, Vector4 _color = { 0.2f, 0.2f,0.2f,1.0f });
	void DrawVector3Controller(const String& _label, Vector3& _values,
		Float32 _resetValue = 0.0f, Float32 _columnWidth = 100.0f);
	void DrawTransformController(const String& _label, Transform& _transform,
		Float32 _resetValue = 0.0f, Float32 _columnWidth = 100.0f);

	//void MaterialControl(const String& _material);

	void DrawLightController(const String& _label, Light& _light);
	void DrawColorEditor(const String& _label, Vector3& _color,
		Float32 _resetValue = 0.0f, Float32 _columnWidth = 100.0f);
}