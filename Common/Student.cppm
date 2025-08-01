module;                                                                           // Global fragment (not part of the module)
  // Empty




/***********************************************************************************************************************************
**  Module CSUF.CPSC131.Student Interface
**
***********************************************************************************************************************************/
export module CSUF.CPSC131.Student;                                               // Primary Module Interface Definition
import std;

export namespace CSUF::CPSC131
{
  class Student
  {
    friend std::istream & operator>>( std::istream & is, Student & student );
    friend struct std::formatter<Student>;

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
      inline static std::size_t nextAvailableID = 2021'02'01;                     // The single quote (aka tick) is a digit separator used only for human readability and is ignored by the compiler

  };  // class Student

  std::istream & operator>>( std::istream & is,       CSUF::CPSC131::Student & student );
  std::ostream & operator<<( std::ostream & os, const CSUF::CPSC131::Student & student );
}    // export namespace CSUF::CPSC131







// Not exported but reachable
/***********************************************************************************************************************************
** Template definitions
***********************************************************************************************************************************/

// Specialize the formatter to accept and process CSUF::CPSC131::Student typed objects
template<>
struct std::formatter<CSUF::CPSC131::Student> : std::formatter<std::string>
{
  // No special parsing options, for now.  So let's inherit and delegate to the formatter<string>::parse function Someday, we might
  // want to allow the client to specify, for example, '[' instead of '{', or the delimiter between fields.
  //
  // constexpr auto parse( auto & ctx )


  auto format( const CSUF::CPSC131::Student & student, auto & ctx ) const
  {
    // This approach converts the student object to a string object with the appropriate formatting.  Then it formats this string
    // abject using std::string's format descriptors.  This added flexibility allows the client to specify a field with, left,
    // right, or center justified, etc.
    //
    // {:?} does what std::quoted(...) does
    // {:L} inserts the thousands separator as defined by the current locale
    //      Note: the default C locale doesn't define a thousand separator
    //            Can set global locale, e.g.,      std::locale::global( std::locale( "en_US.UTF-8" ) );
    //            Can override global locale, e.g., std::format( std::locale("en_US.UTF-8"), "{}", ... )
    const std::string str = std::format( "{{{:?}, {:L}, {}}}", student._name, student._id, student._numOfSemesters );
    return std::formatter<std::string>::format( str, ctx);


    // Alternately, if you don't want to process like a string using the string format descriptors, then
    //
    // return std::format_to( ctx.out(), "{{{:?}, {:L}, {}}}", student._name, student._id, student._numOfSemesters );

  }
};




// Specialize the formatter to accept and process pointer-to-CSUF::CPSC131::Student typed objects
template<>
struct std::formatter<CSUF::CPSC131::Student *> : std::formatter<std::string>
{
  // Let's output our addresses like this:  0x00F2'36BD'325B

  // struct numpunct : std::numpunct<char>
  // {
  //   char        do_thousands_sep() const override { return '\''; }    // Set thousand separator to an apostrophe (aka tick)
  //   std::string do_grouping     () const override { return "\4"; }    // Group digits in sets of 4 (yes, the 4 must be escaped)
  // };
  // inline static std::locale loc{ std::locale{}, new numpunct };       // create this only once

  // Normally, only void pointers can be formatted as a hex address.  This is a good thing since, for example, you want a string
  // literal to be formatted with the string pointed to.  std::format({}, "hello world") should return "hello world" and not the
  // address of where the string is stored.  However, in this case, I really do want the address of where the Student object is
  // stored to be returned.
  auto format( const CSUF::CPSC131::Student * ptr, auto & ctx ) const
  {
    // Considered and discarded the{:L} descriptor because it did not group short, zero-filled numbers (ex: 0x00002F03'BD10  )
    //    std::string str = std::format( loc, "0x{:014LX}", reinterpret_cast<std::uintptr_t>( ptr ) ); which now means I could
    //    remove the tailored struct numpunct above.  But I'll keep it as a future reference.
    //
    // Considered and discarded the {:#p} or {:#P} descriptors because they could not be grouped with {:L}.  I also wanted a
    //    lowercase 0x and uppercase hex digits but {:p} and {:P} were all one way or the other.
    std::string str = std::format( "0x{:012X}", reinterpret_cast<std::uintptr_t>( ptr ) );
    str.insert( str.begin() + 10, '\'' );
    str.insert( str.begin() + 06, '\'' );
    return std::formatter<std::string>::format( str, ctx );
  }
};














/***********************************************************************************************************************************
**  Module CSUF.CPSC131.Student Private Implementation
**
**  Design Decision:  I seriously considered moving the module's implementation unit (Student.cpp file contents) into this file as a
**                    private module fragment (module :private), but ultimately decided against it at this time. First, GCC doesn't
**                    implement private fragments and instead issues a non-suppressible error (a show stopper). Second, overwhelming
**                    "best practices" still suggest to separate even amongst the large community of "header only" advocates mainly
**                    for 1) stronger encapsulation and intellectual property protection, but also 2) because the tools aren't yet
**                    smart enough to stop cascading recompiles when the module's purview hasn't changed. While I respect the
**                    "header only" design pattern, my extensive experience has shown again and again "separate files" is the right
**                    answer.
**
**                    In either case, it is important to separate interface from implementation, which is why the lazy pattern of
**                    implementing functions "inline" as part of the class definition is consistently avoided.
***********************************************************************************************************************************/
// module :private;















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
** Last modified:  27-JUL-2025 (Converted to C++ Modules)
** Last Verified:  27-JUL-2025
** Verified with:  MS Visual Studio 2022 Version 17.14.9,  Compiler Version 19.44.35213 (/std:c++latest)
**                 GCC version 15.1.0 (-std=c++23 )
**                 Clang version 21.0.0 (-std=c++23 -stdlib=libc++)
***************************************************/
