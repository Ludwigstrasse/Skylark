#pragma once
#include "SKEngine/SKSubsystem.h"

namespace Skylark
{
	/**
	 * Minimal subsystem registry (UE: FSubsystemCollectionBase).
	 * - Owns subsystems and provides typed lookup.
	 */
	class FSKSubsystemCollection
	{
	public:
		template <typename TSubsystem, typename... TArgs>
		TSubsystem& Add(TArgs&&... Args)
		{
			static_assert(std::is_base_of_v<ISKEngineSubsystem, TSubsystem>);
			auto Ptr = std::make_unique<TSubsystem>(std::forward<TArgs>(Args)...);
			TSubsystem* Raw = Ptr.get();
			Subsystems.push_back(std::move(Ptr));
			return *Raw;
		}

		template <typename TSubsystem>
		TSubsystem* Get() const
		{
			for (const auto& S : Subsystems)
			{
				if (auto* Casted = dynamic_cast<TSubsystem*>(S.get()))
				{
					return Casted;
				}
			}
			return nullptr;
		}

		void InitializeAll(class FSKEngineContext& Context)
		{
			for (auto& S : Subsystems)
			{
				S->Initialize(Context);
			}
		}

		void DeinitializeAll()
		{
			// Reverse order
			for (auto It = Subsystems.rbegin(); It != Subsystems.rend(); ++It)
			{
				(*It)->Deinitialize();
			}
			Subsystems.clear();
		}

		void TickAll(float DeltaSeconds)
		{
			for (auto& S : Subsystems)
			{
				S->Tick(DeltaSeconds);
			}
		}

	private:
		TArray<TUniquePtr<ISKEngineSubsystem>> Subsystems;
	};
}
