#include "SKShaderCore/SKShaderCore.h"

namespace Skylark
{
	FSKHash64 SKComputeShaderKey(const FSKShaderCompileRequest& Request)
	{
		FSKHash64 H;
		H.AddBytes(&Request.Language, sizeof(Request.Language));
		H.AddBytes(&Request.Stage, sizeof(Request.Stage));
		H.AddString(Request.EntryPoint);
		H.AddString(Request.Profile);
		H.AddString(Request.DebugName);

		if (!Request.Source.FilePath.empty())
		{
			H.AddString("FILE:");
			H.AddString(Request.Source.FilePath);
		}
		else
		{
			H.AddString("CODE:");
			H.AddString(Request.Source.Code);
		}

		for (const auto& D : Request.Defines)
		{
			H.AddString(D.Name);
			H.AddString("=");
			H.AddString(D.Value);
			H.AddString(";");
		}

		for (const auto& P : Request.IncludeDirs)
		{
			H.AddString(P);
			H.AddString(";");
		}

		return H;
	}
}
