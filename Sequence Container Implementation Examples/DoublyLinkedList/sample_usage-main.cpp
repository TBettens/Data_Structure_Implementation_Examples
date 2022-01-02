#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "DoublyLinkedList.hpp"
#include "Student.hpp"



int main()
{
  try
  {
    using CSUF::CPSC131::Student;
    using CSUF::CPSC131::DoublyLinkedList;

    DoublyLinkedList<Student> students;

    Student s;
    for( int i = 0; i < 5; i++ )
    {
      s.name( "Student_" + std::to_string(i) );
      s.semesters(2);
      students.push_front(s);
    }

    DoublyLinkedList<Student> classRoster( students );
    for( int i = 1; i <= 5; i++ )
    {
      s.name( "Student_" + std::to_string(i * 10) );
      s.semesters(2);
      classRoster.push_back(s);
    }

    students = classRoster;
    std::cout << "Front and back:\n" << classRoster.front() << classRoster.back() << "\n\n";

    std::cout << "Range-based for loop traversal:\n";
    for( const auto & student : students ) std::cout << student;                    //requires begin() and end() member functions
    std::cout << "\n\n";

    try
    {
      std::cout << "backward iteration traversal:\n";
      auto crbegin = std::reverse_iterator( students.cend()   );
      auto crend   = std::reverse_iterator( students.cbegin() );
      for( auto i = crbegin; i != crend; ++i ) std::cout << *i;
      std::cout << "\n\n";                                                            // this looped through a std::forward_list list
    }
    catch( std::invalid_argument & ex )
    {
      std::cerr << "\nReverse iterators can't be used for null-terminated lists because end() returns nullptr, which cannot be decremented\n" << ex.what() << "\n\n";
    }

    classRoster.insert( std::next( classRoster.begin(), 2 ), Student( "Bob" ) );


    std::cout << "Pop until empty traversal:\n";
    while( !students.empty() )
    {
      std::cout << students.back();
      students.pop_back();
    }
    std::cout << "\n\n";
  }

  catch( std::exception & ex )
  {
    std::cerr << "Unhandled exception:  " << ex.what() << '\n';
  }
}

// For testing purposes, explicitly instantiate the class template.  Template class member functions are only instantiated, and thus
// semantically checked by the compiler, when used.  Explicitly instantiating the class forces all the member functions to be
// instantiated, and thus semantically checked by the compiler.  It enables the compiler to find errors in your code.
template class CSUF::CPSC131::DoublyLinkedList<CSUF::CPSC131::Student>;
