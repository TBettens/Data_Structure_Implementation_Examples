/***********************************************************************************************************************************
** Class SinglyLinkedList - a very basic example implementation of the Singly Linked List Abstract Data Type
**
**  The ADT's interface is a small subset of std::forward_list defined at https://en.cppreference.com/w/cpp/container/forward_list
**
**  Two major differences between this example and std::forward_list
**  1) This example maintains a tail pointer, the std::forward_list does not. This means SinglyLinkedList can provide operations to
**     the back of the list in constant time O(1) that std::forward_list cannot.
**  2) This example maintains the size of the list, std::forward_list does not.  SinglyLinkedList::size() is a constant O(1)
**     operation, and std::forward_list has no size() operation.
***********************************************************************************************************************************/
#pragma once
#include <compare>                                                                    // week_ordering, strong_quality, compare_weak_order_fallback
#include <cstddef>                                                                    // size_t, ptrdiff_t
#include <initializer_list>                                                           // initializer_list
#include <iostream>                                                                   // cout
#include <iterator>                                                                   // forward_iterator_tag, next()
#include <memory>                                                                     // unique_ptr, make_unique()
#include <stdexcept>                                                                  // length_error, invalid_argument
#include <utility>                                                                    // swap(), move()



namespace CSUF::CPSC131
{
  // Template Class Definition
  template <typename T>
  class SinglyLinkedList
  {
    template <typename U>
    friend void swap( SinglyLinkedList<U> & lhs, SinglyLinkedList<U> & rhs );         // The expected way to make a program-defined type swappable is to provide a
                                                                                      // non-member function swap in the same namespace as the type.
                                                                                      // (https://en.cppreference.com/w/cpp/algorithm/swap)

    private:
      // Types
      template <typename U>  class Iterator_type;                                     // Template class for iterator and const_iterator classes
      struct Node;                                                                    // Specific implementations are responsible for defining their node structure
      struct PrivateMembers;                                                          // A specific implementation's private members (attributes, functions, etc)

      // Member instance attributes
      std::unique_ptr<PrivateMembers> self = nullptr;                                 // See Pimpl Idiom at https://cpppatterns.com/patterns/pimpl.html



    public:
      // Types
      using iterator       = Iterator_type<T      >;                                  // A forward iterator to a read-write value in the list
      using const_iterator = Iterator_type<T const>;                                  // A forward iterator to a read-only value in the list


      // Constructors, destructor, and assignments
      SinglyLinkedList            (                                       );          // default constructor
      SinglyLinkedList            ( SinglyLinkedList const    & original  );          // copy constructor
      SinglyLinkedList            ( SinglyLinkedList         && original  ) noexcept; // move constructor
      SinglyLinkedList            ( std::initializer_list<T>    init_list );          // initialization list constructor
      SinglyLinkedList & operator=( SinglyLinkedList const    & rhs       );          // copy assignment
      SinglyLinkedList & operator=( SinglyLinkedList         && rhs       ) noexcept; // move assignment
     ~SinglyLinkedList            (                                       ) noexcept; // destructor


      // Queries
      bool        empty() const noexcept;                                             // returns true if list has no items
      std::size_t size () const noexcept;                                             // returns the number of elements in the list


      // Iterators
      iterator before_begin();                                                        // Returns a read-write iterator that precedes the list's front element.  Do not dereference this iterator
      iterator begin       ();                                                        // Returns a read-write iterator to the list's front element, end() if list is empty
      iterator end         ();                                                        // Returns a read-write iterator beyond the list's back element.  Do not dereference this iterator

      const_iterator before_begin() const;                                            // Returns a read-only iterator that precedes the list's front element.  Do not dereference this iterator
      const_iterator begin       () const;                                            // Returns a read-only iterator to the list's front element, end() if list is empty
      const_iterator end         () const;                                            // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator

      const_iterator cbefore_begin() const;                                           // Returns a read-only iterator that precedes the list's front element.  Do not dereference this iterator
      const_iterator cbegin       () const;                                           // Returns a read-only iterator to the list's front element, end() if list is empty
      const_iterator cend         () const;                                           // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator


      // Accessors
      T const & front() const;                                                        // Read only access to the list's front element
      T       & front();                                                              // Read-write access to the list's front element
      T const & back () const;                                                        // Read only access to the list's back element
      T       & back ();                                                              // Read-write access to the list's back element


      // Modifiers
      void clear     (                 );                                             // remove all elements setting size to zero
      void push_front( T const & value );                                             // add an element containing "value" to front of list
      void push_back ( T const & value );                                             // add an element containing "value" to back of list
      void pop_front (                 );                                             // remove element at front of list
      void pop_back  (                 ) = delete;                                    // intentionally not provided - cannot remove a node after the last one

      iterator insert_after( const_iterator position, T const & value );              // Inserts an element containing "value" into list after the one occupied at position and returns an iterator to the newly inserted value
      iterator erase_after ( const_iterator position                  );              // Removes from list the element after the one occupied at position and returns an iterator to the element after the one removed


      // Relational Operators
      std::weak_ordering operator<=>( SinglyLinkedList const & rhs ) const;
      bool               operator== ( SinglyLinkedList const & rhs ) const;





    /*******************************************************************************************************************************
    ** Extended the Linked List Implementation Example with some examples of operations solved recursively.
    **
    ** These are not in a typical Singly Linked List interface, but included here for educational discussion.  These types of
    ** exercises are often found in homework or exams.
    *******************************************************************************************************************************/
    public:
      // Add a function, named "reverse", to the Singly Linked List that reverses the list without copying the list or moving data
      // in the list. Implement the function recursively.  The function must be implemented recursively.
      //
      // Client visible interface (the public function)
      void reverse();


      // Add a function, named "find", to the Singly Linked List that returns an iterator to the first Node whose data element
      // matches, or end() if the element is not found.  The function must be implemented recursively. Assume operator+(lhs, rhs) is
      // defined
      //
      // Client visible interface (the public function)
      iterator find( T const & value );


      // Add a function, named "add" that returns the sum of all elements in the list.  The function must be implemented
      // recursively. Assume operator+(lhs, rhs) is defined
      //
      // Client visible interface (the public function)
      T add() const;


      // Add a function, named "forwardPrint", to the Singly Linked List that writes the elements to standard output from front to
      // back. The function must be implemented recursively. Assume operator+(lhs, rhs) is defined
      void forwardPrint() const;


      // Add a function, named "backwardPrint", to the Singly Linked List that writes the elements to standard output from back to
      // front. The function must be implemented recursively. Assume operator+(lhs, rhs) is defined
      void backwardPrint() const;

    private:
      // Extended interface helper functions to demonstrate recursion
      void     reverse      ( Node * current                  );
      iterator find         ( Node * current, const T & value );
      T        add          ( Node * current                  ) const;
      void     forwardPrint ( Node * current                  ) const;
      void     backwardPrint( Node * current                  ) const;
  };  // class SinglyLinkedList










  /*********************************************************************************************************************************
  ** Class SinglyLinkedList<T>::iterator - A singly linked list forward iterator
  **
  *********************************************************************************************************************************/
  template<typename T>   template<typename U>
  class SinglyLinkedList<T>::Iterator_type
  {
    friend class SinglyLinkedList<T>;

    public:
      // Iterator Type Traits - Boilerplate stuff so the iterator can be used with the rest of the standard library
      using iterator_category = std::forward_iterator_tag;
      using value_type        = U;                                                    // U is expected to be either T or T const
      using difference_type   = std::ptrdiff_t;
      using pointer           = value_type *;
      using reference         = value_type &;


      // Compiler synthesized constructors and destructor are fine, just what we want (shallow copies, no ownership)
      Iterator_type(                        ) = delete;                               // Default constructed Iterator_type not allowed - should create end(), if we knew what that was
      Iterator_type( iterator const & other );                                        // Copy constructor when T is non-const, Conversion constructor from non-const to const iterator when T is const
                                                                                      // Note parameter type is intentionally "iterator", not "Iterator_type"
      // Pre and post Increment operators move the position to the next node in the list
      Iterator_type & operator++();                                                   // advance the iterator one node (pre -increment)
      Iterator_type   operator++( int );                                              // advance the iterator one node (post-increment)

      // Dereferencing and member access operators provide access to data. The iterator itself can be constant or non-constant, but,
      // by definition, points to a non-constant linked list.
      reference operator* () const;
      pointer   operator->() const;

      // Equality operators
      bool operator==( Iterator_type const & rhs ) const;                             // Symmetrically compares all const & non-const iterator combinations, with the help of the Conversion constructor above

    private:
      // Member attributes
      Node * _nodePtr = nullptr;

      // Helper functions
      Iterator_type( Node * position );                                               // Implicit conversion constructor from pointer-to-Node to iterator-to-Node
  };  // SinglyLinkedList<T>::Iterator_type

}    // namespace CSUF::CPSC131















/***********************************************************************************************************************************
************************************************************************************************************************************
** Private Implementation
**
** Separating Interface from Implementation is an extremely important concept I hope students will come to appreciate.
************************************************************************************************************************************
***********************************************************************************************************************************/
namespace CSUF::CPSC131
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors, destructor, and assignments
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Default constructor
  template<typename T>
  SinglyLinkedList<T>::SinglyLinkedList()
    : self( std::make_unique<PrivateMembers>() )                                      // construct the list's private members
  {}




  // Copy construction
  template<typename T>
  SinglyLinkedList<T>::SinglyLinkedList( const SinglyLinkedList & original )
    : SinglyLinkedList()                                                              // delegating constructor
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the original list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & current : original )    push_back( current );
  }




  // Move construction
  template<typename T>
  SinglyLinkedList<T>::SinglyLinkedList( SinglyLinkedList && original ) noexcept
    : self( std::move(original.self) )
  {}




  // Initializer List construction
  template<typename T>
  SinglyLinkedList<T>::SinglyLinkedList( std::initializer_list<T> init_list )
    : SinglyLinkedList()                                                              // delegating constructor
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the initializer list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & current : init_list )    push_back( current );
  }





  // Copy assignment
  template<typename T>
  SinglyLinkedList<T> & SinglyLinkedList<T>::operator=( SinglyLinkedList const & rhs )
  {
    if( &rhs != this )
    {
      // Copy the values you have space for, then either truncate this list's leftovers or add the the other list's remaining values
      if( size() <= rhs.size() )
      {
        auto p = begin();
        auto q = rhs.begin();

        while( p !=     end() )    *p++ = *q++;                                       // Let's reuse as many nodes as possible
        while( q != rhs.end() )    push_back( *q++ );                                 // Add the remaining values in the rhs list to this list
      }
      else
      {
        auto p = before_begin();
        auto q = rhs.begin();

        while( q != rhs.end()        )   *++p = *q++;                                 // Let's reuse as many nodes as possible
        while( std::next(p) != end() )   erase_after(p);                              // truncate this list's leftovers (erase_after(p) changes p's next node)
      }
    }
    return *this;
  }




  // Move assignment
  template<typename T>
  SinglyLinkedList<T> & SinglyLinkedList<T>::operator=( SinglyLinkedList && rhs ) noexcept
  {
    if( &rhs != this )
    {
      clear();
      self = std::move( rhs.self );
    }
    return *this;
  }




  // Destructor
  template<typename T>
  SinglyLinkedList<T>::~SinglyLinkedList() noexcept
  { clear(); }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Queries
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // empty() const
  template<typename T>
  bool SinglyLinkedList<T>::empty() const noexcept
  { return size() == 0; }




  // size() const
  template<typename T>
  std::size_t SinglyLinkedList<T>::size() const noexcept
  { return self->_size; }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Iterators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // before_begin()
  // see design approach unique implementation




  // before_begin() const
  template<typename T>
  typename SinglyLinkedList<T>::const_iterator SinglyLinkedList<T>::before_begin() const
  { return const_cast<SinglyLinkedList<T> *>(this)->before_begin(); }                 // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // cbefore_begin() const
  template<typename T>
  typename SinglyLinkedList<T>::const_iterator SinglyLinkedList<T>::cbefore_begin() const
  {  return const_cast<SinglyLinkedList<T> *>(this)->before_begin(); }                // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // begin()
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::begin()
  { return self->_head; }




  // begin() const
  template<typename T>
  typename SinglyLinkedList<T>::const_iterator SinglyLinkedList<T>::begin() const
  { return const_cast<SinglyLinkedList<T> *>(this)->begin(); }                        // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // cbegin() const
  template<typename T>
  typename SinglyLinkedList<T>::const_iterator SinglyLinkedList<T>::cbegin() const
  { return const_cast<SinglyLinkedList<T> *>(this)->begin(); }                        // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // end()
  // see design approach unique implementation



  // end() const
  template<typename T>
  typename SinglyLinkedList<T>::const_iterator SinglyLinkedList<T>::end() const
  { return const_cast<SinglyLinkedList<T> *>(this)->end(); }                          // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // cend() const
  template<typename T>
  typename SinglyLinkedList<T>::const_iterator SinglyLinkedList<T>::cend() const
  { return const_cast<SinglyLinkedList<T> *>(this)->end(); }                          // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Accessors
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // front() const
  template<typename T>
  const T & SinglyLinkedList<T>::front() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant SLL instead of a constant SLL. The
    // "const" at the end of the function signature above is what makes this object constant.  By casting away the const, we call
    // the front() function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only
    // one place, and then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of
    // spread through the code base.
    auto non_const_SLL = const_cast<SinglyLinkedList<T> *>( this );
    return non_const_SLL->front();
  }




  // front()
  template<typename T>
  T & SinglyLinkedList<T>::front()
  {
    if( empty() )   throw std::length_error( "empty list" );

    return *begin();
  }




  // back() const
  template<typename T>
  const T & SinglyLinkedList<T>::back() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant SLL instead of a constant SLL. The
    // "const" at the end of the function signature above is what makes this object constant.  By casting away the const, we call
    // the back() function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only
    // one place, and then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of
    // spread through the code base.
    auto non_const_SLL = const_cast<SinglyLinkedList<T> *>( this );
    return non_const_SLL->back();
  }




  // back()
  template<typename T>
  T & SinglyLinkedList<T>::back()
  {
    if( empty() )   throw std::length_error( "attempt to access data from an empty list" );

    return self->_tail->_data;
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Modifiers
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // clear()
  template<typename T>
  void SinglyLinkedList<T>::clear()
  {
    while( !empty() ) pop_front();
  }




  // push_front()
  template<typename T>
  void SinglyLinkedList<T>::push_front( const T & data )
  {
    // Since a singly linked list cannot look backwards, we can only insert after a given node.  So how can we insert a node before
    // the first one?  Use a special value, beforeBegin() in this case, that says insert at the front of the list.
    insert_after( before_begin(), data );
  }




  // push_back()
  template<typename T>
  void SinglyLinkedList<T>::push_back( const T & data )
  { insert_after( self->_tail, data );}




  // pop_front()
  template<typename T>
  void SinglyLinkedList<T>::pop_front()
  {
    // Since a singly linked list cannot look backwards, we can only remove after a given node.  So how can we remove the first one?
    // Use a special value, beforeBegin() in this case, that says remove the node at the front of the list.
    erase_after( before_begin() );
  }




  // insert_before()
  // see design approach unique implementation




  // erase_after()
  // see design approach unique implementation













  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Relational Operators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // operator<=>
  template<typename T>
  std::weak_ordering SinglyLinkedList<T>::operator<=>( SinglyLinkedList const & rhs ) const
  {
    std::size_t i      = 0;
    std::size_t extent = size() < rhs.size()  ?  size()  :  rhs.size();               // min(size, rhs.size)

    for (auto p = begin(), q = rhs.begin();   i < extent;   ++i, ++p, ++q)
    {
      auto result = std::compare_weak_order_fallback( *p, *q );                       // uses operator== and operator< if operator<=> is unavailable
      if( result != 0 ) return result;
    }
    return size() <=> rhs.size();
  }




  // operator==
  template<typename T>
  bool SinglyLinkedList<T>::operator==( SinglyLinkedList const & rhs ) const
  {
    if( size() != rhs.size() ) return false;

    for (auto p = begin(), end = this->end(), q = rhs.begin();   p != end;   ++p, ++q)
    {
      if( *p != *q ) return false;
    }

    return true;
  }









  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Non-member functions
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // swap()
  template<typename T>
  void swap( SinglyLinkedList<T> & lhs, SinglyLinkedList<T> & rhs )
  {
    using std::swap;                                                                  // Let argument dependent lookup (ADL) find the right swap function
    swap( lhs.self, rhs.self );                                                       // and avoid a explicit call to std::swap()
  }

















  /*********************************************************************************************************************************
  **********************************************************************************************************************************
  ** SinglyLinkedList<>::iterator Member Function Definitions
  **
  *********************************************************************************************************************************/

  // Copy constructor when U is non-const iterator, Conversion constructor from non-const to const iterator when U is a const iterator
  // Type of parameter is intentionally a non-constant iterator
  template<typename T>   template<typename U>
  SinglyLinkedList<T>::Iterator_type<U>::Iterator_type( iterator const & other )      // Notice the parameter type is "iterator", not "Iterator_type"
    : _nodePtr{ other._nodePtr }
  {}




  // Conversion Constructor from pointer-to-Node to iterator
  template<typename T>   template<typename U>
  SinglyLinkedList<T>::Iterator_type<U>::Iterator_type( Node * p )
    : _nodePtr{ p }
  {}




  // operator++   pre-increment
  template<typename T>   template<typename U>
  typename SinglyLinkedList<T>::template Iterator_type<U> &   SinglyLinkedList<T>::Iterator_type<U>::operator++()
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( "Attempt to increment null iterator" );

    _nodePtr = _nodePtr->_next;
    return *this;
  }




  // operator++   post-increment
  template<typename T>   template<typename U>
  typename SinglyLinkedList<T>::template Iterator_type<U>   SinglyLinkedList<T>::Iterator_type<U>::operator++( int )
  {
    auto temp{ *this };
    operator++();                                                                     // Delegate to pre-increment leveraging error checking
    return temp;
  }




  // operator*
  template<typename T>   template<typename U>
  typename SinglyLinkedList<T>::template Iterator_type<U>::reference   SinglyLinkedList<T>::Iterator_type<U>::operator*() const
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( "Attempt to dereference null iterator" );

    return _nodePtr->_data;
  }




  // operator->
  template<typename T>   template<typename U>
  typename SinglyLinkedList<T>::template Iterator_type<U>::pointer   SinglyLinkedList<T>::Iterator_type<U>::operator->() const
  {
    if( _nodePtr == nullptr )  throw std::invalid_argument( "Attempt to dereference null iterator" );

    return &(_nodePtr->_data);
  }




  // operator==
  template<typename T>   template<typename U>
  bool SinglyLinkedList<T>::Iterator_type<U>::operator==( Iterator_type const & rhs ) const
  { return _nodePtr == rhs._nodePtr; }















  /*********************************************************************************************************************************
  **********************************************************************************************************************************
  ** Extended the Linked List Private Members Implementation Example with some examples of operations solved recursively.
  **
  ** These are not in a typical Singly Linked List interface, but included here for educational discussion.  These types of
  ** exercises are often found in homework or exams.
  *********************************************************************************************************************************/

  ///////////////////  Reverse  ///////////////////////
  // Client facing public function
  template<typename T>
  void SinglyLinkedList<T>::reverse()
  {
    auto & head = self->_head;                                                        // an easier to read alias for the head of the list
    auto & tail = self->_tail;                                                        // an easier to read alias for the tail of the list

    reverse( head );                                                                  // Kick off the recursion starting at the head. Note the call to overloaded, private reverse function
    std::swap( head, tail );                                                          // Now swap the head and tail pointers
  }



  // The private helper function
  template<typename T>
  void SinglyLinkedList<T>::reverse( Node * current )
  {
    // Base Case - There is nothing more to do if we're reversing an empty list or we're at the last node in a non-empty list
    if( end() == current || end() == current->_next ) return;

    // Recurse - Move down the list to the next node
    reverse( current->_next );

    // Visit - While unwinding (moving from tail towards head), make the next node point to this node
    current->_next->_next = current;
    current->_next        = end()._nodePtr;
  }






  ///////////////////  Add  ///////////////////////
  // Client facing public function
  template<typename T>
  T SinglyLinkedList<T>::add() const
  {
    return add( self->_head );                                                        // Kick off the recursion starting at the head. Note the call to overloaded, private add function
  }



  // The private helper function
  template<typename T>
  T SinglyLinkedList<T>::add( Node * current ) const
  {
    // Base Case - reached the end of the list.  Can't return zero, T may not be an integer type. Return the value initialized
    //             default value instead
    if( end() == current )   return T();

    // Use a postorder traversal where the right hand side is progressively easier and easier and eventually trivial to solve
    // Visit - progressively return the sum of the left and right sides
    return current->_data +
           // Recurse - solve a smaller, easier problem
           add( current->_next );
  }






  ///////////////////  Find  ///////////////////////
  // Client facing public function
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::find( const T & data )
  {
    return find( self->_head, data );                                                 // Kick off the recursion starting at the head. Note the call to the overloaded, private find function
  }



  // The private helper function
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::find( Node * current, const T & data )
  {
    // Base Case - reached the end of the list without finding what we're looking for
    if( end() == current )   return end();

    // Visit - found what we're looking for in the current node
    if( current->_data == data ) return current;

    // Recurse - keep looking on in a smaller list
    return find( current->_next, data );
  }






  ///////////////////  Forward Print  ///////////////////////
  // Client facing public function
  template<typename T>
  void SinglyLinkedList<T>::forwardPrint() const
  {
    forwardPrint( self->_head );                                                      // Kick off the recursion starting at the head. Note the call to the overloaded, private forwardPrint function
  }



  // The private helper function
  template<typename T>
  void SinglyLinkedList<T>::forwardPrint( Node * current ) const
  {
    // Base Case - reached the end of the list
    if( end() == current )   return;

    // Visit - print the contents
    std::cout << current->_data;
    if( end() != current->_next )   std::cout << ", ";

    // Recurse - print a smaller list
    forwardPrint( current->_next );
  }






  ///////////////////  Backward Print  ///////////////////////
  // Client facing public function
  template<typename T>
  void SinglyLinkedList<T>::backwardPrint() const
  {
    backwardPrint( self->_head );                                                     // Kick off the recursion starting at the head. Note the call to the overloaded, private backwardPrint function
  }



  // The private helper function
  template<typename T>
  void SinglyLinkedList<T>::backwardPrint( Node * current ) const
  {
    // Base Case - reached the end of the list
    if( end() == current )   return;

    // Recurse - print a smaller list
    backwardPrint( current->_next );

    // Visit - print the contents
    if( end() != current->_next )   std::cout << ", ";
    std::cout << current->_data;
  }
}    // namespace CSUF::CPSC131











// There are two SLL implementation examples available, and I hope to do a third someday using arrays/vectors and indexes instead of
// pointers
//    o) Circular SLL with 1 Dummy Node
//    o) Null terminated SLL
//    o) Array-based SLL - (coming someday - exercise left for the reader?)
//
// Note that each implementation uses the same interface.  That is, the SLL as a Data Structure abstraction is not affected by how
//      it is implemented. Separating Interface from Implementation is an extremely important concept I hope students will come to
//      appreciate. Also note a large majority of code is identical to each implementation and that shared, common code is provided
//      above.  The code unique to each implementation is separated into their own files, #included here.
//
//
// Specifically, the following structures and functions are unique to each design approach and must be defined (implemented) in the
// context of class SinglyLinkedList<T> in the included file
//   o)    struct Node          {T _data;   Node * _next;                 };
//   o)    struct PrivateMembers{Node * _head, _tail;  std::size_t _size; };
//
//   o)    iterator  insert_after( const_iterator position, T const & data )
//   o)    iterator  erase_after ( const_iterator position                 )
//   o)    iterator  before_begin(                                         )
//   o)    iterator  end         (                                         )
//
// That's it!  That's all that makes one implementation different from another :)


// A client may #include this file and take the default implementation (circular with one dummy node), or the client may instead
// directly #include the *.hxx file to specifically select a desired implementation
#if !defined( CSUF_CPSC131_SLL_SELECTION )
  #if __has_include( "SinglyLinkedList_circular.hxx" )
    #include "SinglyLinkedList_circular.hxx"

  #elif __has_include( "SinglyLinkedList_null_terminated.hxx" )
    #include "SinglyLinkedList_null_terminated.hxx"

  #else
    #error A specific Singly Linked List design approach file was not found.  Looking for "SinglyLinkedList_circular.hxx" or "SinglyLinkedList_null_terminated.hxx"
  #endif
#endif












/***********************************************************************************************************************************
** (C) Copyright 2022 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The participating authors at California State University's
*Computer Science Department have used their best efforts
** in preparing this code. These efforts include the development, research, and
*testing of the theories and programs to determine
** their effectiveness. The authors make no warranty of any kind, expressed or
*implied, with regard to these programs or to the
** documentation contained within. The authors shall not be liable in any event
*for incidental or consequential damages in
** connection with, or arising out of, the furnishing, performance, or use of
*these libraries and programs.  Distribution without
** written consent from the authors is prohibited.
***********************************************************************************************************************************/

/**************************************************
** Last modified:  03-JAN-2022
** Last Verified:  03-JAN-2022
** Verified with:  MS Visual Studio 2019 Version 16.11.8 (C++20)
**                 GCC version 11.2.1 20211124 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
