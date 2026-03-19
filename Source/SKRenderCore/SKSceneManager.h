#ifndef SKSCENEMANAGER_H
#define SKSCENEMANAGER_H
#include "SKSceneRender.h"
#include "SKViewFamily.h"
namespace SKEngine2
{
	class SKGRAPHIC_API SKSceneManager : public SKMemObject
	{
	public:
		SKSceneManager();
		~SKSceneManager();
		
		void Update(double dAppTime);
		void Draw(double dAppTime);
		static SKSceneManager * ms_pSceneManager;
		FORCEINLINE void AddViewFamily(SKViewFamily* pViewFamily);
		FORCEINLINE SKViewFamily * GetViewFamily(unsigned int i)const;
		FORCEINLINE SKViewFamily * GetViewFamily(const SKString & ViewFamilyName)const;
		FORCEINLINE void ClearAllViewFamily();
		FORCEINLINE void DeleteViewFamily(unsigned int i);
		FORCEINLINE void DeleteViewFamily(SKViewFamily * pViewFamily);
		FORCEINLINE unsigned int GetViewFamilyNum()const;
		
		FORCEINLINE void AddScene(SKScene* pScene);
		FORCEINLINE SKScene * GetScene(unsigned int i)const;
		FORCEINLINE void ClearAllScene();
		FORCEINLINE void DeleteScene(unsigned int i);
		FORCEINLINE void DeleteScene(SKScene * pScene);
		FORCEINLINE unsigned int GetSceneNum()const;
	protected:

		SKArray<SKViewFamily*> m_pViewFamily;
		SKArray<SKScene *> m_pScene;
		
	
	};
	
	FORCEINLINE void SKSceneManager::AddScene(SKScene * pSecen)
	{
		if(pSecen)
		{
			for (unsigned int i = 0 ; i < m_pScene.GetNum() ;i++)
			{
				if (m_pScene[i] == pSecen)
				{
					return ;
				}
			}
			m_pScene.AddElement(pSecen);
		}
	}
	FORCEINLINE SKScene * SKSceneManager::GetScene(unsigned int i)const
	{
		if(i >= m_pScene.GetNum())
			return NULL;
		return m_pScene[i];
	}
	FORCEINLINE void SKSceneManager::ClearAllScene()
	{
		m_pScene.Clear();
	}
	FORCEINLINE void SKSceneManager::DeleteScene(unsigned int i)
	{	
		if(i >= m_pScene.GetNum())
			return ;
		m_pScene.Erase(i);

	}
	FORCEINLINE void SKSceneManager::DeleteScene(SKScene * pScene)
	{
		if (!pScene)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pScene.GetNum() ; i++)
		{
			if(pScene == m_pScene[i])
			{
				m_pScene.Erase(i);
			}
		}
	}

	FORCEINLINE unsigned int SKSceneManager::GetSceneNum()const
	{
		return m_pScene.GetNum();
	}
	FORCEINLINE void SKSceneManager::AddViewFamily(SKViewFamily * pViewFamily)
	{
		if(pViewFamily)
		{
			for (unsigned int i = 0 ; i < m_pViewFamily.GetNum() ;i++)
			{
				if (m_pViewFamily[i] == pViewFamily)
				{
					return ;
				}
			}
			m_pViewFamily.AddElement(pViewFamily);
		}
	}
	FORCEINLINE SKViewFamily * SKSceneManager::GetViewFamily(unsigned int i)const
	{
		if(i >= m_pViewFamily.GetNum())
			return NULL;
		return m_pViewFamily[i];
	}
	FORCEINLINE void SKSceneManager::ClearAllViewFamily()
	{
		m_pViewFamily.Clear();
	}
	FORCEINLINE void SKSceneManager::DeleteViewFamily(unsigned int i)
	{	
		if(i >= m_pViewFamily.GetNum())
			return ;
		m_pViewFamily.Erase(i);

	}
	FORCEINLINE void SKSceneManager::DeleteViewFamily(SKViewFamily * pViewFamily)
	{
		if (!pViewFamily)
		{
			return ;
		}
		for (unsigned int i= 0 ; i <m_pViewFamily.GetNum() ; i++)
		{
			if(pViewFamily == m_pViewFamily[i])
			{
				m_pViewFamily.Erase(i);
			}
		}
	}

	FORCEINLINE unsigned int SKSceneManager::GetViewFamilyNum()const
	{
		return m_pViewFamily.GetNum();
	}

}
#endif