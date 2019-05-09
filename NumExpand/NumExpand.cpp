#include "NumExpand.h"
#include <random>

using namespace NumExpand;

Int NumExpand::Int::operator<<(const size_t& op) const
{
	size_t SHIFT = op / (sizeof(Int::base_type) << 3);
	size_t shift = op % (sizeof(Int::base_type) << 3);
	Int::base_type mask = Int::all_one >>  shift;
	Int::base_type mask_ = ~mask;

	return Int();
}

Int NumExpand::Int::operator>>(const size_t& op) const
{
	return Int();
}

Int NumExpand::Int::rand(base_type max_round)
{
	static std::random_device r;
	static std::default_random_engine e(r());
	static std::uniform_int_distribution<unsigned> time_rand(0, max_round);
	static std::uniform_int_distribution<Int::base_type> uniform_dist(0, static_cast<Int::base_type>(-1));
	unsigned times = time_rand(e);
	Int out(0);
	for (unsigned i = 0; i < times; ++i)
		out += uniform_dist(e);
	return out;
}
