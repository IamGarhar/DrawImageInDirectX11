
#include "directx11_wrapper.h"
#include "renderer.h"
#include "sprite.h"
#include "texture.h"

namespace DirectXWrapper
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
	/// initialization process for directx
	/// </summary>
	HRESULT Manager::Initialize()
	{
		HRESULT h_result = S_OK;

		h_result = Renderer::Manager::Instance().Initialize();
		h_result = Texture::Manager::Instance().Initialize();

		return h_result;
	}

	/// <summary>
	/// termination process for directx
	/// </summary>
	void Manager::Terminate()
	{
		Texture::Manager::Instance().Terminate();
		Renderer::Manager::Instance().Terminate();
	}

	/// <summary>
	/// update process for directx
	/// </summary>
	void Manager::Update()
	{
		Texture::Manager::Instance().Update();
	}

	/// <summary>
	/// draw process for directx
	/// </summary>
	void Manager::Draw()
	{
		Renderer::Manager::Instance().ClearViews();

		Texture::Manager::Instance().Draw();

		Renderer::Manager::Instance().FlipFrameBuffer();
	}
}