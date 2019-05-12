#include "NumExpand.h"
#include <random>

using namespace NumExpand;
using namespace std;

Int& NumExpand::Int::operator<<=(const size_t& op)
{
	if (op == 0) return *this;
	size_t SHIFT = op / (sizeof(Int::base_type) << 3);
	size_t shift = op % (sizeof(Int::base_type) << 3);
	size_t shift_ = (sizeof(Int::base_type) << 3) - shift;
	Int::base_type mask = Int::all_one >> shift;	//00...00	11...111
	Int::base_type mask_ = ~mask;					//11...11	00...000
													// shift	 shift_

	if (shift) {
		size += SHIFT + 1;
		data.resize(size);
		
		auto rit = data.rbegin(), end = data.rend();
		if (size > SHIFT + 1) {
			auto crit = data.crbegin() + SHIFT + 1;
			*rit++ = (*crit & mask_) >> shift_ | (prefix ? (all_one >> shift << shift) : 0);
			for (auto cend = data.crend() - 1; crit != cend; ++crit)
				* rit++ = ((*(crit + 1) & mask_) >> shift_) | ((*crit & mask) << shift);
			*rit++ = ((tail & mask_) >> shift_) | ((*crit & mask) << shift);
		}
		else
			*rit++ = (tail & mask_) >> shift_ | (prefix ? (all_one >> shift << shift) : 0);
		if (rit == end)
			tail <<= shift;
		else {
			*rit++ = tail << shift;
			while (rit != end)
				* rit++ = 0;
			tail = 0;
		}
	}
	else {
		size += SHIFT;
		data.resize(size);

		auto rit = data.rbegin(), end = data.rend();
		for (auto crit = data.crbegin() + SHIFT, end = data.crend(); crit != end; ++crit)
			* rit++ = *crit;
		*rit++ = tail;
		while (rit != end)
			* (rit++) = 0;
		tail = 0;
	}

	return this->shrink();
}

Int& NumExpand::Int::operator>>=(const size_t & op)
{
	if (op == 0) return *this;
	size_t SHIFT = op / (sizeof(Int::base_type) << 3);
	size_t shift = op % (sizeof(Int::base_type) << 3);
	size_t shift_ = (sizeof(Int::base_type) << 3) - shift;
	Int::base_type mask = Int::all_one << shift;	//11...111	00...00
	Int::base_type mask_ = ~mask;					//00...000	11...11
													// shift_	 shift
	if (size) {
		if (SHIFT) {
			if (shift) {
				if (size > SHIFT) {
					tail = (data[SHIFT - 1] & mask) >> shift | (data[SHIFT] & mask_) << shift_;
					auto it = data.begin();
					auto cit = data.cbegin() + SHIFT;
					for (auto end = data.cend() - 1; cit != end; ++cit)
						(*it++) = (*cit & mask) >> shift | (*(cit + 1) & mask_) << shift_;
					*it = (prefix ? ((*cit >> shift) | (all_one >> shift_ << shift_)) : (*cit >> shift));
					size -= SHIFT;
					data.resize(size);
				}
				else {
					if (size == SHIFT) {
						tail = data.back() >> shift;
						if (prefix)
							tail |= all_one >> shift_ << shift_;
					}
					else
						tail = this->PREFIX();
					size = 0;
					data.clear();
				}
			}
			else {
				if (size > SHIFT) {
					tail = data[SHIFT - 1];
					copy(data.cbegin() + SHIFT, data.cend(), data.begin());
					size -= SHIFT;
					data.resize(size);
				}
				else {
					tail = size == SHIFT ? data.back() : this->PREFIX();
					size = 0;
					data.clear();
				}
			}
		}
		else {
			tail >>= shift;
			tail |= (data.front() & mask_) << shift_;
			auto it = data.begin();
			for (auto end = data.end() - 1; it != end; ++it) {
				*it >>= shift;
				*it |= ((*(it + 1)) & mask_) << shift_;
			}
			*it >>= shift;
			if (prefix)
				* it |= all_one >> shift_ << shift_;
		}
	}
	else {
		if (SHIFT)
			tail = this->PREFIX();
		else {
			tail >>= shift;
			if (prefix)
				tail |= all_one >> shift_ << shift_;
		}
	}
	return this->shrink();
}

Int NumExpand::Int::rand(base_type max_round)
{
	static std::random_device r;
	static std::default_random_engine e(r());
	std::uniform_int_distribution<unsigned> time_rand(0, max_round);
	static std::uniform_int_distribution<unsigned long long> uniform_dist(0, Int::all_one);
	
	unsigned times = time_rand(e);
	Int out(static_cast<Int::base_type>(uniform_dist(e)));
	for (unsigned i = 0; i < times; ++i)
		out *= static_cast<Int::base_type>(uniform_dist(e));
	return out;
}
