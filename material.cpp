
#include "directx11_wrapper.h"
#include "material.h"
#include "renderer.h"

namespace Material
{
	/// <summary>
	/// constructor for material
	/// </summary>
	Manager::Manager()
	{
		_ambient  = {};
		_diffuse  = {};
		_specular = {};
		_emission = {};
		_specularIntensity = 0.0f;
		_textureSamplingDisable = 0;
	}

	/// <summary>
	/// set ambient strength
	/// </summary>
	void Manager::SetAmbient(_In_ const DirectX::XMFLOAT4& color)
	{
		_ambient = color;
	}

	/// <summary>
	/// set diffuse strength
	/// </summary>
	void Manager::SetDiffuse(_In_ const DirectX::XMFLOAT4& color)
	{
		_diffuse = color;
	}

	/// <summary>
	/// set specular strength
	/// </summary>
	void Manager::SetSpecular(_In_ const DirectX::XMFLOAT4& color)
	{
		_specular = color;
	}

	/// <summary>
	/// set emission strength
	/// </summary>
	void Manager::SetEmission(_In_ const DirectX::XMFLOAT4& color)
	{
		_emission = color;
	}

	/// <summary>
	/// set specular intensity
	/// </summary>
	void Manager::SetSpecularIntensity(_In_ const float& intensity)
	{
		_specularIntensity = intensity;
	}

	/// <summary>
	/// set the flag for texture sampling disable
	/// </summary>
	void Manager::SetTextureSamplingDisable(_In_ const int& flag)
	{
		_textureSamplingDisable = flag;
	}

	/// <summary>
	/// set material to the constant buffer
	/// </summary>
	void Manager::SetConstantBuffer()
	{
		Renderer::Manager& renderer = Renderer::Manager::Instance();
		renderer.GetDeviceContext().UpdateSubresource(&renderer.GetConstantBufferMaterial(), 0, nullptr, this, 0, 0);
	}
}
