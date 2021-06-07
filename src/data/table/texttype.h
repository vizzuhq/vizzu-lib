#ifndef TEXTTYPE_H
#define TEXTTYPE_H

#include <vector>
#include <string>

namespace Vizzu
{
namespace Data
{

class TextType
{
	friend class TextTable;
public:
	enum Type : uint32_t { Number, String };

	TextType();
	TextType(const Type &type, int count);
	Type getType() const;
private:
	Type type;
	int count;
	static bool isNumber(const std::string &value);
	void update(const std::string &value);
};

typedef std::vector<TextType> TextTypes;

}
}

#endif
