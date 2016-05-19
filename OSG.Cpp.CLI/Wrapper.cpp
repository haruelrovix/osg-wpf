// OSG.Cpp.CLI/Wrapper.cpp

#include "Wrapper.h"
#include "../OSG.Cpp/Core.h"

/// <summary>
/// Renders the OSG Viewer into the specified Window handler.
/// </summary>
/// <param name="hwnd">The Window handler.</param>
void OSG::Cpp::CLI::Wrapper::Render(IntPtr hwnd)
{
	// Get the pointer as window handler
	HWND nativeHWND = (HWND)hwnd.ToPointer();

	// Call the native Render method
	_impl->Render(nativeHWND);
}

/// <summary>
/// Destroys this instance.
/// </summary>
void OSG::Cpp::CLI::Wrapper::Destroy()
{
	// Call the native Destroy method
	_impl->Destroy();
}
