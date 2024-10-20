#pragma once

namespace gmlib::utils::FunctionTrait {

namespace details {

template <int>
struct Index {};

template <int, typename... Ts>
struct TypesImpl;

template <int i, typename T, typename... Ts>
struct TypesImpl<i, T, Ts...> : TypesImpl<i + 1, Ts...> {
protected:
    static T get(Index<i>);
    using TypesImpl<i + 1, Ts...>::get;
};

template <int i, typename T>
struct TypesImpl<i, T> {
protected:
    static T get(Index<i>);

public:
    constexpr static int count = i + 1;
};

template <typename T>
struct TypeContainer {
    using Type = T;
};

} // namespace details

template <typename... Ts>
struct Types : details::TypesImpl<0, Ts...> {
    template <int i>
    struct Container {
        static_assert(i < details::TypesImpl<0, Ts...>::count, "Invalid index");
        using Type = decltype(details::TypesImpl<0, Ts...>::get(details::Index<i>{}));
    };
};

template <>
struct Types<> {
    constexpr static int count = 0;
    template <int>
    struct Container {
        static_assert(false, "Invalid index");
    };
};

template <typename T>
struct FunctionTrait;

template <typename R, typename... A>
struct FunctionTrait<R (*)(A...)> {
    using RetType                         = R;
    using ArgTypes                        = Types<A...>;
    constexpr static bool IsFunctor       = false;
    constexpr static bool IsConst         = false;
    constexpr static bool IsVolatile      = false;
    constexpr static bool IsConstVolatile = IsConst && IsVolatile;
};

template <typename R, typename T, typename... A>
struct FunctionTrait<R (T::*)(A...)> {
    using RetType                         = R;
    using ArgTypes                        = Types<A...>;
    using ClassType                       = T;
    constexpr static bool IsFunctor       = true;
    constexpr static bool IsConst         = false;
    constexpr static bool IsVolatile      = false;
    constexpr static bool IsConstVolatile = IsConst && IsVolatile;
};

template <typename R, typename T, typename... A>
struct FunctionTrait<R (T::*)(A...) const> {
    using RetType                         = R;
    using ArgTypes                        = Types<A...>;
    using ClassType                       = T;
    constexpr static bool IsFunctor       = true;
    constexpr static bool IsConst         = true;
    constexpr static bool IsVolatile      = false;
    constexpr static bool IsConstVolatile = IsConst && IsVolatile;
};

template <typename R, typename T, typename... A>
struct FunctionTrait<R (T::*)(A...) volatile> {
    using RetType                         = R;
    using ArgTypes                        = Types<A...>;
    using ClassType                       = T;
    constexpr static bool IsFunctor       = true;
    constexpr static bool IsConst         = false;
    constexpr static bool IsVolatile      = true;
    constexpr static bool IsConstVolatile = IsConst && IsVolatile;
};

template <typename R, typename T, typename... A>
struct FunctionTrait<R (T::*)(A...) const volatile> {
    using RetType                         = R;
    using ArgTypes                        = Types<A...>;
    using ClassType                       = T;
    constexpr static bool IsFunctor       = true;
    constexpr static bool IsConst         = true;
    constexpr static bool IsVolatile      = true;
    constexpr static bool IsConstVolatile = IsConst && IsVolatile;
};

} // namespace gmlib::utils::FunctionTrait