#ifndef CHART_CONFIG_H
#define CHART_CONFIG_H

#include <functional>
#include <map>
#include <string>
#include <utility>

#include "options.h"

namespace Vizzu::Gen
{

class Config
{
public:
	static std::string paramsJson();
	[[nodiscard]] std::string getParam(const std::string &path) const;
	void setParam(const std::string &path, const std::string &value);
	explicit Config(Options &options, Data::DataTable &table) :
	    options(options),
	    table(table)
	{}

private:
	struct Accessor
	{
		std::string (*get)(const Options &);
		void (*set)(Options &, const std::string &);
	};

	struct ChannelAccessor
	{
		std::string (*get)(const Channel &);
		void (*set)(Channel &, const std::string &);
	};

	template <auto Mptr>
	static const std::pair<std::string_view, Accessor> accessor;

	template <auto Mptr>
	static const std::pair<std::string_view, ChannelAccessor>
	    channel_accessor;

	using Accessors = std::map<std::string_view, Accessor>;
	using ChannelAccessors =
	    std::map<std::string_view, ChannelAccessor>;

	static const Accessors &getAccessors();
	static const ChannelAccessors &getChannelAccessors();

	std::reference_wrapper<Options> options;
	std::reference_wrapper<Data::DataTable> table;

	void setChannelParam(const std::string &path,
	    const std::string &value);
	[[nodiscard]] std::string getChannelParam(
	    const std::string &path) const;
};

}

#endif
