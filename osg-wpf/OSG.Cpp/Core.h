// OSG.Cpp/Core.h
#pragma once

#include <Windows.h>
#include <process.h>

#include <osg\Camera>
#include <osgDB\ReadFile>
#include <osgGA\TrackballManipulator>
#include <osgViewer\api\Win32\GraphicsWindowWin32>
#include <osgViewer\Viewer>

namespace OSG
{
	namespace Cpp
	{
		class __declspec(dllexport) Core
		{
		public:
			/// <summary>
			/// Renders the OSG Viewer into the specified Window handler.
			/// </summary>
			/// <param name="hwnd">The Window handler.</param>
			void Render(HWND hwnd);

			/// <summary>
			/// Destroys this instance.
			/// </summary>
			void Destroy();
		};
	}
}