#include <format>                                                                 // format()
#include <iomanip>                                                                // quoted()
#include <iostream>                                                               // cout, cin, clog ostream, istream, print()
#include <string>                                                                 // string
#include <utility>                                                                // move()

#include "Student.hpp"








/*******************************************************************************
** Private working area for non-member functions, types, etc
*******************************************************************************/
namespace     // unnamed, anonymous namespace
{
  enum class Trace {On, Off};
  constexpr Trace tracing = Trace::Off;
}








namespace CSUF::CPSC131
{
  /*****************************************************************************
  ** Constructors, assignments, and destructor
  *****************************************************************************/
  // Default constructor
  //
  // Because at least one other constructor has been declared, the compiler won't synthesize the default constructor, so define it
  // explicitly
  Student::Student()
  {
    // Unlike a traditional "if" statement, "if constexpr" statement is evaluated at compile time instead of run time. Sense we know
    // at compile time if we want tracing on or off, we can optimize a little by using a compile time decision
    if constexpr( tracing == Trace::On )    std::print( std::clog, "+{:>30} {} - Default Constructor:  Student()\n", *this, this );
 }



  // Copy constructor
  Student::Student( Student const & original )
    : _name          ( original._name           ),
      _id            ( original._id             ),
      _numOfSemesters( original._numOfSemesters )
  {
    if constexpr( tracing == Trace::On )    std::print( std::clog, "+{:>30} {} - Copy Constructor:  Student( const Student & original )\n", *this, this );
  }



  // Move constructor
  Student::Student( Student && donor ) noexcept
    : _name          ( std::move(donor._name          ) ),
      _id            ( std::move(donor._id            ) ),
      _numOfSemesters( std::move(donor._numOfSemesters) )
  {
    if constexpr( tracing == Trace::On )    std::print( std::clog, "+{:>30} {} - Move Constructor:  Student( Student && donor )\n", *this, this );
  }



  // Copy assignment operator
  Student & Student::operator=( Student const & rhs )
  {
    if (this != &rhs)                                         // prevent self assignment
    {
      _name           = rhs._name;
      _numOfSemesters = rhs._numOfSemesters;
      _id             = rhs._id;
    }
    if constexpr( tracing == Trace::On )    std::print( std::clog, "={:>30} {} - Copy Assignment:  operator=( const Student & rhs )\n", *this, this );

    return *this;
  }



  // Move assignment operator
  Student & Student::operator=( Student && rhs ) noexcept
  {
    if (this != &rhs)                                         // prevent self assignment
    {
      _name           = std::move( rhs._name           );
      _numOfSemesters = std::move( rhs._numOfSemesters );
      _id             = std::move( rhs._id             );
    }
    if constexpr( tracing == Trace::On )    std::print( std::clog, "={:>30} {} - Move Assignment:  operator=( Student && rhs )\n", *this, this );

    return *this;
  }



  // Conversion constructor
  Student::Student( std::string name, unsigned nsem )
    : _name( std::move( name ) ), _numOfSemesters( nsem )
  {
    if constexpr( tracing == Trace::On )    std::print( std::clog, "+{:>30} {} - Conversion Constructor:  Student( const std::string & name, unsigned nsem )\n", *this, this );
  }



  // Destructor
  Student::~Student() noexcept
  {
    if constexpr( tracing == Trace::On )    std::print( std::clog, "-{:>30} {} - Destructor:  ~Student()\n", *this, this );
  }






  /*****************************************************************************
  ** Queries
  *****************************************************************************/




  /*****************************************************************************
  ** Modifiers
  *****************************************************************************/
  Student & Student::updateNSemesters()
  {
    _numOfSemesters++;
    return *this;
  }



  Student & Student::name( std::string name )
  {
    _name = std::move( name );
    return *this;
  }



  Student & Student::semesters( unsigned semesters )
  {
    _numOfSemesters = semesters;
    return *this;
  }






  /*****************************************************************************
  ** Insertion and Extraction Operators
  **   Notes:
  **   1) These must be non-member functions
  *****************************************************************************/
  std::ostream & operator<<( std::ostream & os, const Student & student )
  {
    return os << std::format( "{}", student );
  }



  std::istream & operator>>( std::istream & is, Student & student )
  {
    is >> std::quoted( student._name )
       >> student._id
       >> student._numOfSemesters;

    return is;
  }

}    // namespace CSUF::CPSC131







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
** Last modified:  08-JUN-2025
** Last Verified:  11-JUN-2025
** Verified with:  MS Visual Studio 2022 Version 17.14.4,  Compiler Version 19.44.35209 (C++latest)
**                 GCC version 15.1.0 (-std=c++23 ),
**                 Clang version 21.0.0 (-std=c++23 -stdlib=libc++)
***************************************************/
