#pragma once
#include "SKCore/SKCoreMinimal.h"
#include "SKCore/SKObjectId.h"

namespace Skylark
{
	/**
	 * SKField (V9)
	 * - CAE field data model (scalar/vector/tensor) bound to engine geometry.
	 * - Alignments:
	 *   - UE: "data interfaces" feeding render passes.
	 *   - VTK/ParaView: field arrays on points/cells with domains.
	 *   - OCCT: can also bind to TopoId sub-entities when needed.
	 */

	enum class ESKFieldDomain : uint8
	{
		Unknown = 0,
		Vertex,   // per-vertex in tessellation
		Node,     // CAE node
		Element,  // CAE element/cell
		Face,     // CAD face
		Edge,     // CAD edge
	};

	enum class ESKFieldValueType : uint8
	{
		Scalar = 0,
		Vector3,
		Tensor3x3,
	};

	struct FSKFieldKey
	{
		FSKString Name;
		ESKFieldDomain Domain = ESKFieldDomain::Vertex;
		ESKFieldValueType Type = ESKFieldValueType::Scalar;
	};

	struct FSKFieldStats
	{
		float MinValue = 0.0f;
		float MaxValue = 1.0f;
		bool bHasRange = false;
	};

	struct FSKScalarField
	{
		FSKFieldKey Key{};
		FSKFieldStats Stats{};
		TArray<float> Values; // per-vertex/node/element
	};

	struct FSKVectorField
	{
		FSKFieldKey Key{};
		FSKFieldStats Stats{};
		TArray<FSKVector3f> Values;
	};

	/**
	 * Field binding to engine geometry (tessellation mesh)
	 * - For now: per-GeometryKey arrays, aligned to a flattened vertex index.
	 * - Future: per-section, per-element, sparse bindings, and TopoId bindings.
	 */
	struct FSKGeometryFieldBinding
	{
		uint64 GeometryKey = 0;

		// Flattening rule:
		//  - Concatenate each MeshSection's vertices in order.
		//  - Values.size() must match TotalVertexCount for Vertex domain.
		uint32 TotalVertexCount = 0;
	};

	class FSKFieldSet final
	{
	public:
		void Clear();

		// Scalars
		void RegisterScalar(const FSKFieldKey& Key, TArray<float> Values, FSKFieldStats Stats = {});
		const FSKScalarField* FindScalar(const FSKStringView Name) const;

		// Vectors (deform)
		void RegisterVector(const FSKFieldKey& Key, TArray<FSKVector3f> Values, FSKFieldStats Stats = {});
		const FSKVectorField* FindVector(const FSKStringView Name) const;

	private:
		TArray<FSKScalarField> Scalars;
		TArray<FSKVectorField> Vectors;
	};

	class FSKFieldRegistry final
	{
	public:
		void Clear();

		// Bind a fieldset to a geometry key.
		FSKFieldSet& GetOrCreateFieldSet(uint64 GeometryKey);

		const FSKFieldSet* FindFieldSet(uint64 GeometryKey) const;

	private:
		struct FEntry
		{
			uint64 GeometryKey = 0;
			FSKFieldSet Fields;
		};

		TArray<FEntry> Entries;
	};
}
