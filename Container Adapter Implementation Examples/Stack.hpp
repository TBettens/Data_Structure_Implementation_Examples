/***********************************************************************************************************************************
** Class Stack - a very basic example implementation of the Last In First Out (LIFO) Abstract Data Type.  The Stack ADT is an
**               adapter that provides a consistent LIFO interface over an underlying container supporting constant time insert and
**               remove operations
**
**  The Stack ADT's interface is a small subset of std::stack defined at https://en.cppreference.com/w/cpp/container/stack
**
***********************************************************************************************************************************/
#pragma once

#include <array>
#include <stdexcept>                                              // out_of_range
#include <cstddef>                                                // size_t
#include <iterator>                                               // distance()
#include <string>                                                 // to_string()

#include "Vector.hpp"





namespace CSUF::CPSC131
{
  /*********************************************************************************************************************************
  ** Primary Class Definition
  *********************************************************************************************************************************/
  // Default the underlying container to a Vector.  Other choices include any of the Sequence Container types, such as a singly or
  // double linked list, deque, and so on.  The array implementation is specialized below. The underlying container must support the
  // following operations:
  //   o)  insert
  //   o)  erase
  //   o)  size
  //   o)  empty
  //   0)  error detection
  //
  // The implementation of the Stack's operations are designed to pick constant time insert and erase functions appropriate for the
  // underlying container.


  template<typename T, class UnderlyingContainer = CSUF::CPSC131::Vector<T>>
  class Stack
  {
    public:
      // Constructors, destructor, and assignments
      // Compiler synthesized default constructors, assignments, and destructor are okay


      // Queries
      bool        empty() const noexcept;                         // returns true if the stack contains no elements, false otherwise
      std::size_t size () const noexcept;                         // returns the number of elements in the stack


      // Accessors
      T const & top() const;                                      // returns a read only reference to the top of the stack
      T       & top();                                            // returns a read-write reference to the top of the stack


      // Modifiers
      void   push( T const & value );                             // puts the value at the top of the stack and increments size
      void   pop (                 );                             // removes the element at the top of the stack and decrements size


      // Relational Operators
      bool operator==( Stack const & rhs ) const = default;       // returns true if this stack and the rhs stack have the same number of values in the same order, false otherwise
                                                                  // delegates to underlying container;  ordering not supported

    private:
      UnderlyingContainer  _collection;                           // delegate object management
  };


















  /*********************************************************************************************************************************
  ** Partial Specialization Class Definition for a fixed sized Stack over a fixed sized standard array
  *********************************************************************************************************************************/
  template<typename T, std::size_t CAPACITY>
  class Stack<T, std::array<T, CAPACITY>>
  {
    public:
      // Constructors, destructor, and assignments
      // Compiler synthesized default constructors, assignments, and destructor are okay


      // Queries
      bool        empty() const noexcept;                         // returns true if the stack contains no elements, false otherwise
      std::size_t size () const noexcept;                         // returns the number of elements in the stack


      // Accessors
      T const & top() const;                                      // returns a read only reference to the top of the stack
      T       & top();                                            // returns a read-write reference to the top of the stack


      // Modifiers
      void   push( T const & value );                             // puts the value at the top of the stack and increments size
      void   pop (                 );                             // removes the element at the top of the stack and decrements size


      // Relational Operators
      bool operator==( Stack const & rhs ) const;                 // returns true if this stack and the rhs stack have the same number of values in the same order, false otherwise
                                                                  // ordering not supported


    private:
      // Since the "array" structure has, by definition, fixed size and capacity, it's not possible to delegate all the object
      // management responsibilities as was done above. An attribute has been added to maintain the number of elements the client
      // has placed the stack.
      std::size_t             _size = 0;                          // The number of elements in the stack, 0 indicates an empty container
      std::size_t &           _top  = _size;                      // There is just one attribute, call it size or top.  It's the same thing.
                                                                  // There is nothing in the slot indexed by Size/Top, so don't use it to retrieve data
                                                                  // Size/Top represents the next slot in the array to fill
      std::array<T, CAPACITY> _collection;
  };
}  // namespace CSUF::CPSC131


















namespace CSUF::CPSC131
{
  #ifndef CSUF_CPSC131_STACK_HELPER_CONCEPTS
  #define CSUF_CPSC131_STACK_HELPER_CONCEPTS
    // Stack helper concepts used to select which end of the underlying container to use.  You have to push and pop from the same
    // end, but which end makes a difference.  Using the wrong end of the container could result in a linear-time (O(n)) operation
    // instead of the required constant-time (O(1)) operation.
    template <typename Container>
    concept is_front_insertable = requires(Container c)
    { // all are required
      c.push_front( c.front() );
      c.pop_front();
      c.front();
    };

    template <typename Container>
    concept has_size = requires(Container c) {c.size();};
  #endif


















  /*********************************************************************************************************************************
  ** Primary Class Implementation
  ** The implementations defined in this section delegate insert, remove, size, and empty operations to the underlying container.
  *********************************************************************************************************************************/
  // push()
  template<typename T, class UnderlyingContainer>
  void Stack<T, UnderlyingContainer>::push( T const & value )
  {
    // Delegate Stack::push() to the underlying container's push() functions.  Some containers support pushing in constant time only
    // at the front while others only at the back, so be smart about how we delegate.  Our goal is to maintain pushing onto the
    // Stack in constant (or at least amortized constant) time.  Let the underlying container handle error conditions, if any.

    // If the underlying container supports pushing to the front in constant time, then do that.  Otherwise push to the back.
    if constexpr( is_front_insertable<UnderlyingContainer> ) _collection.push_front( value );   // typically node-based structures (singly and doubly linked lists, etc) and deque
    else                                                     _collection.push_back ( value );   // typically array-based structures (vector, etc)
  }






  // pop()
  template<typename T, class UnderlyingContainer>
  void Stack<T, UnderlyingContainer>::pop()
  {
    // Delegate Stack::pop() to the underlying container's pop() function.  The element must be removed from the same end of the
    // underlying container as was added in function Stack::push()

    // If you pushed to the front, then you must pop from the front.  Otherwise pop from the back.
    if constexpr( is_front_insertable<UnderlyingContainer> ) _collection.pop_front();           // typically node-based structures (singly and doubly linked lists, etc) and deque
    else                                                     _collection.pop_back ();           // typically array-based structures (vector, etc)
  }






  // top() const
  template<typename T, class UnderlyingContainer>
  const T & Stack<T, UnderlyingContainer>::top() const
  {
    return const_cast<Stack<T, UnderlyingContainer> *>(this)->top();                            // delegate to the read-write version of Stack::top
  }






  // top()
  template<typename T, class UnderlyingContainer>
  T & Stack<T, UnderlyingContainer>::top()
  {
    // Delegate Stack::top() to the underlying container's accessor function.  If you pushed to the front, then the top of the stack
    // is at the front. Otherwise its at the back.
    if constexpr( is_front_insertable<UnderlyingContainer> ) return _collection.front();        // typically node-based structures (singly and doubly linked lists, etc) and deque
    else                                                     return _collection.back ();        // typically array-based structures (vector, etc)
  }






  // empty() const
  template<typename T, class UnderlyingContainer>
  bool Stack<T, UnderlyingContainer>::empty() const noexcept
  { return _collection.empty(); }






  // size() const
  template<typename T, class UnderlyingContainer>
  std::size_t Stack<T, UnderlyingContainer>::size() const noexcept
  {
    // If the underlying container supports querying for size in constant time, then do that.  Otherwise calculate the size by counting elements in the container.
    if constexpr( has_size<UnderlyingContainer> )    return _collection.size();                                              // typically, everything but std::forward_list
    else                                             return std::distance( _collection.begin(), _collection.end() );         // typically, std::forward_list
  }


















  /*********************************************************************************************************************************
  ** Partial Specialization for a fixed sized Stack over a fixed sized standard array.
  ** An array does not support insertion and erasure, so we need to do it our self.  There isn't anything to delegate to.
  *********************************************************************************************************************************/
  // push()
  template<typename T, std::size_t CAPACITY>
  void Stack<T, std::array<T, CAPACITY>>::push( const T & element )
  {
    // verify there is capacity for another value
    if( _size >= CAPACITY )    throw std::out_of_range( "ERROR:  Attempt to add to an already full stack of " + std::to_string( CAPACITY ) + " elements." );


    // add the value to the back of the container (e.g. top of the stack) and then increment stack's size
    //  Before:  size = 4, capacity = 6
    //      0      1      2      3      4      5
    //   +------+------+------+------+------+------+
    //   | data | data | data | data |      |      |
    //   +------+------+------+------+------+------+
    //                                   ^
    //                               Size/Top
    //
    //
    //  After:  size = 5, capacity = 6
    //      0      1      2      3      4      5
    //   +------+------+------+------+------+------+
    //   | data | data | data | data | data |      |
    //   +------+------+------+------+------+------+
    //                                          ^
    //                                      Size/Top
    _collection[_size] = element;
    ++_size;
  }  // push()






  // pop()
  template<typename T, std::size_t CAPACITY>
  void Stack<T, std::array<T, CAPACITY>>::pop()
  {
    // verify there is something to remove
    if( empty() )    throw std::out_of_range( "ERROR:  Attempt to remove a value from an empty stack" );

    // remove the value from the back of the container (e.g. top of the stack) by decrementing the stack's size.  A more robust
    // implementation would destroy the value removed, but for now let's assume the elements are trivial (hold no resources)
    //  Before:  size = 4, capacity = 6
    //      0      1      2      3      4      5
    //   +------+------+------+------+------+------+
    //   | data | data | data | data |      |      |
    //   +------+------+------+------+------+------+
    //                                   ^
    //                               Size/Top
    //
    //
    //  After:  size = 3, capacity = 6
    //      0      1      2      3      4      5
    //   +------+------+------+------+------+------+
    //   | data | data | data |      |      |      |
    //   +------+------+------+------+------+------+
    //                            ^
    //                        Size/Top
    --_size;
    _collection[_size] = T{};     // We can't really destroy the element (array cells are always populated), but by setting the value to the default might release any resource held
  }






  // top() const
  template<typename T, std::size_t CAPACITY>
  const T & Stack<T, std::array<T, CAPACITY>>::top() const
  {
    return const_cast<Stack<T, std::array<T, CAPACITY>> *>(this)->top();  // delegate to the read-write version of top
  }






  // top()
  template<typename T, std::size_t CAPACITY>
  T & Stack<T, std::array<T, CAPACITY>>::top()
  {
    // verify there is something in the stack to look at
    if( empty() ) throw std::out_of_range( "ERROR:  Attempt to view an value from an empty stack" );

    // Return a reference to the back of the array (top of the stack)
    return _collection[_size - 1];
  }






  // empty() const
  template<typename T, std::size_t CAPACITY>
  bool Stack<T, std::array<T, CAPACITY>>::empty() const noexcept
  { return size() == 0; }






  // size() const
  template<typename T, std::size_t CAPACITY>
  std::size_t Stack<T, std::array<T, CAPACITY>>::size() const noexcept
  { return _size; }






  // operator==  const
  template<typename T, std::size_t CAPACITY>
  bool Stack<T, std::array<T, CAPACITY>>::operator==( Stack const & rhs ) const
  {
    if( _size != rhs._size ) return false;

    for( std::size_t i = 0;  i < _size;  ++i)  if( _collection[i] != rhs._collection[i] )  return false;
    return true;
  }
}    // namespace CSUF::CPSC131














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
** Last modified:  10-OCT-2022
** Last Verified:  10-OCT-2022
** Verified with:  MS Visual Studio 2019 Version 17.3.2 (C++22)
**                 GCC version 12.2.1 20220830 (-std=c++20 ),
**                 Clang version 14.0.6 (-std=c++20 -stdlib=libc++)
***************************************************/
