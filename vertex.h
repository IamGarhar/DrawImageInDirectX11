
#pragma once

namespace Vertex
{
	class Manager
	{
	public:
		// these need to be the same as input-layout
		// just dont worry about alignment here
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT4 Color;
		DirectX::XMFLOAT2 Texcoord;

		Manager();
	};
}
