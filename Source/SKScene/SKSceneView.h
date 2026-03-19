#pragma once
#include "SKCore/SKCoreMinimal.h"

namespace Skylark
{
	/**
	 * RenderProxy: output of SceneGraph culling.
	 * - OSG: CullVisitor produces a drawable list.
	 * - UE: SceneRenderer builds FMeshDrawCommand / Visible primitives.
	 * - OCCT: AIS presentations are collected and drawn.
	 *
	 * V5: minimal proxy: transform + hit-proxy id + opaque geometry key.
	 */
	struct FSKRenderProxy
	{
		FSKMatrix4f LocalToWorld = FSKMatrix4f::Identity();
		uint32 HitProxyId = 0;
		uint64 GeometryKey = 0; // opaque (mesh handle / topo handle / instancing key)
		uint64 MaterialKey = 0; // opaque (StateSet/Material)
	};

	struct FSKSceneView
	{
		TArray<FSKRenderProxy> VisibleProxies;
	};
}
