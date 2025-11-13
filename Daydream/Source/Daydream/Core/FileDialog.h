#pragma once

#include "nfd.hpp"

namespace Daydream
{
	class FileDialog
	{
	public:
		FileDialog();
		~FileDialog();

		static void Init()
		{
			
		}

	protected:

	private:
		Unique<NFD::Guard> nfdGuard;
	};
}
