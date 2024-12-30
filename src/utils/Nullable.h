#ifndef NULLABLE_H
#define NULLABLE_H

#include <exception>

// TODO: Deprecate in favour of Option?

template <typename T>
class Nullable {
public:
	explicit Nullable(T* value = nullptr) : m_Value(value) {}
	~Nullable() {}

	T* unwrap() const {
		if(m_Value == nullptr) {
			std::terminate();
		} else {
			return m_Value;
		}
	}

	T* unwrapOr(T* defaultVal) const {
		return m_Value == nullptr ? defaultVal : m_Value;
	}

	T* unwrapUnsafe() const {
		return m_Value;
	}

	bool isNotNull() const {
		return m_Value != nullptr;
	}

	bool isNull() const {
		return m_Value == nullptr;
	}

private:
	T* m_Value;
};

#endif // NULLABLE_H