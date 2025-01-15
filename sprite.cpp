
#include "directx11_wrapper.h"
#include "sprite.h"
#include "renderer.h"
#include "vertex.h"

namespace Sprite
{
	/// <summary>
	/// constructor for sprite
	/// </summary>
	Manager::Manager()
	{
		VertexBuffer = nullptr;
		Srv = nullptr;

		TexturePath = nullptr;

		Position = {};
		Scale    = {};
		Texcoord = {};
		TexSize  = {};
		Color    = { 1.0f, 1.0f, 1.0f, 1.0f };
		Rotation = 0.0f;

		IsLoad = false;
	}

	/// <summary>
	/// creates Shader-Resource-View from WIC file
	/// </summary>
	HRESULT Manager::CreateSrvFromFile()
	{
		HRESULT h_result = S_OK;

		Srv = nullptr;

		// load WIC image
		DirectX::ScratchImage image;
		h_result = DirectX::LoadFromWICFile(TexturePath, DirectX::WIC_FLAGS_NONE, nullptr, image);
		if (FAILED(h_result))
			return h_result;

		// creates Shader-Resource-View
		h_result = DirectX::CreateShaderResourceView(&Renderer::Manager::Instance().GetDevice(),
			image.GetImage(0, 0, 0), image.GetImageCount(), image.GetMetadata(), &Srv);
		if (FAILED(h_result))
			return h_result;

		IsLoad = true;

		return h_result;
	}

	/// <summary>
	/// creates vertex buffer
	/// </summary>
	HRESULT Manager::CreateVertexBuffer()
	{
		HRESULT h_result = S_OK;

		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		{
			buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
			buffer_desc.ByteWidth      = sizeof(Vertex::Manager) * 4;
			buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		h_result = Renderer::Manager::Instance().GetDevice().CreateBuffer(&buffer_desc, nullptr, &VertexBuffer);

		return h_result;
	}

	/// <summary>
	/// anchor point set to center of sprite
	/// </summary>
	void Manager::SetAnchorPointCenter()
	{
		// start mapping
		D3D11_MAPPED_SUBRESOURCE subresource;
		Renderer::Manager::Instance().GetDeviceContext().Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);

		// temporary data for calculation
		DirectX::XMFLOAT2 half_scale = { Scale.x * 0.5f, Scale.y * 0.5f };
		
		// for rotation
		float angle = static_cast<float>(atan2(static_cast<double>(half_scale.y), static_cast<double>(half_scale.x)));
		float radius = DirectX::XMVectorGetX(DirectX::XMVector2Length(DirectX::XMLoadFloat2(&half_scale)));

		// creates vertex data
		Vertex::Manager* p_vertex = reinterpret_cast<Vertex::Manager*>(subresource.pData);
		{
			// vertex position
			p_vertex[0].Position = { Position.x - static_cast<float>(cos(angle + Rotation)) * radius, Position.y - static_cast<float>(sin(angle + Rotation)) * radius, 0.0f };
			p_vertex[1].Position = { Position.x + static_cast<float>(cos(angle - Rotation)) * radius, Position.y - static_cast<float>(sin(angle - Rotation)) * radius, 0.0f };
			p_vertex[2].Position = { Position.x - static_cast<float>(cos(angle - Rotation)) * radius, Position.y + static_cast<float>(sin(angle - Rotation)) * radius, 0.0f };
			p_vertex[3].Position = { Position.x + static_cast<float>(cos(angle + Rotation)) * radius, Position.y + static_cast<float>(sin(angle + Rotation)) * radius, 0.0f };

			// vertex color
			p_vertex[0].Color = Color;
			p_vertex[1].Color = Color;
			p_vertex[2].Color = Color;
			p_vertex[3].Color = Color;

			// vertex texcoord
			p_vertex[0].Texcoord = Texcoord;
			p_vertex[1].Texcoord = { Texcoord.x + TexSize.x, Texcoord.y };
			p_vertex[2].Texcoord = { Texcoord.x,             Texcoord.y + TexSize.y };
			p_vertex[3].Texcoord = { Texcoord.x + TexSize.x, Texcoord.y + TexSize.y };
		}

		// finish mapping
		Renderer::Manager::Instance().GetDeviceContext().Unmap(VertexBuffer, 0);
	}

	/// <summary>
	/// release the memory
	/// </summary>
	void Manager::Release()
	{
		if (!IsLoad) return;

		// vert buff
		if (VertexBuffer)
		{
			VertexBuffer->Release();
			VertexBuffer = nullptr;
		}

		// srv
		if (Srv)
		{
			Srv->Release();
			Srv= nullptr;
		}

		// texture file path
		if (TexturePath) TexturePath = nullptr;

		IsLoad = false;
	}
}
