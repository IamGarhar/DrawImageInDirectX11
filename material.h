
#pragma once

namespace Material
{
	class Manager
	{
		// color
		DirectX::XMFLOAT4 _ambient;
		DirectX::XMFLOAT4 _diffuse;
		DirectX::XMFLOAT4 _specular;
		DirectX::XMFLOAT4 _emission;

		// intensity
		float _specularIntensity;

		// flags
		int _textureSamplingDisable;

		// this padding is for alignment according to HLSL packing rules
		float _padding[2] = {};

	public:
		Manager();

		// setter
		void SetAmbient (_In_ const DirectX::XMFLOAT4& color);
		void SetDiffuse (_In_ const DirectX::XMFLOAT4& color);
		void SetSpecular(_In_ const DirectX::XMFLOAT4& color);
		void SetEmission(_In_ const DirectX::XMFLOAT4& color);

		void SetSpecularIntensity(_In_ const float& intensity);
		void SetTextureSamplingDisable(_In_ const int& flag);

		void SetConstantBuffer();
	};
}
