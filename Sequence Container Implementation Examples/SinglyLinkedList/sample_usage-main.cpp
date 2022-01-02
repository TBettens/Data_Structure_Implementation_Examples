#include <iostream>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <iterator> // next()

#include "SinglyLinkedList.hpp"
#include "Student.hpp"



namespace
{
  unsigned          count = 0;
  const std::string banner( 40, '=' );

  void runExtendedInterfaceDriver()
  {
    CSUF::CPSC131::SinglyLinkedList<std::string> mySll;

    // fill the list with integers with an easily visible order
    for( auto && i : { "A", "B", "C", "D", "E" } ) mySll.push_back( i );


    // Add all the elements in the container (adding two strings concatenates them)
    std::cout << "\n\n\n\nRecursively add all elements of the list\n"
              << banner << '\n'
              << std::setw( 3 ) << ++count << ":  " << "Sum = " << mySll.add() << '\n'
              << banner << '\n';




    // Reverse the list
    std::cout << "\n\n\nRecursively reverse the contents of the list\n"
              << banner << '\n'
              << std::setw( 3 ) << ++count << ":  Before: ";
    mySll.forwardPrint();
    mySll.reverse();

    std::cout << '\n' << std::setw( 3 ) << ++count << ":  After:  ";
    mySll.forwardPrint();
    std::cout << '\n' << banner << '\n';





    // Search for a specific value in the list
    std::cout << "\n\n\nRecursively search the contents of the list for s specific value\n"
              << banner << '\n'
              << std::setw( 3 ) << ++count << ":  Element 'B' "
              << ( mySll.find( "B" ) != mySll.end() ? "was " : "was not " )
              << "found\n"
              << banner << '\n';


    // Print the list backwards
    std::cout << "\n\n\nPrint the list's contents backwards\n"
              << banner << '\n'
              << std::setw( 3 ) << ++count << ":  ";
    mySll.backwardPrint();
    std::cout << '\n' << banner << '\n';
  }
}    // namespace






int main()
{
  try
  {
    using CSUF::CPSC131::Student;
    using CSUF::CPSC131::SinglyLinkedList;

    // Default construct an empty list
    SinglyLinkedList<Student> students;


    // Insert a few students into the list such that the first one inserted is at the back of the list
    for( int i = 0; i < 5; i++ )
    {
      students.push_front( Student( "Student_" + std::to_string(i) ) );          // name is "Student_0", "Student_1", "Student_2", etc
    }


    // Copy construction - make a copy of "students" and call the copy "classRoster"
    SinglyLinkedList<Student> classRoster( students );


    // Now add more students to the class roster at the back of the list (Can't do this with std::forward_list)
    for( int i = 1; i <= 5; i++ )
    {
      classRoster.push_back( Student("Student_" + std::to_string(i * 10)) );     // name is "Student_10", "Student_20", "Student_30", etc
    }


    // Copy assignment = everything in "students" is discarded, then "students" becomes a copy of "classRoster".
    students = classRoster;

    // Display the student at the front and at the back of the class roster
    std::cout << "Front and back:\n" << classRoster.front() << classRoster.back() << "\n\n";


    // Display all the students in order
    std::cout << "Range-based for loop traversal:\n";
    for( const auto & student : students ) std::cout << student;                // uses SinglyLinkedList::begin() and SinglyLinkedList::end() member functions
    std::cout << "\n\n";


    // Insert into the middle of the list
    classRoster.insert_after( std::next(classRoster.begin(), 2), Student("Bob") );


    // Remove the student at the front of the list until the list is empty
    std::cout << "Pop until empty traversal:\n";
    while( !students.empty() )
    {
      std::cout << students.front() << '\n';
      students.pop_front();
    }


    // Example recursive extensions
    runExtendedInterfaceDriver();
  }

  catch( std::exception & ex )
  {
    std::cerr << "Unhandled exception:  " << ex.what() << '\n';
  }
}  // main()







// For testing purposes, explicitly instantiate the class template.  Template class member functions are only instantiated, and thus
// semantically checked by the compiler, when used.  Explicitly instantiating the class forces all the member functions to be
// instantiated, and thus semantically checked by the compiler.  It enables the compiler to find errors in your code.
template class CSUF::CPSC131::SinglyLinkedList< int >;
