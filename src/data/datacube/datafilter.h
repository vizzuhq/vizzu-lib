#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <cstdint>
#include <functional>

namespace Vizzu::Data
{
struct RowWrapper;

class Filter
{
public:
	using Function = std::function<bool(const RowWrapper &)>;

	Filter() noexcept = default;

	template <class Ptr>
	explicit Filter(Ptr &&wr) :
	    hash(std::hash<bool (*)(const RowWrapper *)>{}(wr.get())),
	    function(
	        [wr = std::shared_ptr{std::move(wr)}](
	            const RowWrapper &row) noexcept -> bool
	        {
		        return (*wr)(&row);
	        })
	{}

	template <class Fun>
	Filter(std::size_t hash, Fun &&function) :
	    hash(hash),
	    function(std::forward<Fun>(function))
	{}

	[[nodiscard]] bool operator==(const Filter &other) const
	{
		return hash == other.hash;
	}

	[[nodiscard]] Filter operator&&(const Filter &other) const
	{
		return {hash == other.hash ? hash : hash ^ other.hash,
		    [fun1 = this->function, fun2 = other.function](
		        const RowWrapper &row) noexcept -> bool
		    {
			    return (!fun1 || fun1(row)) && (!fun2 || fun2(row));
		    }};
	}

	const Function &getFunction() const { return function; }

private:
	std::size_t hash{};
	Function function;
};

}

#endif
