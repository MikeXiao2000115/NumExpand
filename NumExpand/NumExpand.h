#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <iostream>
namespace NumExpand {
	class Int
	{
	public:
		//using base_type = size_t;
		//using base_type = unsigned long long;
		//using base_type = unsigned long;
		//using base_type = unsigned;
		//using base_type = unsigned short;
		using base_type = unsigned char;
		Int() :tail(0), prefix(false), size(0) {}
		Int(const Int& in) :tail(in.tail), data(in.data), prefix(in.prefix), size(in.size) {}
		Int(Int&& in)noexcept :tail(in.tail), data(std::move(in.data)), prefix(in.prefix), size(in.size) {}
		Int(bool positive, std::initializer_list<base_type> list) :prefix(!positive), data(std::rbegin(list) + 1, std::rend(list)), size(list.size() - 1) { tail = *std::rbegin(list); }
		Int(const base_type& in) :tail(in), prefix(false), size(0) {}
		//Int(const float& in) {}
		//Int(const double& in) {}


		const Int& operator=(const Int& op) { prefix = op.prefix; data = op.data; tail = op.tail; size = op.size; return (*this); }
		const Int& operator=(Int&& op)noexcept { prefix = op.prefix; data = std::move(op.data); tail = op.tail; size = op.size; return (*this); }

		~Int() {}

		std::string toString_binary() const {
			std::string out = prefix ? "...1111  " : "...0000  ";
			for (auto it = data.crbegin(), end = data.crend(); it != end; ++it) {
				for (int i = Int::bits_size; i > 0;)
					out += ((*it) & (1 << (--i))) ? "1" : "0";
				out += " ";
			}
			out += " ";
			for (int i = Int::bits_size; i > 0;)
				out += (tail & (1 << (--i))) ? "1" : "0";
			return out;
		}
		std::string toString() const {
			if (this->iszero())
				return "0";
			auto tmp = abs(*this);
			Int r;
			std::string out = "";
			while (tmp.isnzero()) {
				std::tie(tmp, r) = div(tmp, 10);
				out += std::to_string(static_cast<unsigned>(r.tail));
			}
			std::reverse(out.begin(), out.end());
			return (this->isneg()) ? "-" + out : out;
		}

		base_type to_base() const { return tail; }
		bool to_base_safe(base_type & out) const { out = tail; return size == 0 && !prefix; }
		float tof() const { return 0; }
		double tod() const { return 0; }

		Int operator~() const {
			Int out(!prefix, data, ~tail);
			for (auto& it : out.data)
				it = ~it;
			return out;
		}
		friend Int operator|(const Int & q, const Int & p);
		friend Int operator&(const Int & q, const Int & p);
		friend Int operator^(const Int & q, const Int & p);

		Int& operator<<=(const size_t & op);
		Int& operator>>=(const size_t & op);
		Int operator<<(const size_t & op) const { auto tmp = *this; return tmp <<= op; }
		Int operator>>(const size_t & op) const { auto tmp = *this; return tmp >>= op; }

		inline size_t bitlen() const { return (size + 1) << 3; }
		inline bool getbit(size_t pos) const {
			base_type mask = 1 << (pos % Int::bits_size);
			pos /= Int::bits_size;
			return pos ? data[pos - 1] & mask : tail & mask;
		}
		inline Int& setbit(size_t pos, bool to = true) {
			if (!to)
				return clearbit(pos);
			base_type mask = 1 << (pos % Int::bits_size);
			pos /= Int::bits_size;
			if (pos) {
				--pos;
				if (pos < size)
					data[pos] |= mask;
				else if (!prefix) {
					size = pos + 1;
					data.resize(size, 0);
					data[pos] |= mask;
				}
			}
			else
				tail |= mask;
			return this->shrink();
		}
		Int& clearbit(size_t pos) {
			base_type mask = ~(1 << (pos % Int::bits_size));
			pos /= Int::bits_size;
			if (pos) {
				--pos;
				if (pos < size)
					data[pos] &= mask;
				else if (prefix) {
					size = pos + 1;
					data.resize(size, all_one);
					data[pos] &= mask;
				}
			}
			else
				tail &= mask;
			return this->shrink();
		}

		friend bool operator==(const Int & q, const Int & p);
		friend bool operator!=(const Int & q, const Int & p) { return !(q == p); }
		friend bool operator>=(const Int & q, const Int & p);
		friend bool operator<=(const Int & q, const Int & p);
		friend bool operator>(const Int & q, const Int & p);
		friend bool operator<(const Int & q, const Int & p);
		bool ispos() const { return !prefix; }
		bool isneg() const { return prefix; }
		bool iszero() const { return !prefix && size == 0 && tail == 0; }
		bool isnzero() const { return !(this->iszero()); }

		Int& operator++() {
			if (tail == Int::all_one) {
				tail = Int::all_zero;
				auto it = data.begin(), end = data.end();
				while (it != end && *it == Int::all_one)
					* (it++) = Int::all_zero;
				if (it != end)
					++(*it);
				else {
					if (!prefix) {
						data.push_back(1);
						++size;
					}
					else {
						prefix = false;
						size = 0;
						data.clear();
						tail = 0;
					}
				}
			}
			else
				++tail;
			return this->shrink();
		}
		Int operator++(int) { auto tmp = *this; this->operator++(); return tmp; }
		Int& operator--() {
			if (tail == Int::all_zero) {
				tail = Int::all_one;
				auto it = data.begin(), end = data.end();
				while (it != end && *it == Int::all_zero)
					* (it++) = Int::all_one;
				if (it != end)
					--(*it);
				else {
					if (prefix) {
						data.push_back(Int::all_onezero);
						++size;
					}
					else {
						prefix = true;
						size = 0;
						data.clear();
						tail = Int::all_one;
					}
				}
			}
			else
				--tail;
			return this->shrink();
		}
		Int operator--(int) { auto tmp = *this; this->operator--(); return tmp; }

		Int operator-() const { return this->operator~().operator++(); }

		friend Int operator+(const Int & q, const Int & p);
		friend Int operator-(const Int & q, const Int & p);
		friend Int operator*(const Int & q, const Int & p);
		friend Int operator/(const Int & q, const Int & p) { Int Q, R; std::tie(Q, R) = div(q, p); return Q; }
		friend Int operator%(const Int & q, const Int & p) { Int Q, R; std::tie(Q, R) = div(q, p); return R; }
		friend std::pair<Int, Int> div(const Int & q, Int p);
		friend std::pair<Int, Int> div_postive(Int q, Int p);
		Int& operator+=(const Int & op) { (*this) = (*this) + op; return (*this); }
		Int& operator-=(const Int & op) { (*this) = (*this) - op; return (*this); }
		Int& operator*=(const Int & op) { (*this) = (*this) * op; return (*this); }
		Int& operator/=(const Int & op) { (*this) = (*this) / op; return (*this); }
		Int& operator%=(const Int & op) { (*this) = (*this) % op; return (*this); }

		friend Int abs(const Int & op);
		static Int rand(base_type max_round);

	private:
		base_type tail;
		std::vector<base_type> data;
		size_t size;
		bool prefix;

		inline base_type PREFIX() { return prefix ? all_one : all_zero; }
		static inline const Int& longer(const Int & q, const Int & p) { return p.size > q.size ? p : q; }
		static inline const Int& shorter(const Int & q, const Int & p) { return p.size > q.size ? q : p; }
		static inline std::pair<size_t, size_t> ls_size(const Int & q, const Int & p) { return p.size > q.size ? std::make_pair(p.size, q.size) : std::make_pair(q.size, p.size); }

		inline Int& shrink() {
			return (size && data.back() == (prefix ? Int::all_one : Int::all_zero)) ? __shrink__() : *this;
		}
		inline Int& __shrink__() {
			auto s = data.crbegin(), e = data.crend(), it = s;
			while (it != e && *it == (prefix ? Int::all_one : Int::all_zero))
				++it;
			auto shift = it - s;
			size -= shift;
			data.erase(data.end() - shift, data.end());
			return *this;
		}


		static const base_type all_one = static_cast<base_type>(-1);
		static const base_type all_onezero = static_cast<base_type>(-1) ^ 1;
		static const base_type all_zero = static_cast<base_type>(0);
		static const size_t bits_size = sizeof(Int::base_type) << 3;

		Int(const bool& prefix, const base_type & tail) :tail(tail), prefix(prefix), size(0) {}
		Int(const bool& prefix, const std::vector<base_type> & data, const base_type & tail) :tail(tail), data(data), prefix(prefix) { size = data.size(); }
		Int(const bool& prefix, std::vector<base_type> && data, const base_type & tail) :tail(tail), data(std::move(data)), prefix(prefix) { size = this->data.size(); }
		Int(const bool& prefix, std::vector<base_type> && data, const base_type & tail, const size_t & size) :tail(tail), data(std::move(data)), prefix(prefix), size(size) {}
	};

	inline bool NumExpand::operator==(const Int & q, const Int & p)
	{
		if (q.prefix == p.prefix && q.tail == p.tail && q.size == p.size) {
			for (auto qit = q.data.cbegin(), end = q.data.cend(), pit = p.data.cbegin(); qit != end; ++qit, ++pit)
				if (*qit != *pit)
					return false;
			return true;
		}
		else
			return false;
	}

	inline bool operator>=(const Int & q, const Int & p)
	{
		if (q.prefix == p.prefix) {
			if (q.size == p.size) {
				for (auto qit = q.data.crbegin(), end = q.data.crend(), pit = p.data.crbegin(); qit != end; ++qit, ++pit)
					if (*qit != *pit)
						return (*qit) > (*pit);
				return q.tail >= p.tail;
			}
			else return (q.prefix && q.size < p.size) || (!q.prefix && q.size > p.size);
		}
		else return p.prefix;
	}

	inline bool operator<=(const Int & q, const Int & p)
	{
		if (q.prefix == p.prefix) {
			if (q.size == p.size) {
				for (auto qit = q.data.crbegin(), end = q.data.crend(), pit = p.data.crbegin(); qit != end; ++qit, ++pit)
					if (*qit != *pit)
						return (*qit) < (*pit);
				return q.tail <= p.tail;
			}
			else return (q.prefix && q.size > p.size) || (!q.prefix && q.size < p.size);
		}
		else return q.prefix;
	}

	inline bool operator>(const Int & q, const Int & p)
	{
		if (q.prefix == p.prefix) {
			if (q.size == p.size) {
				for (auto qit = q.data.crbegin(), end = q.data.crend(), pit = p.data.crbegin(); qit != end; ++qit, ++pit)
					if (*qit != *pit)
						return (*qit) > (*pit);
				return q.tail > p.tail;
			}
			else return (q.prefix && q.size < p.size) || (!q.prefix && q.size > p.size);
		}
		else return p.prefix;
	}

	inline bool operator<(const Int & q, const Int & p)
	{
		if (q.prefix == p.prefix) {
			if (q.size == p.size) {
				for (auto qit = q.data.crbegin(), end = q.data.crend(), pit = p.data.crbegin(); qit != end; ++qit, ++pit)
					if (*qit != *pit)
						return (*qit) < (*pit);
				return q.tail < p.tail;
			}
			else return (q.prefix && q.size > p.size) || (!q.prefix && q.size < p.size);
		}
		else return q.prefix;
	}

	inline Int NumExpand::operator|(const Int & q, const Int & p)
	{
		if (p.size == 0 && q.size == 0)
			return Int(p.prefix || q.prefix, p.tail | q.tail);
		else {
			size_t l_size, s_size;
			const Int& l = Int::longer(q, p);
			const Int& s = Int::shorter(q, p);
			std::tie(l_size, s_size) = Int::ls_size(q, p);

			std::vector<Int::base_type> data;
			data.reserve(l_size);
			auto src_it = l.data.cbegin();
			for (auto it = s.data.cbegin(), end = s.data.cend(); it != end;)
				data.push_back((*(src_it++)) | (*(it++)));
			for (auto end = l.data.cend(); src_it != end; ++src_it)
				data.push_back(s.prefix ? Int::all_one : *src_it);

			return Int(s.prefix || l.prefix, std::move(data), s.tail | l.tail).shrink();
		}
	}

	inline Int NumExpand::operator&(const Int & q, const Int & p)
	{
		if (p.size == 0 && q.size == 0)
			return Int(p.prefix && q.prefix, p.tail & q.tail);
		else {
			size_t l_size, s_size;
			const Int& l = Int::longer(q, p);
			const Int& s = Int::shorter(q, p);
			std::tie(l_size, s_size) = Int::ls_size(q, p);

			std::vector<Int::base_type> data;
			data.reserve(l_size);
			auto src_it = l.data.cbegin();
			for (auto it = s.data.cbegin(), end = s.data.cend(); it != end;)
				data.push_back((*(src_it++)) & (*(it++)));
			for (auto end = l.data.cend(); src_it != end; ++src_it)
				data.push_back(s.prefix ? *src_it : Int::all_zero);

			return Int(s.prefix && l.prefix, std::move(data), s.tail & l.tail).shrink();
		}
	}

	inline Int NumExpand::operator^(const Int & q, const Int & p)
	{
		if (p.size == 0 && q.size == 0)
			return Int((p.prefix && !q.prefix) || (!p.prefix && q.prefix), p.tail ^ q.tail);
		else {
			size_t l_size, s_size;
			const Int& l = Int::longer(q, p);
			const Int& s = Int::shorter(q, p);
			std::tie(l_size, s_size) = Int::ls_size(q, p);

			std::vector<Int::base_type> data;
			data.reserve(l_size);
			auto src_it = l.data.cbegin();
			for (auto it = s.data.cbegin(), end = s.data.cend(); it != end;)
				data.push_back((*(src_it++)) ^ (*(it++)));
			for (auto end = l.data.cend(); src_it != end; ++src_it)
				data.push_back(s.prefix ? ~(*src_it) : *src_it);

			return Int(s.prefix != l.prefix, std::move(data), s.tail ^ l.tail).shrink();
		}
	}

	inline Int operator+(const Int & q, const Int & p)
	{
		if ((!(p.prefix && q.prefix)) &&
			(p.size == 0 && q.size == 0) &&
			(static_cast<Int::base_type>(p.tail + q.tail) >= p.tail))
			return Int(p.prefix || q.prefix, p.tail + q.tail);
		else {
			size_t l_size, s_size;
			const Int& l = Int::longer(q, p);
			const Int& s = Int::shorter(q, p);
			std::tie(l_size, s_size) = Int::ls_size(q, p);

			std::vector<Int::base_type> data;
			Int::base_type tail = p.tail + q.tail;

			bool up = tail < p.tail;

			data.reserve(l_size + 1);
			auto long_it = l.data.cbegin();
			for (auto it = s.data.cbegin(), end = s.data.cend(); it != end; ++it, ++long_it)
				if (up) {
					if (*it == Int::all_one) {
						data.push_back(*long_it);
						up = true;
					}
					else {
						Int::base_type tmp = *long_it + *it + 1;
						up = tmp < (*it + 1);
						data.push_back(tmp);
					}
				}
				else {
					Int::base_type tmp = *long_it + *it;
					up = tmp < *it;
					data.push_back(tmp);
				}

			if (s.prefix) {
				auto end = l.data.cend();
				if (!up) {
					for (; long_it != end && *long_it == Int::all_zero; ++long_it)
						data.push_back(Int::all_one);
					if (long_it != end) {
						up = true;
						data.push_back(*(long_it++) - 1);
					}
				}
				while (long_it != end)
					data.push_back(*(long_it++));
			}
			else {
				auto end = l.data.cend();
				if (up) {
					for (; long_it != end && *long_it == Int::all_one; ++long_it)
						data.push_back(Int::all_zero);
					if (long_it != end) {
						up = false;
						data.push_back(*(long_it++) + 1);
					}
				}
				while (long_it != end)
					data.push_back(*(long_it++));
			}
			if (up) {
				if (!(s.prefix || l.prefix))
					data.push_back(1);
				else if (s.prefix != l.prefix)
					data.push_back(Int::all_zero);
			}
			else if (s.prefix && l.prefix)
				data.push_back(Int::all_onezero);

			return Int(((!up) && (s.prefix || l.prefix)) || (up && s.prefix && l.prefix), std::move(data), tail).shrink();
		}
	}

	inline Int operator-(const Int & q, const Int & p) { return q + (-p); }

	inline void _add_(Int::base_type & a, Int::base_type & b, Int::base_type & c, Int::base_type upper, Int::base_type cur) {
		c += cur;
		if (c < cur) {
			if (b == static_cast<Int::base_type>(-1)) {
				++a;
				b = 0;
			}
			else
				++b;
		}
		b += upper;
		if (b < upper)
			++a;
	}
	inline void _product_(Int::base_type & A, Int::base_type & B, const Int::base_type & x, const Int::base_type & y) {
		static const Int::base_type shift = (sizeof(Int::base_type) << 2);
		static const Int::base_type mask = static_cast<Int::base_type>(-1) >> shift;
		static const Int::base_type mask_ = ~mask;

		// (d + c) * (b + a)
		Int::base_type a = x & mask;
		Int::base_type b = (x & mask_) >> shift;
		Int::base_type c = y & mask;
		Int::base_type d = (y & mask_) >> shift;

		B = a * c;
		Int::base_type a_d = a * d;
		Int::base_type b_c = b * c;
		A = b * d;

		a_d += b_c;
		if (a_d < b_c)
			A += (1 << shift);
		A += (a_d & mask_) >> shift;
		a_d &= mask;
		a_d <<= shift;
		B += a_d;
		if (B < a_d)
			++A;
	}
	inline void _add_product_(Int::base_type & a, Int::base_type & b, Int::base_type & c, const Int::base_type & x, const Int::base_type & y) {
		static const Int::base_type shift = sizeof(Int::base_type) << 2;
		static const Int::base_type one = static_cast<Int::base_type>(-1);
		static const Int::base_type mask = one >> shift;
		static const Int::base_type mask_ = ~mask;

		Int::base_type cur = (x & mask) * (y & mask);
		Int::base_type a_d = (x & mask) * ((y & mask_) >> shift);
		Int::base_type b_c = ((x & mask_) >> shift) * (y & mask);
		Int::base_type upper = ((x & mask_) >> shift) * ((y & mask_) >> shift);

		a_d += b_c;
		upper += ((a_d & mask_) >> shift) + ((a_d < b_c) ? (1 << shift) : 0);

		a_d = (a_d & mask) << shift;
		cur += a_d;
		if (cur < a_d)
			++upper;

		c += cur;
		if (c < cur) {
			if (b == one)
				++a;
			++b;
		}
		b += upper;

		if (b < upper)
			++a;
	}

	Int inline operator*(const Int & q, const Int & p)
	{
		const Int& q_ = q.prefix ? Int(-q) : q;
		const Int & p_ = p.prefix ? Int(-p) : p;

		size_t l_size, s_size;
		const Int & l = Int::longer(q_, p_);
		const Int & s = Int::shorter(q_, p_);
		std::tie(l_size, s_size) = Int::ls_size(q_, p_);


		std::vector<Int::base_type> data;
		data.reserve(s_size + l_size + 3);

		Int::base_type tail, a = 0, b, c;
		_product_(b, tail, q.tail, p.tail);

		for (size_t shift = 0; shift < s_size + l_size; ++shift) {
			c = b;
			b = a;
			a = 0;
			if (shift < s_size)
				_add_product_(a, b, c, l.tail, s.data[shift]);
			if (shift < l_size)
				_add_product_(a, b, c, s.tail, l.data[shift]);

			for (size_t i = (shift > l_size ? (shift - l_size) : 0); i < std::min(shift, s_size); ++i)
				_add_product_(a, b, c, s.data[i], l.data[shift - i - 1]);
			data.push_back(c);
		}
		data.push_back(b);
		data.push_back(a);
		return ((q.prefix != p.prefix) ? -Int(false, std::move(data), tail) : Int(false, std::move(data), tail)).shrink();
	}

	inline std::pair<Int, Int> div(const Int & q, Int p) //return (Q,R), q = Q*p + R , 0 <= R < abs(p)
	{
		bool p_prefix = p.prefix;
		p = abs(p);
		Int Q, R;
		std::tie(Q, R) = div_postive(abs(q), p);
		if (p_prefix) {
			if (q.prefix)
				return std::make_pair(++Q, p - R);	//q < 0 & p < 0
			else
				return std::make_pair(-Q, R);		//q > 0 & p < 0
		}
		else {
			if (q.prefix)
				return std::make_pair(-(++Q), p - R);	//q < 0 & p > 0
			else
				return std::make_pair(Q, R);			//q > 0 & p > 0
		}
	}
	inline std::pair<Int, Int> div_postive(Int q, Int p) //return (Q,R), q = Q*p + R
	{
		if (q.size == 0 && p.size == 0)
			return std::make_pair(q.tail / p.tail, q.tail % p.tail);
		if (q < p)
			return std::make_pair(0, q);

		//q.size >= p.size
		//q.size != 0
		//q >= p
		std::vector<Int::base_type> data;
		Int::base_type tail = 0;
		if (q.size != p.size) {
			size_t diff = q.size - p.size;

			data.resize(diff, 0);
			if (diff > (Int::all_one / Int::bits_size)) {
				for (size_t i = 0; i < Int::bits_size; ++i)
					p <<= diff;
				p <<= Int::bits_size - 1;
			}
			else
				p <<= (diff + 1) * Int::bits_size - 1;

			for (size_t i = diff; i > 0; --i)
				for (long j = Int::bits_size - 1; j >= 0; --j) {
					if (q >= p) {
						q -= p;
						data[i - 1] |= 1 << j;
					}
					p >>= 1;
				}
		}
		else
			p <<= Int::bits_size - 1;

		for (long j = Int::bits_size - 1; j >= 0; --j) {
			if (q >= p) {
				q -= p;
				tail |= 1 << j;
			}
			p >>= 1;
		}
		return std::pair<Int, Int>(Int(false, std::move(data), tail).shrink(), q);
	}

	inline Int abs(const Int & op) { return op.prefix ? -op : op; }
}