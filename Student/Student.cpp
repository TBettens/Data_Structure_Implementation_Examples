#include <iomanip>     // quoted()
#include <iostream>    // cout, cin, ostream, istream
#include <string>      // string
#include <utility>     // move()

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
    if constexpr( tracing == Trace::On )    std::clog << '+' << *this << ' ' << this << " - Default Constructor:  Student()\n";
 }



  // Copy constructor
  Student::Student( Student const & original )
    : _name          ( original._name           ),
      _id            ( original._id             ),
      _numOfSemesters( original._numOfSemesters )
  {
    if constexpr( tracing == Trace::On )    std::clog << '+' << *this << ' ' << this << " - Copy Constructor:  Student( const Student & original )\n";
  }



  // Move constructor
  Student::Student( Student && donor ) noexcept
    : _name          ( std::move(donor._name          ) ),
      _id            ( std::move(donor._id            ) ),
      _numOfSemesters( std::move(donor._numOfSemesters) )
  {
    if constexpr( tracing == Trace::On )    std::clog << '+' << *this << ' ' << this << " - Move Constructor:  Student( Student && donor )\n";
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
    if constexpr( tracing == Trace::On )    std::clog << '=' << *this << ' ' << this << " - Copy Assignment:  operator=( const Student & rhs )\n";

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
    if constexpr( tracing == Trace::On )    std::clog << '=' << *this << ' ' << this << " - Move Assignment:  operator=( Student && rhs )\n";

    return *this;
  }



  // Conversion constructor
  Student::Student( std::string name, unsigned nsem )
    : _name( std::move( name ) ), _numOfSemesters( nsem )
  {
    if constexpr( tracing == Trace::On )    std::clog << '+' << *this << ' ' << this << " - Conversion Constructor:  Student( const std::string & name, unsigned nsem )\n";
  }



  // Destructor
  Student::~Student() noexcept
  {
    if constexpr( tracing == Trace::On )    std::clog << '-' << *this << ' ' << this << " - Destructor:  ~Student()\n";
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
    return os << '{' << std::quoted( student._name ) << ", " << student._id << ", " << student._numOfSemesters << '}';
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
** (C) Copyright 2022 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The participating authors at California State University's Computer Science Department have used their best efforts
** in preparing this code. These efforts include the development, research, and testing of the theories and programs to determine
** their effectiveness. The authors make no warranty of any kind, expressed or implied, with regard to these programs or to the
** documentation contained within. The authors shall not be liable in any event for incidental or consequential damages in
** connection with, or arising out of, the furnishing, performance, or use of these libraries and programs.  Distribution without
** written consent from the authors is prohibited.
***********************************************************************************************************************************/

/**************************************************
** Last modified:  27-JUL-2021
** Last Verified:  03-JAN-2022
** Verified with:  MS Visual Studio 2019 Version 16.11.8 (C++20)
**                 GCC version 11.2.1 20211124 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
