import std;
import CSUF.CPSC131.Student;
import CSUF.CPSC131.Vector;


namespace    // anonymous
{
  using ::CSUF::CPSC131::Student;
  using ::CSUF::CPSC131::VectorPolicy;
  using ::CSUF::CPSC131::Vector;

  template<typename T>
  void test( T & vector )
  {
    std::print( std::cout, "\n\nTesting {:?}\n", __func__ );

    Student s( "Adam", 2 );
    vector.push_back( s );
    vector.push_back( Student( "Bob", 1 ) );
    vector.push_back( Student( "Dolores", 3 ) );

    // Programming Note:
    //   Providing a range-underlying-spec (the stuff after the second colon) disables debug format, which causes the stuff in the
    //   container to be formatted as an escaped string (like :?s does for string
    //   See https://en.cppreference.com/w/cpp/utility/format/range_formatter.html  std::range_formatter::parse section:
    //    "It calls underlying_.set_debug_format() if: ... there is no range-underlying-spec.
    std::print( std::cout, "\n {:n:}\n\n", vector );                                  // Should be:  "", "", "", Adam, Bob, Dolores



    // add student Carla between Bob and Dolores
    vector.insert( vector.begin() + 3, Student( "Carla" ) );
    std::print( std::cout, "\n {:n:}\n\n", vector );                                  // Should be:  "", "", "", Carla, Adam, Bob, Dolores



    // update Bob's record
    vector[5].updateNSemesters();
    std::print( std::cout, " {}\n", vector[5]);                                       // Should be:  {"Bob", 20210218, 2}



    // remove student Adam (element at index 3)
    vector.erase( vector.begin()+3 );
    std::print( std::cout, "\n {:n:}\n\n", vector );                                  // Should be:  "", "", "", Adam, Bob, Dolores



    // copy construction
    auto aCopy = vector;
    std::print( std::cout, "\n {:n:}\n\n", aCopy );                                   // Should be:  "", "", "", Adam, Bob, Dolores



    vector.clear();
    std::print( std::cout, "\n {:n:}\n\n", aCopy );                                   // Should be:  "", "", "", Adam, Bob, Dolores



    // Capacity breach
    try
    {
      // For purposes of this test, let's assume the capacity is 25 or less
      for( std::size_t i = 0;  i < 25u;  ++i )    vector.push_back( s );
      std::print( std::cout, "\nAll 25 students placed into the vector\n" );
    }
    catch(const std::exception & ex)
    {
       std::print( std::cout, "{}\n", ex.what() );
    }
    std::print( std::cout, "\n\n" );



    vector = aCopy;
    std::print( std::cout, "\nVectors are equal:  {}\n\n", vector == aCopy );         // Should be:  true
  }
}    // anonymous namespace






int main()
{
  try
  {
    std::locale::global( std::locale( "en_US.UTF-8" ) );

    // Constructed providing size and sometimes capacity
    CSUF::CPSC131::Vector<Student, VectorPolicy::FIXED> fixedStudentVector(3, 10);
    CSUF::CPSC131::Vector<Student>                      extendableStudentVector(3);   // in contrast to FixedVector, capacity is not specified
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
