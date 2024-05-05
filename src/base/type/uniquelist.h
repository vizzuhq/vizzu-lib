#ifndef TYPE_UNIQUELIST
#define TYPE_UNIQUELIST

#include <algorithm>
#include <map>
#include <ranges>
#include <utility>

namespace Type
{

template <class T> class UniqueList
{
	struct links_t;
	using container_type = std::map<T, links_t, std::less<>>;

	using link_t = typename container_type::pointer;
	using const_link_t = typename container_type::const_pointer;

	struct links_t
	{
		link_t pre{};
		link_t post{};
		std::size_t ix{};
		typename container_type::iterator it{};
	};

	template <bool forward = true> struct iterator
	{
		const_link_t ptr;
		[[nodiscard]] const T &operator*() const noexcept
		{
			return ptr->first;
		}
		iterator &operator++() noexcept
		{
			if constexpr (forward)
				ptr = ptr->second.post;
			else
				ptr = ptr->second.pre;

			return *this;
		}
		[[nodiscard]] iterator operator++(int) noexcept
		{
			auto tmp = *this;
			++*this;
			return tmp;
		}
		[[nodiscard]] bool operator==(
		    const iterator &other) const noexcept = default;
		[[nodiscard]] bool operator!=(
		    const iterator &other) const noexcept = default;

		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = const T *;
		using reference = const T &;
		using iterator_category = std::forward_iterator_tag;
	};

	void after_push_back(
	    const typename container_type::iterator &item) noexcept
	{
		if (auto &&preLast =
		        std::exchange(last, std::to_address(item))) {
			preLast->second.post = last;
			last->second.ix = preLast->second.ix + 1;
		}
		else
			first = last;
	}

	void insert(typename container_type::node_type &&node) noexcept
	{
		auto &&it = items.insert(std::move(node)).position;
		it->second = {last};
		after_push_back(it);
	}

	void before_remove(links_t &ptr) noexcept
	{
		if (ptr.pre)
			ptr.pre->second.post = ptr.post;
		else
			first = ptr.post;

		if (ptr.post)
			ptr.post->second.pre = ptr.pre;
		else
			last = ptr.pre;
	}

	typename container_type::node_type extract(
	    const typename container_type::iterator &it) noexcept
	{
		before_remove(it->second);
		return items.extract(it);
	}

public:
	bool push_back(const T &value)
	{
		auto &&[it, newly] = items.try_emplace(value, last);
		if (!newly) return false;
		after_push_back(it);
		return true;
	}

	[[nodiscard]] iterator<> begin() const noexcept
	{
		return {first};
	}
	[[nodiscard]] static iterator<> end() noexcept { return {}; }
	[[nodiscard]] iterator<false> rbegin() const noexcept
	{
		return {last};
	}
	[[nodiscard]] static iterator<false> rend() noexcept
	{
		return {};
	}

	[[nodiscard]] bool empty() const noexcept { return !first; }

	void clear() noexcept
	{
		first = last = {};
		items.clear();
	}

	[[nodiscard]] std::size_t size() const noexcept
	{
		return items.size();
	}

	constexpr static std::size_t npos = ~std::size_t{};

	template <class U>
	[[nodiscard]] std::size_t find(U &&val) const noexcept
	{
		auto it = items.find(std::forward<U>(val));
		return it == items.end() ? npos : it->second.ix;
	}

	bool remove(const T &value) noexcept
	{
		if (auto it = items.find(value); it != items.end()) {
			auto &link = it->second;
			for (auto l = link.post; l; l = l->second.post)
				--l->second.ix;
			before_remove(link);
			items.erase(it);
			return true;
		}
		return false;
	}

	[[nodiscard]] bool operator==(
	    const UniqueList &rhs) const noexcept
	{
		return size() == rhs.size()
		    && std::equal(begin(), end(), rhs.begin());
	}

	[[nodiscard]] bool contains(const T &item) const noexcept
	{
		return items.contains(item);
	}

	[[nodiscard]] UniqueList split_by(const UniqueList &by) noexcept
	{
		auto first1 = by.items.begin();
		auto last1 = by.items.end();

		for (auto first2 = items.begin(), last2 = items.end();
		     first2 != last2;
		     ++first2) {
			auto &[k, v] = *first2;
			while (first1 != last1 && first1->first < k) ++first1;
			if (first1 == last1) break;
			if (k < first1->first) continue;
			v.it = first2;
			++first1;
		}

		UniqueList common;
		std::size_t ix{};
		const typename container_type::iterator null_it{};
		for (auto it = first; it;)
			if (auto &links =
			        std::exchange(it, it->second.post)->second;
			    links.it != null_it)
				common.insert(extract(links.it));
			else
				links.ix = ix++;
		return common;
	}

	[[nodiscard]] auto as_set() const noexcept
	{
		return std::ranges::views::keys(items);
	}

	template <class It, class Sentinel = It>
	[[nodiscard]] bool contains_any(It &&first1,
	    Sentinel &&last1) const
	{
		for (auto first2 = items.begin(), last2 = items.end();
		     first1 != last1 && first2 != last2;)
			if (*first1 < first2->first)
				++first1;
			else if (first2->first < *first1)
				++first2;
			else
				return true;
		return false;
	}

	UniqueList() noexcept = default;
	UniqueList(UniqueList &&) noexcept = default;
	UniqueList &operator=(UniqueList &&) noexcept = default;
	UniqueList(const UniqueList &other)
	{
		for (auto &&item : other) push_back(item);
	}
	UniqueList &operator=(const UniqueList &other)
	{
		if (this == &other) return *this;
		clear();
		for (auto &&item : other) push_back(item);
		return *this;
	}

private:
	container_type items;
	link_t first{};
	link_t last{};
};

}

#endif
