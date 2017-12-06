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
#include "Render/renderVertexInput.h"
#include "Render/renderInputAssembly.h"
#include "Render/renderLayerSection.h"
#include "Render/renderRasterizerState.h"
#include "Render/renderMultisample.h"
#include "Render/renderDepthStencil.h"
#include "Render/renderColorBlend.h"
#include "Render/renderDynamicState.h"

#include <iostream>
#include <sstream>
#include <memory>

using namespace cave;

typedef std::basic_string<char> string_type;

string_type			g_ProjectPath;  ///< path to project content
int32_t				g_WinWidth;		/// window width
int32_t				g_WinHeight;	/// window height

static void
printHelpMessage()
{
	string_type MsgStr = "NDDPathFinder display point clouds.\n\n";
	MsgStr += " -h			 - prints this help message\n";
	MsgStr += " -r			 - path to resource files\n";
	MsgStr += " -winSize x y - Set window size\n";

	std::cerr << MsgStr.c_str();
}

bool getComdLineArguments(int argc, char** argv)
{
	string_type::size_type index;

	if (argc < 2)
	{
		printHelpMessage();
		return false;
	}

	string_type pArgStr;
	for (int theIndex = 0; theIndex < argc; ++theIndex)
	{
		pArgStr = argv[theIndex];

		// get resource directory
		index = pArgStr.find("-r");
		if (index != string_type::npos && argv[theIndex + 1] != NULL)
		{
			pArgStr = argv[theIndex + 1];
			g_ProjectPath = pArgStr;
		}

		// get window size
		index = pArgStr.find("-winSize");
		if (index != string_type::npos && argv[theIndex + 1] != NULL && argv[theIndex + 2] != NULL)
		{
			pArgStr = argv[theIndex + 1];
			std::istringstream(pArgStr) >> g_WinWidth;
			pArgStr = argv[theIndex + 2];
			std::istringstream(pArgStr) >> g_WinHeight;
		}
	}

	return true;
}


int main(int argc, char* argv[])
{  
	std::unique_ptr<EngineInstance> engineInstance = nullptr;
	RenderInstance* renderInstance = nullptr;
	RenderDevice* renderDevice = nullptr;
	IFrontend* frontend = nullptr;
	g_WinWidth = 1280;
	g_WinHeight = 768;

	// get commaqnd line arguments
	if (!getComdLineArguments(argc, argv))
		return 1;

	// window cration info
	FrontendWindowInfo windowInfo = {};
	windowInfo.xOffset = windowInfo.yOffset = 50;
	windowInfo.windowWidth = g_WinWidth;
	windowInfo.windowHeight = g_WinHeight;
	windowInfo.fullscreen = false;
	windowInfo.borderLess = false;
	windowInfo.offscreen = false;
	windowInfo.colorBits = 32;
	windowInfo.depthBits = 24;
	windowInfo.windowTitle = "caveSanity";

	try
	{
		// Create engine instance which we use to create all other things
		EngineCreateStruct engineInfo = { 0, "caveSanity", g_ProjectPath.c_str() };
		engineInstance = std::unique_ptr<EngineInstance>(new EngineInstance(engineInfo));

		// Create render instance. Basically the door to the hardware device.
		renderInstance = engineInstance->CreateRenderInstance(RenderInstanceTypes::InstanceVulkan);
		// create a window frontend interface
		frontend = engineInstance->CreateFrontend();
		// create window and fill in OS dependend information 
		frontend->CreateOsWindow(windowInfo);

		// Create a render device
		// Note the windowInfo should be properly filled by the CreateOsWindow call
		// before a device is created
		renderDevice = renderInstance->CreateRenderDevice(windowInfo);
		// Create a swap chain fro this device.
		renderDevice->CreateSwapChain();
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

	// Render section
	RenderLayerSectionInfo sectionInfo;
	sectionInfo.x = sectionInfo.y = 0;
	sectionInfo.width = windowInfo.windowWidth;
	sectionInfo.height = windowInfo.windowHeight;
	RenderLayerSection* layerSection = renderDevice->CreateLayerSection(sectionInfo);
	// load resources
	ResourceManager& rm = renderDevice->GetResourceManager();
	RenderMaterial material = rm.LoadMaterialAsset("ColoredMaterial.asset");
	RenderVertexInput* vertexInput = renderDevice->CreateVertexInput();
	RenderInputAssembly* inputAssembly = renderDevice->CreateInputAssembly();
	// rasterizer state
	HalRasterizerSetup rasterizerInfo;
	RenderRasterizerState* rasterizerState = renderDevice->CreateRasterizerState(rasterizerInfo);
	// multisample state
	HalMultisampleState multisampleInfo;
	RenderMultisample* multisampleState = renderDevice->CreateMultisampleState(multisampleInfo);
	// depth stencil state
	HalDepthStencilSetup depthStencilInfo;
	RenderDepthStencil* depthStencilState = renderDevice->CreateDepthStencilState(depthStencilInfo);
	HalColorBlendState colorBlendInfo;
	HalColorBlendAttachment blendAttachment;
	caveVector<HalColorBlendAttachment> blendAttachmentArray(renderDevice->GetEngineAllocator());
	blendAttachmentArray.Push(blendAttachment);
	RenderColorBlend* colorBlendState = renderDevice->CreateColorBlendState(colorBlendInfo, blendAttachmentArray);
	blendAttachmentArray.Clear();
	// dynamic state
	caveVector<HalDynamicStates> dynamicStates(renderDevice->GetEngineAllocator());
	dynamicStates.Push(HalDynamicStates::Viewport);
	dynamicStates.Push(HalDynamicStates::Scissor);
	RenderDynamicState* dynamicState = renderDevice->CreateDynamicState(dynamicStates);
	dynamicStates.Clear();

	do {


	} while (frontend->HandleWindowMessage());

	renderDevice->ReleaseDynamicState(dynamicState);
	renderDevice->ReleaseColorBlendState(colorBlendState);
	renderDevice->ReleaseDepthStencilState(depthStencilState);
	renderDevice->ReleaseMultisampleState(multisampleState);
	renderDevice->ReleaseRasterizerState(rasterizerState);
	renderDevice->ReleaseInputAssembly(inputAssembly);
	renderDevice->ReleaseVertexInput(vertexInput);
	renderDevice->ReleaseLayerSection(layerSection);
	// release at last
	renderInstance->ReleaseRenderDevice(renderDevice);

	return 0;
}
