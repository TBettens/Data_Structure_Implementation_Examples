#pragma once

// need to include something from the standard library so _LIBCPP_VERSION get defined, so don't move
// these #includes under that conditional compile
#include <compare>
#include <cmath>            // abs()
#include <string>
#include <type_traits>      // is_floating_point<>, decay<>




// C++20 Transition Workaround
//
// Clang (well, more accurately libc++) 13.0.* and before doesn't know how to three-way compare strings yet.
// Update this section as new versions of Clang are released that still do not implement these capabilities.
#if defined( _LIBCPP_VERSION )
#  if _LIBCPP_VERSION < 14'000
     namespace std
     {
       inline strong_ordering operator<=>( const string & lhs, const string & rhs)
       {
         int result = lhs.compare( rhs );
         return result == 0  ?  strong_ordering::equivalent
              : result  < 0  ?  strong_ordering::less
              :                 strong_ordering::greater;
       }


       template<class T1, class T2>
       constexpr weak_ordering  compare_weak_order_fallback( T1 && lhs, T2 && rhs ) noexcept
       {
         bool is_equal;
         if constexpr( std::is_floating_point_v<std::decay_t<T1>>  &&  std::is_floating_point_v<decay_t<T2>> )   is_equal = std::abs( lhs - rhs ) < 1e-9;
         else                                                                                                    is_equal = lhs == rhs;

         return is_equal    ?  weak_ordering::equivalent
              : lhs  < rhs  ?  weak_ordering::less
              :                weak_ordering::greater;
       }

     }
#  else
#    pragma message ("A potentially obsolete C++20 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it")
#  endif
#endif // defined( _LIBCPP_VERSION )
