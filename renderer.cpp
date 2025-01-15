
#include "main.h"
#include "directx11_wrapper.h"
#include "renderer.h"

namespace Renderer
{
	/// <summary>
	/// constructor for directx renderer
	/// </summary>
	Manager::Manager()
	{
		_device        = nullptr;
		_deviceContext = nullptr;

		_swapChain     = nullptr;
		_swapChainDesc = {};

		_featureLevel = {};

		// view
		_rtv_backbuffer = nullptr;
		_dsv_backbuffer = nullptr;

		//-----------------------------------
		// rasterizer
		//-----------------------------------
		for (int c = 0; c < static_cast<int>(CullMode::Maximum); ++c)
		{
			for (int f = 0; f < static_cast<int>(FillMode::Maximum); ++f)
			{
				_rasterizerState[c][f] = {};
			}
		}
		_cullMode = CullMode::Maximum;
		_fillMode = FillMode::Maximum;

		//-----------------------------------
		// blend
		//-----------------------------------
		for (int b = 0; b < static_cast<int>(BlendMode::Maximum); ++b)
		{
			_blendState[b] = {};
		}
		_blendMode = BlendMode::Maximum;

		//-----------------------------------
		// depth stencil
		//-----------------------------------
		for (int d = 0; d < static_cast<int>(DepthEnebleMode::Maximum); ++d)
		{
			_depthStencilState[d] = {};
		}
		_depthEnableMode = DepthEnebleMode::Maximum;

		_samplerState = nullptr;
		_inputLayout  = nullptr;

		// shaders
		_vertexShader = nullptr;
		_pixelShader  = nullptr;

		// constant buffer
		_constantBufferWorld      = nullptr;
		_constantBufferView       = nullptr;
		_constantBufferProjection = nullptr;
		_constantBufferMaterial   = nullptr;

		_viewport = {};
	}

	/// <summary>
	/// instantiate with the Singleton Method Design Pattern
	/// </summary>
	Manager& Manager::Instance()
	{
		static Manager s_instance;
		return s_instance;
	}

	/// <summary>
	/// initialization process for directx renderer
	/// </summary>
	HRESULT Manager::Initialize()
	{
		// creates device and swap chain
		CreateDeviceAndSwapChain();

		// creates RTV and DSV for back buffer, and set them to OM stage
		CreateRtvForBackBuffer();
		CreateDsvForBackBuffer();
		SetRenderTargetsToOutputMerger();

		// creates states
		CreateRasterizerState();
		CreateBlendState();
		CreateDepthStencilState();
		CreateSamplerState();

		// creates shaders and input-layout
		CreateShadersAndInputLayout();

		// creates constant buffers
		CreateConstantBuffers();

		// viewport
		SetViewportToRasterizerState();

		// set input-layout to the Input-Assembler stage
		_deviceContext->IASetInputLayout(_inputLayout);

		// set vertex shader
		_deviceContext->VSSetShader(_vertexShader, nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, &_constantBufferWorld);
		_deviceContext->VSSetConstantBuffers(1, 1, &_constantBufferView);
		_deviceContext->VSSetConstantBuffers(2, 1, &_constantBufferProjection);

		// set pixel shader
		_deviceContext->PSSetShader(_pixelShader, nullptr, 0);
		_deviceContext->PSSetSamplers(0, 1, &_samplerState);
		_deviceContext->PSSetConstantBuffers(0, 1, &_constantBufferMaterial);

		return S_OK;
	}

	/// <summary>
	/// termination process for directx renderer
	/// </summary>
	void Manager::Terminate()
	{
		_device       ->Release();
		_deviceContext->Release();
		_swapChain    ->Release();

		// view
		_rtv_backbuffer->Release();
		_dsv_backbuffer->Release();

		_samplerState->Release();
		_inputLayout ->Release();

		// shader
		_vertexShader->Release();
		_pixelShader ->Release();

		// constant buffer
		_constantBufferWorld      ->Release();
		_constantBufferView       ->Release();
		_constantBufferProjection ->Release();
		_constantBufferMaterial   ->Release();
	}

	/// <summary>
	/// clear view interfaces
	/// </summary>
	void Manager::ClearViews()
	{
		// clear color for back buffer
		float clear_color[4] = { 0.0f, 1.0f, 0.0f, 1.0 };

		// clear Render-Target-View
		_deviceContext->ClearRenderTargetView(_rtv_backbuffer, clear_color);

		// clear Depth-Stencil-View
		_deviceContext->ClearDepthStencilView(_dsv_backbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	/// <summary>
	/// flip frame buffer
	/// </summary>
	void Manager::FlipFrameBuffer()
	{
		_swapChain->Present(0, 0);
	}
}