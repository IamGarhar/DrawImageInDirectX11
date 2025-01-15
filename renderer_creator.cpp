
#include "main.h"
#include "directx11_wrapper.h"
#include "renderer.h"
#include "window.h"
#include "material.h"

namespace Renderer
{
	/// <summary>
	/// creates the display-adapter and a swap-chain
	/// </summary>
	HRESULT Manager::CreateDeviceAndSwapChain()
	{
		HRESULT h_result = S_OK;

		DWORD deviceFlag = 0;

		// set-up the swap chain
		ZeroMemory(&_swapChainDesc, sizeof(_swapChainDesc));
		{
			// back buffer display mode settings
			_swapChainDesc.BufferDesc.Width  = SCREEN_RESOLUTION_WIDTH;
			_swapChainDesc.BufferDesc.Height = SCREEN_RESOLUTION_HEIGHT;
			_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			// refresh rate
			_swapChainDesc.BufferDesc.RefreshRate.Numerator   = 144;
			_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

			// multi-sampling settings
			_swapChainDesc.SampleDesc.Count   = 1;
			_swapChainDesc.SampleDesc.Quality = 0;

			// other back buffer settings
			_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			_swapChainDesc.BufferCount = 1;
			
			// output window settings
			_swapChainDesc.OutputWindow = Window::Manager::Instance().GetWindowHandle();
			_swapChainDesc.Windowed     = Window::Manager::Instance().GetIsWindowedMode();

#ifdef DEBUG_DISP_TEXTOUT
			// display settings for debug textout
			_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			_swapChainDesc.Flags             = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;
			deviceFlag = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#endif
		}

		// creates a device
		D3D_FEATURE_LEVEL featureLevelArray[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// creates device and swap-chain with the latest feature-level possible
		for (auto level : featureLevelArray)
		{
			h_result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlag,
				nullptr, 0, D3D11_SDK_VERSION, &_swapChainDesc, &_swapChain, &_device, &level, &_deviceContext);
			
			if (SUCCEEDED(h_result))
			{
				_featureLevel = level;
				break;
			}
		}

#ifdef DEBUG_DISP_TEXTOUT
		// N.B. the GDI compatible flag
		h_result = _swapChain->ResizeBuffers(0, SCREEN_RESOLUTION_WIDTH, SCREEN_RESOLUTION_HEIGHT, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE);
#endif

		return h_result;
	}

	/// <summary>
	/// creates a Render-Target-View for back-buffer
	/// </summary>
	HRESULT Manager::CreateRtvForBackBuffer()
	{
		HRESULT h_result = S_OK;

		// get buffer from swap chain
		ID3D11Texture2D* p_back_buffer = nullptr;
		h_result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&p_back_buffer));
		
		// create RTV and release temp buffer
		if (p_back_buffer)
		{
			h_result = _device->CreateRenderTargetView(p_back_buffer, nullptr, &_rtv_backbuffer);
		}
		p_back_buffer->Release();

		return h_result;
	}

	/// <summary>
	/// creates a Depth-Stencil-View for back-buffer
	/// </summary>
	HRESULT Manager::CreateDsvForBackBuffer()
	{
		HRESULT h_result = S_OK;

		// settings the depth stencil texture
		D3D11_TEXTURE2D_DESC tex2d_desc;
		ZeroMemory(&tex2d_desc, sizeof(tex2d_desc));
		{
			// size
			tex2d_desc.Width  = _swapChainDesc.BufferDesc.Width;
			tex2d_desc.Height = _swapChainDesc.BufferDesc.Height;
	
			// quality
			tex2d_desc.MipLevels = 1;
			tex2d_desc.ArraySize = 1;
			tex2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			tex2d_desc.SampleDesc = _swapChainDesc.SampleDesc;
			tex2d_desc.Usage      = D3D11_USAGE_DEFAULT;

			// flags
			tex2d_desc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
			tex2d_desc.CPUAccessFlags = 0;
			tex2d_desc.MiscFlags      = 0;
		}

		// creates the depth stencil texture
		ID3D11Texture2D* p_depth_texture = nullptr;
		h_result = _device->CreateTexture2D(&tex2d_desc, nullptr, &p_depth_texture);

		// settings the DSV
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
		ZeroMemory(&dsv_desc, sizeof(dsv_desc));
		{
			dsv_desc.Format = tex2d_desc.Format;
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Flags = 0;
		}

		// creates the DSV
		if (p_depth_texture)
		{
			h_result = _device->CreateDepthStencilView(p_depth_texture, &dsv_desc, &_dsv_backbuffer);
		}

		return h_result;
	}

	/// <summary>
	/// set Render-Target-View and Depth-Stencil-View for back-buffer to the Output-Merger
	/// </summary>
	void Manager::SetRenderTargetsToOutputMerger()
	{
		_deviceContext->OMSetRenderTargets(1, &_rtv_backbuffer, _dsv_backbuffer);
	}

	/// <summary>
	/// creates Rasterizer state for the culling and filling types
	/// </summary>
	HRESULT Manager::CreateRasterizerState()
	{
		HRESULT h_result = S_OK;

		D3D11_RASTERIZER_DESC rasterizer_desc;
		ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));

		// flags
		rasterizer_desc.DepthClipEnable   = TRUE;
		rasterizer_desc.MultisampleEnable = FALSE;

		for (int c = 0; c < static_cast<int>(CullMode::Maximum); ++c)
		{
			rasterizer_desc.CullMode = D3D11_CULL_MODE(c + 1);

			for (int f = 0; f < static_cast<int>(FillMode::Maximum); ++f)
			{
				rasterizer_desc.FillMode = D3D11_FILL_MODE(f + 2);

				// creates Rasterizer state
				h_result = _device->CreateRasterizerState(&rasterizer_desc, &_rasterizerState[c][f]);
			}
		}

		// set mode
		SetRasterizerState(CullMode::Back, FillMode::Solid);

		return h_result;
	}

	/// <summary>
	/// creates blend state
	/// </summary>
	HRESULT Manager::CreateBlendState()
	{
		HRESULT h_result = S_OK;

		D3D11_BLEND_DESC blend_desc;
		ZeroMemory(&blend_desc, sizeof(blend_desc));

		{// flags

			blend_desc.AlphaToCoverageEnable  = FALSE;
			blend_desc.IndependentBlendEnable = FALSE;
			blend_desc.RenderTarget[0].BlendEnable = TRUE;
		}

		{// none blend

			// blend settings for rgb
			blend_desc.RenderTarget[0].SrcBlend  = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].BlendOp   = D3D11_BLEND_OP_ADD;
			
			// blend settings for alpha
			blend_desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
			blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blend_desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
			
			// write mask settings
			blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			h_result = _device->CreateBlendState(&blend_desc, &_blendState[static_cast<int>(BlendMode::None)]);
		}

		{// add blend

			blend_desc.RenderTarget[0].SrcBlend  = D3D11_BLEND_SRC_ALPHA;
			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
			h_result = _device->CreateBlendState(&blend_desc, &_blendState[static_cast<int>(BlendMode::Add)]);
		}

		{// subtract blend

			blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
			h_result = _device->CreateBlendState(&blend_desc, &_blendState[static_cast<int>(BlendMode::Subtract)]);
		}

		{// alpha blend

			blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blend_desc.RenderTarget[0].BlendOp   = D3D11_BLEND_OP_ADD;
			h_result = _device->CreateBlendState(&blend_desc, &_blendState[static_cast<int>(BlendMode::AlphaBlend)]);
		}

		// set mode
		SetBlendMode(BlendMode::AlphaBlend);

		return h_result;
	}

	/// <summary>
	/// creates depth stencil state
	/// </summary>
	HRESULT Manager::CreateDepthStencilState()
	{
		HRESULT h_result = S_OK;

		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));

		// depth
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthFunc   = D3D11_COMPARISON_LESS_EQUAL;

		// stencil
		depth_stencil_desc.StencilEnable = FALSE;

		// depth enable
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		h_result = _device->CreateDepthStencilState(&depth_stencil_desc, &_depthStencilState[static_cast<int>(DepthEnebleMode::Enable)]);

		// depth disable
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		h_result = _device->CreateDepthStencilState(&depth_stencil_desc, &_depthStencilState[static_cast<int>(DepthEnebleMode::Disable)]);

		// set mode
		SetDepthEnableState(DepthEnebleMode::Enable);

		return h_result;
	}

	/// <summary>
	/// creates sampler state for default type
	/// </summary>
	HRESULT Manager::CreateSamplerState()
	{
		HRESULT h_result = S_OK;

		D3D11_SAMPLER_DESC sampler_desc;
		ZeroMemory(&sampler_desc, sizeof(sampler_desc));
		{
			sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;

			// texture coordinate
			sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

			sampler_desc.MipLODBias     = 0;
			sampler_desc.MaxAnisotropy  = 16;
			sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

			// level of detail of mipmap
			sampler_desc.MinLOD = 0;
			sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		}

		// creates state
		h_result = _device->CreateSamplerState(&sampler_desc, &_samplerState);

		// set sampler to the pixel shader
		_deviceContext->PSSetSamplers(0, 1, &_samplerState);

		return h_result;
	}

	/// <summary>
	/// creates shaders and input-layout
	/// </summary>
	HRESULT Manager::CreateShadersAndInputLayout()
	{
		HRESULT h_result = S_OK;

		DWORD compile_flag = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG_HLSL_SHADERS
		compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		// define binary-large-object
		ID3DBlob* errorBlob = nullptr;
		ID3DBlob* vsBlob = nullptr;
		ID3DBlob* psBlob = nullptr;

		//-----------------------------------
		// vertex shader
		//-----------------------------------
		{
			// compile shader file
			h_result = D3DCompileFromFile(L"resource/shader/vertex_shader.hlsl", nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", compile_flag, 0, &vsBlob, &errorBlob);
			
			// error message
			if (FAILED(h_result))
			{
				MessageBox(nullptr, static_cast<LPCSTR>(errorBlob->GetBufferPointer()), "VS", MB_OK | MB_ICONERROR);
			}
			
			// creates vertex shader
			h_result = _device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);
		}

		//-----------------------------------
		// pixel shader
		//-----------------------------------
		{
			// compile shader file
			h_result = D3DCompileFromFile(L"resource/shader/pixel_shader.hlsl", nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", compile_flag, 0, &psBlob, &errorBlob);

			// error message
			if (FAILED(h_result))
			{
				MessageBox(nullptr, static_cast<LPCSTR>(errorBlob->GetBufferPointer()), "PS", MB_OK | MB_ICONERROR);
			}

			// creates pixel shader
			h_result = _device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);
		}

		//-----------------------------------
		// input layout
		//-----------------------------------
		D3D11_INPUT_ELEMENT_DESC input_layout_desc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		h_result = _device->CreateInputLayout(input_layout_desc, static_cast<UINT>(ARRAYSIZE(input_layout_desc)), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &_inputLayout);

		// releases binary-large-object
		vsBlob->Release();
		psBlob->Release();

		return h_result;
	}

	/// <summary>
	/// creates constant buffers
	/// </summary>
	HRESULT Manager::CreateConstantBuffers()
	{
		HRESULT h_result = S_OK;

		// settings for constant buffers
		D3D11_BUFFER_DESC buffer_desc;
		ZeroMemory(&buffer_desc, sizeof(buffer_desc));
		buffer_desc.Usage               = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags      = 0;
		buffer_desc.MiscFlags           = 0;
		buffer_desc.StructureByteStride = sizeof(float);

		// mvp matrix
		buffer_desc.ByteWidth = sizeof(DirectX::XMMATRIX);
		h_result = _device->CreateBuffer(&buffer_desc, nullptr, &_constantBufferWorld);
		h_result = _device->CreateBuffer(&buffer_desc, nullptr, &_constantBufferView);
		h_result = _device->CreateBuffer(&buffer_desc, nullptr, &_constantBufferProjection);

		// material
		buffer_desc.ByteWidth = sizeof(Material::Manager);
		h_result = _device->CreateBuffer(&buffer_desc, nullptr, &_constantBufferMaterial);

		return h_result;
	}

	/// <summary>
	/// setting and set viewport to the Rasterizer state
	/// </summary>
	void Manager::SetViewportToRasterizerState()
	{
		_viewport.Width  = static_cast<FLOAT>(SCREEN_RESOLUTION_WIDTH);
		_viewport.Height = static_cast<FLOAT>(SCREEN_RESOLUTION_HEIGHT);
		_viewport.MinDepth = 0.0f;
		_viewport.MaxDepth = 1.0f;
		_viewport.TopLeftX = 0.0f;
		_viewport.TopLeftY = 0.0f;

		// set viewport to the Rasterizer state
		_deviceContext->RSSetViewports(1, &_viewport);
	}
}
