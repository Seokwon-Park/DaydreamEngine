#pragma once

#include "Component.h"
#include "Daydream/Core/UUID.h"
#include "Daydream/Graphics/Resources/Model.h"

namespace Daydream
{
	class MeshRendererComponent : public Component
	{
	public:
		DEFINE_COMPONENT_NAME(MeshRendererComponent);

		MeshRendererComponent();
		virtual ~MeshRendererComponent();

		virtual void Init() override;
		virtual void Update(Float32 _deltaTime) override {};

		inline void SetMesh(const AssetHandle _meshHandle) { meshHandle = _meshHandle; }
		inline void SetMaterial(const AssetHandle _materialHandle) { materialHandle = _materialHandle; }
		void Render();
		void RenderMeshOnly();

		REFLECT_START()
			ADD_PROPERTY(FieldType::Mesh, meshHandle)
			ADD_PROPERTY(FieldType::Material, materialHandle)
		REFLECT_END()
	protected:

	private:
		AssetHandle meshHandle;
		AssetHandle materialHandle;

		MaterialConstantBufferData materialValue;
		Shared<ConstantBuffer> worldMatrixConstantBuffer;
		Shared<ConstantBuffer> materialCB;
		Shared<ConstantBuffer> entityHandle;

		Shared<Material> maskMaterial;
	};
}
