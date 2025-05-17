#ifndef OPTION_H
#define OPTION_H

#include "Main.h"

template<typename T>
class Option {
public:
	explicit Option() : m_Variant(unit()) {
	}

	Option(const T& some) : m_Variant(unit()) {
		m_Variant.template emplace<1>(some);
	}

	Option(T&& some) : m_Variant(unit()) {
		m_Variant.template emplace<1>(std::move(some));
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

	const T& some() const {
		assert(this->isSome());
		return std::get<1>(m_Variant);
	}

private:
	std::variant<unit, T> m_Variant;
};

template<typename T>
class Option<T*> {
public:
	explicit Option() : m_Ptr(nullptr) {
	}

	static Option newNone() {
		return Option();
	}

	static Option newSome(T* val) {
		Option opt = Option();
		opt.m_Ptr = val;
		return opt;
	}

	bool isNone() const {
		return m_Ptr == nullptr;
	}

	bool isSome() const {
		return m_Ptr != nullptr;
	}

	const T* some() const {
		assert(this->isSome());
		return m_Ptr;
	}

private:
	T* m_Ptr;
};

#endif // OPTION_H