// GCC 15.1.0 workaround for missing container adapter formatters
// Not a standalone header file
//
// Usage:  #include in module purview fragment
//         <version> must be #included before including this file
#pragma once


#if defined( __GLIBCXX__ )                                                            // If GNU's libstdc++ library (GCC or Clang)
  #if ( ( __GLIBC__ << 16 ) + __GLIBC_MINOR__ <= ( 2 << 16 ) + 39 )                   // ldd 2.39 and gcc 15.1.0  (run "ldd --version" and "gcc --version" at the command line)
    namespace std                                                                     // Check lib version and not gcc version, clang may use gnu's libstdc++ library
    {
      // Generic formatter for any adapter exposing a protected container `c`
      template<typename Adaptor, typename Container, typename T>
      requires std::formattable<T, char>
      struct adaptor_formatter : std::range_formatter<T>
      {
        auto format( const Adaptor & adapter, auto & ctx ) const
        {
          struct Accessor : Adaptor                                                   // Derive a class so we can access the protected container member
          {
            static const Container & get( const Adaptor & a )                         // returns a reference to the underlying container
            { return a.*&Accessor::c; }                                               // c is a protected member of the adapter, accessible to derived classes
          };
          return std::range_formatter<T>::format( Accessor::get( adapter ), ctx );
        }
      };

      template<typename T, typename Container>
      struct formatter<stack<T, Container>> : adaptor_formatter<stack<T, Container>, Container, T>
      {};

      template<typename T, typename Container>
      struct formatter<queue<T, Container>> : adaptor_formatter<queue<T, Container>, Container, T>
      {};

      template<typename T, typename Container, typename Compare>
      struct formatter<priority_queue<T, Container, Compare>> : adaptor_formatter<priority_queue<T, Container, Compare>, Container, T>
      {};
    }    // namespace std
  #else
    // gcc: fixed in ldd 2.41 (g++ version 15.1.1), don't know about 2.40
    #pragma message ("A potentially obsolete C++23 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it")
  #endif
#endif
