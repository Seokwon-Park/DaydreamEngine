#pragma once

#include "Daydream/Graphics/Resources/Mesh.h"
#include "Daydream/Graphics/Resources/Texture/Texture2D.h"
#include "Daydream/Graphics/Resources/Texture/TextureCube.h"
#include "Daydream/Graphics/Resources/PipelineState.h"

namespace Daydream
{
    class Skybox
    {
    public:
        Skybox();
        ~Skybox();

        void CreateResources();
        void GenerateDefault();
        void Update();

        void GenerateHDRCubemap(Shared<Texture2D> _texture);
        void GenerateIrradianceCubemap();
        void GeneratePrefilterCubemap();
        void GenerateBRDF();
        void UpdateSkyboxFace(UInt32 _faceIndex, Shared<Texture2D> _texture);

        inline void SetSkyboxResolution(UInt32 _value) { skyboxResolution = _value; }
        inline void SetDiffuseResolution(UInt32 _value) { diffuseResolution = _value; }
        inline void SetSpecularResolution(UInt32 _value) { specularResolution = _value; }

        inline Shared<TextureView> GetSkyboxFaceSRV(UInt32 _faceIndex)
        {
            DAYDREAM_ASSERT(_faceIndex < 6, "{} < 6", _faceIndex);
            return skyboxFaceSRVs[_faceIndex];
        }

        inline Shared<Texture2D> GetBRDF() 
        {
            return BRDFTexture; 
        }
        inline Shared<TextureCube> GetSkyboxTexture()
        {
            return skyboxTextureCube; 
        }
        inline Shared<TextureCube> GetIrradianceTexture()
        {
            return irradianceTextureCube; 
        }
        inline Shared<TextureCube> GetPrefilterTexture()
        { 
            return prefilterTextureCube; 
        }
    private:
        UInt32 skyboxMipLevels = 0;
        UInt32 prefilterMipLevels = 0;

        Bool isUsingSkybox = true;
        Bool isHDR = false;

        UInt32 skyboxResolution = 512;
        UInt32 diffuseResolution = 64;
        UInt32 specularResolution = 512;

        Vector4 roughness = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

        Matrix4x4 cubeFaceProjMatrix;
        Array<Matrix4x4> cubeFaceViewMatrices;
        Array<Matrix4x4> captureViewProjections;
        Array<Shared<ConstantBuffer>> cubeFaceConstantBuffers;
        Array<Shared<ConstantBuffer>> roughnessConstantBuffers;

        //Shared<Material> equirectangularMaterial;
        //Array<Shared<Material>> equirectangularMaterials;
        //Array<Shared<Material>> irradianceMaterials;
        //Array<Shared<Material>> prefilterMaterials;
        //Shared<Material> resizeMaterial;

        //Shared<Framebuffer> captureFramebuffer;
        //Shared<Framebuffer> resizeFramebuffer;
        //Shared<Framebuffer> irradianceFramebuffer;
        //Array<Shared<Framebuffer>> prefilterFramebuffers;

        //Shared<RenderPass> equirectangularRenderPass;
        //Shared<RenderPass> resizeRenderPass;
        //Shared<RenderPass> irradianceRenderPass;

        Shared<PipelineState> equirectangularPSO;
        Shared<PipelineState> irradiancePSO;
        Shared<PipelineState> prefilterPSO;
        Shared<PipelineState> resizePSO;
        Shared<PipelineState> brdfPSO;

        Shared<Mesh> boxMesh;

        Shared<Mesh> quadMesh;

        Shared<Texture2D> equirectangularTexture; // 2D->Cube 원본 텍스쳐
        Shared<TextureView> equirectangularSRV ; // 2D->Cube 원본 텍스쳐뷰
        Shared<Texture2D> equirectangularDropTarget; // dummy(No Texture)

        Array<Shared<Texture2D>> equirectangularResultTextures;
        Array<Shared<Texture2D>> irradianceResultTextures;
        Array<Shared<Texture2D>> prefilterResultTextures;

        Shared<TextureCube> skyboxTextureCube;
        Array<Shared<TextureView>> skyboxFaceRTVs;
        Array<Shared<TextureView>> skyboxFaceSRVs;
        

        Shared<TextureCube> irradianceTextureCube;
        Array<Shared<TextureView>> irradianceRTV;

        Shared<TextureCube> prefilterTextureCube;
        Array<Shared<TextureView>> prefilterRTV;
        
        Shared<Texture2D> BRDFTexture;
        Shared<TextureView> BRDFRTV;

        Shared<Texture2D> resizeTexture;
        Shared<TextureView> resizeRTV;
    };
}