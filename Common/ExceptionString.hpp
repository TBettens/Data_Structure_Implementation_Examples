/***********************************************************************************************************************************
** Common class and functions used in the CSUF 131 Implementation Examples
***********************************************************************************************************************************/
#pragma once

#include <format>                                                           // format()
#include <source_location>                                                  // source_location, current()
#include <string>                                                           // string
#include <string_view>                                                      // string_view

#include <version>                                                          // defines feature-test macros, __cpp_lib_stacktrace
#if defined( __cpp_lib_stacktrace )                                         // Clang 21 does not yet support std::stacktrace.
  #include <stacktrace>                                                     // stacktrace
#endif

namespace CSUF::CPSC131
{
  // Usage:  exceptionString( "your message" );
  //         Let the second paramter default to the current source code location.  It would be unusual to provide anything different.
  //         The default location is captured at the call site, not here.
  std::string exceptionString( const std::string_view message, const std::source_location location = std::source_location::current() )
  {
    return std::format( "{}\n detected in function \"{}\"\n"
                        " at line {}\n"
                        " in file \"{}\"\n\n"
                        "********* Begin Stack Trace *********\n"
                        "{}\n"
                        "********* End Stack Trace *********\n",

                        message,
                        location.function_name(), location.line(), location.file_name(),

                        #ifdef __cpp_lib_stacktrace
                          std::stacktrace::current( 1 )                     // Let's not show this function in the trace, so skip 1
                        #else
                          "  Stack trace not available"
                        #endif
                      );
  }
}  // CSUF::CPSC131












/***********************************************************************************************************************************
** (C) Copyright 2025 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The participating authors at California State University's Computer Science Department have used their best efforts
** in preparing this code. These efforts include the development, research, and testing of the theories and programs to determine
** their effectiveness. The authors make no warranty of any kind, expressed or implied, with regard to these programs or to the
** documentation contained within. The authors shall not be liable in any event for incidental or consequential damages in
** connection with, or arising out of, the furnishing, performance, or use of these libraries and programs.  Distribution without
** written consent from the authors is prohibited.
***********************************************************************************************************************************/

/**************************************************
** Last modified:  08-JUN-2025  (added constexpr and noexcept to iterators)
** Last Verified:  11-JUN-2025
** Verified with:  MS Visual Studio 2022 Version 17.14.4,  Compiler Version 19.44.35209 (C++latest)
**                 GCC version 15.1.0 (-std=c++23 ),
**                 Clang version 21.0.0 (-std=c++23 -stdlib=libc++)
***************************************************/
