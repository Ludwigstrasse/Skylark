#include "SKGeometry/SKMeshBuilder.h"
#include "SKCore/SKAssert.h"

#include <unordered_set>
#include <unordered_map>
#include <cmath>

namespace Skylark
{
	static inline uint64 SKPackEdge(uint32 A, uint32 B)
	{
		const uint32 MinI = (A < B) ? A : B;
		const uint32 MaxI = (A < B) ? B : A;
		return (static_cast<uint64>(MinI) << 32) | static_cast<uint64>(MaxI);
	}

	static inline FSKVector3f SKSub(const FSKVector3f& A, const FSKVector3f& B)
	{
		return FSKVector3f(A.X - B.X, A.Y - B.Y, A.Z - B.Z);
	}

	static inline FSKVector3f SKCross(const FSKVector3f& A, const FSKVector3f& B)
	{
		return FSKVector3f(
			A.Y * B.Z - A.Z * B.Y,
			A.Z * B.X - A.X * B.Z,
			A.X * B.Y - A.Y * B.X);
	}

	static inline float SKDot(const FSKVector3f& A, const FSKVector3f& B)
	{
		return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
	}

	static inline float SKLen(const FSKVector3f& A)
	{
		return std::sqrt(SKDot(A, A));
	}

	static inline FSKVector3f SKNormalize(const FSKVector3f& A)
	{
		const float L = SKLen(A);
		if (L <= 1e-8f)
		{
			return FSKVector3f(0, 0, 1);
		}
		return FSKVector3f(A.X / L, A.Y / L, A.Z / L);
	}

	FSKAabb FSKMeshBuilder::ComputeBounds(const FSKMeshSection& Section)
	{
		FSKAabb B;
		for (const auto& V : Section.Vertices)
		{
			B.Encapsulate(V.Position);
		}
		return B;
	}

	FSKAabb FSKMeshBuilder::ComputeBounds(const FSKMeshData& Mesh)
	{
		FSKAabb B;
		for (const auto& S : Mesh.Sections)
		{
			for (const auto& V : S.Vertices)
			{
				B.Encapsulate(V.Position);
			}
		}
		return B;
	}

	FSKWireframeData FSKMeshBuilder::BuildWireframe(const FSKMeshSection& Section)
	{
		FSKWireframeData W;
		const auto& Idx = Section.Indices;
		if (Idx.size() < 3)
		{
			return W;
		}

		std::unordered_set<uint64> EdgeSet;
		EdgeSet.reserve(Idx.size());

		auto AddEdge = [&](uint32 I0, uint32 I1)
		{
			if (I0 == I1)
			{
				return;
			}
			const uint64 Key = SKPackEdge(I0, I1);
			if (EdgeSet.insert(Key).second)
			{
				W.LineIndices.push_back(I0);
				W.LineIndices.push_back(I1);
			}
		};

		// triangle list
		SK_CHECK((Idx.size() % 3) == 0);
		for (SIZE_T i = 0; i + 2 < Idx.size(); i += 3)
		{
			const uint32 A = Idx[i + 0];
			const uint32 B = Idx[i + 1];
			const uint32 C = Idx[i + 2];
			AddEdge(A, B);
			AddEdge(B, C);
			AddEdge(C, A);
		}

		return W;
	}

	static FSKEdgeIndexLists SKBuildEdgeCategories(const FSKMeshSection& Section, float CreaseAngleDeg)
	{
		FSKEdgeIndexLists Out;
		const auto& Idx = Section.Indices;
		if (Idx.size() < 3)
		{
			return Out;
		}

		struct FEdgeInfo
		{
			uint32 Count = 0;
			FSKVector3f N0{0,0,1};
			FSKVector3f N1{0,0,1};
			bool bHasN0 = false;
			bool bHasN1 = false;
		};

		std::unordered_map<uint64, FEdgeInfo> Edges;
		Edges.reserve(Idx.size());

		const float Pi = 3.14159265358979323846f;
		const float CosThresh = std::cos((CreaseAngleDeg * Pi) / 180.0f);

		SK_CHECK((Idx.size() % 3) == 0);
		for (SIZE_T i = 0; i + 2 < Idx.size(); i += 3)
		{
			const uint32 IA = Idx[i + 0];
			const uint32 IB = Idx[i + 1];
			const uint32 IC = Idx[i + 2];
			if (IA >= Section.Vertices.size() || IB >= Section.Vertices.size() || IC >= Section.Vertices.size())
			{
				continue;
			}

			const auto& A = Section.Vertices[IA].Position;
			const auto& B = Section.Vertices[IB].Position;
			const auto& C = Section.Vertices[IC].Position;

			const auto AB = SKSub(B, A);
			const auto AC = SKSub(C, A);
			const auto N = SKNormalize(SKCross(AB, AC));

			auto Add = [&](uint32 I0, uint32 I1)
			{
				if (I0 == I1)
				{
					return;
				}
				const uint64 Key = SKPackEdge(I0, I1);
				auto& E = Edges[Key];
				E.Count++;
				if (!E.bHasN0)
				{
					E.N0 = N;
					E.bHasN0 = true;
				}
				else if (!E.bHasN1)
				{
					E.N1 = N;
					E.bHasN1 = true;
				}
			};

			Add(IA, IB);
			Add(IB, IC);
			Add(IC, IA);
		}

		Out.All.reserve(Edges.size() * 2);
		Out.Boundary.reserve(Edges.size());
		Out.Sharp.reserve(Edges.size());

		for (const auto& KV : Edges)
		{
			const uint64 Key = KV.first;
			const uint32 I0 = static_cast<uint32>(Key >> 32);
			const uint32 I1 = static_cast<uint32>(Key & 0xFFFFFFFFu);

			Out.All.push_back(I0);
			Out.All.push_back(I1);

			const auto& E = KV.second;
			if (E.Count <= 1)
			{
				Out.Boundary.push_back(I0);
				Out.Boundary.push_back(I1);
				continue;
			}

			if (E.bHasN0 && E.bHasN1)
			{
				const float Dot = SKDot(E.N0, E.N1);
				// sharp if angle > threshold => dot < cos(threshold)
				if (Dot < CosThresh)
				{
					Out.Sharp.push_back(I0);
					Out.Sharp.push_back(I1);
				}
			}
		}

		return Out;
	}

	FSKMeshDerivedData FSKMeshBuilder::BuildDerived(const FSKMeshData& Mesh, const FSKGeometryBuildSettings& Settings)
	{
		FSKMeshDerivedData D;
		D.SectionWireframes.reserve(Mesh.Sections.size());
		D.SectionEdges.reserve(Mesh.Sections.size());

		for (const auto& S : Mesh.Sections)
		{
			if (Settings.bBuildWireframe)
			{
				D.SectionWireframes.push_back(BuildWireframe(S));
			}
			else
			{
				D.SectionWireframes.push_back(FSKWireframeData{});
			}

			if (Settings.bBuildEdgeCategories)
			{
				D.SectionEdges.push_back(SKBuildEdgeCategories(S, Settings.CreaseAngleDeg));
			}
			else
			{
				D.SectionEdges.push_back(FSKEdgeIndexLists{});
			}
		}
		return D;
	}
}
