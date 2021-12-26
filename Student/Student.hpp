#pragma once

#include <cstddef>    // size_t
#include <iostream>
#include <string>



namespace CSUF::CPSC131
{
  class Student
  {
    friend std::ostream & operator<<( std::ostream & os, const Student & student );
    friend std::istream & operator>>( std::istream & is,       Student & student );

    public:
      // Constructors, assignments, destructor
      Student            (                                           );           // Default constructor
      Student            ( Student const &  original                 );           // Copy constructor
      Student            ( Student       && donor                    ) noexcept;  // Move constructor
      Student & operator=( Student const &  rhs                      );           // Copy assignment operator
      Student & operator=( Student       && rhs                      ) noexcept;  // Move assignment operator
      Student            ( std::string      name, unsigned nsem = 1U );           // Conversion constructor
     ~Student            (                                           ) noexcept;  // Destructor

      // Modifiers
      Student & updateNSemesters(                       );
      Student & name            ( std::string name      );
      Student & semesters       ( unsigned    semesters );

      // Relational comparison
      auto operator<=>( const Student & ) const = default;                        // Defines all 6 relational operators, symmetrically
                                                                                  // e.g.  given std::string a; Student b;  then both a < b and b < a are valid
    private:
      // private instance attributes (every instance (object) has its own set of these)

      // Let's say our requirement is to sort Students by
      //    1) name, then by
      //    2) ID, then by
      //    3) number of semesters
      // In order for operator<=> to order (sort) properly, the physical order of the member attributes must physically match the sort criteria
      std::string   _name;                                                        // Student's full name
      std::size_t   _id             = nextAvailableID++;                          // Automatically assigned student unique identifier
      unsigned      _numOfSemesters = 0;                                          // Number of semesters completed

      // private class attributes  (all objects share a single set of these attributes)
      inline static std::size_t nextAvailableID = 2021'02'01;                     // The single quote is a digit separator used only for human readability and is ignored by the compiler

  };  // class Student
}    // namespace CSUF::CPSC131












/***********************************************************************************************************************************
** (C) Copyright 2021 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The participating authors at California State University's Computer Science Department have used their best efforts
** in preparing this code. These efforts include the development, research, and testing of the theories and programs to determine
** their effectiveness. The authors make no warranty of any kind, expressed or implied, with regard to these programs or to the
** documentation contained within. The authors shall not be liable in any event for incidental or consequential damages in
** connection with, or arising out of, the furnishing, performance, or use of these libraries and programs.  Distribution without
** written consent from the authors is prohibited.
***********************************************************************************************************************************/

/**************************************************
** Last modified:  21-OCT-2021  (refactored C++20 workarounds)
** Last Verified:  01-AUG-2021
** Verified with:  MS Visual Studio 2019 Version 16.10.2 (C++20)
**                 GCC version 11.1.1 20210721 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
