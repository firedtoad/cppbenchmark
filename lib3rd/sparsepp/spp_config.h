#if !defined(spp_config_h_guard)
#define spp_config_h_guard

// --------------------------------------------------
// Sparsepp config macros
// some can be overriden on the command line
// --------------------------------------------------
#ifndef SPP_NAMESPACE
#define SPP_NAMESPACE spp
#endif

#ifndef spp_
#define spp_ SPP_NAMESPACE
#endif

#ifndef SPP_DEFAULT_ALLOCATOR
#if (defined(SPP_USE_SPP_ALLOC) && SPP_USE_SPP_ALLOC) && defined(_MSC_VER)
// -----------------------------------------------------------------------------
// When building with the Microsoft compiler, we use a custom allocator because
// the default one fragments memory when reallocating. This is desirable only
// when creating large sparsepp hash maps. If you create lots of small hash_maps,
// define the following before including spp.h:
//     #define SPP_DEFAULT_ALLOCATOR spp::libc_allocator
// -----------------------------------------------------------------------------
#define SPP_DEFAULT_ALLOCATOR spp_::spp_allocator
#define SPP_INCLUDE_SPP_ALLOC
#else
#define SPP_DEFAULT_ALLOCATOR spp_::libc_allocator
#endif
#endif

#ifndef SPP_GROUP_SIZE
// must be 32 or 64
#define SPP_GROUP_SIZE 32
#endif

#ifndef SPP_ALLOC_SZ
// must be power of 2 (0 = agressive alloc, 1 = smallest memory usage, 2 = good compromise)
#define SPP_ALLOC_SZ 0
#endif

#ifndef SPP_STORE_NUM_ITEMS
// 1 uses a little bit more memory, but faster!!
#define SPP_STORE_NUM_ITEMS 1
#endif

// ---------------------------------------------------------------------------
// Compiler detection code (SPP_ proprocessor macros) derived from Boost
// libraries. Therefore Boost software licence reproduced below.
// ---------------------------------------------------------------------------
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// ---------------------------------------------------------------------------

// Boost like configuration
// ------------------------
#if defined __clang__

#if defined(i386)
#include <cpuid.h>
inline void spp_cpuid(int info[4], int InfoType)
{
    __cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
}
#endif

#define SPP_POPCNT __builtin_popcount
#define SPP_POPCNT64 __builtin_popcountll

#define SPP_HAS_CSTDINT

#ifndef __has_extension
#define __has_extension __has_feature
#endif

#if !__has_feature(cxx_exceptions) && !defined(SPP_NO_EXCEPTIONS)
#define SPP_NO_EXCEPTIONS
#endif

#if !__has_feature(cxx_rtti) && !defined(SPP_NO_RTTI)
#define SPP_NO_RTTI
#endif

#if !__has_feature(cxx_rtti) && !defined(SPP_NO_TYPEID)
#define SPP_NO_TYPEID
#endif

#if defined(__int64) && !defined(__GNUC__)
#define SPP_HAS_MS_INT64
#endif

#define SPP_HAS_NRVO

// Branch prediction hints
#if defined(__has_builtin)
#if __has_builtin(__builtin_expect)
#define SPP_LIKELY(x) __builtin_expect(x, 1)
#define SPP_UNLIKELY(x) __builtin_expect(x, 0)
#endif
#endif

// Clang supports "long long" in all compilation modes.
#define SPP_HAS_LONG_LONG

#if !__has_feature(cxx_constexpr)
#define SPP_NO_CXX11_CONSTEXPR
#endif

#if !__has_feature(cxx_decltype)
#define SPP_NO_CXX11_DECLTYPE
#endif

#if !__has_feature(cxx_decltype_incomplete_return_types)
#define SPP_NO_CXX11_DECLTYPE_N3276
#endif

#if !__has_feature(cxx_defaulted_functions)
#define SPP_NO_CXX11_DEFAULTED_FUNCTIONS
#endif

#if !__has_feature(cxx_deleted_functions)
#define SPP_NO_CXX11_DELETED_FUNCTIONS
#endif

#if !__has_feature(cxx_explicit_conversions)
#define SPP_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#endif

#if !__has_feature(cxx_default_function_template_args)
#define SPP_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#endif

#if !__has_feature(cxx_generalized_initializers)
#define SPP_NO_CXX11_HDR_INITIALIZER_LIST
#endif

#if !__has_feature(cxx_lambdas)
#define SPP_NO_CXX11_LAMBDAS
#endif

#if !__has_feature(cxx_local_type_template_args)
#define SPP_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#endif

#if !__has_feature(cxx_raw_string_literals)
#define SPP_NO_CXX11_RAW_LITERALS
#endif

#if !__has_feature(cxx_reference_qualified_functions)
#define SPP_NO_CXX11_REF_QUALIFIERS
#endif

#if !__has_feature(cxx_generalized_initializers)
#define SPP_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#endif

#if !__has_feature(cxx_rvalue_references)
#define SPP_NO_CXX11_RVALUE_REFERENCES
#endif

#if !__has_feature(cxx_static_assert)
#define SPP_NO_CXX11_STATIC_ASSERT
#endif

#if !__has_feature(cxx_alias_templates)
#define SPP_NO_CXX11_TEMPLATE_ALIASES
#endif

#if !__has_feature(cxx_variadic_templates)
#define SPP_NO_CXX11_VARIADIC_TEMPLATES
#endif

#if !__has_feature(cxx_user_literals)
#define SPP_NO_CXX11_USER_DEFINED_LITERALS
#endif

#if !__has_feature(cxx_alignas)
#define SPP_NO_CXX11_ALIGNAS
#endif

#if !__has_feature(cxx_trailing_return)
#define SPP_NO_CXX11_TRAILING_RESULT_TYPES
#endif

#if !__has_feature(cxx_inline_namespaces)
#define SPP_NO_CXX11_INLINE_NAMESPACES
#endif

#if !__has_feature(cxx_override_control)
#define SPP_NO_CXX11_FINAL
#endif

#if !(__has_feature(__cxx_binary_literals__) || __has_extension(__cxx_binary_literals__))
#define SPP_NO_CXX14_BINARY_LITERALS
#endif

#if !__has_feature(__cxx_decltype_auto__)
#define SPP_NO_CXX14_DECLTYPE_AUTO
#endif

#if !__has_feature(__cxx_init_captures__)
#define SPP_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
#endif

#if !__has_feature(__cxx_generic_lambdas__)
#define SPP_NO_CXX14_GENERIC_LAMBDAS
#endif

#if !__has_feature(__cxx_generic_lambdas__) || !__has_feature(__cxx_relaxed_constexpr__)
#define SPP_NO_CXX14_CONSTEXPR
#endif

#if !__has_feature(__cxx_return_type_deduction__)
#define SPP_NO_CXX14_RETURN_TYPE_DEDUCTION
#endif

#if !__has_feature(__cxx_variable_templates__)
#define SPP_NO_CXX14_VARIABLE_TEMPLATES
#endif

#if __cplusplus < 201400
#define SPP_NO_CXX14_DIGIT_SEPARATORS
#endif

#if defined(__has_builtin) && __has_builtin(__builtin_unreachable)
#define SPP_UNREACHABLE_RETURN(x) __builtin_unreachable();
#endif

#define SPP_ATTRIBUTE_UNUSED __attribute__((__unused__))

#ifndef SPP_COMPILER
#define SPP_COMPILER "Clang version " __clang_version__
#endif

#define SPP_CLANG 1

#elif defined __GNUC__

#define SPP_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

//  definition to expand macro then apply to pragma message
// #define VALUE_TO_STRING(x) #x
// #define VALUE(x) VALUE_TO_STRING(x)
// #define VAR_NAME_VALUE(var) #var "="  VALUE(var)
// #pragma message(VAR_NAME_VALUE(SPP_GCC_VERSION))

#if defined(i386)
#include <cpuid.h>
inline void spp_cpuid(int info[4], int InfoType)
{
    __cpuid_count(InfoType, 0, info[0], info[1], info[2], info[3]);
}
#endif

// __POPCNT__ defined when the compiled with popcount support
// (-mpopcnt compiler option is given for example)
#ifdef __POPCNT__
// slower unless compiled iwith -mpopcnt
#define SPP_POPCNT __builtin_popcount
#define SPP_POPCNT64 __builtin_popcountll
#endif

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || (__cplusplus >= 201103L)
#define SPP_GCC_CXX11
#endif

#if __GNUC__ == 3
#if defined(__PATHSCALE__)
#define SPP_NO_TWO_PHASE_NAME_LOOKUP
#define SPP_NO_IS_ABSTRACT
#endif

#if __GNUC_MINOR__ < 4
#define SPP_NO_IS_ABSTRACT
#endif

#define SPP_NO_CXX11_EXTERN_TEMPLATE
#endif

#if __GNUC__ < 4
//
// All problems to gcc-3.x and earlier here:
//
#define SPP_NO_TWO_PHASE_NAME_LOOKUP
#ifdef __OPEN64__
#define SPP_NO_IS_ABSTRACT
#endif
#endif

// GCC prior to 3.4 had     #pragma once too but it didn't work well with filesystem links
#if SPP_GCC_VERSION >= 30400
#define SPP_HAS_PRAGMA_ONCE
#endif

#if SPP_GCC_VERSION < 40400
// Previous versions of GCC did not completely implement value-initialization:
// GCC Bug 30111, "Value-initialization of POD base class doesn't initialize
// members", reported by Jonathan Wakely in 2006,
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=30111 (fixed for GCC 4.4)
// GCC Bug 33916, "Default constructor fails to initialize array members",
// reported by Michael Elizabeth Chastain in 2007,
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=33916 (fixed for GCC 4.2.4)
// See also: http://www.boost.org/libs/utility/value_init.htm    #compiler_issues
#define SPP_NO_COMPLETE_VALUE_INITIALIZATION
#endif

#if !defined(__EXCEPTIONS) && !defined(SPP_NO_EXCEPTIONS)
#define SPP_NO_EXCEPTIONS
#endif

//
// Threading support: Turn this on unconditionally here (except for
// those platforms where we can know for sure). It will get turned off again
// later if no threading API is detected.
//
#if !defined(__MINGW32__) && !defined(linux) && !defined(__linux) && !defined(__linux__)
#define SPP_HAS_THREADS
#endif

//
// gcc has "long long"
// Except on Darwin with standard compliance enabled (-pedantic)
// Apple gcc helpfully defines this macro we can query
//
#if !defined(__DARWIN_NO_LONG_LONG)
#define SPP_HAS_LONG_LONG
#endif

//
// gcc implements the named return value optimization since version 3.1
//
#define SPP_HAS_NRVO

// Branch prediction hints
#define SPP_LIKELY(x) __builtin_expect(x, 1)
#define SPP_UNLIKELY(x) __builtin_expect(x, 0)

//
// Dynamic shared object (DSO) and dynamic-link library (DLL) support
//
#if __GNUC__ >= 4
#if (defined(_WIN32) || defined(__WIN32__) || defined(WIN32)) && !defined(__CYGWIN__)
// All Win32 development environments, including 64-bit Windows and MinGW, define
// _WIN32 or one of its variant spellings. Note that Cygwin is a POSIX environment,
// so does not define _WIN32 or its variants.
#define SPP_HAS_DECLSPEC
#define SPP_SYMBOL_EXPORT __attribute__((__dllexport__))
#define SPP_SYMBOL_IMPORT __attribute__((__dllimport__))
#else
#define SPP_SYMBOL_EXPORT __attribute__((__visibility__("default")))
#define SPP_SYMBOL_IMPORT
#endif

#define SPP_SYMBOL_VISIBLE __attribute__((__visibility__("default")))
#else
// config/platform/win32.hpp will define SPP_SYMBOL_EXPORT, etc., unless already defined
#define SPP_SYMBOL_EXPORT
#endif

//
// RTTI and typeinfo detection is possible post gcc-4.3:
//
#if SPP_GCC_VERSION > 40300
#ifndef __GXX_RTTI
#ifndef SPP_NO_TYPEID
#define SPP_NO_TYPEID
#endif
#ifndef SPP_NO_RTTI
#define SPP_NO_RTTI
#endif
#endif
#endif

//
// Recent GCC versions have __int128 when in 64-bit mode.
//
// We disable this if the compiler is really nvcc with C++03 as it
// doesn't actually support __int128 as of CUDA_VERSION=7500
// even though it defines __SIZEOF_INT128__.
// See https://svn.boost.org/trac/boost/ticket/8048
//     https://svn.boost.org/trac/boost/ticket/11852
// Only re-enable this for nvcc if you're absolutely sure
// of the circumstances under which it's supported:
//
#if defined(__CUDACC__)
#if defined(SPP_GCC_CXX11)
#define SPP_NVCC_CXX11
#else
#define SPP_NVCC_CXX03
#endif
#endif

#if defined(__SIZEOF_INT128__) && !defined(SPP_NVCC_CXX03)
#define SPP_HAS_INT128
#endif
//
// Recent GCC versions have a __float128 native type, we need to
// include a std lib header to detect this - not ideal, but we'll
// be including <cstddef> later anyway when we select the std lib.
//
// Nevertheless, as of CUDA 7.5, using __float128 with the host
// compiler in pre-C++11 mode is still not supported.
// See https://svn.boost.org/trac/boost/ticket/11852
//
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#if defined(_GLIBCXX_USE_FLOAT128) && !defined(__STRICT_ANSI__) && !defined(SPP_NVCC_CXX03)
#define SPP_HAS_FLOAT128
#endif

// C++0x features in 4.3.n and later
//
#if (SPP_GCC_VERSION >= 40300) && defined(SPP_GCC_CXX11)
// C++0x features are only enabled when -std=c++0x or -std=gnu++0x are
// passed on the command line, which in turn defines
// __GXX_EXPERIMENTAL_CXX0X__.
#define SPP_HAS_DECLTYPE
#define SPP_HAS_RVALUE_REFS
#define SPP_HAS_STATIC_ASSERT
#define SPP_HAS_VARIADIC_TMPL
#define SPP_HAS_CSTDINT
#else
#define SPP_NO_CXX11_DECLTYPE
#define SPP_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define SPP_NO_CXX11_RVALUE_REFERENCES
#define SPP_NO_CXX11_STATIC_ASSERT
#endif

// C++0x features in 4.4.n and later
//
#if (SPP_GCC_VERSION < 40400) || !defined(SPP_GCC_CXX11)
#define SPP_NO_CXX11_AUTO_DECLARATIONS
#define SPP_NO_CXX11_AUTO_MULTIDECLARATIONS
#define SPP_NO_CXX11_CHAR16_T
#define SPP_NO_CXX11_CHAR32_T
#define SPP_NO_CXX11_HDR_INITIALIZER_LIST
#define SPP_NO_CXX11_DEFAULTED_FUNCTIONS
#define SPP_NO_CXX11_DELETED_FUNCTIONS
#define SPP_NO_CXX11_TRAILING_RESULT_TYPES
#define SPP_NO_CXX11_INLINE_NAMESPACES
#define SPP_NO_CXX11_VARIADIC_TEMPLATES
#endif

#if SPP_GCC_VERSION < 40500
#define SPP_NO_SFINAE_EXPR
#endif

// GCC 4.5 forbids declaration of defaulted functions in private or protected sections
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ == 5) || !defined(SPP_GCC_CXX11)
#define SPP_NO_CXX11_NON_PUBLIC_DEFAULTED_FUNCTIONS
#endif

// C++0x features in 4.5.0 and later
//
#if (SPP_GCC_VERSION < 40500) || !defined(SPP_GCC_CXX11)
#define SPP_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#define SPP_NO_CXX11_LAMBDAS
#define SPP_NO_CXX11_LOCAL_CLASS_TEMPLATE_PARAMETERS
#define SPP_NO_CXX11_RAW_LITERALS
#endif

// C++0x features in 4.6.n and later
//
#if (SPP_GCC_VERSION < 40600) || !defined(SPP_GCC_CXX11)
#define SPP_NO_CXX11_CONSTEXPR
#define SPP_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#endif

// C++0x features in 4.7.n and later
//
#if (SPP_GCC_VERSION < 40700) || !defined(SPP_GCC_CXX11)
#define SPP_NO_CXX11_FINAL
#define SPP_NO_CXX11_TEMPLATE_ALIASES
#define SPP_NO_CXX11_USER_DEFINED_LITERALS
#define SPP_NO_CXX11_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS
#endif

// C++0x features in 4.8.n and later
//
#if (SPP_GCC_VERSION < 40800) || !defined(SPP_GCC_CXX11)
#define SPP_NO_CXX11_ALIGNAS
#endif

// C++0x features in 4.8.1 and later
//
#if (SPP_GCC_VERSION < 40801) || !defined(SPP_GCC_CXX11)
#define SPP_NO_CXX11_DECLTYPE_N3276
#define SPP_NO_CXX11_REF_QUALIFIERS
#define SPP_NO_CXX14_BINARY_LITERALS
#endif

// C++14 features in 4.9.0 and later
//
#if (SPP_GCC_VERSION < 40900) || (__cplusplus < 201300)
#define SPP_NO_CXX14_RETURN_TYPE_DEDUCTION
#define SPP_NO_CXX14_GENERIC_LAMBDAS
#define SPP_NO_CXX14_DIGIT_SEPARATORS
#define SPP_NO_CXX14_DECLTYPE_AUTO
#if !((SPP_GCC_VERSION >= 40801) && (SPP_GCC_VERSION < 40900) && defined(SPP_GCC_CXX11))
#define SPP_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
#endif
#endif

// C++ 14:
#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#define SPP_NO_CXX14_CONSTEXPR
#endif
#if !defined(__cpp_variable_templates) || (__cpp_variable_templates < 201304)
#define SPP_NO_CXX14_VARIABLE_TEMPLATES
#endif

//
// Unused attribute:
#if __GNUC__ >= 4
#define SPP_ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif
//
// __builtin_unreachable:
#if SPP_GCC_VERSION >= 40800
#define SPP_UNREACHABLE_RETURN(x) __builtin_unreachable();
#endif

#ifndef SPP_COMPILER
#define SPP_COMPILER "GNU C++ version " __VERSION__
#endif

// ConceptGCC compiler:
//   http://www.generic-programming.org/software/ConceptGCC/
#ifdef __GXX_CONCEPTS__
#define SPP_HAS_CONCEPTS
#define SPP_COMPILER "ConceptGCC version " __VERSION__
#endif

#elif defined _MSC_VER

#include <intrin.h> // for __popcnt()

#define SPP_POPCNT_CHECK // slower when defined, but we have to check!
#define spp_cpuid(info, x) __cpuid(info, x)

#define SPP_POPCNT __popcnt
#if (SPP_GROUP_SIZE == 64 && INTPTR_MAX == INT64_MAX)
#define SPP_POPCNT64 __popcnt64
#endif

// Attempt to suppress VC6 warnings about the length of decorated names (obsolete):
#pragma warning(disable : 4503) // warning: decorated name length exceeded

#define SPP_HAS_PRAGMA_ONCE
#define SPP_HAS_CSTDINT

//
// versions check:
// we don't support Visual C++ prior to version 7.1:
#if _MSC_VER < 1310
#error "Antique compiler not supported"
#endif

#if _MSC_FULL_VER < 180020827
#define SPP_NO_FENV_H
#endif

#if _MSC_VER < 1400
// although a conforming signature for swprint exists in VC7.1
// it appears not to actually work:
#define SPP_NO_SWPRINTF

// Our extern template tests also fail for this compiler:
#define SPP_NO_CXX11_EXTERN_TEMPLATE

// Variadic macros do not exist for VC7.1 and lower
#define SPP_NO_CXX11_VARIADIC_MACROS
#endif

#if _MSC_VER < 1500 // 140X == VC++ 8.0
#undef SPP_HAS_CSTDINT
#define SPP_NO_MEMBER_TEMPLATE_FRIENDS
#endif

#if _MSC_VER < 1600 // 150X == VC++ 9.0
// A bug in VC9:
#define SPP_NO_ADL_BARRIER
#endif

// MSVC (including the latest checked version) has not yet completely
// implemented value-initialization, as is reported:
// "VC++ does not value-initialize members of derived classes without
// user-declared constructor", reported in 2009 by Sylvester Hesp:
// https:    //connect.microsoft.com/VisualStudio/feedback/details/484295
// "Presence of copy constructor breaks member class initialization",
// reported in 2009 by Alex Vakulenko:
// https:    //connect.microsoft.com/VisualStudio/feedback/details/499606
// "Value-initialization in new-expression", reported in 2005 by
// Pavel Kuznetsov (MetaCommunications Engineering):
// https:    //connect.microsoft.com/VisualStudio/feedback/details/100744
// See also: http:    //www.boost.org/libs/utility/value_init.htm    #compiler_issues
// (Niels Dekker, LKEB, May 2010)
#define SPP_NO_COMPLETE_VALUE_INITIALIZATION

#ifndef _NATIVE_WCHAR_T_DEFINED
#define SPP_NO_INTRINSIC_WCHAR_T
#endif

//
// check for exception handling support:
#if !defined(_CPPUNWIND) && !defined(SPP_NO_EXCEPTIONS)
#define SPP_NO_EXCEPTIONS
#endif

//
// __int64 support:
//
#define SPP_HAS_MS_INT64
#if defined(_MSC_EXTENSIONS) || (_MSC_VER >= 1400)
#define SPP_HAS_LONG_LONG
#else
#define SPP_NO_LONG_LONG
#endif

#if (_MSC_VER >= 1400) && !defined(_DEBUG)
#define SPP_HAS_NRVO
#endif

#if _MSC_VER >= 1500 // 150X == VC++ 9.0
#define SPP_HAS_PRAGMA_DETECT_MISMATCH
#endif

//
// disable Win32 API's if compiler extensions are
// turned off:
//
#if !defined(_MSC_EXTENSIONS) && !defined(SPP_DISABLE_WIN32)
#define SPP_DISABLE_WIN32
#endif

#if !defined(_CPPRTTI) && !defined(SPP_NO_RTTI)
#define SPP_NO_RTTI
#endif

//
// TR1 features:
//
#if _MSC_VER >= 1700
//      #define SPP_HAS_TR1_HASH	// don't know if this is true yet.
//      #define SPP_HAS_TR1_TYPE_TRAITS	// don't know if this is true yet.
#define SPP_HAS_TR1_UNORDERED_MAP
#define SPP_HAS_TR1_UNORDERED_SET
#endif

//
// C++0x features
//
//   See above for SPP_NO_LONG_LONG

// C++ features supported by VC++ 10 (aka 2010)
//
#if _MSC_VER < 1600
#define SPP_NO_CXX11_AUTO_DECLARATIONS
#define SPP_NO_CXX11_AUTO_MULTIDECLARATIONS
#define SPP_NO_CXX11_LAMBDAS
#define SPP_NO_CXX11_RVALUE_REFERENCES
#define SPP_NO_CXX11_STATIC_ASSERT
#define SPP_NO_CXX11_DECLTYPE
#endif // _MSC_VER < 1600

#if _MSC_VER >= 1600
#define SPP_HAS_STDINT_H
#endif

// C++11 features supported by VC++ 11 (aka 2012)
//
#if _MSC_VER < 1700
#define SPP_NO_CXX11_FINAL
#endif // _MSC_VER < 1700

// C++11 features supported by VC++ 12 (aka 2013).
//
#if _MSC_FULL_VER < 180020827
#define SPP_NO_CXX11_DEFAULTED_FUNCTIONS
#define SPP_NO_CXX11_DELETED_FUNCTIONS
#define SPP_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS
#define SPP_NO_CXX11_FUNCTION_TEMPLATE_DEFAULT_ARGS
#define SPP_NO_CXX11_RAW_LITERALS
#define SPP_NO_CXX11_TEMPLATE_ALIASES
#define SPP_NO_CXX11_TRAILING_RESULT_TYPES
#define SPP_NO_CXX11_VARIADIC_TEMPLATES
#define SPP_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
#define SPP_NO_CXX11_DECLTYPE_N3276
#endif

// C++11 features supported by VC++ 14 (aka 2014) CTP1
#if (_MSC_FULL_VER < 190021730)
#define SPP_NO_CXX11_REF_QUALIFIERS
#define SPP_NO_CXX11_USER_DEFINED_LITERALS
#define SPP_NO_CXX11_ALIGNAS
#define SPP_NO_CXX11_INLINE_NAMESPACES
#define SPP_NO_CXX14_DECLTYPE_AUTO
#define SPP_NO_CXX14_INITIALIZED_LAMBDA_CAPTURES
#define SPP_NO_CXX14_RETURN_TYPE_DEDUCTION
#define SPP_NO_CXX11_HDR_INITIALIZER_LIST
#endif

// C++11 features not supported by any versions
#define SPP_NO_CXX11_CHAR16_T
#define SPP_NO_CXX11_CHAR32_T
#define SPP_NO_CXX11_CONSTEXPR
#define SPP_NO_SFINAE_EXPR
#define SPP_NO_TWO_PHASE_NAME_LOOKUP

// C++ 14:
#if !defined(__cpp_binary_literals) || (__cpp_binary_literals < 201304)
#define SPP_NO_CXX14_BINARY_LITERALS
#endif

#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#define SPP_NO_CXX14_CONSTEXPR
#endif

#if (__cplusplus < 201304) // There's no SD6 check for this....
#define SPP_NO_CXX14_DIGIT_SEPARATORS
#endif

#if !defined(__cpp_generic_lambdas) || (__cpp_generic_lambdas < 201304)
#define SPP_NO_CXX14_GENERIC_LAMBDAS
#endif

#if !defined(__cpp_variable_templates) || (__cpp_variable_templates < 201304)
#define SPP_NO_CXX14_VARIABLE_TEMPLATES
#endif

#endif

// from boost/config/suffix.hpp
// ----------------------------
#ifndef SPP_ATTRIBUTE_UNUSED
#define SPP_ATTRIBUTE_UNUSED
#endif

/*
  Try to persuade compilers to inline.
*/
#ifndef SPP_FORCEINLINE
#if defined(__GNUC__)
#define SPP_FORCEINLINE __inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define SPP_FORCEINLINE __forceinline
#else
#define SPP_FORCEINLINE inline
#endif
#endif

#endif // spp_config_h_guard
