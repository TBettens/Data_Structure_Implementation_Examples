import std;
import CSUF.CPSC131.Student;
import CSUF.CPSC131.SinglyLinkedList;

namespace  // unnamed, anonymous namespace
{
  unsigned          count = 0;
  const std::string banner( 40, '=' );



  void runExtendedInterfaceDriver()
  {
    // Create and fill a list with strings with an easily visible order
    CSUF::CPSC131::SinglyLinkedList<std::string, CSUF::CPSC131::SllPolicy::NTL> mySll{ "A", "B", "C", "D", "E" };



    // Add up all the elements in the container (adding two strings concatenates them)
    std::print( std::cout, "\n\n\n\n"
                           "Recursively add all elements of the list\n"
                           "{2}\n"
                           "{0:>3}:  Sum:  {1}\n"
                           "{2}\n",
                           ++count, mySll.add(), banner );
    // Alternative that doesn't require the definition of "banner"
    // std::print( std::cout, "\n\n\n\n"
    //                        "Recursively add all elements of the list\n"
    //                        "{2:=>{3}}\n"
    //                        "{0:>3}:  Sum = {1:}\n"
    //                        "{2:=>{3}}\n",
    //                        ++count, mySll.add(), "", 40 );



    // Reverse a copy of the list
    auto myCopiedSll{ mySll };
    myCopiedSll.reverse();

    std::print( std::cout, "\n\n\nRecursively reverse the contents of the list\n"
                           "{3}\n"
                           "{0:>3}: Before: {1:n:}\n"
                           "   "
                           ": After:  {2:n:}\n"
                           "{3}\n",
                ++count,
                mySll,
                myCopiedSll,
                banner );





    // Search for a specific value in the list
    std::print( std::cout, "\n\n\n"
                           "Recursively search the contents of the list for s specific value\n"
                           "{2}\n"
                           "{0:>3}:  Element 'B' {1} found\n"
                           "{2}\n",
                           ++count, ( mySll.find( "B" ) != mySll.end() ? "was" : "was not"), banner );




    // Print the list backwards
    std::ostringstream forward, backward;
    mySll.forwardPrint ( forward  );
    mySll.backwardPrint( backward );

    std::print( std::cout, "\n\n\n"
                           "Print the list's contents forwards and backwards\n"
                           "{3}\n"
                           "{0:>3}:  Forward:    {1}\n"
                           "   " ":  Backwards:  {2}\n"
                           "{3}\n",
                           ++count, forward.str(), backward.str(), banner );
  }
}    // anonymous namespace






int main()
{
  try
  {
    std::locale::global( std::locale( "en_US.UTF-8" ) );

    using CSUF::CPSC131::Student;
    using CSUF::CPSC131::SinglyLinkedList;

    // Default construct an empty list
    SinglyLinkedList<Student> students;


    // Insert a few students into the list such that the first one inserted is at the back of the list
    for( int i = 0; i < 5; i++ )
    {
      students.push_front( Student( std::format("Student_{:02}", i) ) );                        // name is "Student_0", "Student_1", "Student_2", etc
    }


    // Copy construction - make a copy of "students" and call the copy "classRoster"
    SinglyLinkedList<Student> classRoster( students );


    // Now add more students to the class roster at the back of the list (Can't do this with std::forward_list)
    for( int i = 1; i <= 5; i++ )
    {
      classRoster.push_back( Student( std::format("Student_{:02}", i*10) ) );                   // name is "Student_10", "Student_20", "Student_30", etc
    }


    // Copy assignment = everything in "students" is discarded, then "students" becomes a copy of "classRoster".
    students = classRoster;

    // Display the student at the front and at the back of the class roster
    std::print( std::cout, "Front and back:\n  {},  {}\n\n", classRoster.front(), classRoster.back() );


    // Display all the students in order
    std::cout << "Range-based for loop traversal:\n";
    for( const auto & student : students ) std::print( std::cout, "{}\n", student);             // uses SinglyLinkedList::begin() and SinglyLinkedList::end() member functions
    std::print( std::cout, "\n\n");


    // Insert into the middle of the list
    classRoster.insert_after( std::next(classRoster.begin(), 2), Student("Bob") );


    // Remove the student at the front of the list until the list is empty
    std::println( std::cout, "Pop until empty traversal:" );
    while( !students.empty() )
    {
      std::print( std::cout, "{}\n", students.front() );
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
template class CSUF::CPSC131::SinglyLinkedList<int,         CSUF::CPSC131::SllPolicy::CIRCULAR>;
template class CSUF::CPSC131::SinglyLinkedList<std::string, CSUF::CPSC131::SllPolicy::CIRCULAR>;

template class CSUF::CPSC131::SinglyLinkedList<int,         CSUF::CPSC131::SllPolicy::NTL>;
template class CSUF::CPSC131::SinglyLinkedList<std::string, CSUF::CPSC131::SllPolicy::NTL>;
