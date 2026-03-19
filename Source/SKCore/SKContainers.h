#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace Skylark
{
	template <typename T>
	using TArray = std::vector<T>;

	template <typename K, typename V>
	using TMap = std::unordered_map<K, V>;

	template <typename T>
	using TSet = std::unordered_set<T>;

	template <typename T>
	using TUniquePtr = std::unique_ptr<T>;

	template <typename T>
	using TSharedPtr = std::shared_ptr<T>;

	template <typename T>
	using TWeakPtr = std::weak_ptr<T>;
}
