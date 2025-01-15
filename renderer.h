
#pragma once

#ifdef _DEBUG
#define DEBUG_DISP_TEXTOUT
#define DEBUG_HLSL_SHADERS
#endif

namespace Renderer
{
	//--------------------------------------------------------
	// constant
	//--------------------------------------------------------
	// screen parameter
	constexpr int SCREEN_SIZE_WIDTH  = 960;
	constexpr int SCREEN_SIZE_HEIGHT = 540;

	constexpr int SCREEN_RESOLUTION_WIDTH  = 1920;
	constexpr int SCREEN_RESOLUTION_HEIGHT = 1080;

	//--------------------------------------------------------
	// enumrator
	//--------------------------------------------------------
	/// <summary>
	/// enumeration of culling modes
	/// </summary>
	enum class CullMode
	{
		None,
		Front,
		Back,

		Maximum
	};

	/// <summary>
	/// enumeration of filling modes
	/// </summary>
	enum class FillMode
	{
		Wireframe,
		Solid,

		Maximum
	};

	/// <summary>
	/// enumeration of blending modes
	/// </summary>
	enum class BlendMode
	{
		None,
		Add,
		Subtract,
		AlphaBlend,

		Maximum
	};

	/// <summary>
	/// enumeration of depth enable mode
	/// </summary>
	enum class DepthEnebleMode
	{
		Enable,
		Disable,

		Maximum
	};

	//--------------------------------------------------------
	// manager class
	//--------------------------------------------------------
	class Manager
	{
		// device
		ID3D11Device* _device;
		ID3D11DeviceContext* _deviceContext;

		// swap chain
		IDXGISwapChain* _swapChain;
		DXGI_SWAP_CHAIN_DESC _swapChainDesc;

		// feature level
		D3D_FEATURE_LEVEL _featureLevel;

		// view
		ID3D11RenderTargetView* _rtv_backbuffer;
		ID3D11DepthStencilView* _dsv_backbuffer;

		// rasterizer
		ID3D11RasterizerState* _rasterizerState
			[static_cast<int>(CullMode::Maximum)]
			[static_cast<int>(FillMode::Maximum)];
		CullMode _cullMode;
		FillMode _fillMode;

		// blend
		ID3D11BlendState* _blendState[static_cast<int>(BlendMode::Maximum)];
		BlendMode _blendMode;

		// depth stencil
		ID3D11DepthStencilState* _depthStencilState[static_cast<int>(DepthEnebleMode::Maximum)];
		DepthEnebleMode _depthEnableMode;

		// sampler
		ID3D11SamplerState* _samplerState;

		// input layout
		ID3D11InputLayout* _inputLayout;

		// shaders
		ID3D11VertexShader* _vertexShader;
		ID3D11PixelShader*  _pixelShader;

		// constant buffers
		ID3D11Buffer* _constantBufferWorld;
		ID3D11Buffer* _constantBufferView;
		ID3D11Buffer* _constantBufferProjection;
		ID3D11Buffer* _constantBufferMaterial;

		// viewport
		D3D11_VIEWPORT _viewport;

		//-----------------------------------
		// private funcs
		//-----------------------------------
		HRESULT CreateDeviceAndSwapChain();
		HRESULT CreateRtvForBackBuffer();
		HRESULT CreateDsvForBackBuffer();

		void SetRenderTargetsToOutputMerger();

		// states
		HRESULT CreateRasterizerState();
		HRESULT CreateBlendState();
		HRESULT CreateDepthStencilState();
		HRESULT CreateSamplerState();

		HRESULT CreateShadersAndInputLayout();
		HRESULT CreateConstantBuffers();

		void SetViewportToRasterizerState();

		//-----------------------------------
		// public funcs
		//-----------------------------------
	public:
		Manager();
		static Manager& Instance();

		HRESULT Initialize();
		void Terminate();

		void ClearViews();
		void FlipFrameBuffer();

		// setter
		void SetRasterizerState(_In_ const CullMode& cullMode, _In_ const FillMode& fillMode);
		void SetCullingMode(_In_ const CullMode& cullMode);
		void SetFillingMode(_In_ const FillMode& fillMode);

		void SetBlendMode(_In_ const BlendMode& blendMode);
		void SetDepthEnableState(_In_ const DepthEnebleMode& depthStencilMode);

		void SetMatrixWorldViewProjection2D();

		// getter
		ID3D11Device& GetDevice();
		ID3D11DeviceContext& GetDeviceContext();
		ID3D11Buffer& GetConstantBufferMaterial();
	};
}
