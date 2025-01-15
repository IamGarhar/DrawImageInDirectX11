
#pragma once

namespace Application
{
	class Manager
	{
	public:
		static Manager& Instance();

		int  Initialize();
		void Terminate();
		void Run();
	};
}
