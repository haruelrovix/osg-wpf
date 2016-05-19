// OSG.Cpp.CLI/Wrapper.h
#pragma once

#include <Windows.h>

using namespace System;

namespace OSG
{
	namespace Cpp
	{
		class Core;

		namespace CLI
		{
			public ref class Wrapper
			{
			public:
				/// <summary>
				/// Renders the OSG Viewer into the specified Window handler.
				/// </summary>
				/// <param name="hwnd">The Window handler.</param>
				void Render(IntPtr hwnd);

				/// <summary>
				/// Destroys this instance.
				/// </summary>
				void Destroy();

			private:
				/// <summary>
				/// The pointer for this Wrapper class.
				/// </summary>
				Cpp::Core* _impl;
			};
		}
	}
}