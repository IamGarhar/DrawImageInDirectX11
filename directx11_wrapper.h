
#pragma once

#pragma warning(push)
#pragma warning(disable: 4005)

// instructs the preprocessor to include the specified header file
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxtex.h>

#pragma warning(pop)

// specify libraries to link with the linker
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "directxtex.lib")

namespace DirectXWrapper
{
	class Manager
	{
	public:
		static Manager& Instance();

		HRESULT Initialize();
		void Terminate();
		void Update();
		void Draw();
	};
}
