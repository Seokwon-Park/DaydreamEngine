#pragma once

namespace Daydream
{
	struct Light
	{
        enum LightType {
            Directional = 0,
            Point = 1,
            Spot = 2
        };
        UInt32 type = Directional;
        Vector3 color = Vector3();
        Vector3 position = Vector3();
        Float32 intensity = 1.0f;
        Vector3 direction = Vector3(0.0f,0.0f,1.0f); 
        Float32 padding;
	};
}