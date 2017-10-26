/*
Copyright (c) <2017> <Udo Lugauer>
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
*/

#include "engineInstance.h"
#include "engineError.h"

#include <iostream>
#include <memory>

using namespace cave;

int main(int argc, char* argv[])
{  
	std::unique_ptr<EngineInstance> engineInstance = nullptr;
	RenderInstance* renderInstance = nullptr;
	RenderDevice* renderDevice = nullptr;
	IFrontend* frontend = nullptr;

	// window cration info
	FrontendWindowInfo windowInfo = {};
	windowInfo.xOffset = windowInfo.yOffset = 50;
	windowInfo.windowWidth = 800;
	windowInfo.windowHeight = 600;
	windowInfo.fullscreen = false;
	windowInfo.borderLess = false;
	windowInfo.colorBits = 32;
	windowInfo.depthBits = 24;
	windowInfo.windowTitle = "caveSanity";

	try
	{
		// Create engine instance which we use to create all other things
		EngineCreateStruct engineInfo = { 0, "caveSanity" };
		engineInstance = std::unique_ptr<EngineInstance>(new EngineInstance(engineInfo));

		// Create render instance. Basically the door to the hardware device.
		renderInstance = engineInstance->CreateRenderInstance(RenderInstanceTypes::InstanceVulkan);
		// create a window frontend interface
		frontend = engineInstance->CreateFrontend();
		// create window and fill in OS dependend information 
		frontend->CreateOsWindow(windowInfo);

		// Create a render device
		renderDevice = renderInstance->CreateRenderDevice();
		// Create a swap chain fro this device.
		// Note the windowInfo should be properly filled by the CreateOsWindow call
		// before a swap chain is created
		renderDevice->CreateSwapChain(windowInfo);
	}
	catch (cave::EngineError err)
	{
		std::cerr << err.what();
		return -1;
	}
	catch (...)
	{
		std::cerr << "Failed to create engine instance\n";
		return -1;
	}

	// Initialization went successful dispaly window
	frontend->DisplayWindow();

	renderInstance->ReleaseRenderDevice(renderDevice);

	do {


	} while (frontend->HandleWindowMessage());

	return 0;
}
