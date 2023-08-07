#include <exception>
#include <iomanip>    // quoted()
#include <iostream>
#include <string>
#include <vector>

#include "Student.hpp"
#include "Vector.hpp"



namespace    // anonymous
{
  using ::CSUF::CPSC131::Student;
  using ::CSUF::CPSC131::VectorPolicy;
  using ::CSUF::CPSC131::Vector;

  template<typename T>
  void test( T & vector )
  {
    std::cout << "\n\nTesting " << std::quoted( __func__ ) << '\n';

    Student s( "Adam", 2 );
    vector.push_back( s );
    vector.push_back( Student( "Bob", 1 ) );
    vector.push_back( Student( "Dolores", 3 ) );


    // Should be:  "", "", "", Adam, Bob, Dolores
    std::cout << "\n ";
    for( const auto & student : vector ) std::cout << student << ", ";
    std::cout << "\n\n";




    // add student Carla between Bob and Dolores
    vector.insert( vector.begin() + 3, Student( "Carla" ) );

    // Should be:  "", "", "", Carla, Adam, Bob, Dolores
    std::cout << "\n ";
    for( const auto & student : vector ) std::cout << student << ", ";
    std::cout << "\n\n";




    // update Bob's record
    vector[5].updateNSemesters();

    // Should be:  {"Bob", 20210218, 2}
    std::cout << ' ' << vector[5] << '\n';


    // remove student Adam (element at index 3)
    vector.erase( vector.begin()+3 );

    // Should be:  "", "", "", Adam, Bob, Dolores
    std::cout << "\n ";
    for( const auto & student : vector ) std::cout << student << ", ";
    std::cout << "\n\n";




    // Copy and assignment
    auto aCopy = vector;                                              // copy construction

    // Should be:  "", "", "", Adam, Bob, Dolores
    std::cout << "\n ";
    for( const auto & student : aCopy ) std::cout << student << ", ";
    std::cout << "\n\n";



    vector.clear();

    // Should be:  "", "", "", Adam, Bob, Dolores
    std::cout << "\n ";
    for( const auto & student : aCopy ) std::cout << student << ", ";
    std::cout << "\n\n";


    // Capacity breach
    try
    {
      // For purposes of this test, let's assume the capacity is 25 or less
      for( std::size_t i = 0;  i < 25u;  ++i )    vector.push_back( s );
      std::cout << "\nAll 25 students placed into the vector\n ";
    }
    catch(const std::exception & ex)
    {
      std::cout << ex.what() << '\n';
    }
    std::cout << "\n\n";



    vector = aCopy;                                                   // assignment;

    // Should be:  true
    std::cout << "\nVectors are equal:  " << ( vector == aCopy ) << "\n\n";
  }
}    // anonymous namespace






int main()
{
  try
  {
    std::cout << std::boolalpha;                                                      // display boolean values as true and false instead of non-zero and zero

    // Constructed providing size and sometimes capacity
    CSUF::CPSC131::Vector<Student, VectorPolicy::FIXED> fixedStudentVector(3, 10);
    CSUF::CPSC131::Vector<Student>                      extendableStudentVector(3);    // in contrast to FixedVector, capacity is not specified
    std::vector<Student>                                standardStudentVector(3);

    // Constructed with a list of Students
    Vector<Student> initializedExtendableVector = { { "Beth"      },
                                                    { "Chris",  5 },
                                                    { "Amanda", 7 } };

    // Constructed with class template argument deduction (CTAD) (see https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)
    Student s{ "Barbara" };
    Vector  myVector{ s, s, s, { "Tom" } };

    test( fixedStudentVector          );
    test( extendableStudentVector     );
    test( standardStudentVector       );
    test( initializedExtendableVector );
    test( myVector                    );
  }

  catch( std::exception & ex )
  {
    std::cerr << "Unhandled exception:  " << ex.what() << '\n';
  }

  return 0;
}


// For testing purposes, explicitly instantiate the class template.  Template class member functions are only instantiated, and thus
// semantically checked by the compiler, when used.  Explicitly instantiating the class forces all the member functions to be
// instantiated, and thus semantically checked by the compiler.  It enables the compiler to find errors in your code.
template class CSUF::CPSC131::Vector<Student, VectorPolicy::EXTENDABLE>;
template class CSUF::CPSC131::Vector<Student, VectorPolicy::FIXED>;

template class CSUF::CPSC131::Vector<int, VectorPolicy::EXTENDABLE>;
template class CSUF::CPSC131::Vector<int, VectorPolicy::FIXED>;
