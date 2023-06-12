#ifndef TYPE_UNIQUETYPE
#define TYPE_UNIQUETYPE

namespace Type
{

template <typename Type, class UniqueTypeAsId> class UniqueType
{
public:
	typedef Type UnderlyingType;
	UniqueType() {}
	explicit UniqueType(const Type &value) : value(value) {}
	operator const Type &() const { return value; }
	operator Type &() { return value; }

protected:
	Type value;
};

}

#endif
