import std;
import CSUF.CPSC131.BinarySearchTree;


int main()
{
  try
  {
    using std::print, std::println, std::cout, std::cerr;
    using CSUF::CPSC131::BinarySearchTree;                                //      NOT BALANCED       //        BALANCED         //
                                                                          //             Ricardo     //       Ellen             //
    BinarySearchTree<std::string, double> studentGrades, gradeBook;       //             /           //       /   \             //
    studentGrades.insert( {"Ricardo", 2.5 } );                            //         Ellen           //   Chen     Kumar        //
    studentGrades.insert( {"Ellen",   3.5 } );                            //          /  \           //            /   \        //
    studentGrades.insert( {"Chen",    2.5 } );                            //       Chen  Kevin       //        Kevin   Ricardo  //
    studentGrades.insert( {"Kevin",   3.25} );                            //                \        //                         //
    studentGrades.insert( {"Kumar",   3.05} );                            //                Kumar    //                         //

    // exercise the copy assignment operator
    gradeBook   = studentGrades;
    studentGrades["Barbara"] = 4.0;


    // find grade of one student
    {
      std::string myKey = "Ellen";
      auto        value = studentGrades.find( myKey );
      if( value != studentGrades.end() ) print( cout, "{}'s grade point average is {:.2f}  (aka {:m})\n\n", value->first, value->second, *value );
    }

    // does a particular student exist in the tree?
    print( cout, "Kyle is {}a member of the tree\n\n",  studentGrades.contains( "Kyle" ) ? "" : "not " );


    // print the entire BST using recursion (but as a client, you don't know its being done recursively, right?)
    println( cout, "studentGrades.printInorder():");
    studentGrades.printInorder( cout );
    print( cout, "\n\n");

    // Now print the entire BST again, but this time use the standard formatting
    print( cout, "In-order printout for the students and their GPAs\n"
                 "{}\n\n",
                 studentGrades );



    // print the entire BST using a range-based for loop and structured bindings
    print( cout, "for( auto && [student, gpa] : studentGrades ):\n" );
    for( auto && [student, gpa] : studentGrades )
    {
      print( cout, "{:>8}'s grade point average is {:.2f}\n", student, gpa );
    }
    println(cout);



    // print some of the tree - everything from "Ricardo" to the beginning walking backwards - using a traditional for loop, iterators, and std::pair
    print( cout, "for gradebook [Ricardo .. begin()]:\n" );
    for( auto i = gradeBook.find( "Ricardo" ); i != gradeBook.end(); --i )
    {
      print( cout, "{:>8}'s grade point average is {:.2f}\n", i->first, i->second );
    }
    println( cout );


    // print the entire BST in reverse by creating appropriate reverse iterators
    //
    // null-terminated structures, like my BST, usually cannot increment or decrement end(). so I'm going to fudge by adding something
    // greater than "Ricardo" to use as the end
    //      auto rbegin = std::reverse_iterator( gradeBook.end() );
    {
      println( cout, "for gradebook [rbegin .. rend):" );
      gradeBook.insert( { "zzzz", 0.0 } );
      auto rbegin = std::reverse_iterator( gradeBook.find( "zzzz" ) );
      auto rend   = std::reverse_iterator( gradeBook.begin() );
      for( auto i = rbegin; i != rend; ++i )
      {
        // Because my BST is a null-terminated tree, the ranges algorithms that use the reverse iterators, like range_format, trip
        // the static analyzers that look for possible null pointer dereferences.
        //   print( cout, "{:>8}'s grade point average is {:.2f}\n", i->first, i->second );
        cout << std::setw( 8 ) << std::right << i->first << "'s grade point average is " << std::showpoint << std::fixed << std::setprecision( 2 ) << i->second << '\n';
      }
      println( cout );
      gradeBook.erase( "zzzz" );
    }

    // exercise a few more capabilities
    if( gradeBook.getHeight() != 2 ) print( cerr, "Tree height does not match expected\n" );

    gradeBook.erase( {"Ellen"} );
    if( gradeBook.getHeight() != 2 ) print( cerr, "Tree height does not match expected\n" );

    print( cout, "Tree sum: {}\n"
                 "Tree max: {}\n",
                 studentGrades.getSum(),
                 studentGrades.getMaxValue() );

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
    cout << "for( auto && [key, value] : testTree ):\n";
    for( auto && [key, value] : testTree )
    {
      cout << "Key: " << key << '\n';
    }
    cout << '\n';

    // And again with range formatting
    print( cout, "format \"{{}}\", testTree\n"
                 "{}\n", testTree );
  }

  catch( const std::exception & ex )
  {
    std::cerr << ex.what() << '\n';
  }
}







// Explicit instantiation - a technique to ensure all functions of the template are created and semantically checked.  By default,
// only functions called get instantiated so you won't know it has compile errors until you actually call it.
namespace CSUF::CPSC131
{
  template class BinarySearchTree<std::string, double     >;
  template class BinarySearchTree<unsigned,    int        >;
  template class BinarySearchTree<double,      std::string>;
}
