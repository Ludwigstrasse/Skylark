#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCAD/SKCadMesh.h"

namespace Skylark
{
	struct FSKCadTessellationParams
	{
		float ChordalDeviation = 0.2f;
		float AngularDeviationDeg = 12.0f;
		bool bComputeNormals = true;
		bool bComputeUVs = false;
		bool bGenerateEdgeMesh = true;  // CAD 常用：边线/轮廓线
	};

	class ISKCadModel
	{
	public:
		virtual ~ISKCadModel() = default;
		virtual const FSKString& GetSourcePath() const = 0;
		virtual const FSKMeshData* GetDisplayMesh() const = 0;
	};

	class ISKCadImporter
	{
	public:
		virtual ~ISKCadImporter() = default;
		virtual bool CanImport(const char* FilePathUtf8) const = 0;
		virtual TSharedPtr<ISKCadModel> Import(const char* FilePathUtf8, const FSKCadTessellationParams& Params) = 0;
	};

	/**
	 * OCCT Bridge（未来落地）：
	 * - 提供 TopoDS_Shape -> Tessellation 的高性能实现
	 * - 提供“拓扑命名/选择”映射：TopoDS_* <-> FSKTopoId
	 */
	class ISKCadKernelBridge
	{
	public:
		virtual ~ISKCadKernelBridge() = default;
		virtual bool LoadCadFile(const char* FilePathUtf8) = 0;
		virtual bool Tessellate(const FSKCadTessellationParams& Params, FSKMeshData& OutMesh) = 0;
	};
}
