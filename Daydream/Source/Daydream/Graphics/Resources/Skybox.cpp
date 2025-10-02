#include "DaydreamPCH.h"
#include "Skybox.h"

#include "Daydream/Core/ResourceManager.h"
#include "Daydream/Graphics/Core/Renderer.h"
#include "Daydream/Graphics/Utility/MeshGenerator.h"

namespace Daydream
{
    Skybox::Skybox()
    {
        TextureDesc textureDesc{};
        textureDesc.width = 512;
        textureDesc.height = 512;
        textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
        textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
        skyboxTextureCube = TextureCube::CreateEmpty(textureDesc);
        skyboxTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));

        textureDesc.width = 32;
        textureDesc.height = 32;
        irradianceTextureCube = TextureCube::CreateEmpty(textureDesc);
        irradianceTextureCube->SetSampler(ResourceManager::GetResource<Sampler>("LinearClampToEdge"));


        cubeFaceViewMatrices =
        {
            Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
            Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
            Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, 1.0f)),
            Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, 1.0f,  0.0f)),
            Matrix4x4::LookTo(Vector3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f,  0.0f))
        };
        cubeFaceProjMatrix = Matrix4x4::Perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        cubeFaceConstantBuffers.resize(6);
        for (int i = 0; i < 6; i++)
        {
            cubeFaceConstantBuffers[i] = ConstantBuffer::Create(sizeof(Matrix4x4));
            captureViewProjections.push_back(cubeFaceProjMatrix * cubeFaceViewMatrices[i]);
            captureViewProjections[i].MatrixTranspose();
            cubeFaceConstantBuffers[i]->Update(&captureViewProjections[i], sizeof(Matrix4x4));
        }

        equirectangularRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
        equirectangularPSO = ResourceManager::GetResource<PipelineState>("EquirectangularPSO");

        equirectangularMaterials.resize(6);
        equirectangularResultTextures.resize(6);
        for (int i = 0; i < 6; i++)
        {
            equirectangularMaterials[i] = Material::Create(equirectangularPSO);
            equirectangularMaterials[i]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
            equirectangularMaterials[i]->SetTexture2D("Texture", equirectangularTexture);
        }

        // Resize Capture setup
        resizeRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
        resizePSO = ResourceManager::GetResource<PipelineState>("ResizePSO");

        irradianceRenderPass = ResourceManager::GetResource<RenderPass>("RGBA16FRenderPass");
        irradiancePSO = ResourceManager::GetResource<PipelineState>("IrradiancePSO");
        irradianceMaterials.resize(6);
        irradianceResultTextures.resize(6);
        for (int i = 0; i < 6; i++)
        {
            irradianceMaterials[i] = Material::Create(irradiancePSO);
            irradianceMaterials[i]->SetConstantBuffer("Camera", cubeFaceConstantBuffers[i]);
        }
        equirectangularDropTarget = ResourceManager::GetResource<Texture2D>("Resource\\NoTexture.png");

        brdfPSO = ResourceManager::GetResource<PipelineState>("BRDFPSO");

        float vertices[] = {
            -1.0f,-1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        };

        UInt32 indices[6] = { 0,1,2,2,3,0 };
        resizeVB = VertexBuffer::CreateStatic(sizeof(vertices), 20, vertices);
        resizeIB = IndexBuffer::Create(indices, 6);
        quadMesh = Mesh::Create(resizeVB, resizeIB);

        auto meshData = MeshGenerator::CreateCube();
        Array<Vector3> positions;
        for (Vertex v : meshData.vertices)
        {
            positions.push_back(v.position);
        }
        boxVB = VertexBuffer::CreateStatic(sizeof(Vector3) * positions.size(), 12, positions.data());
        boxIB = IndexBuffer::Create(meshData.indices.data(), meshData.indices.size());
        boxMesh = Mesh::Create(boxVB, boxIB);
    }

    void Skybox::Init()
    {
        //////////////////////////////////////Create Default Skybox TextureCubes;
        equirectangularTexture = ResourceManager::GetResource<Texture2D>("Resource\\skybox.hdr");
        GenerateHDRCubemap(equirectangularTexture);
        GenerateIrradianceCubemap();
        //GenerateBRDF();
    }

    void Skybox::GenerateHDRCubemap(Shared<Texture2D> _texture)
    {
        equirectangularTexture = _texture;
        for (int i = 0; i < 6; i++)
        {
            equirectangularMaterials[i]->SetTexture2D("Texture", equirectangularTexture);
        }

        FramebufferDesc fbDesc;
        fbDesc.width = skyboxTextureCube->GetWidth();
        fbDesc.height = skyboxTextureCube->GetHeight();

        captureFramebuffer = Framebuffer::Create(equirectangularRenderPass, fbDesc);

        equirectangularResultTextures.clear();
        equirectangularResultTextures.resize(6);
        Daydream::TextureDesc textureDesc{};
        textureDesc.width = skyboxTextureCube->GetWidth();
        textureDesc.height = skyboxTextureCube->GetHeight();
        textureDesc.bindFlags = RenderBindFlags::ShaderResource;
        textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
        for (int i = 0; i < 6; i++)
        {
            equirectangularRenderPass->Begin(captureFramebuffer);

            equirectangularPSO->Bind();
            boxMesh->Bind();
            equirectangularMaterials[i]->Bind();
            Renderer::Submit(boxMesh->GetIndexCount());

            equirectangularRenderPass->End();

            equirectangularResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
            Renderer::GetRenderDevice()->CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), equirectangularResultTextures[i]);
            skyboxTextureCube->Update(i, equirectangularResultTextures[i]);
            oldTextures.push_back(equirectangularResultTextures[i]);
        }
    }

    void Skybox::GenerateIrradianceCubemap()
    {
        for (int i = 0; i < 6; i++)
            irradianceMaterials[i]->SetTextureCube("TextureCubemap", skyboxTextureCube);

        FramebufferDesc fbDesc;
        fbDesc.width = 32;
        fbDesc.height = 32;

        irraidanceFramebuffer = Framebuffer::Create(irradianceRenderPass, fbDesc);

        irradianceResultTextures.clear();
        irradianceResultTextures.resize(6);
        Daydream::TextureDesc textureDesc{};
        textureDesc.width = 32;
        textureDesc.height = 32;
        textureDesc.bindFlags = RenderBindFlags::ShaderResource;
        textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;
        for (int i = 0; i < 6; i++)
        {
            irradianceRenderPass->Begin(irraidanceFramebuffer);

            irradiancePSO->Bind();
            boxMesh->Bind();
            irradianceMaterials[i]->Bind();
            Renderer::Submit(boxMesh->GetIndexCount());

            irradianceRenderPass->End();

            irradianceResultTextures[i] = Texture2D::CreateEmpty(textureDesc);
            Renderer::GetRenderDevice()->CopyTexture2D(irraidanceFramebuffer->GetColorAttachmentTexture(0), irradianceResultTextures[i]);
            irradianceTextureCube->Update(i, irradianceResultTextures[i]);
            oldTextures.push_back(irradianceResultTextures[i]);
        }
    }

    void Skybox::GenerateBRDF()
    {
        FramebufferDesc fbDesc;
        fbDesc.width = skyboxTextureCube->GetWidth();
        fbDesc.height = skyboxTextureCube->GetHeight();

        captureFramebuffer = Framebuffer::Create(equirectangularRenderPass, fbDesc);

        quadMesh = Mesh::Create(resizeVB, resizeIB);

        TextureDesc textureDesc{};
        textureDesc.width = skyboxTextureCube->GetWidth();
        textureDesc.height = skyboxTextureCube->GetHeight();
        textureDesc.bindFlags = RenderBindFlags::ShaderResource;
        textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

        equirectangularRenderPass->Begin(captureFramebuffer);

        brdfPSO->Bind();
        quadMesh->Bind();
        Renderer::Submit(quadMesh->GetIndexCount());

        equirectangularRenderPass->End();

        BRDFTexture = Texture2D::CreateEmpty(textureDesc);
        Renderer::GetRenderDevice()->CopyTexture2D(captureFramebuffer->GetColorAttachmentTexture(0), BRDFTexture);
    }
    void Skybox::UpdateSkyboxFace(UInt32 _faceIndex, Shared<Texture2D> _texture)
    {
        FramebufferDesc fbDesc;
        fbDesc.width = skyboxTextureCube->GetWidth();
        fbDesc.height = skyboxTextureCube->GetHeight();

        resizeFramebuffer = Framebuffer::Create(resizeRenderPass, fbDesc);
        //if (texture->GetWidth() != textureCube->GetWidth() ||
        //	texture->GetHeight() != textureCube->GetHeight())
        //{
        TextureDesc textureDesc{};
        textureDesc.width = skyboxTextureCube->GetWidth();
        textureDesc.height = skyboxTextureCube->GetHeight();
        textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
        textureDesc.format = RenderFormat::R16G16B16A16_FLOAT;

        resizeMaterial = Material::Create(resizePSO);
        resizeMaterial->SetTexture2D("Texture", _texture);

        resizeRenderPass->Begin(resizeFramebuffer);
        resizePSO->Bind();
        quadMesh->Bind();
        resizeMaterial->Bind();
        Renderer::Submit(resizeIB->GetIndexCount());
        resizeRenderPass->End();

        resizeResultTexture = Texture2D::CreateEmpty(textureDesc);
        Renderer::GetRenderDevice()->CopyTexture2D(resizeFramebuffer->GetColorAttachmentTexture(0), resizeResultTexture);
        skyboxTextureCube->Update(_faceIndex, resizeResultTexture);
        oldTextures.push_back(resizeResultTexture);
    }
}