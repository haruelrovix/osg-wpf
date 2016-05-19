// OSG.Cpp/Core.cpp
#include "Core.h"

#pragma region Global Variables

/// <summary>
/// Holds the OSG Viewer
/// </summary>
osg::ref_ptr<osgViewer::Viewer> g_viewer;

/// <summary>
/// Rendering status
/// </summary>
bool g_finished;

#pragma endregion

#pragma region Private Methods

/// <summary>
/// Renders the frame.
/// </summary>
void render(void*)
{
	// Keep the rendering as long as the viewer's work isn't done
	while (!g_viewer->done())
	{
		g_viewer->frame();
	}

	// The rendering is done, set the status to Finished
	g_finished = true;
}

/// <summary>
/// Creates the viewer.
/// </summary>
/// <param name="hwnd">The Window handler.</param>
/// <returns>True if there is no error during the creating process.</returns>
bool createViewer(HWND hwnd)
{
	// Get the dimensions of the window handle 
	RECT rect;
	GetWindowRect(hwnd, &rect);

	// WindowData is used to pass in the Win32 window handle attached the GraphicsContext::Traits structure 
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(hwnd);

	// Create osg's graphics context traits 
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;

	// Set location and size of the window 
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->inheritedWindowData = windata;
	traits->pbuffer = false;

	// We must set the pixelformat before we can create the OSG Rendering Surface 
	PIXELFORMATDESCRIPTOR pixelFormat =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // Specifies the size of this data structure. This value should be set to sizeof(PIXELFORMATDESCRIPTOR). 
		1,	// Specifies the version of this data structure. This value should be set to 1. 
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // A set of bit flags that specify properties of the pixel buffer 
		PFD_TYPE_RGBA, // Specifies the type of pixel data.  PFD_TYPE_RGBA or PFD_TYPE_COLORINDEX 
		24,	// Specifies the number of color bitplanes in each color buffer. For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes 
		0,  // cRedBits	  Specifies the number of red bitplanes in each RGBA color buffer. 
		0,  // cRedShift  Specifies the shift count for red bitplanes in each RGBA color buffer. 
		0,0,// cGreenBits, cGreenShift 
		0,0,// cBlueBits , cBlueShift 
		0,0,// cAlphaBits, cAlphaShift 
		0,  // cAccumBits  Specifies the total number of bitplanes in the accumulation buffer. 
		0,  // cAccumRedBits Specifies the number of red bitplanes in the accumulation buffer. 
		0,0,0,// cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits 
		24,// cDepthBits     Specifies the depth of the depth (z-axis) buffer. 
		0, // cStencilBits   Specifies the depth of the stencil buffer. 
		0, // cAuxBuffers    Specifies the number of auxiliary buffers. Auxiliary buffers are not supported. 
		PFD_MAIN_PLANE, // iLayerType Ignored. Earlier implementations of OpenGL used this member, but it is no longer used. 
		0, // bReserved      Specifies the number of overlay and underlay planes. Bits 0 through 3 specify up to 15 overlay planes and bits 4 through 7 specify up to 15 underlay planes 
		0, // dwLayerMask    Ignored. Earlier implementations of OpenGL used this member, but it is no longer used. 
		0, // dwVisibleMask  Specifies the transparent color or index of an underlay plane. When the pixel type is RGBA, dwVisibleMask is a transparent RGB color value 
		0  // dwDamageMask   Ignored. Earlier implementations of OpenGL used this member, but it is no longer used. 
	};

	// Retrieves a device context for the client area. (ATL) 
	HDC hdc = ::GetDC(hwnd);
	if (hdc == 0)
	{
		// Destroys the window associated with the CWindow object and sets m_hWnd to NULL. 
		::DestroyWindow(hwnd);
		return false;
	}

	// The ChoosePixelFormat OpenGL function attempts to match an appropriate pixel format supported by a device context to a given pixel format specification. 
	int pixelFormatIndex = ::ChoosePixelFormat(hdc, &pixelFormat);
	if (pixelFormatIndex == 0)
	{
		// (MFC)Releases the display device context of a container of a windowless control, freeing the device context for use by other applications 
		::ReleaseDC(hwnd, hdc);
		::DestroyWindow(hwnd);
		return false;
	}

	// The SetPixelFormat OpenGL function sets the pixel format of the specified device context to the format specified by the iPixelFormat index. 
	if (!::SetPixelFormat(hdc, pixelFormatIndex, &pixelFormat))
	{
		::ReleaseDC(hwnd, hdc);
		::DestroyWindow(hwnd);
		return false;
	}

	// Create graphics context 
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setGraphicsContext(gc.get());
	camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

	camera->setDrawBuffer(GL_BACK);
	camera->setReadBuffer(GL_BACK);

	// Create the viewer and attach the camera to it 
	g_viewer = new osgViewer::Viewer;
	g_viewer->addSlave(camera.get());

	g_viewer->setCamera(camera.get());
	g_viewer->setSceneData(osgDB::readNodeFile("cessna.osg"));
	g_viewer->setKeyEventSetsDone(0);
	g_viewer->setCameraManipulator(new osgGA::TrackballManipulator);

	// The viewer isn't rendering yet, set the status to False
	g_finished = false;

	return true;
}

#pragma endregion

#pragma region Public Methods

/// <summary>
/// Renders the OSG Viewer into the specified Window handler.
/// </summary>
/// <param name="hwnd">The Window handler.</param>
void OSG::Cpp::Core::Render(HWND hwnd)
{
	if (createViewer(hwnd))
	{
		// Create a rendering thread
		_beginthread(render, 0, NULL);
	}
}

/// <summary>
/// Destroys this instance.
/// </summary>
void OSG::Cpp::Core::Destroy()
{
	// Set viewer's work to Done
	g_viewer->setDone(true);

	// Get the rendering status
	while (!g_finished) Sleep(10);

	// Release the memory
	g_viewer = NULL;
	g_finished = NULL;
}

#pragma endregion