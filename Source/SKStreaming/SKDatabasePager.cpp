#include "SKStreaming/SKDatabasePager.h"
#include "SKScene/SKSceneGraph.h"

namespace Skylark
{
	void FSKDatabasePager::RequestNodeAsync(FSKString Uri, FOnNodeReady OnReady)
	{
		if (!Queue)
		{
			return;
		}

		Queue->Enqueue([this, Uri = std::move(Uri), OnReady = std::move(OnReady)]()
		{
			// V10: placeholder load/build.
			auto Node = std::make_shared<FSKNode>("Loaded:" + Uri);

			std::lock_guard<std::mutex> Lock(Mutex);
			FPendingReady R;
			R.OnReady = OnReady;
			R.Node = std::move(Node);
			Completed.push_back(std::move(R));
		});
	}

	void FSKDatabasePager::TickMainThread()
	{
		TArray<FPendingReady> Local;
		{
			std::lock_guard<std::mutex> Lock(Mutex);
			Local.swap(Completed);
		}

		for (auto& R : Local)
		{
			if (R.OnReady)
			{
				R.OnReady(std::move(R.Node));
			}
		}
	}
}
