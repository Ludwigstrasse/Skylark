#ifndef  SKSHADERFUNCTION_H
#define  SKSHADERFUNCTION_H
#include "SKObject.h"
#include "SKPutNode.h"
#include "SKName.h"
#include "SKBitArray.h"
namespace SKEngine2
{
	class SKMaterial;
	class SKStream;
	class SKShaderMainFunction;
	class SKGRAPHIC_API SKShaderFunction : public SKObject
	{
		//RTTI
		DECLARE_RTTI;
		DECLARE_INITIAL_NO_CLASS_FACTORY;
	public:
		virtual ~SKShaderFunction() = 0;
	protected:
		SKUsedName m_ShowName;
		SKShaderFunction(const SKUsedName & ShowName,SKMaterial * pMaterial);
		SKShaderFunction();
		SKArray<SKInputNode *> m_pInput;
		SKArray<SKOutputNode *> m_pOutput;
		SKMaterial * m_pOwner;
		bool m_bIsVisited;
		FORCEINLINE void SetOwner(SKMaterial * pOwner)
		{
			SKMAC_ASSERT(pOwner);
			m_pOwner = pOwner;
		}

	public:
		friend class SKMaterial;
		virtual bool IsHaveOutPut()
		{
			if (m_pOutput.GetNum() > 0)
			{
				return m_pOutput[0]->IsConnection();
			}
			return false;
		}
		virtual void ResetInShaderName() = 0;
		virtual bool GetInputValueString(SKString &OutString)const;
		virtual bool GetOutPutValueString(SKString &OutString)const;
		virtual bool GetFuntionString(SKString &OutString)const = 0;
		bool GetShaderTreeString(SKString &OutString);
		bool ClearShaderTreeStringFlag();
		

		SKInputNode * GetInputNode(unsigned int uiNodeID)const;
		SKInputNode * GetInputNode(const SKString & NodeName)const;
		FORCEINLINE unsigned int GetInputNodeNum()const
		{
			return m_pInput.GetNum();
		}
		SKOutputNode * GetOutputNode(unsigned int uiNodeID)const;
		SKOutputNode * GetOutputNode(const SKString & NodeName)const;
		FORCEINLINE unsigned int GetOutputNodeNum()const
		{
			return m_pOutput.GetNum();
		}
		FORCEINLINE const SKUsedName & GetShowName()const
		{
			return m_ShowName;
		}

		SKString GetValueEqualString(const SKOutputNode * pOutPutNode,const SKInputNode * pInputNode)const;


		virtual bool IsValidNodeToThis(SKShaderFunction * pShaderFunction);

		virtual bool CheckChildNodeValidToThis(SKArray<SKShaderFunction *> & NoValidShaderFunctionArray);

		virtual bool GetAllChildNode(SKArray<SKShaderFunction *> & ChildNodeArray);

		virtual bool CheckChildNodeValidAll(SKMap<SKShaderFunction *,SKArray<SKShaderFunction *>> & NoValidMap);

		virtual bool HaveThisChild(SKShaderFunction * pShaderFunction);

		virtual void GetNoLightFunctionParentNode(SKArray<SKShaderFunction *> & ChildNodeArray);

		virtual bool ResetValueType();


	};
	DECLARE_Ptr(SKShaderFunction);
	SKTYPE_MARCO(SKShaderFunction);
}
#endif