#include "Sandbox2D.h"

#include "imgui/imgui.h"

static Daydream::Renderer2D r2d;

namespace Daydream
{
	Sandbox2D::Sandbox2D()
	{
		//float squareVertices[4 * 9] = {
		//		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		//		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		//		 0.5f,	0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		//		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		//};

		//float squareVertices2[4 * 9] = {
		//	-0.3f, -0.4f, .5f, 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,
		//	 -0.3f,  0.6f, .5f,0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
		//	 0.7f, 0.6f, 1.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 0.0f,
		//	 0.7f, -0.4f, 1.0f,0.0f,1.0f,0.0f,1.0f, 1.0f, 1.0f,
		//};

		//Daydream::BufferLayout layout =
		//{
		//	{ Daydream::ShaderDataType::Float3, "a_Position", "POSITION"},
		//	{ Daydream::ShaderDataType::Float4, "a_Color", "COLOR"},
		//	{ Daydream::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
		//};
		////squareVB = Daydream::VertexBuffer::CreateStatic(squareVertices, sizeof(squareVertices), layout.GetStride());
		//squareVB = Daydream::VertexBuffer::CreateDynamic(sizeof(squareVertices), layout.GetStride());
		//squareVB->SetData(squareVertices, sizeof(squareVertices));

		//squareVB2 = Daydream::VertexBuffer::CreateDynamic(sizeof(squareVertices2), layout.GetStride());
		//squareVB2->SetData(squareVertices2, sizeof(squareVertices2));

		//uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		//squareIB = Daydream::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		//uint32_t squareIndices2[6] = { 0, 1, 2, 2, 3, 0 };
		//squareIB2 = Daydream::IndexBuffer::Create(squareIndices2, sizeof(squareIndices2) / sizeof(uint32_t));


		//vs = Daydream::Shader::Create("Asset/Shader/SpriteVS.hlsl", Daydream::ShaderType::Vertex, Daydream::ShaderLoadMode::File);
		//ps = Daydream::Shader::Create("Asset/Shader/SpritePS.hlsl", Daydream::ShaderType::Pixel, Daydream::ShaderLoadMode::File);


		//vs3d = Daydream::Shader::Create("Asset/Shader/ModelVS.hlsl", Daydream::ShaderType::Vertex, Daydream::ShaderLoadMode::File);
		//ps3d = Daydream::Shader::Create("Asset/Shader/ModelPS.hlsl", Daydream::ShaderType::Pixel, Daydream::ShaderLoadMode::File);


		//Daydream::BufferLayout inputlayout = {
		//{ Daydream::ShaderDataType::Float3, "a_Position", "POSITION"},
		//{ Daydream::ShaderDataType::Float4, "a_Color", "COLOR"},
		//{ Daydream::ShaderDataType::Float2, "a_TexCoord", "TEXCOORD"}
		//};

		//camera.SetPosition({ 0.0f,0.0f,-2.0f });
		//cameraPos = camera.GetViewProjectionMatrix();
		//viewProjMat = Daydream::ConstantBuffer::Create(sizeof(Daydream::Matrix4x4));
		//viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));

		//auto path = Daydream::FilePath("Asset/Texture/Checkerboard.png");
		//Daydream::TextureDesc textureDesc{};
		//textureDesc.bindFlags = Daydream::RenderBindFlags::ShaderResource;
		//textureDesc.format = Daydream::RenderFormat::R8G8B8A8_UNORM_SRGB;
		//texture = Daydream::Texture2D::Create(path, textureDesc);

		////texture2 = Daydream::Texture2D::CreateEmpty(textureDesc);

		//Daydream::RenderPassDesc rpDesc;

		//Daydream::RenderPassAttachmentDesc attach{};
		//attach.format = Daydream::RenderFormat::R8G8B8A8_UNORM;
		//attach.loadOp = Daydream::AttachmentLoadOp::Clear;
		//attach.storeOp = Daydream::AttachmentStoreOp::Store;
		//rpDesc.colorAttachments.push_back(attach);

		//attach.format = Daydream::RenderFormat::D24_UNORM_S8_UINT;
		//attach.loadOp = Daydream::AttachmentLoadOp::Clear;
		//attach.storeOp = Daydream::AttachmentStoreOp::Store;
		//rpDesc.depthAttachment = attach;

		//renderPass = Daydream::RenderPass::Create(rpDesc);
		//renderPass->SetClearColor(Daydream::Color::Blue);

		//Daydream::FramebufferDesc fbDesc;
		//fbDesc.width = 320;
		//fbDesc.height = 180;
		////fbDesc.width = 1600;
		////fbDesc.height = 900;

		//framebuffer = Daydream::Framebuffer::Create(renderPass, fbDesc);

		//Daydream::PipelineStateDesc desc;
		//desc.vertexShader = vs;
		//desc.pixelShader = ps;
		//desc.renderPass = renderPass;

		//pso = Daydream::PipelineState::Create(desc);

		//desc.vertexShader = vs3d;
		//desc.pixelShader = ps3d;
		//desc.renderPass = renderPass;

		//pso3d = Daydream::PipelineState::Create(desc);

		//material3d = Daydream::Material::Create(pso3d);

		//material3d->SetConstantBuffer("Camera", viewProjMat);

		//material = Daydream::Material::Create(pso);
		////material = pso->CreateMaterial(); // 이것도 가능

		//material->SetTexture2D("Texture", texture);
		//material->SetConstantBuffer("Camera", viewProjMat);

		//mesh = Mesh::Create(squareVB, squareIB);
		////mesh = Mesh::Create();
		////mesh->Load("F:/DaydreamReboot/Sandbox/Asset/Model/Lowpoly_tree_sample.fbx");
		//model = MakeShared<Model>();
		//model->Load("Asset/Model/Lowpoly_tree_sample.fbx");
	}

	void Sandbox2D::OnUpdate(Float32 _deltaTime)
	{
		//Daydream::RenderCommand::SetClearColor(Daydream::Color::White);
		//Daydream::RenderCommand::Clear();

		//if (Daydream::Input::GetKeyPressed(Daydream::Key::Q))
		//{
		//	camera.SetPosition(camera.GetPosition() + Daydream::Vector3(0.0f, 0.0f, 1.0f) * _deltaTime);
		//	cameraPos = camera.GetViewProjectionMatrix();
		//	viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		//}
		//if (Daydream::Input::GetKeyPressed(Daydream::Key::E))
		//{
		//	camera.SetPosition(camera.GetPosition() + Daydream::Vector3(0.0f, 0.0f, -1.0f) * _deltaTime);
		//	cameraPos = camera.GetViewProjectionMatrix();
		//	viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		//}

		//if (Daydream::Input::GetKeyPressed(Daydream::Key::W))
		//{
		//	camera.SetPosition(camera.GetPosition() + Daydream::Vector3(0.0f, 1.0f, 0.0f) * _deltaTime);
		//	cameraPos = camera.GetViewProjectionMatrix();
		//	viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		//}

		//if (Daydream::Input::GetKeyPressed(Daydream::Key::A))
		//{
		//	camera.SetPosition(camera.GetPosition() + Daydream::Vector3(-1.0f, 0.0f, 0.0f) * _deltaTime);
		//	cameraPos = camera.GetViewProjectionMatrix();
		//	viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		//}

		//if (Daydream::Input::GetKeyPressed(Daydream::Key::S))
		//{
		//	camera.SetPosition(camera.GetPosition() + Daydream::Vector3(0.0f, -1.0f, 0.0f) * _deltaTime);
		//	cameraPos = camera.GetViewProjectionMatrix();
		//	viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		//}

		//if (Daydream::Input::GetKeyPressed(Daydream::Key::D))
		//{
		//	camera.SetPosition(camera.GetPosition() + Daydream::Vector3(1.0f, 0.0f, 0.0f) * _deltaTime);
		//	cameraPos = camera.GetViewProjectionMatrix();
		//	viewProjMat->Update(&cameraPos.mat, sizeof(Daydream::Matrix4x4));
		//}
		// 
		//renderPass->Begin(framebuffer);
		//////camera.SetPosition({ 0.5f, 0.5f, 0.0f });

		//pso3d->Bind();

		////Daydream::Renderer::BeginScene(camera);
		//material3d->Bind();


		//pso->Bind();
		//mesh->Bind();
		//material->Bind();
		//Daydream::Renderer::Submit(mesh->GetIndexCount());
		//


		////squareVB->Bind();
		////squareIB->Bind();

		//////viewProjMat->Bind(0, Daydream::DaydreamVertexBit);
		//////texture->Bind(0);
		//////Daydream::RenderCommand::DrawIndexed(squareIB->GetCount());
		////Daydream::Renderer::Submit(squareIB->GetCount());

		////squareVB2->Bind();
		////squareIB2->Bind();

		//////viewProjMat->Bind(0, Daydream::DaydreamVertexBit);
		//////texture->Bind(0);
		//////Daydream::RenderCommand::DrawIndexed(squareIB->GetCount());
		////Daydream::Renderer::Submit(squareIB2->GetCount());

		//renderPass->End();
		////framebuffer->End();

		////r2d.TestTick();
		////Daydream::Renderer2D::BeginScene(camera);
		////Daydream::Renderer2D::DrawQuad(Daydream::Vector3(0.2f, 0.2f, 0.0f), Daydream::Vector2(1.0f, 1.0f), { 1.0f,1.0f,1.0f,1.0f });
		////Daydream::Renderer2D::DrawQuad(Daydream::Vector3(-0.2f, -0.2f, 0.0f), Daydream::Vector2(1.0f, 1.0f), { 1.0f,1.0f,1.0f,1.0f });
		////Daydream::Renderer2D::EndScene();
		////va->Bind();
		////vs->Bind();
		////ps->Bind();
		//////Daydream::Matrix4x4 transform = Daydream::Math::Translate(Daydream::Matrix4x4(), Daydream::Vector4(1.0f, 1.0f, 0.0f));
		//////transform.Transpose();
	}

	void Sandbox2D::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();

		//ImGui::Begin("ImGui texture Image Test(Engine Viewport)");
		//ImGui::Image((ImTextureID)texture->GetImGuiHandle(), ImVec2(texture->GetWidth(), texture->GetHeight()));
		//ImGui::Image((ImTextureID)framebuffer->GetColorAttachmentTexture(0)->GetImGuiHandle(), ImVec2(framebuffer->GetWidth(), framebuffer->GetHeight()));
		////ImGui::Image((ImTextureID)(ID3D11ShaderResourceView*)texture->GetNativeHandle(), ImVec2(texture->GetWidth(), texture->GetHeight()));
		////ImGui::Image((ImTextureID)static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(texture->GetNativeHandle())), ImVec2(texture->GetWidth() * 2, texture->GetHeight() * 2));
		////ImGui::Image((ImTextureID)(texture->GetNativeHandle()), ImVec2(texture->GetWidth() * 2, texture->GetHeight() * 2));
		//ImGui::End();
	}
}

