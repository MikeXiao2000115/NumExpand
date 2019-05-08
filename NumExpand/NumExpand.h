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

		const Int& operator=(const Int& op) { prefix = op.prefix; data = op.data; tail = op.tail; size = op.size; return (*this); }
		const Int& operator=(Int&& op)noexcept { prefix = op.prefix; data = std::move(op.data); tail = op.tail; size = op.size; return (*this); }

		~Int() {}
		std::string toString_binary() {
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

		Int operator~() const {
			Int out(!prefix, data, ~tail);
			for (auto& it : out.data)
				it = ~it;
			return out;
		}
		friend Int operator|(const Int & q, const Int & p);
		friend Int operator&(const Int & q, const Int & p);
		friend Int operator^(const Int & q, const Int & p);

		friend bool operator==(const Int & q, const Int & p);

		friend Int operator+(const Int & q, const Int & p);
		const Int& operator+=(const Int & op) { return (*this = *this + op); }
	private:
		base_type tail;
		std::vector<base_type> data;
		size_t size;
		bool prefix;

		static inline bool fast_cal(const Int & q, const Int & p) { return p.size == 0 && q.size == 0; }
		static inline const Int& longer(const Int & q, const Int & p) { return p.size > q.size ? p : q; }
		static inline const Int& shorter(const Int & q, const Int & p) { return p.size > q.size ? q : p; }
		static inline std::pair<size_t, size_t> ls_size(const Int & q, const Int & p) { return p.size > q.size ? std::make_pair(p.size, q.size) : std::make_pair(q.size, p.size); }

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
		if (q.prefix == p.prefix && q.tail == p.tail && q.data.size() == p.data.size()) {
			for (auto qit = q.data.cbegin(), end = q.data.cend(), pit = p.data.cbegin(); qit != end; ++qit, ++pit)
				if (*qit != *pit)
					return false;
			return true;
		}
		else
			return false;
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

			return Int(s.prefix || l.prefix, std::move(data), s.tail | l.tail);
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

			return Int(s.prefix && l.prefix, std::move(data), s.tail & l.tail);
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

			return Int((s.prefix && !l.prefix) || (!s.prefix && l.prefix), std::move(data), s.tail ^ l.tail);
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

			bool prefix;
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
				if (!up){
					for (; *long_it == Int::all_zero && long_it != end; ++long_it)
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
				for (auto end = l.data.cend(); long_it != end; ++long_it)
					;
			}



			return Int();
		}
	}
