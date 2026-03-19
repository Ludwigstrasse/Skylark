#ifndef SKSORTLIGHT_H
#define SKSORTLIGHT_H
#include "SKPointLight.h"
#include "SKDirectionLight.h"
namespace SKEngine2
{
	class LightPriority
	{
	public:
		LightPriority(){}
		~LightPriority(){}
		bool operator()(SKLight *pLight1,SKLight * pLight2)
		{
			if (pLight1->GetLightType() == pLight2->GetLightType())
			{
				SKLocalLight * pLocalLight1 = DynamicCast<SKLocalLight>(pLight1);
				if (pLocalLight1)
				{
					SKLocalLight * pLocalLight2 = DynamicCast<SKLocalLight>(pLight2);

					if (pLocalLight1->GetCastShadow() == pLocalLight2->GetCastShadow())
					{
						if (pLocalLight1->GetCastShadow())
						{
							if (pLocalLight1->GetLightType() == SKLight::LT_POINT)
							{
								if (((SKPointLight *)pLocalLight1)->GetShadowType() > ((SKPointLight *)pLocalLight2)->GetShadowType())
								{
									return 0;
								}
								else
								{
									if (((SKPointLight *)pLocalLight1)->GetShadowType() == ((SKPointLight *)pLocalLight2)->GetShadowType())
									{
										if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
										{
											return 0;
										}
										else
										{
											return 1;
										}
									}
									else
									{
										return 1;
									}
									
								}
							}
							else if (pLocalLight1->GetLightType() == SKLight::LT_DIRECTION)
							{
								if (((SKDirectionLight *)pLocalLight1)->GetShadowType() > ((SKDirectionLight *)pLocalLight2)->GetShadowType())
								{
									return 0;
								}
								else
								{
									if (((SKDirectionLight *)pLocalLight1)->GetShadowType() == ((SKDirectionLight *)pLocalLight2)->GetShadowType())
									{
										if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
										{
											return 0;
										}
										else
										{
											return 1;
										}
									}
									else
									{
										return 1;
									}
									
								}
							}
							else
							{
								if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
								{
									return 0;
								}
								else
								{
									return 1;
								}
							}
							
						}
						else
						{
							if (pLocalLight1->HaveLightFun() > pLocalLight2->HaveLightFun())
							{
								return 0;
							}
							else
							{
								return 1;
							}
						}
					}
					else
					{
						if(pLocalLight1->GetCastShadow() < pLocalLight2->GetCastShadow())
						{
							return 1;
						}
						else
						{
							return 0;
						}
					}
				}
				else
				{
					return 1;
				}
			}
			else
			{
				if(pLight1->GetLightType() < pLight2->GetLightType())
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}


	};
	

}

#endif