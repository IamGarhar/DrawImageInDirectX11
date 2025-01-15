
#pragma once

namespace Texture
{
	constexpr char* TEXTURE_FILE_PATH = "resource/texture/test.png";

	class Manager : public Sprite::Manager
	{
	public:
		static Manager& Instance();

		HRESULT Initialize() override;
		void Terminate() override;
		void Update() override;
		void Draw() override;
	};
}
