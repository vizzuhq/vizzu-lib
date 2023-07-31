#ifndef TEST_ARGUMENTS
#define TEST_ARGUMENTS

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <utility>

namespace test
{

class arguments
{
public:
	explicit arguments(std::list<std::string> &&argv,
	    std::list<std::string>&& def = {})
	{
		args = std::move(argv);

		if (args.empty()) args = std::move(def);
	}

	void add_option(char name,
	    const std::string &description,
	    const std::function<void()> &handler)
	{
		options.insert(
		    {std::string("-") + name, {description, handler, false}});
	}

	void add_option(char name,
	    const std::string &description,
	    const std::function<void(const std::string &)> &handler)
	{
		options.insert({std::string("-") + name,
		    {description,
		        [=, this]()
		        {
			        handler(pop());
		        },
		        true}});
	}

	void process()
	{
		do {
			options.at(pop()).handler();
		}
		while (has_more());
	}

	void print_help() const
	{
		std::cout << "options:\n";
		for (const auto &option : options)
			std::cout << "  " << option.first
			          << (option.second.has_parameter ? " PARAM  "
			                                          : "        ")
			          << option.second.description << "\n";
		std::cout << "\n";
	}

private:
	struct record
	{
		std::string description;
		std::function<void()> handler;
		bool has_parameter;
	};

	std::list<std::string> args;
	std::map<std::string, record> options;

	[[nodiscard]] bool has_more() const { return !args.empty(); }

	std::string pop()
	{
		if (has_more()) {
			std::string res = args.front();
			args.pop_front();
			return res;
		}
		throw std::logic_error("arguments not found");
	}
};

}

#endif
