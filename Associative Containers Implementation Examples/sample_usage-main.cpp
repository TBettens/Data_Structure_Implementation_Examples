#include <iostream>
#include <string>
#include <iterator>   // next(), prev(), advance()

#include "BST-AVL.hpp"



int main()
{
  using CSUF::CPSC131::BinarySearchTree;
                                                                        //             Ricardo     //
  BinarySearchTree<std::string, double> studentGrades, gradeBook;       //             /           //
  studentGrades.insert( {"Ricardo", 2.5 } );                            //         Ellen           //
  studentGrades.insert( {"Ellen",   3.5 } );                            //          /  \           //
  studentGrades.insert( {"Chen",    2.5 } );                            //       Chen  Kevin       //
  studentGrades.insert( {"Kevin",   3.25} );                            //                \        //
  studentGrades.insert( {"Kumar",   3.05} );                            //                Kumar    //

  // exercise the copy assignment operator
  gradeBook   = studentGrades;
  studentGrades["Barbara"] = 4.0;


  // find grade of one student
  {
    std::string myKey = "Ellen";
    auto        value = studentGrades.find( myKey );
    if( value != studentGrades.end() ) std::cout << "Grade of " << value->first << " is " << value->second << "\n\n";
  }

  // Does a particular student exist in the tree?
  std::cout << "Kyle " << ( studentGrades.contains( "Kyle" ) ? " is " : " is not " ) << " a member of the tree\n\n";

  // print the entire BST using recursion (but as a client, you don't know its being done recursively, right?)
  std::cout << "studentGrades.printInorder():\n";
  studentGrades.printInorder();
  std::cout << "\n\n";



  // print the entire BST using a range-based for loop and structured bindings
  std::cout << "for( auto && [student, gpa] : studentGrades ):\n";
  for( auto && [student, gpa] : studentGrades )
  {
    std::cout << "Grade of " << student << " is " << gpa << '\n';
  }
  std::cout << '\n';



  // print some of the tree - everything from "Ricardo" to the beginning walking backwards - using a traditional for loop, iterators, and std::pair
  std::cout << "for gradebook [Ricardo .. begin()]:\n";
  for( auto i = gradeBook.find( "Ricardo" ); i != gradeBook.end(); --i )
  {
    std::cout << "Grade of " << i->first << " is " << i->second << '\n';
  }
  std::cout << '\n';


  // print the entire BST in reverse by creating appropriate reverse iterators
  //
  // null-terminated structures, like my BST, usually cannot increment or decrement end(). so I'm going to fudge by adding something
  // greater than "Ricardo" to use as the end
  //      auto rbegin = std::reverse_iterator( gradeBook.end() );
  {
    std::cout << "for gradebook [rbegin .. rend):\n";
    gradeBook.insert( { "zzzz", 0.0 } );
    auto rbegin = std::reverse_iterator( gradeBook.find( "zzzz" ) );
    auto rend   = std::reverse_iterator( gradeBook.begin() );
    for( auto i = rbegin; i != rend; ++i )
    {
      std::cout << "Grade of " << i->first << " is " << i->second << '\n';
    }
    std::cout << '\n';
    gradeBook.erase( "zzzz" );
  }

  // exercise a few more capabilities
  if( gradeBook.getHeight() != 3 ) std::cerr << "Tree height does not match expected\n";

  gradeBook.erase( {"Ellen"} );
  if( gradeBook.getHeight() != 2 ) std::cerr << "Tree height does not match expected\n";

  std::cout << "Tree sum: " << studentGrades.getSum() << '\n';
  std::cout << "Tree max: " << studentGrades.getMaxValue() << '\n';

    // In Class Exercises from "07 - AVL Tree Animation.pptx"
  BinarySearchTree<int, int> testTree;
  testTree[14];
  testTree[17];
  testTree[11];
  testTree[ 7];
  testTree[53];
  testTree[ 4];
  testTree[13];
  testTree[12];
  testTree[ 8];
  testTree.erase( 53 );
  testTree.erase( 11 );
  testTree.erase(  8 );

  testTree.clear();

  testTree.insert( {15, 0} );
  testTree.insert( {20, 0} );
  testTree.insert( {24, 0} );
  testTree.insert( {10, 0} );
  testTree.insert( {13, 0} );
  testTree.insert( { 7, 0} );
  testTree.insert( {30, 0} );
  testTree.insert( {36, 0} );
  testTree.insert( {25, 0} );
  testTree.erase (  24     );
  testTree.erase (  20     );
  testTree.erase (  10     );


  // print the entire BST using a range-based for loop and structured bindings
  std::cout << "for( auto && [key, value] : testTree ):\n";
  for( auto && [key, value] : testTree )
  {
    std::cout << "Key: " << key << '\n';
  }
  std::cout << '\n';
}







// Explicit instantiation - a technique to ensure all functions of the template are created and semantically checked.  By default,
// only functions called get instantiated so you won't know it has compile errors until you actually call it.
namespace CSUF::CPSC131
{
  template class BinarySearchTree<std::string, double     >;
  template class BinarySearchTree<unsigned,    int        >;
  template class BinarySearchTree<double,      std::string>;
}
