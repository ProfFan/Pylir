//  Copyright 2022 Markus Böck
//
//  Licensed under the Apache License v2.0 with LLVM Exceptions.
//  See https://llvm.org/LICENSE.txt for license information.
//  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#pragma once

#include <pylir/Support/Macros.hpp>

#include <type_traits>
#include <utility>

namespace pylir::Diag
{
/// This class templates allows for arbitrary types to provide the location of where a diagnostic should go within the
/// diagnostics subsystem. Types opt into this behaviour by providing a template specialization for 'LocationProvider'
/// and the implementing specific methods. These are:
///
/// `static std::pair<std::size_t, std::size_t> getRange` callable with `T` OR 'T' and 'void*':
///     This method HAS to be implemented and returns the source of range of T via two indices. These refer to the
///     offset of the UTF32 text within the `Document` instance of the file `T` has been parsed in.
///     The first index refers to the inclusive start of 'T', while the second index refers to the exclusive end of 'T'
///     (in other words, the first index, that is not part of T. This may be one past the very last element in the
///      document).
///
///     Additionally, one may optionally add a second 'void*' parameter as context which is the optional argument
///     passed as the second argument to 'rangeLoc'. It's purpose is for 'T's that are not capable of getting source
///     locations themselves to get some kind of context. Since this requires close collaboration with the caller of
///     'rangeLoc' it should be done with great care.
///
/// 'static std::size_t getPoint' callable with 'T':
///     This method maybe OPTIONALLY implemented at which point it may be called 'pointLoc'. It's purpose is for 'T'
///     which are very large (in terms of source range) or otherwise hard to look at, to provide a specific location
///     by which they are most recognizable. For a function or class in the Python AST eg. this may be their name.
///     If this function is not implemented, then 'pointLoc' will instead use the start position returned by 'getRange'.
template <class T, class = void>
struct LocationProvider;

template <class T>
struct LocationProvider<T, std::enable_if_t<std::is_integral_v<T>>>
{
    static std::pair<std::size_t, std::size_t> getRange(T value) noexcept
    {
        return {value, value + 1};
    }
};

template <class U, class V>
struct LocationProvider<std::pair<U, V>, std::enable_if_t<std::is_integral_v<U> && std::is_integral_v<V>>>
{
    static std::pair<std::size_t, std::size_t> getRange(std::pair<U, V> value) noexcept
    {
        return value;
    }
};

template <class T, class = void>
struct hasLocationProviderRangeOneArg : std::false_type
{
};

template <class T>
struct hasLocationProviderRangeOneArg<
    T, std::void_t<decltype(LocationProvider<std::decay_t<T>>::getRange(std::declval<std::decay_t<T>>()))>>
    : std::true_type
{
};

template <class T, class = void>
struct hasLocationProviderRangeTwoArg : std::false_type
{
};

template <class T>
struct hasLocationProviderRangeTwoArg<T, std::void_t<decltype(LocationProvider<std::decay_t<T>>::getRange(
                                             std::declval<std::decay_t<T>>(), std::declval<const void*>()))>>
    : std::true_type
{
};

/// Convenient function for calling the 'LocationProvider::getRange' specialization of 'value'. If the type of 'value'
/// does not have a specialization of 'LocationProvider::getRange' it triggers a static assertion. A second argument
/// may optionally be specified which will then lead to an attempt at calling `getRange` with it as second argument
/// if it exists. It serves as context for the 'getRange' implementation of 'value'. In any other case, including when
/// 'context' is nullptr, 'getRange' is called with just 'value'.
template <class T>
std::pair<std::size_t, std::size_t> rangeLoc(const T& value, const void* context = nullptr)
{
    if constexpr (hasLocationProviderRangeTwoArg<T>{})
    {
        if (context)
        {
            return LocationProvider<T>::getRange(value, context);
        }
    }
    if constexpr (hasLocationProviderRangeOneArg<T>{})
    {
        return LocationProvider<T>::getRange(value);
    }
    else
    {
        static_assert(hasLocationProviderRangeTwoArg<T>{}, "No LocationProvider<T>::getRange implementation found");
        PYLIR_UNREACHABLE;
    }
}

template <class T, class = void>
struct hasLocationProviderPointer : std::false_type
{
};

template <class T>
struct hasLocationProviderPointer<
    T, std::void_t<decltype(LocationProvider<std::decay_t<T>>::getPoint(std::declval<std::decay_t<T>>()))>>
    : std::true_type
{
};

/// Convenient function for calling the 'LocationProvider::getPoint' specialization of 'value' if it exists. If it does
/// not exist, a default implementation is called, which returns the starting index of 'value's 'getRange'
/// implementation.
template <class T>
std::size_t pointLoc(const T& value)
{
    if constexpr (hasLocationProviderPointer<T>{})
    {
        return LocationProvider<T>::getPoint(value);
    }
    else
    {
        return rangeLoc(value).first;
    }
}

template <class T>
constexpr bool hasLocationProvider_v =
    std::disjunction_v<hasLocationProviderRangeOneArg<T>, hasLocationProviderRangeTwoArg<T>>;

} // namespace pylir::Diag
