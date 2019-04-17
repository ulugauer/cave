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
#pragma once

/// @file halTypes.h
///       Backend types

/** \addtogroup backend
*  @{
*
*/

#include "engineDefines.h"

#include <memory>

namespace cave
{

#define HAL_SUBPASS_EXTERNAL            (~0U)	///< Special value for subpass before or after present
#define HAL_WHOLE_SIZE					(~0ULL)	///< Special value for memory size

// forward
class HalSampler;

/**
*  @brief A strongly typed enum class representing device extension queries
*/
enum class HalExtensionCaps
{
	SwapChainSupport = 0,		///< Device supports swap chains
	DirectGLSL = 1,				///< Device supports GLSL shaders (vulkan only)
};

/**
*  @brief A strongly typed enum class representing device capabilities query
*/
enum class HalDeviceFeatureCaps
{
    TextureCompressionBC = 0,   ///< BC compressed texture are supported
    SamplerAnisotropy = 1,      ///< Anisotropic filtering support
};

/**
*  @brief A strongly typed enum class representing image formts
*/
enum class HalImageFormat
{
	Undefined = 0,	///< Undefined format
	R8G8B8A8UNorm = 37,
	R8G8B8A8SNorm = 38,
	B8G8R8A8UNorm = 44,
	B8G8R8A8SNorm = 50,
	R16G16B16A16SFloat = 97,
	R32G32SFloat = 103,
	R32G32B32SFloat = 106,
	R32G32B32A32SFloat = 109,
	D32SFloat = 126,
	D24UnormS8Uint = 129,
	D32SFloatS8Uint = 130,
	BC1RgbUnorm = 131,
	BC1RgbSrgb = 132,
	BC1RgbaUnorm = 133,
	BC1RgbaSrgb = 134,
	BC2Unorm = 135,
	BC2Srgb = 136,
	BC3Unorm = 137,
	BC3Srgb = 138,
	BC4Unorm = 139,
	BC4Snorm = 140,
	BC5Unorm = 141,
	BC5Snorm = 142,
	BC7Unorm = 145,
	BC7Srgb = 146,
};

/**
* @brief A strongly typed enum class representing image type
*/
enum class HalImageType
{
    Image1D = 0,
    Image2D = 1,
    Image3D = 2
};

/**
* @brief A strongly typed enum class representing image view types
*/
enum class HalImageViewType 
{
    Image1D = 0,
    Image2D = 1,
    Image3D = 2,
    ImageCube = 3,
    Image1DArray = 4,
    Image2DArray = 5,
    ImageCubeArray = 6
};

/**
 * @brief A strongly typed enum class representing component swizzle
 */
enum HalComponentSwizzle 
{
    Identity = 0,
    Zero = 1,
    One = 2,
    Red = 3,
    Green = 4,
    Blue = 5,
    Alpha = 6
};

/**
 * @brief A strongly typed enum class representing image aspect flags
 */
enum class HalImageAspectFlagBits 
{
    Color = 0x0001,
    Depth = 0x0002,
    Stencil = 0x0004,
    MetaData = 0x0008,
    Plane0 = 0x0010,
    Plane1 = 0x0020,
    Plane2 = 0x0040,
    MemoryPlane0Ext = 0x0080,
    MemoryPlane1Ext = 0x0100,
    MemoryPlane2Ext = 0x0200,
    MemoryPlane3Ext = 0x0400
};

typedef uint32_t HalImageAspectFlags;   ///< HalImageAspectFlagBits flags

/**
 * @brief A strongly typed enum class representing image usage flags
 */
enum class HalImageUsageFlagBits 
{
    TransferSrc = 0x0001,
    TransferDst = 0x0002,
    Sampled = 0x0004,
    Storage = 0x0008,
    ColorAttachment = 0x0010,
    DepthAttachment = 0x0020,
    TransientAttachment = 0x0040,
    InputAttachment = 0x0080,
    ShadingRateImageNV = 0x0100,
};

typedef uint32_t HalImageUsageFlags;   ///< HalImageUsageFlagBits flags

/**
*  @brief A strongly typed enum class representing component types
*/
enum class HalComponentType
{
	UInt8 = 1,
	Int8 = 2,
	UInt16 = 3,
	Int16 = 4,
	UInt32 = 5,
	Int32 = 6,
	UInt64 = 7,
	Int64 = 8,
	Float16 = 9,
	Float32 = 10,
	Float64 = 11,
};

/**
*  @brief A strongly typed enum class representing the sample count
*/
enum class HalSampleCount
{
	SampleCount1 = 1,
	SampleCount2 = 2,
	SampleCount4 = 4,
	SampleCount8 = 8,
	SampleCount16 = 16,
	SampleCount32 = 32,
	SampleCount64 = 64
};
typedef uint32_t HalSampleCountFlags;	///< Combined sample count flags

/**
*  @brief A strongly typed enum class representing attachment load operations
*/
enum class HalAttachmentLoadOperation
{
	Load = 0,
	Clear = 1,
	DontCare = 2
};

/**
*  @brief A strongly typed enum class representing attachment store operations
*/
enum class HalAttachmentStoreOperation
{
	Store = 0,
	DontCare = 1
};

/**
*  @brief A strongly typed enum class representing image layout format
*/
enum class HalImageLayout
{
	Undefined = 0,
	General = 1,
	ColorAttachment = 2,
	DepthStencilAttachment = 3,
	DepthStencilReadOnly = 4,
	ShaderReadOnly = 5,
	TransferSrc = 6,
	TransferDst = 7,
	Preinitialized = 8,
	PresentSrcKHR = 9,
	SharedPresentSrcKHR = 10
};

/**
*  @brief A strongly typed enum class representing subpass desc bits
*/
enum class HalSubpassDescriptionBits
{
};

/**
*  @brief A strongly typed enum class representing pipeline bind points
*/
enum class HalPipelineBindPoints
{
	Graphics = 0,
	Compute = 1
};

/**
*  @brief A strongly typed enum class representing pipeline staging points
*/
enum class HalPipelineStageBits
{
	TopOfPipe = 0x1,
	DrawIndirect = 0x2,
	VertexInput = 0x4,
	VertexShader = 0x8,
	TessellationControlShader = 0x10,
	TessellationEvaluationShader = 0x20,
	GeometryShader = 0x40,
	FragmentShader = 0x80,
	EarlyFragmentTests = 0x100,
	LateFragmentTests = 0x200,
	ColorAttachmentOutput = 0x400,
	ComputeShader = 0x800,
	Transfer = 0x1000,
	BottomOfPipe = 0x2000,
	Host = 0x4000,
};
typedef uint32_t HalPipelineStageFlags;		///< Combined pipeline stage flags

/**
*  @brief A strongly typed enum class representing access types
*/
enum HalAccessBits
{
	AccessNone = 0x0,
	IndirectCommandRead = 0x1,
	IndexRead = 0x2,
	VertexAttributeRead = 0x4,
	UniformRead = 0x8,
	InputAttachmentRead = 0x10,
	ShaderRead = 0x20,
	ShaderWrite = 0x40,
	ColorAttachmentRead = 0x80,
	ColorAttachmentWrite = 0x100,
	DepthStencilAttachmentRead = 0x200,
	DepthStencilAttachmentWrite = 0x400,
	TransferRead = 0x800,
	TransferWrite = 0x1000,
	HostRead = 0x2000,
	HostWrite = 0x4000,
	MemoryRead = 0x8000,
	MemoryWrite = 0x10000,
};
typedef uint32_t HalAccessFlags;		///< Combined access flags

/**
*  @brief A strongly typed enum class representing memory properties
*/
enum class HalMemoryPropertyBits
{
	DeviceLocal = 0x01,
	HostVisible = 0x02,
	HostCoherent = 0x04,
	HostCached = 0x08,
	LazilyAllocated = 0x10,
};
typedef uint32_t HalMemoryPropertyFlags;		///< Combined memory property flags


/**
*  @brief A strongly typed enum class representing subpass dependencies
*/
enum class HalDependencyBits
{
	DependencyNone = 0x0,
	DependencyByRegion = 0x1
};
typedef uint32_t HalDependencyFlags;		///< Combined dependency flags


/**
*  @brief A strongly typed enum class representing polygon cull mode
*/
enum class HalCullMode
{
	NoCull = 0,
	Front = 1,
	Back = 2,
	FrontAndBack = 3
};

/**
*  @brief A strongly typed enum class representing polygons front face
*/
enum class HalFrontFace
{
	CounterClockwise = 0,
	Clockwise = 1
};

/**
*  @brief A strongly typed enum class representing polygon render mode
*/
enum class HalPolygonMode
{
	Fill = 0,
	Line = 1,
	Point = 2
};

/**
*  @brief A strongly typed enum class representing stencil operations
*/
enum class HalStencilOp
{
	Keep = 0,
	Zero = 1,
	Replace = 2,
	IncrementClamp = 3,
	DecrementClamp = 4,
	Invert = 5,
	IncrementWarp = 6,
	DecrementWarp = 7
};

/**
*  @brief A strongly typed enum class representing depth compare operations
*/
enum class HalCompareOp
{
	Never = 0,
	Less = 1,
	Equal = 2,
	LessEqual = 3,
	Greater = 4,
	NotEqual = 5,
	GreaterEqual = 6,
	AlwaysPass = 7
};

/**
*  @brief A strongly typed enum class representing blending factors
*/
enum class HalBlendFactor
{
	Zero = 0,
	One = 1,
	SrcColor = 2,
	OneMinusSrcColor = 3,
	DstColor = 4,
	OneMinusDstColor = 5,
	SrcAlpha = 6,
	OneMinusSrcAlpha = 7,
	DstAlpha = 8,
	OneMinusDstAlpha = 9,
	ConstantColor = 10,
	OneMinusConstantColor = 11,
	ConstantAlpha = 12,
	OneMinusConstantAlpha = 13,
	SrcAlphaSaturate = 14,
	Src1Color = 15,
	OneMinusSrc1Color = 16,
	Src1Alpha = 17,
	OneMinusSrc1Alpha = 18
};

/**
*  @brief A strongly typed enum class representing blending operations
*/
enum class HalBlendOp
{
	Add = 0,
	Subtract = 1,
	ReverseSubtract = 2,
	Min = 3,
	Max = 4
};

/**
*  @brief A strongly typed enum class representing color write mask
*/
enum class HalColorComponents
{
	Red = 0x1,
	Green = 0x2,
	Blue = 0x4,
	Alpha = 0x8
};

typedef uint32_t HalColorComponentFlags;	///< Combined color component flags

/**
*  @brief A strongly typed enum class representing logic operations
*/
enum class HalLogicOp
{
	Clear = 0,
	And = 1,
	AndReverse = 2,
	Copy = 3,
	AndInverted = 4,
	NoOp = 5,
	Xor = 6,
	Or = 7,
	Nor = 8,
	Equivalent = 9,
	Invert = 10,
	OrReverse = 11,
	CopyInverted = 12,
	OrInverted = 13,
	Nand = 14,
	Set = 15
};

/**
*  @brief A strongly typed enum class representing dynamic states
*/
enum class HalDynamicStates
{
	Viewport = 0,
	Scissor = 1,
	LineWidth = 2,
	DepthBias = 3,
	BlendConstants = 4,
	DepthBounds = 5,
	StencilCompareMask = 6,
	StencilWriteMask = 7,
	StencilReference = 8
};

/**
*  @brief A strongly typed enum class for sampler creation
*/
enum class HalSamplerCreateFlagBits 
{
    SubSampled = 0x0001,
    SubSampledCoarseReconstruction = 0x0002,
};

typedef uint32_t HalSamplerCreateFlags;		///< Sampler creation info

/**
*  @brief A strongly typed enum class representing texture filtering
*/
enum class HalFilter 
{
    Nearest = 0,
    Linear = 1,
    Cubic = 1000015000,
};

/**
*  @brief A strongly typed enum class representing texture mipmap filtering
*/
enum class HalSamplerMipmapMode 
{
    Nearest = 0,
    Linear = 1,
};

/**
*  @brief A strongly typed enum class representing texture wrapping modes
*/
enum class HalSamplerAddressMode 
{
    Repeat = 0,
    MirroredRepeat = 1,
    ClampToEdge = 2,
    ClampToBorder = 3,
    MirrorClampToEdge = 4,
};

enum class HalBorderColor 
{
    FloatTransparentBlack = 0,
    IntTransparentBlack = 1,
    FloatOpaqueBlack = 2,
    IntOpaqueBlack = 3,
    FloatOpaqueWhite = 4,
    IntOpaqueWhite = 5,
};

/**
*  @brief A strongly typed enum class representing the shader stages
*/
enum class HalShaderStages
{
	Vertex = 0x1,
	TessellationControl = 0x2,
	TessellationEvaluation = 0x4,
	Geometry = 0x8,
	Fragment = 0x10,
	Compute = 0x20
};
typedef uint32_t HalShaderStagesFlags;	///< Shader stages flags

/**
*  @brief A strongly typed enum class representing descriptor types
*/
enum class HalDescriptorType 
{
	Sampler = 0,
	CombinedImageSampler = 1,
	SampledImage = 2,
	StorageImage = 3,
	UniformTexelBuffer = 4,
	StorageTexelBuffer = 5,
	UniformBuffer = 6,
	StorageBuffer = 7,
	UniformBufferDynamic = 8,
	StorageBufferDynamic = 9,
	InputAttachment = 10,
};

/**
*  @brief A strongly typed enum class representing descriptor layout
*/
enum HalDescriptorSetLayoutCreateBits
{
	// empty right now
};

typedef uint32_t HalDescriptorSetLayoutCreateFlags;		///< Descriptor layout create flags

/**
*  @brief A strongly typed enum class representing descriptor pool create flags
*/
enum class HalDescriptorPoolCreateFlagBits 
{
	FreeDescriptorSet = 0x01,
};

typedef uint32_t HalDescriptorPoolCreateFlags;		///< Descriptor pool create flags

/**
*  @brief A strongly typed enum class representing input rate
*/
enum class HalVertexInputRate 
{
	Vertex = 0,
	Instance = 1,
} ;

/**
*  @brief A strongly typed enum class representing command pool queue family
*/
enum class HalCommandPoolQueueFamily
{
	Graphics = 1,
	Presentation = 2
};

/**
*  @brief A strongly typed enum class representing command pool queue family
*/
enum class HalCommandPoolUsage
{
	CreateTransient = 0x1,
	ResetCommandBuffer = 0x2
};

typedef uint32_t HalCommandPoolUsageFlags;	///< Command pool usage flags

/**
*  @brief A strongly typed enum class representing command buffer level
*/
enum class HalCommandBufferLevel
{
	PrimaryLevel = 0,
	SecondaryLevel = 1
};

/**
*  @brief A strongly typed enum class representing index types
*/
enum class HalIndexType
{
	UInt16 = 0,
	UInt32 = 1
};

/**
*  @brief A strongly typed enum class representing command buffer usage
*/
enum class HalCommandBufferUsage
{
	OneTimeSubmit = 0x1,
	RenderPassContinue = 0x2,
	SimultaneousUse = 0x4
};
typedef uint32_t HalCommandBufferUsageFlags;	///< Command buffer usage flags


/**
*  @brief A strongly typed enum class representing buffer creation
*/
enum class HalBufferCreate
{
	SparseBinding = 0x0001,
	SparseResidency = 0x0002,
	SparseAliased = 0x0004,
};
typedef uint32_t HalBufferCreateFlags;	///< Buffer create flags

/**
*  @brief A strongly typed enum class representing buffer usage
*/
enum class HalBufferUsage
{
	TransferSource = 0x0001,
	TransferDest = 0x0002,
	UniformTexelBuffer = 0x0004,
	StorageTexelBuffer = 0x0008,
	UniformBuffer = 0x0010,
	StorageBuffer = 0x0020,
	IndexBuffer = 0x0040,
	VertexBuffer = 0x0080,
	IndirectBuffer = 0x0100,
};
typedef uint32_t HalBufferUsageFlags;	///< Buffer usage flags

/**
*  @brief A strongly typed enum class representing primitive topology
*/
enum class HalPrimitiveTopology
{
	PointList = 0,
	LineList = 1,
	LineStrip = 2,
	TriangleList = 3,
	TriangleStrip = 4,
	TriangleFan = 5,
	LineListWidthAdjacency = 6,
	LineStripWidthAdjacency = 7,
	TriangleListWidthAdjacency = 8,
	TriangleStripWidthAdjacency = 9,
	PatchList = 10,
};

/**
*  @brief A strongly typed enum class representing buffer sharing mode
*/
enum class HalBufferShareMode
{
	Exclusive = 1,
	Comcurrent = 2,
};

/**
*  @brief A strongly typed enum class representing subpass drawing commands
*/
enum class HalSubpassContents
{
	Inline = 0,					// render pass commands are in primary command buffer
	SecondarCommandBuffer = 1	// render pass command are in secondary command buffer
};

/**
* @brief Structure for device capabilities
*/
struct CAVE_INTERFACE HalDeviceExtensions
{
	union
	{
		struct {
			bool bSwapChainSupport : 1;		///< swap chain support
			bool bGLSLSupport : 1;			///< GLSL shader supported (Vulkan only)
		} bits;

		uint32_t u32Values;
	} caps;	///< combiend value
};

/**
* @brief Structure for device features
*/
struct CAVE_INTERFACE HalDeviceFeatures
{
	union
	{
		struct {
			bool bTextureCompressionBC : 1;		///< Support of BC compressed textures
            bool bSamplerAnisotropy : 1;		///< Support anisotropic filtering
		} bits;

		uint32_t u32Values;
	} caps;	///< combiend value
};

/**
* @brief Rasterizer state setup
*/
struct CAVE_INTERFACE HalRasterizerSetup
{
	HalCullMode _cullMode;			///< Polygon culling mode
	HalFrontFace _frontFace;		///< Front face mode
	bool _depthClampEnable;			///< Clamp depth values to depth range
	bool _depthBiasEnable;			///< Add constant value to depth
	float _depthBiasConstantFactor;	///< Constant bias value
	float _depthBiasClamp;			///< Depth bias clamp value
	float _depthBiasSlopeFactor;	///< Depth bias slope
	float _lineWidth;				///< Line thickness
	HalPolygonMode _polygonMode;	///< Polygon render mode
	bool _rasterizerDiscardEnable;	///< Disable rasterizer state

	HalRasterizerSetup()
	{
		// defaults
		_cullMode = HalCullMode::Back;
		_frontFace = HalFrontFace::CounterClockwise;
		_depthClampEnable = false;
		_depthBiasEnable = false;
		_depthBiasConstantFactor = 0;
		_depthBiasClamp = 0;
		_depthBiasSlopeFactor = 0;
		_lineWidth = 1;
		_polygonMode = HalPolygonMode::Fill;
		_rasterizerDiscardEnable = false;
	}
};

/**
* @brief Multisample state
*/
struct CAVE_INTERFACE HalMultisampleState
{
	bool _alphaToCoverageEnable;			///< Enable temporary coverage value based on the alpha value
	bool _alphaToOneEnable;					///< Enable replacing alpah output to one
	float _minSampleShading;				///< Minimum fraction value
	uint32_t* _pSampleMask;					///< Coverage bitmask
	HalSampleCount _rasterizationSamples;	///< Sample count
	bool _sampleShadingEnable;				///< Enable per sample fragment execution

	HalMultisampleState()
	{
		_alphaToCoverageEnable = false;
		_alphaToOneEnable = false;
		_minSampleShading = 1;
		_pSampleMask = nullptr;
		_rasterizationSamples = HalSampleCount::SampleCount1;
		_sampleShadingEnable = false;
	}
};

/**
* @brief Stencil operation state setup
*/
struct CAVE_INTERFACE HalStencilOpSetup
{
	HalStencilOp _failOp;		///< Operation on failing stencil test samples
	HalStencilOp _passOp;		///< Operation on passing stencil test samples
	HalStencilOp _depthFailOp;	///< Operation on failing depth test samples
	HalStencilOp _depthPassOp;	///< Operation on passing depth test samples
	HalCompareOp _compareOp;	///< Stencil compare operation
	uint32_t _compareMask;		///< Bitmaks of tested stencil values
	uint32_t _writeMask;		///< Bitmask of stencil values written to framebuffer
	uint32_t _reference;		///< Reference value used for comparisons

	HalStencilOpSetup()
	{
		_failOp = HalStencilOp::Keep;
		_passOp = HalStencilOp::Keep;
		_depthFailOp = HalStencilOp::Keep;
		_depthPassOp = HalStencilOp::Keep;
		_compareOp = HalCompareOp::AlwaysPass;
		_compareMask = 0;
		_writeMask = 0;
		_reference = 0;
	}
};

/**
* @brief Depth stencil state setup
*/
struct CAVE_INTERFACE HalDepthStencilSetup
{
	bool _depthTestEnable;			///< Enable depth testing
	bool _depthWriteEnable;			///< Enable depth writes
	HalCompareOp _depthCompareOp;	///< Depth compare operation
	bool _depthBoundsTestEnable;	///< Enable depth bounds testing
	bool _stencilTestEnable;		///< Enable stencil testing
	HalStencilOpSetup _front;		///< Front face stencil parameter setup
	HalStencilOpSetup _back;		///< Back face stencil parameter setup
	float _minDepthBounds;			///< Min depth value used in depth bound test
	float _maxDepthBounds;			///< Min depth value used in depth bound test

	HalDepthStencilSetup()
	{
		_depthTestEnable = false;
		_depthWriteEnable = false;
		_depthCompareOp = HalCompareOp::Never;
		_depthBoundsTestEnable = false;
		_stencilTestEnable = false;
		_minDepthBounds = 0;
		_maxDepthBounds = 1;
	}
};

/**
* @brief Color blending attachment data
*/
struct CAVE_INTERFACE HalColorBlendAttachment
{
	bool _blendEnable;							///< Enable blending for this attachment
	HalBlendFactor _srcColorBlendFactor;		///< Select source blend factor
	HalBlendFactor _dstColorBlendFactor;		///< Select dest blend factor
	HalBlendOp _colorBlendOp;					///< Select blend operation applied to color values
	HalBlendFactor _srcAlphaBlendFactor;		///<  Select source alpha blend factor
	HalBlendFactor _dstAlphaBlendFactor;		///< Select cdst alpha blend factor
	HalBlendOp _alphaBlendOp;					///< Select blend operation applied to alpha values
	HalColorComponentFlags _colorWriteMask;		///< Determines to which color compoent we are writing

	HalColorBlendAttachment()
	{
		_blendEnable = false;
		_srcColorBlendFactor = HalBlendFactor::One;
		_dstColorBlendFactor = HalBlendFactor::Zero;
		_colorBlendOp = HalBlendOp::Add;
		_srcAlphaBlendFactor = HalBlendFactor::One;
		_dstAlphaBlendFactor = HalBlendFactor::Zero;
		_alphaBlendOp = HalBlendOp::Add;
		_colorWriteMask = static_cast<uint32_t>(HalColorComponents::Red)
			| static_cast<uint32_t>(HalColorComponents::Green)
			| static_cast<uint32_t>(HalColorComponents::Blue)
			| static_cast<uint32_t>(HalColorComponents::Alpha);
	}
};


/**
* @brief Color blending state
*/
struct CAVE_INTERFACE HalColorBlendState
{
	bool _logicOpEnable;		///< Enable logical operations
	HalLogicOp _logicOp;		///< Select logical operation
	float _blendConstants[4];	///< RGBA constant values used for blending

	HalColorBlendState()
	{
		_logicOpEnable = false;
		_logicOp = HalLogicOp::Copy;
		_blendConstants[0] = 0;
		_blendConstants[1] = 0;
		_blendConstants[2] = 0;
		_blendConstants[3] = 0;
	}
};

/**
* @brief Structure specifying a push constant range
*/
struct CAVE_INTERFACE HalPushConstantRange
{
	HalShaderStagesFlags _shaderStagesFlags; ///< Enabled shader stages
	uint32_t _offset;	///< Range offset in bytes (must be a multiple of 4)
	uint32_t _size;		///< Range size on bytes (must be a multiple of 4)
};

/**
* @brief Renderpass attachment description
*/
struct CAVE_INTERFACE HalRenderPassAttachment
{
	HalImageFormat _format;		///< Image format
	HalSampleCount _samples;	///< Sample count
	HalAttachmentLoadOperation _loadOp;	///< Select load operation for color / depth
	HalAttachmentStoreOperation _storeOp;	///< Select store operation for color / depth
	HalAttachmentLoadOperation _loadStencilOp;	///< Select load operation for stencil
	HalAttachmentStoreOperation _storeStencilOp;	///< Select store operation for stencil
	HalImageLayout _initialLayout;	///< Layout at render pass start
	HalImageLayout _finalLayout;	///< Layout at render pass end
};

/**
* @brief Renderpass attachment reference
*/
struct CAVE_INTERFACE HalAttachmentReference
{
	uint32_t _attachment;		///< Reference index into HalRenderPassAttachment array
	HalImageLayout _layout;		///< Layout at render pass time
};

/**
* @brief Render sub-pass description
*/
struct CAVE_INTERFACE HalSubpassDescription
{
	HalPipelineBindPoints _pipelineBindPoint;				///< Pipeline bind point
	uint32_t  _inputAttachmentCount;						///< Input attachment count
	const HalAttachmentReference*    _pInputAttachments;	///< Input attachemnt array
	uint32_t _colorAttachmentCount;							///< Color attachment count
	const HalAttachmentReference* _pColorAttachments;		///< Color attachemnt array
	const HalAttachmentReference* _pResolveAttachments;		///< Resolve attachemnt array same size as _pColorAttachments
	const HalAttachmentReference* _pDepthStencilAttachment; ///< Depth attachemnt array (only one)
	uint32_t _preserveAttachmentCount;						///< Preserve attachment count
	const uint32_t* _pPreserveAttachments;					///< Preserve attachemnt array

	HalSubpassDescription()
	{
		_pipelineBindPoint = HalPipelineBindPoints::Graphics;
		_inputAttachmentCount = 0;
		_pInputAttachments = nullptr;
		_colorAttachmentCount = 0;
		_pColorAttachments = nullptr;
		_pResolveAttachments = nullptr;
		_pDepthStencilAttachment = nullptr;
		_preserveAttachmentCount = 0;
		_pPreserveAttachments = nullptr;
	}
};

/**
* @brief Render sub-pass dependency description
*/
struct CAVE_INTERFACE HalSubpassDependency
{
	uint32_t _srcSubpass;					///< Subpass index of first subpass
	uint32_t _dstSubpass;					///< Subpass index of second subpass
	HalPipelineStageFlags _srcStageMask;	///< Source stage mask
	HalPipelineStageFlags _dstStageMask;	///< Dest stage mask
	HalAccessFlags _srcAccessMask;			///< Source access mask
	HalAccessFlags _dstAccessMask;			///< Dest access mask
	HalDependencyFlags  _dependencyFlags;	///< Dependency flags

	HalSubpassDependency()
	{
		_srcSubpass = 0;
		_dstSubpass = 0;
		_srcStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::Host);
		_dstStageMask = static_cast<HalPipelineStageFlags>(HalPipelineStageBits::Host);
		_srcAccessMask = HalAccessBits::HostRead;
		_dstAccessMask = HalAccessBits::HostRead;
		_dependencyFlags = static_cast<HalDependencyFlags>(HalDependencyBits::DependencyNone);
	}
};

/**
* @brief Render pass create info
*/
struct CAVE_INTERFACE HalRenderPassInfo
{
	uint32_t _attachmentCount;						///< Render pass attachment count
	const HalRenderPassAttachment* _pAttachments;	///< Render pass attachments
	uint32_t _subpassCount;							///< Render sub-pass attachment count
	const HalSubpassDescription* _pSubpasses;		///< Render sub-pass attachments
	uint32_t _dependencyCount;						///< Render pass dependency attachment count
	const HalSubpassDependency*  _pDependencies;	///< Render pass dependencies

	HalRenderPassInfo()
	{
		_attachmentCount = 0;
		_pAttachments = nullptr;
		_subpassCount = 0;
		_pSubpasses = nullptr;
		_dependencyCount = 0;
		_pDependencies = nullptr;
	}
};

/**
* @brief Vertex binding description
*/
struct CAVE_INTERFACE HalVertexInputBindingDescription
{
	uint32_t _binding;					///< Binding point
	uint32_t _stride;					///< Vertex stride
	HalVertexInputRate	_inputRate;		///< Attribute per vertex or instance
};

/**
* @brief Vertex attribute description
*/
struct CAVE_INTERFACE HalVertexInputAttributeDescription
{
	uint32_t _location;			///< Shader binding location
	uint32_t _binding;			///< From which location to take the data (HalVertexInputBindingDescription)
	HalImageFormat _format;		///< Size and type of data
	uint32_t _offset;			///< Attribute offset relative from start in bytes
};

/**
* @brief Vertex input description
*/
struct CAVE_INTERFACE HalVertexInputStateInfo 
{
	uint32_t _vertexBindingDescriptionCount;									///< Number of vertex bindings descriptions
	const HalVertexInputBindingDescription* _pVertexBindingDescriptions;		///< Pointer to HalVertexInputBindingDescription array
	uint32_t _vertexAttributeDescriptionCount;									///< Number of of vertex attribute descriptions
	const HalVertexInputAttributeDescription* _pVertexAttributeDescriptions;	///< Pointer to HalVertexInputAttributeDescription array
};

/**
* @brief Vertex asaembly description
*/
struct CAVE_INTERFACE HalInputAssemblyInfo
{
	HalPrimitiveTopology _topology;		///< Primitive topology
	bool _primitiveRestartEnable;		///< Enable primitive restart
};

/**
* @brief Pipeline layout descriptor binding
*/
struct CAVE_INTERFACE HalDescriptorSetLayoutBinding
{
	uint32_t _binding;					///< Binding number of this entry. Relates to shader binding number	
	HalDescriptorType _descriptorType;	///< Resource type
	uint32_t _descriptorCount;			///< Number of descriptors (for arrays) otherwise set to 1
	HalShaderStagesFlags _stageFlags;	///< In which pipeline stages can access this resource
	HalSampler* _pImmutableSamplers;	///< Sampler pointer
};

/**
* @brief Structure specifying a descriptor set layout
*/
struct CAVE_INTERFACE HalDescriptorSetLayout
{
	HalDescriptorSetLayoutCreateFlags _flags;			///< Creation flags
	uint32_t _bindingCount;								///< Array count
	const HalDescriptorSetLayoutBinding* _pBindings;	///< Array of bindings
};

/**
* @brief Structure specifying the allocation parameters for descriptor sets
*/
struct CAVE_INTERFACE HalDescriptorSetAllocateInfo
{
	uint32_t _descriptorSetCount; ///< Determines the number of descriptor sets to be allocated from the pool
	const HalDescriptorSetLayout* _pSetLayouts;	///< Sn array of descriptor set layouts, with each member specifying how the corresponding descriptor set is allocated.
};

/**
* @brief Commnad Pool create info
*/
struct CAVE_INTERFACE HalCommandPoolInfo
{
	HalCommandPoolQueueFamily _queueFamily;		///< To which queue this commnad pool belongs
	HalCommandPoolUsageFlags _flags;			///< Usage flags
};

/**
* @brief Commnad buffer create info
*/
struct CAVE_INTERFACE HalCommandBufferInfo
{
	HalCommandBufferLevel _level;		///< To which queue this commnad pool belongs
	uint32_t _bufferCount;				///< Amount of command buffers to create
};

/**
* @brief Command buffer begin info
*/
struct CAVE_INTERFACE HalCommandBufferBeginInfo
{
	HalCommandBufferUsageFlags _flags;	///< Command buffer usage flags
};

/**
* @brief Device buffer create info
*/
struct CAVE_INTERFACE HalBufferInfo
{
	uint64_t _size;							///< Buffer size in bytes
	HalBufferCreateFlags _create;			///< Buffer create flags
	HalBufferUsageFlags _usage;				///< Usage flags
	HalBufferShareMode _shareMode;			///< Buffer share mode
	HalMemoryPropertyFlags _properties;		///< Memory properties
	uint32_t _queueFamilyIndexCount;		///< Index count
	const uint32_t* _queueFamilyIndices;	///< Queue family indices array
		
	HalBufferInfo()
	{
		_size = 0;
		_create = 0;
		_usage = static_cast<HalBufferUsageFlags>(HalBufferUsage::UniformBuffer);
		_shareMode = HalBufferShareMode::Exclusive;
		_properties = 0;
		_queueFamilyIndexCount = 0;
		_queueFamilyIndices = nullptr;
	}
};

/**
* @brief Device image create info
*/
struct CAVE_INTERFACE HalImageInfo
{
	HalImageFormat _format;					///< Image format
    HalImageType _type;                     ///< Image type
	uint32_t _width;						///< Image width in pixels
	uint32_t _height;						///< Image height in pixels
	uint32_t _depth;						///< Image depth
	uint32_t _slices;						///< Image array count
	uint32_t _level;						///< Image level count
	uint32_t _componentCount;				///< amount of components (1-4)
	uint32_t _componentSize;				///< size of a single component in bytes (1-4)
	HalSampleCount _sampleCount;			///< MSAA sample count
	HalMemoryPropertyFlags _properties;		///< Memory properties
    HalImageUsageFlags _usage;              ///< Texture usage Flag
	uint32_t _queueFamilyIndexCount;		///< Index count
	const uint32_t* _queueFamilyIndices;	///< Queue family indices array

	HalImageInfo()
	{
		_format = HalImageFormat::Undefined;
        _type = HalImageType::Image2D;
		_width = 0;
		_height = 0;
		_depth = 0;
		_slices = 0;
		_level = 0;
		_componentCount = 0;
		_componentSize = 0;
		_sampleCount = HalSampleCount::SampleCount1;
        _usage = static_cast<HalImageUsageFlags>(HalImageUsageFlagBits::TransferSrc);
		_properties = 0;
		_queueFamilyIndexCount = 0;
		_queueFamilyIndices = nullptr;
	}
};

/**
* @brief Hal component swizzle info
*/
struct CAVE_INTERFACE HalComponentMapping
{
    HalComponentSwizzle    _r;
    HalComponentSwizzle    _g;
    HalComponentSwizzle    _b;
    HalComponentSwizzle    _a;
};

/**
* @brief Hal sub-resource info
*/
struct CAVE_INTERFACE HalImageSubresourceRange
{
    HalImageAspectFlags _aspectMask;
    uint32_t _baseMipLevel;
    uint32_t _levelCount;
    uint32_t _baseArrayLayer;
    uint32_t _layerCount;
};

/**
* @brief Hal sub-resource info
*/
struct CAVE_INTERFACE HalImageSubresourceLayers
{
    HalImageAspectFlags   _aspectMask;
    uint32_t              _mipLevel;
    uint32_t              _baseArrayLayer;
    uint32_t              _layerCount;
};

/**
* @brief Hal image view create info
*/
struct CAVE_INTERFACE HalImageViewInfo
{
    HalImageFormat _format;             ///< Image format
    HalImageViewType _type;             ///< View type
    HalComponentMapping _components;    ///< Component swizzle
    HalImageSubresourceRange _subresourceRange; ///< View sub-resource info

    HalImageViewInfo()
    {
        _format = HalImageFormat::Undefined;
        _type = HalImageViewType::Image2D;
        _components._r = HalComponentSwizzle::Identity;
        _components._g = HalComponentSwizzle::Identity;
        _components._b = HalComponentSwizzle::Identity;
        _components._a = HalComponentSwizzle::Identity;
        _subresourceRange._aspectMask = static_cast<HalImageAspectFlags>(HalImageAspectFlagBits::Color);
        _subresourceRange._baseMipLevel = 0;
        _subresourceRange._levelCount = 0;
        _subresourceRange._baseArrayLayer = 0;
        _subresourceRange._layerCount = 0;
    }
};

/**
* @brief Hal sampler create info
*/
struct CAVE_INTERFACE HalSamplerCreateInfo
{
    HalSamplerCreateFlags    _flags;
    HalFilter                _magFilter;
    HalFilter                _minFilter;
    HalSamplerMipmapMode     _mipmapMode;
    HalSamplerAddressMode    _addressModeU;
    HalSamplerAddressMode    _addressModeV;
    HalSamplerAddressMode    _addressModeW;
    float                    _mipLodBias;
    bool                     _anisotropyEnable;
    float                    _maxAnisotropy;
    bool                     _compareEnable;
    HalCompareOp             _compareOp;
    float                    _minLod;
    float                    _maxLod;
    HalBorderColor           _borderColor;
    bool                     _unnormalizedCoordinates;

    HalSamplerCreateInfo()
    {
        _flags = 0;
        _magFilter = HalFilter::Nearest;
        _minFilter = HalFilter::Nearest;
        _mipmapMode = HalSamplerMipmapMode::Nearest;
        _addressModeU = HalSamplerAddressMode::Repeat;
        _addressModeV = HalSamplerAddressMode::Repeat;
        _addressModeW = HalSamplerAddressMode::Repeat;
        _mipLodBias = 0;
        _anisotropyEnable = false;
        _maxAnisotropy = 0;
        _compareEnable = false;
        _compareOp = HalCompareOp::AlwaysPass;
        _minLod = 0;
        _maxLod = 0;
        _borderColor = HalBorderColor::FloatOpaqueBlack;
        _unnormalizedCoordinates = false;
    }
};

/**
* @brief Structure specifying descriptor pool size
*/
struct CAVE_INTERFACE HalDescriptorPoolSize
{
	HalDescriptorType	_type;				///< Must be a valid HalDescriptorType value
	uint32_t            _descriptorCount;	///< Must be greater than 0
};

/**
* @brief Structure specifying parameters of a newly created descriptor pool
*/
struct CAVE_INTERFACE HalDescriptorPoolInfo 
{
	HalDescriptorPoolCreateFlags	_flags;			///< Bitmask of HalDescriptorPoolCreateFlagBits specifying certain supported operations on the pool.
	uint32_t						_maxSets;		///< The maximum number of descriptor sets that can be allocated from the pool.
	uint32_t						_poolSizeCount;	///< The number of elements in pPoolSizes.
	const HalDescriptorPoolSize*	_pPoolSizes;	///< Pointer to an array of HalDescriptorPoolSize structures, each containing a descriptor type and number of descriptors of that type to be allocated in the pool.
};

/**
* @brief Framebuffer color clear value
*/
union CAVE_INTERFACE HalClearColorValue
{
	float       float32[4];		///< float clear values
	int32_t     int32[4];		///< int clear values
	uint32_t    uint32[4];		///< uint clear values
};

/**
* @brief Framebuffer depth stencil clear value
*/
struct CAVE_INTERFACE HalClearDepthStencilValue 
{
	float       _depth;			///< Depth clear value
	uint32_t    _stencil;		///< Stencil clear
};	

/**
* @brief Union of clear values
*/
union CAVE_INTERFACE HalClearValue
{
	HalClearColorValue           _color;			///< Color clear
	HalClearDepthStencilValue    _depthStencil;		///< Depth stencil clear
};

/**
* @brief 2D extend
*/
struct CAVE_INTERFACE HalExtent2D
{
	uint32_t    _width;		///< Width
	uint32_t    _height;	///< Height
};

/**
* @brief 2D rect spec
*/
struct CAVE_INTERFACE HalRect2D
{
	int32_t _x;			///< X offset
	int32_t _y;			///< Y offset
	uint32_t _width;	///< Extend in x
	uint32_t _height;	///< Extend in y
};

/**
* @brief 3D extend
*/
struct CAVE_INTERFACE HalExtent3D
{
    uint32_t    _width;		///< Width
    uint32_t    _height;	///< Height
    uint32_t    _depth;	///< Depth
};

/**
* @brief 3D offset
*/
struct CAVE_INTERFACE HalOffset3D
{
    int32_t    _x; ///< x offset
    int32_t    _y; ///< y offset
    int32_t    _z; ///< z offset
};

/**
* @brief Imagee copy information struct
*/
struct CAVE_INTERFACE HalImageCopy
{
    HalImageSubresourceLayers _srcLayers;   ///< setup to use in copy from the source image
    HalOffset3D _srcOffset;                 ///< offset in texel in the source image
    HalImageSubresourceLayers _dstLayers;   ///< setup to use in copy from the dest image
    HalOffset3D _dstOffset;                 ///< offset in texel in the dest image
    HalExtent3D _extent;                    ///< size in texel to copy of the image
};

}

/** @}*/

