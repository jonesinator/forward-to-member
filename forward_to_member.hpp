/**
 * This file provides two macros FORWARD_TO_MEMBER and FORWARD_TO_MEMBER_AS. The purpose of these
 * macros is to simplify the fairly common case below:
 *
 *     class foo
 *     {
 *     public:
 *         void method()
 *         {
 *             do_stuff();
 *         }
 *     };
 *
 *     class bar
 *     {
 *     private:
 *         foo f;
 *
 *     public:
 *         void method()
 *         {
 *             f.method();
 *         }
 *     };
 *
 * This pattern of exposing the public method of a member as a public method of the containing class
 * is a reasonably common occurence in certain systems. This is a simple example, but things become
 * more complicated as the method is overloaded and has different const/volatile specifiers. It is
 * the coders responsibility to make sure that the signature of the method in bar matches the
 * signature of the method in foo. The FORWARD_TO_MEMBER macro simplifies this:
 *
 *     class foo
 *     {
 *     public:
 *         void method()
 *         {
 *             do_stuff();
 *         }
 *     };
 *
 *     class bar
 *     {
 *     private:
 *         foo f;
 *
 *     public:
 *         FORWARD_TO_MEMBER(f, method);
 *     };
 *
 * Here FOWARD_TO_MEMBER is responsible for matching the signature in foo, and can deal with all
 * overloads of the method in foo. Unfortunately, given the amount of variadic templates and SFINAE
 * usage abuse of this macro could probably produce some pretty bad compile times.
 *
 * Another unfortunate aspect of this library is that the member must be declared before invoking
 * the FORWARD_TO_MEMBER macro. This disallows an often-used convention where the public methods are
 * at the top of the class and the members are at the bottom.
 */

#ifndef __INCLUDE_GUARD_FORWARD_MEMBER_HPP__
#define __INCLUDE_GUARD_FORWARD_MEMBER_HPP__

#include <memory>

namespace detail
{

/**
 * Base case for is_shared_ptr. Assume everything is not a shared pointer except for the
 * specializations defined below.
 */
template<typename T>
struct is_shared_ptr : public std::false_type { };

/**
 * Specialization allowing is_shared_ptr to correctly identify plain shared pointers.
 */
template<typename T>
struct is_shared_ptr<std::shared_ptr<T>> : public std::true_type { };

/**
 * Specialization allowing is_shared_ptr to correctly identify const shared pointers.
 */
template<typename T>
struct is_shared_ptr<const std::shared_ptr<T>> : public std::true_type { };

/**
 * Specialization allowing is_shared_ptr to correctly identify volatile shared pointers.
 */
template<typename T>
struct is_shared_ptr<volatile std::shared_ptr<T>> : public std::true_type { };

/**
 * Specialization allowing is_shared_ptr to correctly identify const volatile shared pointers.
 */
template<typename T>
struct is_shared_ptr<const volatile std::shared_ptr<T>> : public std::true_type { };

/**
 * Gets the "underlying" (i.e. raw, no const, no volatile, no pointer, and no reference modifiers on
 * the type. This is the general case.
 */
template<typename T>
struct forward_member_underlying_type
{
    using type = typename std::remove_cv<
                     typename std::remove_pointer<
                         typename std::remove_reference<T>::type>::type>::type;
};

/**
 * Gets the "underlying" (i.e. raw, no const, no volatile, no pointer, and no reference modifiers on
 * the type. This is the special case for shared pointers where we extract the pointed-to type.
 */
template<typename T>
struct forward_member_underlying_type<std::shared_ptr<T>>
{
    using type = typename std::remove_cv<
                     typename std::remove_pointer<
                         typename std::remove_reference<T>::type>::type>::type;
};

/**
 * Gets the "underlying" (i.e. raw, no const, no volatile, no pointer, and no reference modifiers on
 * the type. This is the special case for const shared pointers where we extract the pointed-to
 * type.
 */
template<typename T>
struct forward_member_underlying_type<const std::shared_ptr<T>>
{
    using type = typename std::remove_cv<
                     typename std::remove_pointer<
                         typename std::remove_reference<T>::type>::type>::type;
};

/**
 * Gets the "underlying" (i.e. raw, no const, no volatile, no pointer, and no reference modifiers on
 * the type. This is the special case for volatile shared pointers where we extract the pointed-to
 * type.
 */
template<typename T>
struct forward_member_underlying_type<volatile std::shared_ptr<T>>
{
    using type = typename std::remove_cv<
                     typename std::remove_pointer<
                         typename std::remove_reference<T>::type>::type>::type;
};

/**
 * Gets the "underlying" (i.e. raw, no const, no volatile, no pointer, and no reference modifiers on
 * the type. This is the special case for const volatile shared pointers where we extract the
 * pointed-to type.
 */
template<typename T>
struct forward_member_underlying_type<const volatile std::shared_ptr<T>>
{
    using type = typename std::remove_cv<
                     typename std::remove_pointer<
                         typename std::remove_reference<T>::type>::type>::type;
};

} /* End namespace detail. */

/**
 * Generates code which exposes a function in some class that invokes a method (potentially having
 * several overloads) on one of the class's members. The member can be a value, reference, pointer,
 * or shared_ptr with any combination of constness and volatileness. The exposed function will
 * inherit the constness and volatileness of the member's function so it can be correctly invoked on
 * const or volatile objects when needed.
 *
 * @param m The name of the member variable on which the function should be called.
 * @param f The name of the function to invoke on the member variable.
 * @param n The name of the function to expose in the class.
 */
#define FORWARD_TO_MEMBER_AS(m, f, n)                                                              \
    using member_type_##m##_##f##_##n = detail::forward_member_underlying_type<decltype(m)>::type; \
                                                                                                   \
    /**                                                                                            \
     * Helper class that can extract the function "traits" for a function with multiple overloads. \
     * Overload resolution is achieved through the variadic template arguments.                    \
     */                                                                                            \
    template <typename T, typename... TArgs>                                                       \
    class function_traits_##m##_##f##_##n                                                          \
    {                                                                                              \
    private:                                                                                       \
        /**                                                                                        \
         * Each type of function gets a struct with a unique size so we can do comparisons on the  \
         * size later.                                                                             \ 
         */                                                                                        \
        typedef struct { char pad[1]; } c_struct;                                                  \
        typedef struct { char pad[2]; } v_struct;                                                  \
        typedef struct { char pad[3]; } cv_struct;                                                 \
        typedef struct { char pad[4]; } p_struct;                                                  \
                                                                                                   \
        /**                                                                                        \
         * Overload for const member functions. If we can cast the member function to a            \
         * "retval (args...) const" then the function is a const function.                         \
         */                                                                                        \
        template <typename... UArgs>                                                               \
        static c_struct Check(decltype(static_cast<decltype(                                       \
            std::declval<T>().f(std::declval<UArgs>()...)) (T::*)(UArgs...) const>(&T::f),         \
            uint64_t()));                                                                          \
                                                                                                   \
        /**                                                                                        \
         * Overload for volatile member functions. If we can cast the member function to a         \
         * "retval (args...) volatile" then the function is a volatile function.                   \
         */                                                                                        \
        template <typename... UArgs>                                                               \
        static v_struct Check(decltype(static_cast<decltype(                                       \
            std::declval<T>().f(std::declval<UArgs>()...)) (T::*)(UArgs...) volatile>(&T::f),      \
            uint64_t()));                                                                          \
                                                                                                   \
        /**                                                                                        \
         * Overload for const volatile member functions. If we can cast the member function to a   \
         * "retval (args...) const volatile" then the function is a const volatile function.       \
         */                                                                                        \
        template <typename... UArgs>                                                               \
        static cv_struct Check(decltype(static_cast<decltype(                                      \
            std::declval<T>().f(std::declval<UArgs>()...)) (T::*)(UArgs...) const volatile>(&T::f),\
            uint64_t()));                                                                          \
                                                                                                   \
        /**                                                                                        \
         * Overload for undecorated (plain) member functions. If all of the above overloads fail   \
         * then this overload will be selected.                                                    \
         */                                                                                        \
        template <typename... UArgs>                                                               \
        static p_struct Check(...);                                                                \
                                                                                                   \
    public:                                                                                        \
        static constexpr bool is_plain     = sizeof(Check<TArgs...>(0)) == sizeof(p_struct);       \
        static constexpr bool is_const     = sizeof(Check<TArgs...>(0)) == sizeof(c_struct);       \
        static constexpr bool is_volatile  = sizeof(Check<TArgs...>(0)) == sizeof(v_struct);       \
        static constexpr bool is_cv        = sizeof(Check<TArgs...>(0)) == sizeof(cv_struct);      \
        static constexpr bool has_const    = sizeof(Check<TArgs...>(0)) == sizeof(c_struct) ||     \
                                             sizeof(Check<TArgs...>(0)) == sizeof(cv_struct);      \
        static constexpr bool has_volatile = sizeof(Check<TArgs...>(0)) == sizeof(v_struct) ||     \
                                             sizeof(Check<TArgs...>(0)) == sizeof(cv_struct);      \
    };                                                                                             \
                                                                                                   \
    /**                                                                                            \
     * Invoker overload for calling a non-const function on a value member.                        \
     */                                                                                            \
    template <typename T, typename... TArgs>                                                       \
    static auto invoke_##m##_##f##_##n(T& member, TArgs&&... args)                                 \
        -> typename std::enable_if<                                                                \
               !function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::has_const, \
               decltype(member.f(std::forward<TArgs>(args)...))>::type                             \
    {                                                                                              \
        return member.f(std::forward<TArgs>(args)...);                                             \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Invoker overload for calling a const function on a value member.                            \
     */                                                                                            \
    template <typename T, typename... TArgs>                                                       \
    static auto invoke_##m##_##f##_##n(const T& member, TArgs&&... args)                           \
        -> typename std::enable_if<                                                                \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::has_const,  \
               decltype(member.f(std::forward<TArgs>(args)...))>::type                             \
    {                                                                                              \
        return member.f(std::forward<TArgs>(args)...);                                             \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Invoker overload for calling a non-const function on a pointer member.                      \
     */                                                                                            \
    template <typename T, typename... TArgs>                                                       \
    static auto invoke_##m##_##f##_##n(T* member, TArgs&&... args)                                 \
        -> typename std::enable_if<                                                                \
               !function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::has_const, \
               decltype(member->f(std::forward<TArgs>(args)...))>::type                            \
    {                                                                                              \
        return member->f(std::forward<TArgs>(args)...);                                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Invoker overload for calling a const function on a pointer member.                          \
     */                                                                                            \
    template <typename T, typename... TArgs>                                                       \
    static auto invoke_##m##_##f##_##n(const T* member, TArgs&&... args)                           \
        -> typename std::enable_if<                                                                \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::has_const,  \
               decltype(member->f(std::forward<TArgs>(args)...))>::type                            \
    {                                                                                              \
        return member->f(std::forward<TArgs>(args)...);                                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Invoker overload for calling a non-const function on a shared pointer member.               \
     */                                                                                            \
    template <typename T, typename... TArgs>                                                       \
    static auto invoke_##m##_##f##_##n(const std::shared_ptr<T>& member, TArgs&&... args)          \
        -> typename std::enable_if<                                                                \
               !function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::has_const, \
               decltype(member->f(std::forward<TArgs>(args)...))>::type                            \
    {                                                                                              \
        return member->f(std::forward<TArgs>(args)...);                                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Invoker overload for calling a const function on a shared pointer member.                   \
     */                                                                                            \
    template <typename T, typename... TArgs>                                                       \
    static auto invoke_##m##_##f##_##n(const std::shared_ptr<T>& member, TArgs&&... args)          \
        -> typename std::enable_if<                                                                \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::has_const,  \
               decltype(member->f(std::forward<TArgs>(args)...))>::type                            \
    {                                                                                              \
        return member->f(std::forward<TArgs>(args)...);                                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Candidate for the function that actually gets exposed. This function will be selected if    \
     * the member function being invoked is neither const nor volatile.
     */                                                                                            \
    template <typename... TArgs>                                                                   \
    auto n(TArgs&&... args)                                                                        \
        -> typename std::enable_if<                                                                \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::is_plain,   \
               decltype(invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...))>::type            \
    {                                                                                              \
        return invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...);                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Candidate for the function that actually gets exposed. This function will be selected if    \
     * the member function being invoked is volatile and the member that the function is being     \
     * invoked on is NOT a shared_ptr. If the member is a shared_ptr then the function we expose   \
     * can't be volatile because shared_ptr<T>::get is not marked as volatile.                     \
     */                                                                                            \
    template <typename... TArgs>                                                                   \
    auto n(TArgs&&... args) volatile                                                               \
        -> typename std::enable_if<                                                                \
               !detail::is_shared_ptr<decltype(m)>::value &&                                       \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::is_volatile,\
               decltype(invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...))>::type            \
    {                                                                                              \
        return invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...);                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Candidate for the function that actually gets exposed. This function will be selected if    \
     * the member function being invoked is volatile and the member that the function is being     \
     * invoked on is a shared_ptr. In this case the function cannot be marked as volatile because  \
     * shared_ptr<T>::get is not marked as volatile.                                               \
     */                                                                                            \
    template <typename... TArgs>                                                                   \
    auto n(TArgs&&... args)                                                                        \
        -> typename std::enable_if<                                                                \
               detail::is_shared_ptr<decltype(m)>::value &&                                        \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::is_volatile,\
               decltype(invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...))>::type            \
    {                                                                                              \
        return invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...);                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Candidate for the function that actually gets exposed. This function will be selected if    \
     * the member function being invoked is const and not volatile. In this case we can mark the   \
     * function as const since it should be able to be invoked on a const object.                  \
     */                                                                                            \
    template <typename... TArgs>                                                                   \
    auto n(TArgs&&... args) const                                                                  \
        -> typename std::enable_if<                                                                \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::is_const,   \
               decltype(invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...))>::type            \
    {                                                                                              \
        return invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...);                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Candidate for the function that actually gets exposed. This function will be selected if    \
     * the member function being invoked is const volatile and the member that the function is     \
     * being invoked on is NOT a shared_ptr. If the member is a shared_ptr then the function we    \
     * exposed can't be volatile because shared_ptr<T>::get is not marked as volatile.             \
     */                                                                                            \
    template <typename... TArgs>                                                                   \
    auto n(TArgs&&... args) const volatile                                                         \
        -> typename std::enable_if<                                                                \
               !detail::is_shared_ptr<decltype(m)>::value &&                                       \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::is_cv,      \
               decltype(invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...))>::type            \
    {                                                                                              \
        return invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...);                            \
    }                                                                                              \
                                                                                                   \
    /**                                                                                            \
     * Candidate for the function that actually gets exposed. This function will be selected if    \
     * the member function being invoked is const volatile and the member that the function is     \
     * being invoked on is a shared_ptr. In this case the function cannot be marked as volatile    \
     * because shared_ptr<T>::get is not marked as volatile. The function can still be marked as   \
     * const.                                                                                      \
     */                                                                                            \
    template <typename... TArgs>                                                                   \
    auto n(TArgs&&... args) const                                                                  \
        -> typename std::enable_if<                                                                \
               detail::is_shared_ptr<decltype(m)>::value &&                                        \
               function_traits_##m##_##f##_##n<member_type_##m##_##f##_##n, TArgs...>::is_cv,      \
               decltype(invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...))>::type            \
    {                                                                                              \
        return invoke_##m##_##f##_##n(m, std::forward<TArgs>(args)...);                            \
    }                                                                                              \

/**
 * Same as FORWARD_TO_MEMBER_AS except the name of the exposed function is the same as the name of
 * the function being invoked on the member.
 */
#define FORWARD_TO_MEMBER(m, f) \
    FORWARD_TO_MEMBER_AS(m, f, f)

#endif /* __INCLUDE_GUARD_FORWARD_MEMBER_HPP__ */

