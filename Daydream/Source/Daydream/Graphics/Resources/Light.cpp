#include "DaydreamPCH.h"
#include "Daydream/Graphics/Resources/Light.h"

namespace Daydream
{
    Light::Light()
    {
        lightViewProjectionBuffer = ConstantBuffer::Create(sizeof(LightSpaceData));

        UpdateProjectionMatrix();
    }
    void Light::UpdateViewMatrix(const Transform& _transform)
    {
        Vector3 direction = _transform.GetForward();
        Vector3 up = _transform.GetUp();
        lightViewMatrix = Matrix4x4::CreateLookTo(-direction * 10.0f, direction, up);

        UpdateViewProjectionMatrix();
    }

    void Light::UpdateProjectionMatrix()
    {
        lightProjectionMatrix = Matrix4x4::CreateOrthographic(-20.0f, 20.0f, -20.0f, 20.0f, -200.0f, 200.0f);
        
        UpdateViewProjectionMatrix();
    }

    void Light::UpdateViewProjectionMatrix()
    {
        lightViewProjectionMatrix = lightViewMatrix * lightProjectionMatrix;
        lightViewProjectionMatrix.Transpose();

        LightSpaceData data;
        data.viewProjection = lightViewProjectionMatrix;

        lightViewProjectionBuffer->Update(&data, sizeof(LightSpaceData));
    }
}