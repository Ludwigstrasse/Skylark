#ifndef SKIMAGE_H
#define SKIMAGE_H
#include "SKSystem.h"
#include "SKMemManager.h"
#include "SKFile.h"
namespace SKEngine2
{
	class SKSYSTEM_API SKImage
	{
	public:
		enum // Image Format
		{
			IF_BMP,
			IF_TGA,
			IF_MAX
		};
		static TCHAR ms_ImageFormat[IF_MAX][10];
		SKImage();
		virtual ~SKImage() = 0;
		virtual bool Load(const TCHAR *pFileName) = 0;
		virtual bool LoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize) = 0;
		virtual const unsigned char *GetPixel(unsigned int x, unsigned int y)const = 0;
		FORCEINLINE unsigned int GetBPP()const
		{
			return m_uiBPP;
		}
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{
			return m_uiHeight;
		}
		FORCEINLINE const unsigned char *GetImg()const
		{
			return m_pImageDate;
		}
		FORCEINLINE unsigned int GetImgDateSize()const
		{
			return m_uiDataSize;
		}
		FORCEINLINE unsigned char* GetPalette()const   // Return a pointer to VGA palette
		{
			return m_pPalette;
		}
	protected:
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiBPP;
		unsigned char *m_pImageDate;
		unsigned int m_uiDataSize;
		unsigned char *m_pData;
		unsigned char *m_pPalette;
	};
	class SKSYSTEM_API SKBMPImage : public SKImage
	{
	public:
		SKBMPImage();
		~SKBMPImage();
		virtual bool Load(const TCHAR* pFilename);
		virtual bool LoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize);
		virtual const unsigned char *GetPixel(unsigned int x, unsigned int y)const;
		

	private:
		
		unsigned int m_uiEnc;
		
		unsigned int m_uiPlanes;
		unsigned int uiImgOffset;
		
		

		// Internal workers
		bool GetFile(const TCHAR* pFilename);
		bool ReadBmpHeader();
		bool LoadBmpRaw();
		bool LoadBmpRLE8();
		bool LoadBmpPalette();
		void FlipImg(); // Inverts image data, BMP is stored in reverse scanline order
		
	};
	class SKSYSTEM_API SKTGAImage : public SKImage
	{
	public:
		SKTGAImage();
		~SKTGAImage();
		virtual bool Load(const TCHAR* pFilename);
		virtual bool LoadFromBuffer(unsigned char * pBuffer,unsigned int uiSize);
		virtual const unsigned char *GetPixel(unsigned int x, unsigned int y)const;
	private:

		unsigned char m_cEnc;
		

		// Internal workers
		bool ReadHeader();
		bool LoadRawData();
		bool LoadTgaRLEData();
		bool LoadTgaPalette();
		void BGRtoRGB();
		void FlipImg();
	};
	
	class SKSYSTEM_API SKDDSImage : public SKImage
	{
	public:
		SKDDSImage()
		{

		}
		~SKDDSImage()
		{

		}
	};
}
#endif