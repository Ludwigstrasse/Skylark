#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKLWC/SKLWC.h"

namespace Skylark
{
	class FSKGeometryRegistry;
	class FSKFieldRegistry;
	class FSKAnnotationManager;

	/**
	 * RenderProxy: output of SceneGraph culling.
	 * - OSG: CullVisitor produces a drawable list.
	 * - UE: SceneRenderer builds visible primitives.
	 * - OCCT: AIS presentations are collected and drawn.
	 */
	struct FSKRenderProxy
	{
		FSKMatrix4f LocalToWorld = FSKMatrix4f::Identity();
		uint32 HitProxyId = 0;
		uint64 GeometryKey = 0; // opaque (mesh handle / topo handle)
		uint64 MaterialKey = 0; // opaque (StateSet/Material)
		int32 SectionIndex = -1; // -1 means all sections; otherwise draw only this mesh section
	};

	/**
	 * InstanceBatch: culling result for instanced geometry.
	 * - UE: instance data ends up in per-instance buffers and a single draw call.
	 * - OSG: can be implemented via instanced drawables.
	 */
	struct FSKInstanceBatch
	{
		uint64 GeometryKey = 0;
		uint64 MaterialKey = 0;
		int32 SectionIndex = -1; // -1 means all sections; otherwise draw only this mesh section
		TArray<FSKMatrix4f> InstanceLocalToWorld;
		TArray<uint32> InstanceHitProxyId;
	};

	struct FSKSceneView
	{
		// Non-owning pointer to geometry registry for renderer-side access.
		const FSKGeometryRegistry* GeometryRegistry = nullptr;
		const FSKFieldRegistry* FieldRegistry = nullptr;
		const FSKAnnotationManager* AnnotationManager = nullptr;

		// LWC (V11): camera-relative origin used for converting double transforms to float for GPU.
		FSKVector3d ViewOrigin = FSKVector3d(0.0, 0.0, 0.0);

		// Visible non-instanced proxies
		TArray<FSKRenderProxy> VisibleProxies;

		// Visible instance batches (grouped)
		TArray<FSKInstanceBatch> VisibleInstances;
	};
}
