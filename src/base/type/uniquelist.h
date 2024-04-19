#ifndef TYPE_UNIQUELIST
#define TYPE_UNIQUELIST

#include <algorithm>
#include <list>
#include <optional>
#include <stdexcept>

namespace Type
{

template <typename T> class UniqueList
{
public:
	using Items = std::list<T>;

	bool pushBack(const T &value)
	{
		auto nonexists = !includes(value);
		if (nonexists) items.push_back(value);
		return nonexists;
	}

	[[nodiscard]] auto begin() const { return items.begin(); }
	[[nodiscard]] auto end() const { return items.end(); }

	[[nodiscard]] auto rbegin() const { return items.rbegin(); }
	[[nodiscard]] auto rend() const { return items.rend(); }

	[[nodiscard]] bool empty() const { return items.empty(); }
	void clear() { items.clear(); }
	[[nodiscard]] size_t size() const { return items.size(); }

	void remove(const T &value)
	{
		if (auto it = std::find(items.begin(), items.end(), value);
		    it != items.end())
			items.erase(it);
	}

	bool operator==(const UniqueList<T> &other) const = default;

	[[nodiscard]] bool includes(const T &item) const
	{
		return getIndex(item).has_value();
	}

	[[nodiscard]] std::optional<int> getIndex(const T &item) const
	{
		int ix{};
		for (auto it = items.begin(); it != items.end(); ++it, ++ix)
			if (*it == item) return ix;
		return std::nullopt;
	}

	void remove(const UniqueList<T> &other)
	{
		for (auto &item : other) remove(item);
	}

	void section(const UniqueList<T> &other)
	{
		for (auto it = items.begin(); it != items.end();)
			if (!other.includes(*it))
				it = items.erase(it);
			else
				++it;
	}

private:
	Items items;
};

}

#endif
