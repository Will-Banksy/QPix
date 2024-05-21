#ifndef NULLABLE_H
#define NULLABLE_H

#include <exception>

template <typename T>
class Nullable {
public:
	explicit Nullable(T* value = nullptr) : m_Value(value) {}
	~Nullable() {}

	T* unwrap() {
		if(m_Value == nullptr) {
			std::terminate();
		} else {
			return m_Value;
		}
	}

	T* unwrapOr(T* defaultVal) {
		return m_Value == nullptr ? defaultVal : m_Value;
	}

	T* unwrapUnsafe() {
		return m_Value;
	}

	bool isNotNull() {
		return m_Value != nullptr;
	}

	bool isNull() {
		return m_Value == nullptr;
	}

private:
	T* m_Value;
};

#endif // NULLABLE_H