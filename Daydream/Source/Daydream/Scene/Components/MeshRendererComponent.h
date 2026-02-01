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
		inline AssetHandle GetMesh() { return meshHandle; }
		inline void SetMaterial(const AssetHandle _materialHandle) { materialHandle = _materialHandle; }
		inline AssetHandle GetMaterial() { return materialHandle; }

		void Render();
		void RenderMeshOnly();
		void RenderDepth();

		REFLECT_START()
			ADD_PROPERTY(FieldType::Mesh, meshHandle)
			ADD_PROPERTY(FieldType::Material, materialHandle)
		REFLECT_END()
	protected:

	private:
		AssetHandle meshHandle;
		AssetHandle materialHandle;


		Shared<Material> maskMaterial;
		Shared<Material> lightMaterial;
	};
}
