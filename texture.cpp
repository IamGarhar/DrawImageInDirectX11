
#include "directx11_wrapper.h"
#include "sprite.h"
#include "renderer.h"
#include "vertex.h"
#include "material.h"
#include "texture.h"

namespace Texture
{
	/// <summary>
	/// instantiate with the Singleton Method Design Pattern
	/// </summary>
	Manager& Manager::Instance()
	{
		static Manager s_instance;
		return s_instance;
	}

	/// <summary>
	/// initialization process for texture
	/// </summary>
	HRESULT Manager::Initialize()
	{
		HRESULT h_result = S_OK;

		// texture path
		TexturePath = new wchar_t[512];
		mbstowcs_s(0, TexturePath, strlen(TEXTURE_FILE_PATH) + 1, TEXTURE_FILE_PATH, _TRUNCATE);

		h_result = CreateSrvFromFile();
		h_result = CreateVertexBuffer();

		// setting param
		Renderer::Manager renderer = Renderer::Manager::Instance();
		Position  = { Renderer::SCREEN_SIZE_WIDTH * 0.5f,  Renderer::SCREEN_SIZE_HEIGHT * 0.5f  };
		Scale     = { Renderer::SCREEN_SIZE_WIDTH * 0.75f, Renderer::SCREEN_SIZE_HEIGHT * 0.75f };
		Texcoord  = { 0.0f, 0.0f };
		TexSize   = { 1.0f, 1.0f };

		return h_result;
	}

	/// <summary>
	/// termination process for texture
	/// </summary>
	void Manager::Terminate()
	{
		Release();
	}

	/// <summary>
	/// update process for texture
	/// </summary>
	void Manager::Update()
	{

	}

	/// <summary>
	/// draw process for texture
	/// </summary>
	void Manager::Draw()
	{
		// renderer settings before draw texture
		Renderer::Manager renderer = Renderer::Manager::Instance();
		renderer.SetDepthEnableState(Renderer::DepthEnebleMode::Disable);
		renderer.SetBlendMode(Renderer::BlendMode::None);

		// setting data for Input-Assembler stage
		UINT stride = sizeof(Vertex::Manager);
		UINT offset = 0;
		renderer.GetDeviceContext().IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
		renderer.GetDeviceContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// calculate mvp matrix
		renderer.SetMatrixWorldViewProjection2D();

		// material
		Material::Manager material;
		ZeroMemory(&material, sizeof(material));
		material.SetDiffuse({ 1.0f, 1.0f, 1.0f, 1.0f });
		material.SetConstantBuffer();

		// draw
		renderer.GetDeviceContext().PSSetShaderResources(0, 1, &Srv);
		SetAnchorPointCenter();
		renderer.GetDeviceContext().Draw(4, 0);

		// renderer settings after draw texture
		renderer.SetDepthEnableState(Renderer::DepthEnebleMode::Enable);
	}
}
