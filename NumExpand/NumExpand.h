#pragma once
#include <vector>
#include <string>
#include <tuple>
namespace NumExpand {
	class Int
	{
	public:
		using base_type = unsigned long;
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
				for (int i = (sizeof(base_type) << 3); i > 0;)
					out += ((*it) & (1 << (--i))) ? "1" : "0";
				out += " ";
			}
			out += " ";
			for (int i = (sizeof(base_type) << 3); i > 0;)
				out += (tail & (1 << (--i))) ? "1" : "0";
			return out;
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

		Int operator<<(const size_t & op) const;
		Int operator>>(const size_t & op) const;
		Int operator<<=(const size_t & op) { auto tmp = *this; *this = this->operator<<(op); return tmp; }
		Int operator>>=(const size_t & op) { auto tmp = *this; *this = this->operator>>(op); return tmp; }

		friend bool operator==(const Int & q, const Int & p);
		friend bool operator!=(const Int & q, const Int & p) { return !(q == p); }
		friend bool operator>=(const Int & q, const Int & p);
		friend bool operator<=(const Int & q, const Int & p);
		friend bool operator>(const Int & q, const Int & p);
		friend bool operator<(const Int & q, const Int & p);

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
		friend std::pair<Int, Int> div(const Int & q, const Int & p);
		const Int& operator+=(const Int & op) { return (*this = *this + op); }
		const Int& operator-=(const Int & op) { return (*this = *this - op); }

		friend Int abs(const Int & op);
		static Int rand(base_type max_round);

	private:
		base_type tail;
		std::vector<base_type> data;
		size_t size;
		bool prefix;

		static inline bool fast_cal(const Int & q, const Int & p) { return p.size == 0 && q.size == 0; }
		static inline const Int& longer(const Int & q, const Int & p) { return p.size > q.size ? p : q; }
		static inline const Int& shorter(const Int & q, const Int & p) { return p.size > q.size ? q : p; }
		static inline std::pair<size_t, size_t> ls_size(const Int & q, const Int & p) { return p.size > q.size ? std::make_pair(p.size, q.size) : std::make_pair(q.size, p.size); }
		static inline std::pair<base_type, base_type> full_product(base_type x, base_type y) {
			static const base_type shift = (sizeof(base_type) << 2);
			static const base_type mask = all_one >> shift;
			static const base_type mask_ = ~mask;

			// (d + c) * (b + a)
			base_type a = x & mask;
			base_type b = (x & mask_) >> shift;
			base_type c = y & mask;
			base_type d = (y & mask_) >> shift;

			base_type a_c = a * c;
			base_type a_d = a * d;
			base_type b_c = b * c;
			base_type b_d = b * d;

			a_d += b_c;
			if (a_d < b_c)
				b_d += (1 << shift);
			b_d += (a_d & mask_) >> shift;
			a_d &= mask;
			a_d <<= shift;
			a_c += a_d;
			if (a_c < a_d)
				++b_d;
			return std::make_pair(b_d, a_c);
		}

		inline Int & shrink() {
			return (data.back() == (prefix ? Int::all_one : Int::all_zero)) ? __shrink__() : *this;
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
			else return (q.prefix && q.size > p.size) || (!q.prefix && q.size < p.size);
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
			else return (q.prefix && q.size < p.size) || (!q.prefix && q.size > p.size);
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
			else return (q.prefix && q.size > p.size) || (!q.prefix && q.size < p.size);
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
			else return (q.prefix && q.size < p.size) || (!q.prefix && q.size > p.size);
		}
		else return q.prefix;
	}

	inline Int NumExpand::operator|(const Int & q, const Int & p)
	{
		if (Int::fast_cal(p, q))
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
		if (Int::fast_cal(p, q))
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
		if (Int::fast_cal(p, q))
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
			Int::fast_cal(p, q) &&
			(p.tail + q.tail >= p.tail))
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
						auto tmp = *long_it + *it + 1;
						up = tmp < (*it + 1);
						data.push_back(tmp);
					}
				}
				else {
					auto tmp = *long_it + *it;
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
					for (; long_it != end && *long_it == Int::all_zero; ++long_it)
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
				if (s.prefix || l.prefix) {
					if (s.prefix && l.prefix)
						;
					else
						data.push_back(Int::all_zero);
				}
				else
					data.push_back(1);
			}
			else if (s.prefix && l.prefix)
				data.push_back(Int::all_onezero);

			return Int((s.prefix || l.prefix) && (!up || (s.prefix && l.prefix)), std::move(data), tail).shrink();
		}
	}

	inline Int operator-(const Int & q, const Int & p) { return q + (-p); }

	Int operator*(const Int & q, const Int & p)
	{
		const Int& q_ = q.prefix ? Int(-q) : q;
		const Int& p_ = p.prefix ? Int(-p) : p;

		size_t l_size, s_size;
		const Int& l = Int::longer(q_, p_);
		const Int& s = Int::shorter(q_, p_);
		std::tie(l_size, s_size) = Int::ls_size(q_, p_);


		std::vector<Int::base_type> data;
		data.reserve(s_size + l_size + 3);

		Int::base_type tail, a, b, c;
		std::tie(b, tail) = Int::full_product(q.tail, p.tail);
		
		size_t shift = 1;
		for (; shift < s_size; ++shift)
			;
		for (; shift < l_size; ++shift)
			;
		for (; shift < s_size + l_size; ++shift)
			;


		return Int();
	}

	inline Int abs(const Int & op) { return op.prefix ? -op : op; }
}