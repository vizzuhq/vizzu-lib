#include "easingfunc.h"

using namespace Anim;

EaseFuncBase EaseFunc::create(const std::string &name)
{
	if (name == "ease-in-sine") return in<&sine>;
	if (name == "ease-out-sine") return out<&sine>;
	if (name == "ease-in-out-sine") return inOut<&sine>;

	if (name == "ease-in-quad") return in<&quad>;
	if (name == "ease-out-quad") return out<&quad>;
	if (name == "ease-in-out-quad") return inOut<&quad>;

	if (name == "ease-in-cubic") return in<&cubic>;
	if (name == "ease-out-cubic") return out<&cubic>;
	if (name == "ease-in-out-cubic") return inOut<&cubic>;

	if (name == "ease-in-quart") return in<&quart>;
	if (name == "ease-out-quart") return out<&quart>;
	if (name == "ease-in-out-quart") return inOut<&quart>;

	if (name == "ease-in-quint") return in<&quint>;
	if (name == "ease-out-quint") return out<&quint>;
	if (name == "ease-in-out-quint") return inOut<&quint>;

	if (name == "ease-in-expo") return in<&expo>;
	if (name == "ease-out-expo") return out<&expo>;
	if (name == "ease-in-out-expo") return inOut<&expo>;

	if (name == "ease-in-circ") return in<&circ>;
	if (name == "ease-out-circ") return out<&circ>;
	if (name == "ease-in-out-circ") return inOut<&circ>;

	if (name == "ease-in-back") return in<&back>;
	if (name == "ease-out-back") return out<&back>;
	if (name == "ease-in-out-back") return inOut<&back>;

	if (name == "ease-in-elastic") return in<&elastic>;
	if (name == "ease-out-elastic") return out<&elastic>;
	if (name == "ease-in-out-elastic") return inOut<&elastic>;

	return nullptr;
}
