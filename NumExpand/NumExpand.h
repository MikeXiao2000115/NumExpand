#pragma once
#include <vector>
#include <string>
namespace NumExpand {
	class Int
	{
	public:
		Int() :tail(0), prefix(false) {}
		Int(const Int& in) :tail(in.tail), data(in.data), prefix(in.prefix) {}
		Int(Int&& in)noexcept :tail(in.tail), data(std::move(in.data)), prefix(in.prefix) {}
		Int(bool positive, std::initializer_list<unsigned long> list) :prefix(!positive), data(std::rbegin(list) + 1, std::rend(list)) { tail = *std::rbegin(list); }

		Int(const unsigned long& in):tail(in),prefix(false){}

		~Int() {}
		std::string toString_binary() {
			std::string out = prefix ? "...1111  " : "...0000  ";
			for (auto it = data.crbegin(), end = data.crend(); it != end; ++it) {
				for (int i = (sizeof(unsigned long) << 3) - 1; i >= 0; --i)
					out += ((*it) & (1 << i)) ? "1" : "0";
				out += " ";
			}
			out += " ";
			for (int i = (sizeof(unsigned long) << 3) - 1; i >= 0; --i)
				out += (tail & (1 << i)) ? "1" : "0";
			return out;
		}

		inline Int operator~() const {
			Int out(!prefix, data, ~tail);
			for (auto& it : out.data)
				it = ~it;
			return out;
		}

		inline friend bool operator==(const Int& q, const Int& p);


	private:
		unsigned long tail;
		std::vector<unsigned long> data;
		bool prefix;
		Int(const bool& prefix, const std::vector<unsigned long>& data, const unsigned long& tail) :tail(tail), data(data), prefix(prefix) {}
		Int(const bool& prefix, std::vector<unsigned long>&& data, const unsigned long& tail) :tail(tail), data(std::move(data)), prefix(prefix) {}
	};
}
inline bool NumExpand::operator==(const Int& q, const Int& p)
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