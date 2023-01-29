/***********************************************************************************************************************************
** Class DoublyLinkedList - a very basic example implementation of the Doubly Linked List Abstract Data Type
**
**  The ADT's interface is a small subset of std::list defined at https://en.cppreference.com/w/cpp/container/list
**
**  One major differences between this example and std::list
**  1) Like std::list. this example provides the rbegin() and rend() operations as part of the doubly linked list abstraction, but
**     unlike std:list that returns revere_iterators, this example returns (normal) iterators.  rbegin() still returns an iterator
**     to the last element in the list, but to move to the next element the iterator needs to be decremented, not incremented as
**     with a true reverse iterator.
***********************************************************************************************************************************/
#pragma once
#include <compare>                                                                    // week_ordering, strong_quality
#include <cstddef>                                                                    // size_t
#include <initializer_list>                                                           // initializer_list
#include <memory>                                                                     // unique_ptr, make_unique<>()
#include <stdexcept>                                                                  // length_error
#include <utility>                                                                    // swap(), move()



namespace CSUF::CPSC131
{
  // Template Class Definition
  template <typename T>
  class DoublyLinkedList
  {
    template <typename U>
    friend void swap( DoublyLinkedList<U> & lhs, DoublyLinkedList<U> & rhs );         // The expected way to make a program-defined type swappable is to provide a
                                                                                      // non-member function swap in the same namespace as the type.
                                                                                      // (https://en.cppreference.com/w/cpp/algorithm/swap)

    private:
      // Types
      template <typename U> class  Iterator_type;                                     // Template class for iterator and const_iterator classes
      struct Node;                                                                    // Specific implementations are responsible for defining their node structure
      struct PrivateMembers;                                                          // A specific implementation's private members (attributes, functions, etc)

      // Member attributes
      std::unique_ptr<PrivateMembers> self;                                           // See Pimpl Idiom at https://en.cppreference.com/w/cpp/language/pimpl and https://cpppatterns.com/patterns/pimpl.html



    public:
      // Types
      using iterator       = Iterator_type<T      >;                                  // A bi-directional iterator to a read-write value in the list
      using const_iterator = Iterator_type<T const>;                                  // A bi-directional iterator to a read-only value in the list


      // Constructors, destructor, and assignments
      DoublyLinkedList            (                                       );          // default constructor
      DoublyLinkedList            ( DoublyLinkedList const    & original  );          // copy constructor
      DoublyLinkedList            ( DoublyLinkedList         && original  ) noexcept; // move constructor
      DoublyLinkedList            ( std::initializer_list<T>    init_list );          // initialization list constructor
      DoublyLinkedList & operator=( DoublyLinkedList const    & rhs       );          // copy assignment
      DoublyLinkedList & operator=( DoublyLinkedList         && rhs       ) noexcept; // move assignment
     ~DoublyLinkedList            (                                       ) noexcept; // destructor


      // Queries
      bool        empty() const noexcept;                                             // returns true if list has no items
      std::size_t size () const noexcept;                                             // returns the number of elements in the list
      T const &   front() const;                                                      // return list's front element
      T const &   back () const;                                                      // return list's back element


      // Iterators
      iterator begin();                                                               // Returns a read-write iterator to the list's front element, end() if list is empty
      iterator end  ();                                                               // Returns a read-write iterator beyond the list's back element.  Do not dereference this iterator

      const_iterator begin () const;                                                  // Returns a read-only iterator to the list's front element, end() if list is empty
      const_iterator end   () const;                                                  // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator

      const_iterator cbegin() const;                                                  // Returns a read-only iterator to the list's front element, end() if list is empty
      const_iterator cend  () const;                                                  // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator


      // Accessors
      T & front();                                                                    // Read-write access to the list's front element
      T & back ();                                                                    // Read-write access to the list's back element


      // Modifiers
      void clear     (                 );                                             // erase all elements setting size to zero
      void push_front( T const & value );                                             // add an element containing "value" to front of list
      void push_back ( T const & value );                                             // add an element containing "value" to back of list
      void pop_front (                 );                                             // erase element at front of list
      void pop_back  (                 );                                             // erase element at back of list

      iterator insert( const_iterator position, T const & value );                    // Inserts an element containing "value" into list before the one occupied at position and returns an iterator to the newly inserted value
      iterator erase ( const_iterator position                  );                    // Removes from list the element occupied at position and returns an iterator to the element after the one removed


      // Relational Operators
      std::weak_ordering operator<=>( DoublyLinkedList const & rhs ) const;
      bool               operator== ( DoublyLinkedList const & rhs ) const;
  };  // class DoublyLinkedList












  /*******************************************************************************
  ** Class DoublyLinkedList<T>::iterator - A doubly linked bi-directional iterator
  *******************************************************************************/
  template<typename T>   template<typename U>
  class DoublyLinkedList<T>::Iterator_type
  {
    friend class DoublyLinkedList<T>;

    public:
      // Iterator Type Traits - Boilerplate stuff so the iterator can be used with the rest of the standard library
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type        = U;
      using difference_type   = std::ptrdiff_t;
      using pointer           = value_type *;
      using reference         = value_type &;


      // Compiler synthesized constructors and destructor are fine, just what we want (shallow copies, no ownership) but needed to
      // explicitly say that because there is also a user defined constructor
      Iterator_type(                        ) = delete;                               // Default constructed Iterator_type not allowed - should create end(), if we knew what that was
      Iterator_type( iterator const & other );                                        // Copy constructor when T is non-const, Conversion constructor from non-const to const iterator when T is const

      // Pre and post Increment operators move the position to the next node in the list
      Iterator_type & operator++();                                                   // advance the iterator one node (pre -increment)
      Iterator_type   operator++( int );                                              // advance the iterator one node (post-increment)
                                                                                      // Note parameter type is intentionally "iterator", not "Iterator_type"
      // Pre and post Decrement operators move the position to the previous node in the list
      Iterator_type & operator--();                                                   // retreat the iterator one node (pre -decrement)
      Iterator_type   operator--( int );                                              // retreat the iterator one node (post-decrement)

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
  };  // DoublyLinkedList<T>::Iterator_type

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
  DoublyLinkedList<T>::DoublyLinkedList()
    : self( std::make_unique<PrivateMembers>() )
  {}



  // Copy constructor
  template<typename T>
  DoublyLinkedList<T>::DoublyLinkedList( const DoublyLinkedList & original )
    : DoublyLinkedList()
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the original list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & element : original )    push_back( element );
  }



  // Move constructor
  template<typename T>
  DoublyLinkedList<T>::DoublyLinkedList( DoublyLinkedList && original ) noexcept
    : self{ std::move( original.self ) }
  {}



  // Initializer list constructor
  template<typename T>
  DoublyLinkedList<T>::DoublyLinkedList( std::initializer_list<T> init_list )
    : DoublyLinkedList()
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the initializer list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & element : init_list )    push_back( element );
  }



  // Copy assignment
  template<typename T>
  DoublyLinkedList<T> & DoublyLinkedList<T>::operator=( DoublyLinkedList const & rhs )
  {
    if( &rhs != this )
    {
      auto p = begin();
      auto q = rhs.begin();

      // Copy the values you have space for, then either truncate this list's leftovers or add the the other list's remaining values
      if( size() <= rhs.size() )
      {
        while( p !=     end() )    *p++ = *q++;                               // Let's reuse as many nodes as possible
        while( q != rhs.end() )    push_back( *q++ );                         // Add the remaining values in the rhs list to this list
      }
      else
      {
        while( q      != rhs.end () )   *p++ = *q++;                          // Let's reuse as many nodes as possible
        while( size() != rhs.size() )   pop_back();                           // truncate this list's leftovers
      }
    }
    return *this;
  }



  // Move assignment
  template<typename T>
  DoublyLinkedList<T> & DoublyLinkedList<T>::operator=( DoublyLinkedList && rhs ) noexcept
  {
    if (&rhs != this)
    {
      clear();
      self = std::move( rhs.self );
    }
    return *this;
  }



  // Destructor
  template<typename T>
  DoublyLinkedList<T>::~DoublyLinkedList() noexcept
  { clear(); }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Queries
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // empty() const
  template<typename T>
  bool DoublyLinkedList<T>::empty() const noexcept
  {
    return size() == 0;
  }



  // size() const
  template<typename T>
  std::size_t DoublyLinkedList<T>::size() const noexcept
  { return self->_size; }



  // front() const
  template<typename T>
  const T & DoublyLinkedList<T>::front() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant DLL instead of a constant DLL. The "const"
    // at the end of the function signature above is what makes this object constant.  By casting away the const, we call the front()
    // function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only one place, and
    // then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of spread through the
    // code base.
    auto non_const_DLL = const_cast<DoublyLinkedList<T> *>( this );
    return non_const_DLL->front();
  }



  // back() const
  template<typename T>
  const T & DoublyLinkedList<T>::back() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant DLL instead of a constant DLL. The
    // "const" at the end of the function signature above is what makes this object constant.  By casting away the const, we call
    // the back() function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only
    // one place, and then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of
    // spread through the code base.
    auto non_const_DLL = const_cast<DoublyLinkedList<T> *>( this );
    return non_const_DLL->back();
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Iterators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // begin()
  template<typename T>
  typename DoublyLinkedList<T>::iterator DoublyLinkedList<T>::begin()
  { return self->_head; }



  // begin() const
  template<typename T>
  typename DoublyLinkedList<T>::const_iterator DoublyLinkedList<T>::begin() const
  { return const_cast<DoublyLinkedList<T> *>(this)->begin(); }                        // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // cbegin() const
  template<typename T>
  typename DoublyLinkedList<T>::const_iterator DoublyLinkedList<T>::cbegin() const
  { return const_cast<DoublyLinkedList<T> *>(this)->begin(); }                        // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // end()
  // see design approach unique implementation



  // end() const
  template<typename T>
  typename DoublyLinkedList<T>::const_iterator DoublyLinkedList<T>::end() const
  { return const_cast<DoublyLinkedList<T> *>(this)->end(); }                          // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // cend() const
  template<typename T>
  typename DoublyLinkedList<T>::const_iterator DoublyLinkedList<T>::cend() const
  { return const_cast<DoublyLinkedList<T> *>(this)->end(); }                          // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Accessors
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // front()
  template<typename T>
  T & DoublyLinkedList<T>::front()
  {
    if( empty() )   throw std::length_error( "empty list" );

    return self->_head->_data;
  }



  // back()
  template<typename T>
  T & DoublyLinkedList<T>::back()
  {
    if( empty() )    throw std::length_error( "attempt to access data from an empty list" );
    return self->_tail->_data;
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Modifiers
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // clear()
  template<typename T>
  void DoublyLinkedList<T>::clear()
  { while( !empty() ) pop_front(); }



  // push_front()
  template<typename T>
  void DoublyLinkedList<T>::push_front( T const & value )
  { insert( self->_head, value ); }



  // pop_front
  template<typename T>
  void DoublyLinkedList<T>::pop_front()
  { erase( self->_head ); }



  // push_back()
  template<typename T>
  void DoublyLinkedList<T>::push_back( T const & value )
  { insert( end(), value  ); }



  // pop_back()
  template<typename T>
  void DoublyLinkedList<T>::pop_back()
  { erase( self->_tail ); }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Relational Operators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // operator<=>
  template<typename T>
  std::weak_ordering DoublyLinkedList<T>::operator<=>( DoublyLinkedList const & rhs ) const
  {
    std::size_t i      = 0;
    std::size_t extent = size() < rhs.size()  ?  size()  :  rhs.size();               // min(size, rhs.size)

    for( auto p = begin(), q = rhs.begin();   i < extent;   ++i, ++p, ++q )
    {
      auto result = std::compare_weak_order_fallback( *p, *q );                       // uses operator== and operator< if operator<=> is unavailable
      if( result != 0 ) return result;
    }
    return size() <=> rhs.size();
  }




  // operator==
  template<typename T>
  bool DoublyLinkedList<T>::operator==( DoublyLinkedList const & rhs ) const
  {
    if( size() != rhs.size() ) return false;

    for( auto p = begin(), end = this->end(), q = rhs.begin();   p != end;   ++p, ++q )
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
  void swap( DoublyLinkedList<T> & lhs, DoublyLinkedList<T> & rhs )
  {
    using std::swap;                                                                  // Let argument dependent lookup (ADL) find the right swap function
    swap( lhs.self, rhs.self );                                                       // and avoid a explicit call to std::swap()
  }












  /*********************************************************************************************************************************
  **********************************************************************************************************************************
  ** DoublyLinkedList<>::iterator Member Function Definitions
  **
  *********************************************************************************************************************************/

  // Copy constructor when U is non-const iterator, Conversion constructor from non-const to const iterator when U is a const iterator
  // Type of parameter is intentionally a non-constant iterator
  template<typename T>   template<typename U>
  DoublyLinkedList<T>::Iterator_type<U>::Iterator_type( iterator const & other )      // Notice the parameter type is "iterator", not "Iterator_type"
    : _nodePtr{ other._nodePtr }
  {}




  // Conversion Constructor from pointer-to-Node to iterator
  template<typename T>   template<typename U>
  DoublyLinkedList<T>::Iterator_type<U>::Iterator_type( Node * p )
    : _nodePtr{ p }
  {}




  // operator++   pre-increment
  template<typename T>   template<typename U>
  typename DoublyLinkedList<T>::template Iterator_type<U> &   DoublyLinkedList<T>::Iterator_type<U>::operator++()
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( "Attempt to increment null iterator.  Cannot increment end() for a null-terminated list" );

    _nodePtr = _nodePtr->_next;
    return *this;
  }




  // operator++   post-increment
  template<typename T>   template<typename U>
  typename DoublyLinkedList<T>::template Iterator_type<U>   DoublyLinkedList<T>::Iterator_type<U>::operator++( int )
  {
    auto temp{ *this };
    operator++();                                                                     // Delegate to pre-increment leveraging error checking
    return temp;
  }




  // operator--   pre-decrement
  template<typename T>   template<typename U>
  typename DoublyLinkedList<T>::template Iterator_type<U> &   DoublyLinkedList<T>::Iterator_type<U>::operator--()
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( "Attempt to decrement null iterator.  Cannot decrement end() for a null-terminated list" );

    _nodePtr = _nodePtr->_prev;
    return *this;
  }




  // operator--   post-decrement
  template<typename T>   template<typename U>
  typename DoublyLinkedList<T>::template Iterator_type<U>    DoublyLinkedList<T>::Iterator_type<U>::operator--( int )
  {
    auto temp( *this );
    operator--();                                                                     // Delegate to pre-decrement leveraging error checking
    return temp;
  }




  // operator*
  template<typename T>   template<typename U>
  typename DoublyLinkedList<T>::template Iterator_type<U>::reference   DoublyLinkedList<T>::Iterator_type<U>::operator*() const
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( "Attempt to dereference null iterator" );

    return _nodePtr->_data;
  }




  // operator->
  template<typename T>   template<typename U>
  typename DoublyLinkedList<T>::template Iterator_type<U>::pointer   DoublyLinkedList<T>::Iterator_type<U>::operator->() const
  {
    if( _nodePtr == nullptr )  throw std::invalid_argument( "Attempt to dereference null iterator" );

    return &(_nodePtr->_data);
  }




  // operator==
  template<typename T>   template<typename U>
  bool DoublyLinkedList<T>::Iterator_type<U>::operator==( Iterator_type const & rhs ) const
  { return _nodePtr == rhs._nodePtr; }
}    // namespace CSUF::CPSC131
























// There are two DLL implementation examples available, and I hope to do a third someday using arrays/vectors and indexes instead of
// pointers
//    o) Circular DLL with 1 Dummy Node
//    o) Null terminated DLL
//    o) Array-based DLL - (coming someday - exercise left for the reader?)
//
// Note that each implementation uses the same interface.  That is, the DLL as a Data Structure abstraction is not affected by how
//      it is implemented. Separating Interface from Implementation is an extremely important concept I hope students will come to
//      appreciate. Also note a large majority of code is identical to each implementation and that shared, common code is provided
//      above.  The code unique to each implementation is separated into their own files, #included here.
//
//
// Specifically, the following structures and functions are unique to each design approach and must be defined (implemented) in the
// context of class SinglyLinkedList<T> in the included file
//   o)    struct Node          {T _data;   Node * _next, _prev;          };
//   o)    struct PrivateMembers{Node * _head, _tail;  std::size_t _size; };
//
//   o)    iterator  insert( const_iterator position, T const & data )
//   o)    iterator  erase ( const_iterator position                 )
//   o)    iterator  end   (                                         )
//
// That's it!  That's all that makes one implementation different from another :)

// A client may #include this file and take the default implementation (circular with one dummy node), or the client may instead
// directly #include the *.hxx file to specifically select a desired implementation
#if !defined( CSUF_CPSC131_DLL_SELECTION )
  #if __has_include( "DoublyLinkedList_circular.hxx" )
    #include "DoublyLinkedList_circular.hxx"

  #elif __has_include( "DoublyLinkedList_null_terminated.hxx" )
    #include "DoublyLinkedList_null_terminated.hxx"

  #else
    #error A specific Doubly Linked List design approach file was not found.  Looking for "DoublyLinkedList_circular.hxx" or "DoublyLinkedList_null_terminated.hxx"
  #endif
#endif












/***********************************************************************************************************************************
** (C) Copyright 2022 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The participating authors at California State University's Computer Science Department have used their best efforts
** in preparing this code. These efforts include the development, research, and testing of the theories and programs to determine
** their effectiveness. The authors make no warranty of any kind, expressed or implied, with regard to these programs or to the
** documentation contained within. The authors shall not be liable in any event for incidental or consequential damages in
** connection with, or arising out of, the furnishing, performance, or use of these libraries and programs.  Distribution without
** written consent from the authors is prohibited.
***********************************************************************************************************************************/

/**************************************************
** Last modified:  03-JAN-2022
** Last Verified:  03-JAN-2022
** Verified with:  MS Visual Studio 2019 Version 16.11.8 (C++20)
**                 GCC version 11.2.1 20211124 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
