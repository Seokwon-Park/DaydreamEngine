#pragma once

namespace Daydream
{
    enum LightType : UInt32
    {
        Directional = 0,
        Point = 1,
        Spot = 2
    };

    struct DirectionalLight
    {
        Vector3 direction;
        Float32 intensity;
        Vector3 color;    
        Float32 padding1; 
    }; 

    struct PointLight 
    {
        Vector3 position;
        Float32 range; 
        Vector3 color;
        Float32 intensity;
    }; 

    struct SpotLight 
    {
        Vector3 position;
        Float32 range; 
        Vector3 direction;
        Float32 intensity;
        Vector3 color;
        Float32 innerConeCos;
        Vector3 padding1;
        Float32 outerConeCos;
    };

	struct Light
	{
        UInt32 type = LightType::Directional;
        Vector3 direction;
        Float32 intensity = 1.0f;
        Vector3 color = { 1.0f, 1.0f, 1.0f };
        Float32 range = 10.0f;   // Point, Spot Ÿ���� ���
        Vector3 position;        // Point, Spot Ÿ���� ���
        Float32 spotInnerAngle;  // Spot Ÿ�Ը� ���
        Float32 spotOuterAngle;  // Spot Ÿ�Ը� ���
	};
}