#include "SKParameter.h"
using namespace SKEngine2;
unsigned int SKGlobalParameter::ms_GpuSkinBoneNum = 50;
bool SKGlobalParameter::ms_bDrawSkelecton = true;
bool SKGlobalParameter::ms_bIsCacheName = false;
bool SKGlobalParameter::ms_bIsCacheShader = true;
bool SKGlobalParameter::ms_bIsCacheTexture = false;
bool SKGlobalParameter::ms_bIsCacheVertexFormat = false;
bool SKGlobalParameter::ms_bIsCacheFont = false;
bool SKGlobalParameter::ms_bIsCacheSkelectionMesh = false;
bool SKGlobalParameter::ms_bIsCacheStaticMesh = false;
bool SKGlobalParameter::ms_bIsCacheAnimSet = false;
bool SKGlobalParameter::ms_bIsCacheMaterial = false;
SKGlobalParameter::SKGlobalParameter()
{

}
SKGlobalParameter::~SKGlobalParameter()
{


}