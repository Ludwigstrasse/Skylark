#include "SKField/SKField.h"

namespace Skylark
{
	void FSKFieldSet::Clear()
	{
		Scalars.clear();
		Vectors.clear();
	}

	void FSKFieldSet::RegisterScalar(const FSKFieldKey& Key, TArray<float> Values, FSKFieldStats Stats)
	{
		FSKScalarField F;
		F.Key = Key;
		F.Values = std::move(Values);
		F.Stats = Stats;
		Scalars.push_back(std::move(F));
	}

	const FSKScalarField* FSKFieldSet::FindScalar(const FSKStringView Name) const
	{
		for (const auto& F : Scalars)
		{
			if (F.Key.Name == Name)
			{
				return &F;
			}
		}
		return nullptr;
	}

	void FSKFieldSet::RegisterVector(const FSKFieldKey& Key, TArray<FSKVector3f> Values, FSKFieldStats Stats)
	{
		FSKVectorField F;
		F.Key = Key;
		F.Values = std::move(Values);
		F.Stats = Stats;
		Vectors.push_back(std::move(F));
	}

	const FSKVectorField* FSKFieldSet::FindVector(const FSKStringView Name) const
	{
		for (const auto& F : Vectors)
		{
			if (F.Key.Name == Name)
			{
				return &F;
			}
		}
		return nullptr;
	}

	void FSKFieldRegistry::Clear()
	{
		Entries.clear();
	}

	FSKFieldSet& FSKFieldRegistry::GetOrCreateFieldSet(uint64 GeometryKey)
	{
		for (auto& E : Entries)
		{
			if (E.GeometryKey == GeometryKey)
			{
				return E.Fields;
			}
		}
		FEntry E;
		E.GeometryKey = GeometryKey;
		Entries.push_back(std::move(E));
		return Entries.back().Fields;
	}

	const FSKFieldSet* FSKFieldRegistry::FindFieldSet(uint64 GeometryKey) const
	{
		for (const auto& E : Entries)
		{
			if (E.GeometryKey == GeometryKey)
			{
				return &E.Fields;
			}
		}
		return nullptr;
	}
}
