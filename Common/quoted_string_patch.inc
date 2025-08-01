// GCC 15.1.0 workaround for missing operator>>(istream &, std::quoted(text))
// Not a standalone header file
//
// Usage:  replace std::quoted(...) with quoted(...)
//         <version> must be #included before including this file
#pragma once


struct quoted
{
  std::string & _text;


  quoted( std::string & text )
    : _text{ text }
  {}


  std::istream & operator()( std::istream & s )
  {
    #if defined( __GLIBCXX__ )                                               // If GNU's libstdc++ library (GCC or Clang)
      #if ( ( __GLIBC__ << 16 ) + __GLIBC_MINOR__ <= ( 2 << 16 ) + 39 )      // ldd 2.39 and gcc 15.1.0  (run "ldd --version" and "gcc --version" at the command line)
        const char delim  = '"';
        const char escape = '\\';
        char       c;

        s >> c;
        if( c != delim )
        {
          s.unget();
          s >> _text;
          return s;
        }

        _text.clear();
        auto flags = s.flags( s.flags() & ~std::ios_base::skipws );          // save stream state and then set "do not skip white space"
        do
        {
          s >> c;
          if( c == escape ) s >> c;
          else if( c == delim ) break;
          _text += c;
        } while( ( true ) );

        s.setf( flags );
      #else
        #pragma message( "A potentially obsolete C++23 workaround is present.  Either remove the workaround if no longer needed, or update the version number requiring it" )
      #endif
    #else
        s >> std::quoted( _text );
    #endif
    return s;
  }
};

std::istream & operator>>( std::istream & s, quoted && obj )
{
  return obj( s );
}
