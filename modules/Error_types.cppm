module;

#include <variant>
#include <string>
#include <expected>

export module error_t;

export {
struct InternalError {
	enum Type {
		SaveCorrupt,
		SaveNotFound,
		SaveWriteFailed,
		SaveReadFailed,
		SaveVersionMismatch
	};

	Type type;
};

struct ParseError {
	enum Type {
		EmptyLine,
		TooManyArgs
	};

	Type type;
};

struct NoSuchCmd {
	std::string cmd;
};

struct ArgumentError {
	unsigned long long expected;
	unsigned long long got;
};

struct NoSuchAlpaca {
	std::string name;
};

struct InvalidNumber {
	enum Type {
		NotANumber,
		TrailingChar,
		Zero,
		Negative,
		TooBig
	};

	Type type;
};

struct NoMoney {
	unsigned long long needed;
	unsigned long long got;
};

struct NoSuchAchievement {
	std::string name;
};

struct AlpacaAlreadyExist{
	std::string name;
};

struct AlreadyDaily {};

struct GameEnd {};

using Error = std::variant<
	InternalError,
	ParseError,
	NoSuchCmd,
	ArgumentError,
	NoSuchAlpaca,
	InvalidNumber,
	NoMoney,
	NoSuchAchievement,
	AlpacaAlreadyExist,
	AlreadyDaily,
	GameEnd
>;
}

template <typename T>
struct error_type {
	using Errtype = T;
};

template <>
struct error_type <InternalError::Type> {
	using Errtype = InternalError;
};

template <>
struct error_type <InvalidNumber::Type> {
	using Errtype = InvalidNumber;
};

template <>
struct error_type <ParseError::Type> {
	using Errtype = ParseError;
};

template<typename T>
using error_type_t = typename error_type<T>::Errtype;

export
template <typename T>
constexpr std::unexpected<Error> error (T x) {
	return std::unexpected(Error{error_type_t<T>{x}});
}