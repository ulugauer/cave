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
#include "Render/renderRenderPass.h"

#include "Base/caveSanityTestDevice.h"
#include "Base/caveSanityTestMappedVbo.h"
#include "Base/caveSanityTestDeviceVbo.h"

#include <iostream>
#include <sstream>
#include <memory>

using namespace cave;

typedef std::basic_string<char> string_type;

string_type			g_ProjectPath;  ///< path to project content
int32_t				g_WinWidth;		/// window width
int32_t				g_WinHeight;	/// window height


/// holds a test name and a pointer to the test class
typedef struct
{
	const char			*m_name;	///< the name of the test
	CaveSanityTestBase	*m_test;    ///< pointer to the test class
} testElement;

#undef CAVE_SANITY_TEST_ITERATE
#define CAVE_SANITY_TEST_ITERATE(_testName) \
    { #_testName, NULL },

static testElement
testList[] =
{
#include "caveSanityTestList.h"
};

static void
initTestList()
{
	uint32_t curTest = 0;

#undef CAVE_SANITY_TEST_ITERATE
#define CAVE_SANITY_TEST_ITERATE(_testName) \
    testList[curTest++].m_test = new _testName;

#include "caveSanityTestList.h"
}

static void
destroyTestList()
{
	uint32_t curTest = 0;

#undef CAVE_SANITY_TEST_ITERATE
#define CAVE_SANITY_TEST_ITERATE(_testName) \
   delete testList[curTest++].m_test;

#include "caveSanityTestList.h"
}

static bool
executeTest(RenderDevice *device, RenderCommandPool* commandPool, RenderPass* renderPass, CaveSanityTestBase *curTest, const char *testName, userContextData* pUserData)
{
	bool success = true;

	std::cerr << testName << "\n";

	if (curTest->IsSupported(device))
	{
		success = curTest->Run(device, commandPool, renderPass, pUserData);
		curTest->Cleanup(device, pUserData);
	}

	return success;
}


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
	RenderCommandPool* graphicsCommandPool = nullptr;
	IFrontend* frontend = nullptr;
	g_WinWidth = 640;
	g_WinHeight = 480;
	int32_t testPassed = 0;
	int32_t testFailed = 0;


	// get commaqnd line arguments
	if (!getComdLineArguments(argc, argv))
		return -1;

	if (g_ProjectPath.empty())
	{
		std::cerr << "No valid resource path specified\n";
		return -1;
	}

	// user data
	userContextData userData = { (uint32_t)g_WinWidth, (uint32_t)g_WinHeight };

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
		// allocate a graphics command pool
		HalCommandPoolInfo commandPoolInfo;
		commandPoolInfo._flags = 0;
		commandPoolInfo._queueFamily = HalCommandPoolQueueFamily::Graphics;
		graphicsCommandPool = renderDevice->CreateCommandPool(commandPoolInfo);

		// print some device info
		uint32_t major, minor, patch;
		renderDevice->GetApiVersion(major, minor, patch);
		std::cerr << "API Vesrion: " << major << "." << minor << "." << patch << std::endl;
		renderDevice->GetDriverVersion(major, minor, patch);
		std::cerr << "Driver Vesrion: " << major << "." << minor << "." << patch << std::endl;
		std::cerr << "Device Name: " << renderDevice->GetDeviceName() << "\n\n";
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


	// Right now we need a common render pass for all tests.
	// This will change one we can do offscreen rendering.
	HalRenderPassAttachment renderAttachment;
	renderAttachment._format = renderDevice->GetSwapChainImageFormat();
	renderAttachment._samples = HalSampleCount::SampleCount1;
	renderAttachment._loadOp = HalAttachmentLoadOperation::Clear;
	renderAttachment._storeOp = HalAttachmentStoreOperation::Store;
	renderAttachment._loadStencilOp = HalAttachmentLoadOperation::DontCare;
	renderAttachment._storeStencilOp = HalAttachmentStoreOperation::DontCare;
	renderAttachment._initialLayout = HalImageLayout::Undefined;
	renderAttachment._finalLayout = HalImageLayout::PresentSrcKHR;
	HalAttachmentReference attachRef;
	attachRef._attachment = 0;
	attachRef._layout = HalImageLayout::ColorAttachment;
	HalSubpassDescription subpassDesc;
	subpassDesc._pipelineBindPoint = HalPipelineBindPoints::Graphics;
	subpassDesc._colorAttachmentCount = 1;
	subpassDesc._pColorAttachments = &attachRef;
	HalSubpassDependency dependency;
	dependency._srcSubpass = HAL_SUBPASS_EXTERNAL;	// special subpass
	dependency._dstSubpass = 0;	// our subpass
	dependency._srcStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
	dependency._srcAccessMask = HalAccessBits::AccessNone;
	dependency._dstStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::ColorAttachmentOutput);
	dependency._dstAccessMask = HalAccessBits::ColorAttachmentRead | HalAccessBits::ColorAttachmentWrite;
	dependency._dependencyFlags = static_cast<HalDependencyFlags>(HalDependencyBits::DependencyNone);
	HalRenderPassInfo renderPassInfo;
	renderPassInfo._attachmentCount = 1;
	renderPassInfo._pAttachments = &renderAttachment;
	renderPassInfo._subpassCount = 1;
	renderPassInfo._pSubpasses = &subpassDesc;
	renderPassInfo._dependencyCount = 1;
	renderPassInfo._pDependencies = &dependency;
	RenderPass* renderPass = renderDevice->CreateRenderPass(renderPassInfo);
	
	// with a renderpass object we can create our swap chain framebuffers
	if (!renderDevice->CreateSwapChainFramebuffers(renderPass))
	{
		std::cerr << "Failed to create swap chain framebuffers\n";
	}

	// allocate buffer for readback
	uint32_t readSize = userData.winWidth * userData.winHeight * 4;
	void* pixelBuffer = renderDevice->GetEngineAllocator()->Allocate(readSize, 4);

	// init test list
	initTestList();

	// run tests
	for (uint32_t i = 0; i < sizeof(testList) / sizeof(testList[0]); i++)
	{
		bool passed = executeTest(renderDevice, graphicsCommandPool, renderPass, testList[i].m_test, testList[i].m_name, &userData);

		if (pixelBuffer)
			renderDevice->ReadPixels(pixelBuffer);

		if (passed)
			testPassed++;
		else
			testFailed++;
	}

	std::cerr << "Tests passed: " << testPassed << "\n";
	std::cerr << "Tests failed: " << testFailed << "\n";

	do {
		
	} while (frontend->HandleWindowMessage());

	// destroy test list
	destroyTestList();

	// release resources
	renderDevice->ReleaseRenderPass(renderPass);
	if (pixelBuffer)
		renderDevice->GetEngineAllocator()->Deallocate(pixelBuffer);
	// release at last
	renderDevice->ReleaseCommandPool(graphicsCommandPool);
	renderInstance->ReleaseRenderDevice(renderDevice);

	return 0;
}
