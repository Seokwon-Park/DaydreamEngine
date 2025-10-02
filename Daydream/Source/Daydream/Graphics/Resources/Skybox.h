#pragma once

#include "Daydream.h"

namespace Daydream
{
    class Skybox
    {
    public:
        Skybox();

        void Init();

        void GenerateHDRCubemap(Shared<Texture2D> _texture);
        void GenerateIrradianceCubemap();
        void GenerateBRDF();
        void UpdateSkyboxFace(UInt32 _faceIndex, Shared<Texture2D> _texture);

        Shared<Texture2D> GetBRDF() { return BRDFTexture; }
        Shared<TextureCube> GetSkyboxTexture() { return skyboxTextureCube; }
        Shared<TextureCube> GetIrradianceTexture() { return irradianceTextureCube; }
    private:
        bool isUsingSkybox = true;
        bool isHDR = false;

        Matrix4x4 cubeFaceProjMatrix;
        Array<Matrix4x4> cubeFaceViewMatrices;
        Array<Matrix4x4> captureViewProjections;
        Array<Shared<ConstantBuffer>> cubeFaceConstantBuffers;

        Array<Shared<Material>> equirectangularMaterials;
        Array<Shared<Material>> irradianceMaterials;
        Shared<Material> resizeMaterial;

        Shared<Framebuffer> captureFramebuffer;
        Shared<Framebuffer> resizeFramebuffer;
        Shared<Framebuffer> irraidanceFramebuffer;

        Shared<RenderPass> equirectangularRenderPass;
        Shared<RenderPass> resizeRenderPass;
        Shared<RenderPass> irradianceRenderPass;

        Shared<PipelineState> equirectangularPSO;
        Shared<PipelineState> irradiancePSO;
        Shared<PipelineState> resizePSO;
        Shared<PipelineState> brdfPSO;

        Shared<VertexBuffer> boxVB;
        Shared<IndexBuffer> boxIB;
        Shared<Mesh> boxMesh;

        Shared<VertexBuffer> resizeVB;
        Shared<IndexBuffer> resizeIB;
        Shared<Mesh> quadMesh;

        Shared<Texture2D> equirectangularTexture; // 2D->Cube 원본 텍스쳐

        Shared<Texture2D> equirectangularDropTarget; // 걍 더미(No Texture)

        Shared<Texture2D> resizeResultTexture;
        Array<Shared<Texture2D>> equirectangularResultTextures;
        Array<Shared<Texture2D>> irradianceResultTextures;
        Array<Shared<Texture2D>> oldTextures;

        Shared<Texture2D> BRDFTexture;
        Shared<TextureCube> skyboxTextureCube;
        Shared<TextureCube> irradianceTextureCube;
    };
}