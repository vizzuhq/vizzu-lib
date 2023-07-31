#ifndef TYPE_UNIQUETYPE
#define TYPE_UNIQUETYPE

namespace Type
{

template <typename Type, class> class UniqueType
{
public:
	constexpr UniqueType() = default;
	constexpr explicit UniqueType(const Type &value) : value(value) {}
	constexpr operator const Type &() const { return value; }
	constexpr operator Type &() { return value; }

protected:
	Type value;
};

}

#endif
