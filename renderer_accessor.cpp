
#include "main.h"
#include "directx11_wrapper.h"
#include "renderer.h"
#include "window.h"

namespace Renderer
{
	//--------------------------------------------------------
	// setter
	//--------------------------------------------------------
	/// <summary>
	/// set up the Rasterizer state
	/// </summary>
	void Manager::SetRasterizerState(_In_ const CullMode& cullMode, _In_ const FillMode& fillMode)
	{
		_deviceContext->RSSetState(_rasterizerState[static_cast<int>(cullMode)][static_cast<int>(fillMode)]);
		_cullMode = cullMode;
		_fillMode = fillMode;
	}

	/// <summary>
	/// set up the culling mode
	/// </summary>
	void Manager::SetCullingMode(_In_ const CullMode& cullMode)
	{
		_deviceContext->RSSetState(_rasterizerState[static_cast<int>(cullMode)][static_cast<int>(_fillMode)]);
		_cullMode = cullMode;
	}

	/// <summary>
	/// set up the filling mode
	/// </summary>
	void Manager::SetFillingMode(_In_ const FillMode& fillMode)
	{
		_deviceContext->RSSetState(_rasterizerState[static_cast<int>(_cullMode)][static_cast<int>(fillMode)]);
		_fillMode = fillMode;
	}

	/// <summary>
	/// set up the blending mode
	/// </summary>
	void Manager::SetBlendMode(_In_ const BlendMode& blendMode)
	{
		_deviceContext->OMSetBlendState(_blendState[static_cast<int>(blendMode)], {}, 0xffffffff);
		_blendMode = blendMode;
	}

	/// <summary>
	/// set up the depth enable mode
	/// </summary>
	void Manager::SetDepthEnableState(_In_ const DepthEnebleMode& depthEnableMode)
	{
		_deviceContext->OMSetDepthStencilState(_depthStencilState[static_cast<int>(depthEnableMode)], NULL);
		_depthEnableMode = depthEnableMode;
	}

	/// <summary>
	/// set MVP matrix for 2D
	/// </summary>
	void Manager::SetMatrixWorldViewProjection2D()
	{
		DirectX::XMMATRIX mtx_world;
		DirectX::XMMATRIX mtx_view;
		DirectX::XMMATRIX mtx_projection;

		{// world matrix

			mtx_world = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
			_deviceContext->UpdateSubresource(_constantBufferWorld, 0, nullptr, &mtx_world, 0, 0);
		}

		{// view matrix

			mtx_view = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
			_deviceContext->UpdateSubresource(_constantBufferView, 0, nullptr, &mtx_view, 0, 0);
		}

		{// projection matrix

			// left-handed coordinate system
			mtx_projection = DirectX::XMMatrixOrthographicOffCenterLH
			(0.0f, static_cast<float>(Window::WINDOW_SIZE_WIDTH), static_cast<float>(Window::WINDOW_SIZE_HEIGHT), 0.0f, 0.0f, 1.0f);

			mtx_projection = DirectX::XMMatrixTranspose(mtx_projection);
			_deviceContext->UpdateSubresource(_constantBufferProjection, 0, nullptr, &mtx_projection, 0, 0);
		}
	}

	//--------------------------------------------------------
	// getter
	//--------------------------------------------------------
	/// <summary>
	/// get pointer for device
	/// </summary>
	ID3D11Device& Manager::GetDevice()
	{
		return *_device;
	}

	/// <summary>
	/// get pointer for device context
	/// </summary>
	ID3D11DeviceContext& Manager::GetDeviceContext()
	{
		return *_deviceContext;
	}

	/// <summary>
	/// get pointer for constant buffer of material
	/// </summary>
	ID3D11Buffer& Manager::GetConstantBufferMaterial()
	{
		return *_constantBufferMaterial;
	}
}
