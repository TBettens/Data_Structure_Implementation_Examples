import std;
import CSUF.CPSC131.Student;
import CSUF.CPSC131.DoublyLinkedList;



int main()
{
  try
  {
    std::locale::global( std::locale( "en_US.UTF-8" ) );

    using CSUF::CPSC131::Student;
    using CSUF::CPSC131::DoublyLinkedList;

    DoublyLinkedList<Student> students;

    Student s;
    for( int i = 0; i < 5; i++ )
    {
      s.name( std::format( "Student_{:02}", i ) );
      s.semesters(2);
      students.push_front(s);
    }

    DoublyLinkedList<Student> classRoster( students );
    for( int i = 1; i <= 5; i++ )
    {
      s.name( std::format( "Student_{:02}", i*10 ) );
      s.semesters(2);
      classRoster.push_back(s);
    }

    students = classRoster;
    std::print( std::cout, "Front and back:\n"
                           "{},   {}\n\n",
                           classRoster.front(), classRoster.back() );



    std::print( std::cout, "Range-based for loop traversal:\n"
                           "{:n:}\n\n",                                                 // requires begin(), end() and operator1= member functions
                           students );


    try
    {
      std::print( std::cout, "Backward traversal with reverse views:\n"                 // may not use reverse iterator member
                             "{:n:}\n\n",                                               // functions, but instead creates them
                             students | std::views::reverse );                          // itself using the normal begin() and end()
                                                                                        // iterator functions



      std::println( std::cout, "Backward traversal with reverse iterators:");           // explicitly use the reverse iterators
      for( auto i = students.crbegin(); i != students.crend(); ++i ) std::print( std::cout, "{}, ", *i );
      std::print( std::cout, "\n\n" );
    }
    catch( std::invalid_argument & ex )
    {
      std::print( std::cerr, "\nReverse iterators can't be used for null-terminated DoublyLinkedList because end() returns nullptr, which cannot be decremented\n{}\n\n", ex.what() );
    }

    classRoster.insert( std::next( classRoster.begin(), 2 ), Student( "Bob" ) );


    std::println( std::cout, "Backward traversal by popping until empty:");
    while( !students.empty() )
    {
      std::print( std::cout, "{}, ", students.back() );
      students.pop_back();
    }
    std::print( std::cout, "\n\n" );
  }

  catch( std::exception & ex )
  {
    std::print( std::cerr, "Unhandled exception:  {}\n", ex.what() );
  }
}

// For testing purposes, explicitly instantiate the class template.  Template class member functions are only instantiated, and thus
// semantically checked by the compiler, when used.  Explicitly instantiating the class forces all the member functions to be
// instantiated, and thus semantically checked by the compiler.  It enables the compiler to find errors in your code.
template class CSUF::CPSC131::DoublyLinkedList<CSUF::CPSC131::Student, CSUF::CPSC131::DllPolicy::CIRCULAR>;
template class CSUF::CPSC131::DoublyLinkedList<int,                    CSUF::CPSC131::DllPolicy::CIRCULAR>;
template class CSUF::CPSC131::DoublyLinkedList<std::string,            CSUF::CPSC131::DllPolicy::CIRCULAR>;

template class CSUF::CPSC131::DoublyLinkedList<CSUF::CPSC131::Student, CSUF::CPSC131::DllPolicy::NTL>;
template class CSUF::CPSC131::DoublyLinkedList<int,                    CSUF::CPSC131::DllPolicy::NTL>;
template class CSUF::CPSC131::DoublyLinkedList<std::string,            CSUF::CPSC131::DllPolicy::NTL>;
