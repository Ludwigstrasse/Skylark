#ifndef SKVERTEXFORMAT_H
#define SKVERTEXFORMAT_H
#include "SKObject.h"
#include "SKBind.h"
namespace SKEngine2
{
	class SKStream;
	class SKGRAPHIC_API SKVertexFormat : public SKBind
	{
		//PRIORITY
		
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL
	public:

		SKVertexFormat();
		virtual ~SKVertexFormat();
		
		enum
		{
			VF_POSITION,
			VF_TEXCOORD,
			VF_NORMAL,
			VF_TANGENT,
			VF_BINORMAL,
			VF_PSIZE,
			VF_COLOR,
			VF_FOG,
			VF_DEPTH,
			VF_BLENDWEIGHT,
			VF_BLENDINDICES,
			VF_MAX
		};
		struct VERTEXFORMAT_TYPE
		{
		public:
			VERTEXFORMAT_TYPE()
			{
				OffSet = 0;
				DateType = 0;
				Semantics = 0;
				SemanticsIndex = 0;
			}
			~VERTEXFORMAT_TYPE()
			{

			}
			UINT OffSet;
			UINT DateType;
			UINT Semantics;
			UINT SemanticsIndex;
		};
	public:
		friend class SKRenderer;
		friend class SKDX9Renderer;

		friend class SKVShaderKey;
		friend class SKPShaderKey;
		friend class SKResourceManager;
		friend class SKShaderKey;

	protected:
		virtual bool OnLoadResource(SKResourceIdentifier *&pID);		
		virtual bool OnReleaseResource(SKResourceIdentifier *pID);
		unsigned int m_uiVertexFormatCode;
	public:
		
		SKArray<SKVertexFormat::VERTEXFORMAT_TYPE> m_FormatArray;
		FORCEINLINE unsigned int GetVertexFormatCode()const
		{
			return m_uiVertexFormatCode;
		}

	};
	DECLARE_Ptr(SKVertexFormat);
	SKTYPE_MARCO(SKVertexFormat);

}
#endif