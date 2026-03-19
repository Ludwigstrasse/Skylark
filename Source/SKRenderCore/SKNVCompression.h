#ifndef SKNVCOMPRESSION_H
#define SKNVCOMPRESSION_H
#include "SKnvtt.h"
#include "SKSystem.h"
#include "SKMemManager.h"
#include "SKGraphic.h"
#include "SKArray.h"
namespace SKEngine2
{
	//this code is copyed from unreal 


	// Structures required by nv Texture Tools 2 library.
	struct SKNVOutputHandler : public nvtt::OutputHandler
	{
	public:
		SKNVOutputHandler()
		{
		}
		~SKNVOutputHandler()
		{
		}
		void ReserveMemory(unsigned int PreAllocateSize )
		{
			//m_CompressDate.SetBufferNum(PreAllocateSize);
		}

		virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel)
		{}

		virtual bool writeData(const void * data, int size)
		{
			SKArray<unsigned char> Temp;
			Temp.SetBufferNum(size);	
			SKMemcpy(Temp.GetBuffer(),data,size);
			m_CompressDate.AddElement(Temp,0,Temp.GetNum());
			return true;
		}

		SKArray<unsigned char> m_CompressDate;
	};

	struct SKNVErrorHandler : public nvtt::ErrorHandler
	{
	public:
		SKNVErrorHandler() : 
		bSuccess(TRUE)
		{

		}

		virtual void error(nvtt::Error e)
		{

			SKOutPutDebugString(nvtt::errorString(e));	
			bSuccess = FALSE;
		}

		bool bSuccess;
	};


	/** Helper struct that encapsulates everything needed for the NVidia DXT compression tool. */
	struct SKGRAPHIC_API SKNVCompression
	{
		enum //NV FORMAT
		{
			NV_RGB,
			NV_ARGB = NV_RGB,
			NV_DXT3,
			NV_DXT5,
			NV_MAX
		};
		/**
		* Initialize the settings and pre-allocate memory for the compressed result.
		*
		* @param	SourceData				Source texture data to DXT compress, in BGRA 8bit per channel unsigned format.
		* @param	PixelFormat				Texture format
		* @param	SizeX					Number of texels along the X-axis
		* @param	SizeY					Number of texels along the Y-axis
		* @param	SRGB					Whether the texture is in SRGB space
		* @param	bIsNormalMap			Whether the texture is a normal map
		*/
		void	Setup( void* SourceData, unsigned int PixelFormat, unsigned int SizeX, unsigned int SizeY, bool SRGB, bool bIsNormalMap);

		/** Performs the DXT compression. */
		bool	Process();

		/** Handles the output from the NVidia DXT-compression tool. Contains the resulting data buffer. */
		SKNVOutputHandler			OutputHandler;
		/** Handles any errors from the NVidia DXT-compression tool. */
		SKNVErrorHandler				ErrorHandler;
		/** NVidia object that contains options that describes the input data. */
		nvtt::InputOptions			InputOptions;
		/** NVidia object that contains compression options that describes the compression work. */
		nvtt::CompressionOptions	CompressionOptions;
		/** NVidia object that contains output options. */
		nvtt::OutputOptions			OutputOptions;
		/** NVidia object that performs that DXT compression. */
		nvtt::Compressor			Compressor;
	};

}
#endif