
#pragma once

namespace Sprite
{
	class Manager
	{
	protected:
		ID3D11Buffer* VertexBuffer;
		ID3D11ShaderResourceView* Srv;

		wchar_t* TexturePath;

		DirectX::XMFLOAT2 Position;
		DirectX::XMFLOAT2 Scale;
		DirectX::XMFLOAT2 Texcoord;
		DirectX::XMFLOAT2 TexSize;
		DirectX::XMFLOAT4 Color;
		float Rotation;

		bool IsLoad;

		//-----------------------------------
		// protected funcs
		//-----------------------------------
		HRESULT CreateSrvFromFile();
		HRESULT CreateVertexBuffer();

		void SetAnchorPointCenter();

		void Release();

		//-----------------------------------
		// public funcs
		//-----------------------------------
	public:
		Manager();

		virtual HRESULT Initialize() = 0;
		virtual void Terminate() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}
