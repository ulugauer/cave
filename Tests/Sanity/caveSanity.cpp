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

#include "../Common/caveImageUtil.h"
#include "../Common/caveHtmlLog.h"

#include "Base/caveSanityTestDevice.h"
#include "Base/caveSanityTestMappedVbo.h"
#include "Base/caveSanityTestDeviceVbo.h"
#include "Base/caveSanityTestIndexBuffer.h"
#include "Base/caveSanityTestUniformBuffer.h"
#include "Base/caveSanityTestDepth.h"
#include "Base/caveSanityTestPushConstants.h"
#include "Base/caveSanityTestTexture2D.h"
#include "Base/caveSanityTestFrameBuffer.h"
#include "Base/caveSanityTestMsaa.h"

#include <iostream>
#include <sstream>
#include <memory>

using namespace cave;

typedef std::basic_string<char> string_type;

// command line arguments
string_type			g_ProjectPath;			///< path to project content
int32_t				g_WinWidth;				///< window width
int32_t				g_WinHeight;			///< window height
bool				g_WriteImage = false;	///< write output images
string_type			g_OutDirName;			///< path to write output images
bool				g_CompareImage = false;	///< check gold images
string_type			g_GoldDirName;			///< path to gold images

bool				g_WriteLog = true;		///< by default write log file
CaveHtmlLog			g_LogHMTL;				///< class looging the results in HTML



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
executeTest(RenderDevice *device, RenderCommandPool* commandPool, CaveSanityTestBase *curTest, const char *testName, userContextData* pUserData)
{
	bool success = true;

	std::cerr << testName << "\n";

	if (g_WriteLog)
	{
		g_LogHMTL.LogBeginRow();
		g_LogHMTL.LogMessage(testName);
		g_LogHMTL.LogNextColumn();
	}

	if (curTest->IsSupported(device))
	{
		success = curTest->Run(device, commandPool, pUserData);
		curTest->Cleanup(device, pUserData);
	}

	return success;
}


static void
printHelpMessage()
{
	string_type MsgStr = "Cave sanity application .\n\n";
	MsgStr += " -h\t\t\t- prints this help message\n";
	MsgStr += " -noLogFile\t\t- don't write a log file but output all messages to std out\n";
	MsgStr += " -r\t\t\t- path to resource files\n";
	MsgStr += " -o [outdir]\t\t- path to image output (directory must exist)\n";
	MsgStr += " -g [comparedir]\t- directory which contains the gold images\n";
	MsgStr += " -winSize x y\t\t- Set window size\n";

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
		index = pArgStr.find("-help");
		if (index != string_type::npos)
		{
			printHelpMessage();
			return false;
		}

		// get resource directory
		index = pArgStr.find("-r");
		if (index != string_type::npos && argv[theIndex + 1] != NULL)
		{
			pArgStr = argv[theIndex + 1];
			g_ProjectPath = pArgStr;
		}

		// check if we should write no log file
		index = pArgStr.find("-noLogFile");
		if (index != string_type::npos)
		{
			g_WriteLog = false;
		}

		// check if we should write images
		index = pArgStr.find("-o");
		if (index != string_type::npos && argv[theIndex + 1] != NULL)
		{
			pArgStr = argv[theIndex + 1];
			g_OutDirName = pArgStr;
			g_WriteImage = true;
		}

		// check if we should compare images
		index = pArgStr.find("-g");
		if (index != string_type::npos && argv[theIndex + 1] != NULL)
		{
			pArgStr = argv[theIndex + 1];
			g_GoldDirName = pArgStr;
			g_CompareImage = true;
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
	windowInfo.stencilBits = 8;
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

	// asynchronously load our image assets for lader usage
	ResourceManager& rm = renderDevice->GetResourceManager();
	rm.LoadImageAsset("UVChecker-dxt5.dds");
	
	// create our swap chain framebuffers
	if (!renderDevice->CreateSwapChainFramebuffers())
	{
		std::cerr << "Failed to create swap chain framebuffers\n";
	}

	// setup log file
	if (g_WriteLog && !g_LogHMTL.LogInit(false, "CaveSanityTest", "CaveBackendTest: backend tests"))
	{
		std::cerr << "Can't create log file.\n";
		exit(1);
	}

	if (g_WriteLog)
	{
		uint32_t major, minor, patch;

		g_LogHMTL.LogBeginParagraph();
		g_LogHMTL.LogMessage("<b>System Information</b>");

#ifdef _WIN32
		g_LogHMTL.LogMessage("Windows 64 bit");
#elif defined(__linux__)
		g_LogHMTL.LogMessage("Linux 64 bit");
#endif
		g_LogHMTL.LogEndParagraph();

		// context information
		g_LogHMTL.LogBeginParagraph();
		g_LogHMTL.LogMessage("<b>Device Information</b>");
		g_LogHMTL.LogMessage("Device Name: <font color=\"#8B4726\">%s</font>", renderDevice->GetDeviceName());
		renderDevice->GetDriverVersion(major, minor, patch);
		g_LogHMTL.LogMessage("Driver Version: <font color=\"#8B4726\">%d.%d.%d</font>", major, minor, patch);
		g_LogHMTL.LogMessage("");
		g_LogHMTL.LogMessage("Backend: <font color=\"#8B4726\">%s</font>", "Vulkan");
		renderDevice->GetApiVersion(major, minor, patch);
		g_LogHMTL.LogMessage("API Version: <font color=\"#8B4726\">%d.%d.%d</font>", major, minor, patch);
		g_LogHMTL.LogEndParagraph();

		g_LogHMTL.LogBeginParagraph();
		g_LogHMTL.LogMessage("Tests:");
		g_LogHMTL.LogBeginTable();
	}

	// allocate buffer for readback
	uint32_t readSize = userData.winWidth * userData.winHeight * 4;
	void* pixelBuffer = renderDevice->GetEngineAllocator()->Allocate(readSize, 4);

	// init test list
	initTestList();

	// run tests
	for (uint32_t i = 0; i < sizeof(testList) / sizeof(testList[0]); i++)
	{
		bool passed = executeTest(renderDevice, graphicsCommandPool, testList[i].m_test, testList[i].m_name, &userData);

		if (pixelBuffer && (g_WriteImage || g_CompareImage))
			renderDevice->ReadPixels(pixelBuffer);

		// write image 
		if (pixelBuffer && g_WriteImage)
		{
			string_type file = g_OutDirName;
			file += testList[i].m_name;
			file += ".png";

			image_t image = { (unsigned char *)pixelBuffer, userData.winWidth, userData.winHeight, userData.winWidth * 4, 4, true };
			WritePng(file.c_str(), &image);
		}

		if (g_CompareImage && testList[i].m_test->IsImageCompareSupported(renderDevice))
		{
			string_type refFile = g_GoldDirName;
			refFile += testList[i].m_name;
			refFile += ".png";
			image_t refImage;

			if (ReadPng(refFile.c_str(), &refImage))
			{
				image_t image = { (unsigned char *)pixelBuffer, userData.winWidth, userData.winHeight, userData.winWidth * 4, 4, true };
				passed &= CmpImage(&refImage, &image, NULL);

				delete refImage.pixels;
			}
			else
			{
				std::cerr << "caveSanity Test: gold image: " << refFile.c_str() << " not found\n";
			}

		}

		if (g_WriteLog && testList[i].m_test->IsSupported(renderDevice))
		{
			g_LogHMTL.LogMessage(
				"    <font color=\"%s\">%s</font>",
				passed ? "#00FF00" : "#FF0000",
				passed ? "passed" : "failed");

			if (pixelBuffer && g_WriteImage)
			{
				string_type file = g_OutDirName;
				file += testList[i].m_name;
				file += ".png";

				g_LogHMTL.LogNextColumn();
				g_LogHMTL.LogThumbnailImage(file.c_str(), 50, 50);
			}

			g_LogHMTL.LogEndRow();
		}
		else if (g_WriteLog)
		{
			g_LogHMTL.LogMessage(
				"    <font color=\"%s\">%s</font>",
				"#0000FF",
				"not supported");
			g_LogHMTL.LogEndRow();
		}


		if (passed)
			testPassed++;
		else
			testFailed++;
	}

	std::cerr << "Tests passed: " << testPassed << "\n";
	std::cerr << "Tests failed: " << testFailed << "\n";

	if (g_WriteLog)
	{
		g_LogHMTL.LogEndTable();
		g_LogHMTL.LogEndParagraph();
	}


	do {
		
	} while (frontend->HandleWindowMessage());

	if (g_WriteLog)
	{
		g_LogHMTL.LogCleanup();
	}

	// destroy test list
	destroyTestList();

	// release resources
	if (pixelBuffer)
		renderDevice->GetEngineAllocator()->Deallocate(pixelBuffer);
	// release at last
	renderDevice->ReleaseCommandPool(graphicsCommandPool);
	renderInstance->ReleaseRenderDevice(renderDevice);

	return 0;
}
