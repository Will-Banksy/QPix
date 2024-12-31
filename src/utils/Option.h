#ifndef OPTION_H
#define OPTION_H

#include "Main.h"

// TODO: Test this

template<typename T>
class Option {
public:
	explicit Option() : m_Variant(unit()) {
	}

	static Option newNone() {
		return Option();
	}

	template<typename... Args>
	static Option newSome(Args&&... args) {
		Option opt = Option();
		opt.m_Variant.template emplace<1>(std::forward<Args>(args)...);
		return opt;
	}

	bool isNone() const {
		return m_Variant.index() == 0;
	}

	bool isSome() const {
		return m_Variant.index() == 1;
	}

	T& some() {
		assert(this->isSome());
		return std::get<1>(m_Variant);
	}

private:
	std::variant<unit, T> m_Variant;
};

#endif // OPTION_H