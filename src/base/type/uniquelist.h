#ifndef TYPE_UNIQUELIST
#define TYPE_UNIQUELIST

#include <algorithm>
#include <list>
#include <optional>

namespace Type
{

template <typename T> class UniqueList
{
public:
	typedef std::list<T> Items;

	bool pushBack(const T &value)
	{
		auto it = std::find(items.begin(), items.end(), value);
		if (it == items.end()) {
			items.push_back(value);
			return true;
		}
		return false;
	}

	bool insertAt(size_t index, const T &value)
	{
		auto it = std::find(items.begin(), items.end(), value);
		if (it == items.end()) {
			index = std::min(index, items.size());
			auto posIt = items.begin();
			std::advance(posIt, index);
			items.insert(posIt, value);
			return true;
		}
		return false;
	}

	const T &at(size_t index) const
	{
		auto posIt = items.begin();
		std::advance(posIt, std::min(index, items.size()));
		if (posIt != items.end())
			return *posIt;
		else
			throw std::out_of_range("");
	}

	auto begin() const { return items.begin(); }
	auto end() const { return items.end(); }

	auto rbegin() const { return items.rbegin(); }
	auto rend() const { return items.rend(); }

	bool empty() const { return items.empty(); }
	void clear() { items.clear(); }
	size_t size() const { return items.size(); }

	bool remove(const T &value)
	{
		auto it = std::find(items.begin(), items.end(), value);
		if (it == items.end())
			return false;
		else {
			items.erase(it);
			return true;
		}
	}

	bool operator==(const UniqueList<T> &other) const
	{
		return items == other.items;
	}

	bool includes(const T &item) const
	{
		auto it = std::find(items.begin(), items.end(), item);
		return it != items.end();
	}

	std::optional<int> getIndex(const T &item) const
	{
		auto it = std::find(items.begin(), items.end(), item);
		return it != items.end() ? std::distance(items.begin(), it)
		                         : std::optional<int>{};
	}

	void remove(const UniqueList<T> &other)
	{
		for (auto &item : other) remove(item);
	}

	void section(const UniqueList<T> &other)
	{
		auto it = items.begin();
		while (it != items.end()) {
			auto next = it;
			++next;
			if (!other.includes(*it)) items.erase(it);
			it = next;
		}
	}

private:
	Items items;
};

}

#endif
