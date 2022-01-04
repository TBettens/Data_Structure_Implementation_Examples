/***********************************************************************************************************************************
** Class Queue - a very basic example implementation of the First In First Out (FIFO) Abstract Data Type.  The Queue ADT is an
**               adapter that provides a consistent FIFO interface over an underlying container supporting constant time insert and
**               remove operations
**
** The Queue class is an adapter class, meaning the operations provided by the Queue Abstract Data Type (ADT) are delegated to one
** of the other basic sequential ADTs.  Each of these other sequential ADTs have unique properties that can be exploited to satisfy
** constant (or at least amortized constant) time push(), pop(), front() and back() Queue operations.
**
**   1) Using Lists, or other data structures, that allow pushing to the back and popping from the front in constant time.  For
**      example, double linked lists (e.g., std::list, CSUF::CPSC131::DoublyLinkedList), singly linked lists that maintain a tail
**      pointer (e.g., CSUF::CPSC131::SinglyLinkedList), or adjustable arrays that grow in both directions (e.g., std::deque).
**
**   2) Using Vectors, or other adjustable array-like data structures, that allow pushing to the back in amortized constant time and
**      pre-allocating storage (capacity).  When capacity is exceed, addition storage is obtained from the heap.  For example,
**      vectors (std::vector, fixed and extendable CSUF::CPSC131::Vector).
**
**   3) Using Arrays, or other fixed size and capacity data structures, that pre-allocating and populate storage (size and capacity)
**      and have no push() or pop() support.  For example, std::array, native array
**
** A Queue alias is defined based on the underlying data structure and should be used by clients, much like we typically use
** std::string (also an alias) instead of the std::basic_string<...> specialization.
**
** The Queue ADT's interface is a small subset of std::queue defined at https://en.cppreference.com/w/cpp/container/queue
**
***********************************************************************************************************************************/
#pragma once

#include <array>
#include <cstddef>                                                // size_t
#include <iterator>                                               // distance()
#include <stdexcept>                                              // out_of_range
#include <string>                                                 // to_string()
#include <type_traits>                                            // conditional_t, is_bounded_array_v

#include "SinglyLinkedList.hpp"




// Users normally use the general Queue<T, Container> class defined below and not the container specific classes Queue_Over_* directly.  For example:
//     Queue<Student> myQueue;
//
// defines an object of type Queue_Over_List called myQueue which defaults the underlying container to a singly linked list, while
//      Queue<Student, Vector<Student>> myQueue;
//
// defines an object of type Queue_Over_Vector called myQueue that uses an extendable vector as the underlying container






namespace CSUF::CPSC131
{
  /*********************************************************************************************************************************
  ** Queue class definition 1
  **
  ** Data structures meeting the Double Ended concept are typically double linked lists, singly linked lists that maintain a tail
  ** pointer (not std::forward_list), double ended queue (std::deque), etc.
  *********************************************************************************************************************************/
  template<typename T, typename UnderlyingContainer>
  class Queue_Over_List
  {
    public:
      // Constructors, destructor, and assignments
      // Compiler synthesized default constructors, assignments, and destructor are okay


      // Queries
      bool        empty() const noexcept;                         // returns true if the queue contains no elements, false otherwise
      std::size_t size () const noexcept;                         // returns the number of elements in the queue


      // Accessors
      T const & front() const;                                    // returns a read only reference to the front of the queue
      T       & front();                                          // returns a read-write reference to the front of the queue
      T const & back () const;                                    // returns a read only reference to the rear of the queue
      T       & back ();                                          // returns a read-write reference to the rear of the queue


      // Modifiers
      void   push ( T const & value );                            // puts the value at the front of the queue and increments size
      void   pop  (                 );                            // removes the element at the rear of the queue and decrements size


      // Relational Operators
      bool operator==( Queue_Over_List const & rhs ) const = default;       // returns true if this queue and the rhs queue have the same number of values in the same order, false otherwise


    private:
      UnderlyingContainer _collection;                            // delegate object management
  }; // Queue_Over_List


















  /*********************************************************************************************************************************
  ** Queue class definition 2
  **
  ** Data structures meeting the Indexable concept are typically vectors (sometimes called adjustable arrays).  These structures typically
  ** define values in consecutive memory locations, allocate their storage dynamically, and reallocate capacity when needed.
  *********************************************************************************************************************************/
  template<typename T, typename UnderlyingContainer>
  class Queue_Over_Vector
  {
    public:
      // Constructors, destructor, and assignments
      // Compiler synthesized default constructors, assignments, and destructor are okay


      // Queries
      bool        empty() const noexcept;                         // returns true if the queue contains no elements, false otherwise
      std::size_t size () const noexcept;                         // returns the number of elements in the queue


      // Accessors
      T const & front() const;                                    // returns a read only reference to the front of the queue
      T       & front();                                          // returns a read-write reference to the front of the queue
      T const & back () const;                                    // returns a read only reference to the rear of the queue
      T       & back ();                                          // returns a read-write reference to the rear of the queue


      // Modifiers
      void   push ( T const & value );                            // puts the value at the front of the queue and increments size
      void   pop  (                 );                            // removes the element at the rear of the queue and decrements size


      // Relational Operators
      bool operator==( Queue_Over_Vector const & rhs ) const;    // returns true if this queue and the rhs queue have the same number of values in the same order, false otherwise


    private:
      std::size_t         _size  = 0;                             // number of elements in the queue, 0 indicates an empty container
      std::size_t         _front = 0;                             // index of the value at the front of the queue
      UnderlyingContainer _collection;                            // delegate object management
  }; // Queue_Over_Vector


















  /*********************************************************************************************************************************
  ** Queue class definition 3
  **
  ** Using std::array
  *********************************************************************************************************************************/
  template<typename T, typename UnderlyingContainer>
  class Queue_Over_Array
  {
    public:
      // Constructors, destructor, and assignments
      Queue_Over_Array(                                 );                        // Default constructor
      Queue_Over_Array( Queue_Over_Array const  & other );                        // Copy constructor
      Queue_Over_Array( Queue_Over_Array       && other ) noexcept;               // Move constructor
     ~Queue_Over_Array(                                 ) noexcept;               // Destructor

      Queue_Over_Array & operator=( Queue_Over_Array const  & rhs );              // Copy assignment
      Queue_Over_Array & operator=( Queue_Over_Array       && rhs ) noexcept;     // Move assignment


      // Queries
      bool        empty() const noexcept;                         // returns true if the queue contains no elements, false otherwise
      std::size_t size () const noexcept;                         // returns the number of elements in the queue


      // Accessors
      T const & front() const;                                    // returns a read only reference to the front of the queue
      T       & front();                                          // returns a read-write reference to the front of the queue
      T const & back () const;                                    // returns a read only reference to the rear of the queue
      T       & back ();                                          // returns a read-write reference to the rear of the queue


      // Modifiers
      void   push ( T const & value );                            // puts the value at the front of the queue and increments size
      void   pop  (                 );                            // removes the element at the rear of the queue and decrements size


      // Relational Operators
      bool operator==( Queue_Over_Array const & rhs ) const;      // returns true if this queue and the rhs queue have the same number of values in the same order, false otherwise


    private:
      // Since the "array" structure has, by definition, fixed size and capacity, it's not possible to delegate all the object
      // management responsibilities as was done above. An attribute has been added to maintain the number of elements the client
      // has placed the queue.
      std::size_t         _front = 0;                             // index of the value at the front of the queue
      std::size_t         _size  = 0;                             // number of elements in the queue, 0 indicates an empty container
      UnderlyingContainer _collection;

      const std::size_t CAPACITY = std::size(_collection);        // note we used the more general global std::size() function, not the member function - it handles both native bounded arrays and std::array
  }; // Queue_Over_Array


















  #ifndef CSUF_CPSC131_QUEUE_HELPER_CONCEPTS
  #define CSUF_CPSC131_QUEUE_HELPER_CONCEPTS
    // Queue helper concepts used to select which end of the underlying container to use.  You have to push and pop from different
    // ends, but which end makes a difference.  Some containers allow pushing to the back, but not popping from the back (SSL with a
    // tail pointer, for example).  So we'll define double ended as being able to push to the back and pop from the front.  Using
    // the wrong end of the container could result in a linear-time (O(n)) operation instead of the required constant-time (O(1))
    // operation.
    template <typename Container>
    concept is_list_like = requires(Container c)
    { // all are required
      c.push_back( c.front() );
      c.pop_front();
      c.front();
      c.back();
      c.size();
    };

    // Vector-like containers usually dynamically allocate storage, have variable size, maintain capacity, and are indexable in
    // constant time. Capacity may be either fixed or extendable.  Values are added to extendable capacity containers in amortized
    // constant time.
    template<typename Container>
    concept is_vector_like = requires(Container c)
    { // all are required
      c[0u];
      c.at(0u);
      c.size();
      c.capacity();
      c.push_back( c.at(0u) );
    };

    // Array-like containers are also indexable in constant time but, unlike Vector-like containers, have fixed size and usually
    // statically allocate storage.  Every slot in the array is always populated with a value.  There is no notion of adding to or
    // removing from an array - the size is always constant.
    template<typename Container>
    concept is_array_like = std::is_bounded_array_v<Container> || requires( Container c )
    { // all are required
      c[0u];
      std::size(c);
    };
  #endif


  // Static polymorphic selection of Queue implantation.  Selects the implementation that matches the container's properties
  template <typename T, typename Container = CSUF::CPSC131::SinglyLinkedList<T>>
  using Queue = std::conditional_t< is_list_like  < Container >,  Queue_Over_List  < T, Container >,       // selection order is important
                std::conditional_t< is_vector_like< Container >,  Queue_Over_Vector< T, Container >,
                std::conditional_t< is_array_like < Container >,  Queue_Over_Array < T, Container >,
                                                                  void                            > > >;
}  // namespace CSUF::CPSC131


















/**********************************************************************************************************************************
***********************************************************************************************************************************
**  PRIVATE  IMPLEMENTATIONS
**
**********************************************************************************************************************************/

// Queue Over List
namespace CSUF::CPSC131
{
  /*********************************************************************************************************************************
  ** Double Ended Underlying Container Implementations
  **
  ** Data structures meeting the Double Ended concepts are typically double linked lists, singly linked lists that maintain a tail
  ** pointer (not std::forward_list), double ended queue (std::deque), etc.
  **
  ** All Queue operations are delegated to the underlying container.  New values are pushed to the back and popped from the front:
  **
  ** BEFORE             A      B      C
  **                    ^             ^
  **                  front         back (aka rear)
  ** size = 3
  **
  ** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  **
  ** AFTER push(D)      A      B      C      D
  **                    ^                    ^
  **                  front                back (aka rear)
  ** size = 4
  **
  ** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  **
  ** AFTER pop()               B      C      D
  **                           ^             ^
  **                         front         back (aka rear)
  ** size = 3
  **
  *********************************************************************************************************************************/
  // push()
  template<typename T, typename UnderlyingContainer>
  void Queue_Over_List<T, UnderlyingContainer>::push( T const & value )
  { _collection.push_back ( value ); }                                                          // must push and pop at opposite ends






  // pop()
  template<typename T, typename UnderlyingContainer>
  void Queue_Over_List<T, UnderlyingContainer>::pop()
  { _collection.pop_front(); }                                                                  // must push and pop at opposite ends






  // front() const
  template<typename T, typename UnderlyingContainer>
  const T & Queue_Over_List<T, UnderlyingContainer>::front() const
  { return const_cast<Queue_Over_List<T, UnderlyingContainer> *>(this)->front(); }              // delegate to the read-write version of Queue::front






  // front()
  template<typename T, typename UnderlyingContainer>
  T & Queue_Over_List<T, UnderlyingContainer>::front()
  { return _collection.front(); }






  // back() const
  template<typename T, typename UnderlyingContainer>
  const T & Queue_Over_List<T, UnderlyingContainer>::back() const
  { return const_cast<Queue_Over_List<T, UnderlyingContainer> *>(this)->back(); }               // delegate to the read-write version of Queue::back






  // back()
  template<typename T, typename UnderlyingContainer>
  T & Queue_Over_List<T, UnderlyingContainer>::back()
  { return _collection.back (); }






  // empty() const
  template<typename T, typename UnderlyingContainer>
  bool Queue_Over_List<T, UnderlyingContainer>::empty() const noexcept
  { return _collection.empty(); }






  // size() const
  template<typename T, typename UnderlyingContainer>
  std::size_t Queue_Over_List<T, UnderlyingContainer>::size() const noexcept
  { return _collection.size(); }
}    // namespace, Queue Over List


















// Queue Over Vector
namespace CSUF::CPSC131
{
  /*********************************************************************************************************************************
  ** Vector (Variable Size and Extendable Capacity) Container Implementations
  **
  ** Data structures meeting the Indexable concept are typically vectors (sometimes called adjustable arrays).  These structures typically
  ** define values in consecutive memory locations, allocate their storage dynamically, and reallocate capacity when needed.
  **
  ** Modulo (aka circular) algorithms and arithmetic are applied here.  We insert to the right of *occupied* slots (the back) of the
  ** container. The slot to fill (sometimes called the rear of the queue) is calculated as "(front + size) modulo capacity".
  ** For example:
  **
  ** Example 1                                         || Example 2:
  **                                                   ||
  **   BEFORE:    index:  0   1   2   3   4   5        ||   BEFORE:    index:  0   1   2   3   4   5
  **              value:  D   -   -   A   B   C        ||              value:  -   A   B   C   D   -
  **                                  ^                ||                          ^
  **                                front              ||                        front
  **   size     = 4                                    ||   size     = 4
  **   capacity = 6                                    ||   capacity = 6
  **   front    = 3                                    ||   front    = 1
  **                                                   ||
  **   - - - - - - - - - - - - - - - - - - - - - - - - ||   - - - - - - - - - - - - - - - - - - - - - - - -
  **                                                   ||
  **   AFTER:     index:  0   1   2   3   4   5        ||   AFTER:     index:  0   1   2   3   4   5
  **   push('E')  value:  D   E   -   A   B   C        ||   push('E')  value:  -   A   B   C   D   E
  **                                  ^                ||                          ^
  **                                front              ||                        front
  **   size     = 5 (updated)                          ||   size     = 5 (updated)
  **   capacity = 6                                    ||   capacity = 6
  **   front    = 3                                    ||   front    = 1
  **
  **
  ** When inserting into the queue and their is no more capacity, get a bigger container and move the elements from the smaller
  ** to the bigger container.  For example,:
  **
  ** Example 3:
  **
  **   BEFORE:    index:  0   1   2   3   4   5
  **              value:  D   E   F   A   B   C
  **                                  ^
  **                                front
  **   size     = 6
  **   capacity = 6
  **   front    = 3
  **
  **   .................................................................
  **   AFTER:     index:  0   1   2   3   4   5   6   7   8   9  10  11
  **   push('G')  value:  -   -   -   A   B   C   D   E   F   G   -   -
  **                                  ^
  **                                front
  **   size     = 7   (updated)
  **   capacity = 12  (updated)
  **   front    = 3   (updated)
  **
  *********************************************************************************************************************************/
  // push()
  template<typename T, typename UnderlyingContainer>
  void Queue_Over_Vector<T, UnderlyingContainer>::push( T const & value )                       // must push and pop at opposite ends
  {
    auto capacity = _collection.capacity();
    if( capacity == 0 ) _collection.push_back( value );                                         // special case to get things started

    else
    {
      // When there is no more capacity available, allow the underlying container to get more capacity as we start moving leading
      // values to the back.  We'll delegate the actual memory management tasks to the underlying container.  Note that the
      // underlying container will throw an exception if more capacity cannot be obtained.  This might happen when using a fixed
      // capacity vector.
      if( _size == capacity )                                                                   // size of queue is equal to the container's capacity
      {
        for( std::size_t i = 0; i < _front; ++i ) _collection.push_back( std::move( _collection[i] ) );
        capacity = _collection.capacity();
      }

      auto rear = ( _front + _size ) % capacity;                                                // modulo arithmetic over the container's capacity

      if( rear == _collection.size() ) _collection.push_back( value );                          // careful not to over-index the underlying container.
      else                             _collection[rear]    = value;
    }

    ++_size;
  }






  // pop()
  template<typename T, typename UnderlyingContainer>
  void Queue_Over_Vector<T, UnderlyingContainer>::pop()                                         // must push and pop at opposite ends
  {
    if( empty() )   throw std::out_of_range( "ERROR:  Attempt to remove a value from an empty queue" );


    _collection[_front] = T{};                                                                  // We can't really delete the value, but we can free any held resources by setting it to the default value;
    _front              = ( _front + 1 ) % _collection.capacity();                              // move the front of the queue (modulo arithmetic over the container's capacity)

    --_size;
  }






  // front() const
  template<typename T, typename UnderlyingContainer>
  const T & Queue_Over_Vector<T, UnderlyingContainer>::front() const
  { return const_cast<Queue_Over_Vector<T, UnderlyingContainer> *>(this)->front(); }            // delegate to the read-write version of Queue_Over_Vector::front






  // front()
  template<typename T, typename UnderlyingContainer>
  T & Queue_Over_Vector<T, UnderlyingContainer>::front()
  {
    if( empty() )   throw std::out_of_range( "ERROR:  Attempt to access a value from the front of an empty queue" );
    return _collection[_front];
  }






  // back() const
  template<typename T, typename UnderlyingContainer>
  const T & Queue_Over_Vector<T, UnderlyingContainer>::back() const
  { return const_cast<Queue_Over_Vector<T, UnderlyingContainer> *>(this)->back(); }             // delegate to the read-write version of Queue_Over_Vector::back






  // back()
  template<typename T, typename UnderlyingContainer>
  T & Queue_Over_Vector<T, UnderlyingContainer>::back()
  {
    if( empty() )   throw std::out_of_range( "ERROR:  Attempt to access a value from the back of an empty queue" );
    auto back = ( _front + _size - 1 ) % _collection.capacity();                                // The back value is at size-1 then apply the modulo arithmetic
    return _collection[back];
  }






  // empty() const
  template<typename T, typename UnderlyingContainer>
  bool Queue_Over_Vector<T, UnderlyingContainer>::empty() const noexcept
  { return size() == 0; }






  // size() const
  template<typename T, typename UnderlyingContainer>
  std::size_t Queue_Over_Vector<T, UnderlyingContainer>::size() const noexcept
  { return _size; }






  // operator== const
  template<typename T, typename UnderlyingContainer>
  bool Queue_Over_Vector<T, UnderlyingContainer>::operator==( Queue_Over_Vector const & rhs ) const
  {
    if( _size != rhs._size ) return false;

    for( std::size_t i = _front, j = rhs._front, s = 0;   s < _size;   ++s )
    {
      if( _collection[i] != rhs._collection[j] )   return false;
      i = ( i + 1 ) %     _collection.capacity();
      j = ( j + 1 ) % rhs._collection.capacity();
    }

    return true;
  }
}  // namespace,  Queue Over Vector


















// Queue Over Array
namespace CSUF::CPSC131
{
  /*********************************************************************************************************************************
  ** Array (Fixed Size and Capacity) Container Implementations
  **
  ** The approach for arrays is very similar to vectors above, except an array does not support insertion (push_back), and every
  ** cell has a value (fixed size) so we need to do it our self.  There really isn't anything to delegate to. Modulo (aka circular)
  ** algorithms and arithmetic are applied here.  We insert to the right of *occupied* slots (the back) of the container. The slot
  ** to fill (sometimes called the rear of the queue) is calculated as "(front + size) modulo capacity".  An exception is thrown
  ** when attempting to push an value into the queue but the underlying array has no more room.
  ** For example:
  **
  ** Example 1                                         || Example 2:
  **                                                   ||
  **   BEFORE:    index:  0   1   2   3   4   5        ||   BEFORE:    index:  0   1   2   3   4   5
  **              value:  D   -   -   A   B   C        ||              value:  -   A   B   C   D   -
  **                                  ^                ||                          ^
  **                                front              ||                        front
  **   size     = 4                                    ||   size     = 4
  **   capacity = 6                                    ||   capacity = 6
  **   front    = 3                                    ||   front    = 1
  **                                                   ||
  **   - - - - - - - - - - - - - - - - - - - - - - - - ||   - - - - - - - - - - - - - - - - - - - - - - - -
  **                                                   ||
  **   AFTER:     index:  0   1   2   3   4   5        ||   AFTER:     index:  0   1   2   3   4   5
  **   push('E')  value:  D   E   -   A   B   C        ||   push('E')  value:  -   A   B   C   D   E
  **                                  ^                ||                          ^
  **                                front              ||                        front
  **   size     = 5 (updated)                          ||   size     = 5 (updated)
  **   capacity = 6                                    ||   capacity = 6
  **   front    = 3                                    ||   front    = 1
  **
  **
  *********************************************************************************************************************************/
  // Default constructor
  template<typename T, typename UnderlyingContainer>
  Queue_Over_Array<T, UnderlyingContainer>::Queue_Over_Array() = default;






  // Copy constructor
  template<typename T, typename UnderlyingContainer>
  Queue_Over_Array<T, UnderlyingContainer>::Queue_Over_Array( Queue_Over_Array const & other )
  {
    // copy as many values as possible.  Truncation occurs when other is bigger than *this
    std::size_t extent = CAPACITY < other._size  ?  CAPACITY  :  other._size;

    // copy the values while incrementing size
    for( std::size_t j = other._front;  _size < extent;  j = (j+1) % other.CAPACITY, ++_size )   _collection[_size] = other._collection[j];
  }






  // Move constructor
  template<typename T, typename UnderlyingContainer>
  Queue_Over_Array<T, UnderlyingContainer>::Queue_Over_Array( Queue_Over_Array && other ) noexcept
  {
    // Copy as many values as possible.  Truncation occurs when other is bigger than *this
    std::size_t extent = (CAPACITY < other._size)  ?  CAPACITY  :  other._size;

    // move the values while incrementing size
    for( std::size_t j = other._front;  _size < extent;  j = (j+1) % other.CAPACITY, ++_size )   _collection[_size] = std::move( other._collection[j] );
  }






  // Destructor
  template<typename T, typename UnderlyingContainer>
  Queue_Over_Array<T, UnderlyingContainer>::~Queue_Over_Array() noexcept = default;






  // Copy assignment
  template<typename T, typename UnderlyingContainer>
  Queue_Over_Array<T, UnderlyingContainer> & Queue_Over_Array<T, UnderlyingContainer>::operator=( Queue_Over_Array const & rhs )
  {
    if (this != &rhs)
    {
      // Copy as many values as possible.  Truncation occurs when rhs is bigger than *this
      std::size_t extent = (CAPACITY < rhs._size)  ?  CAPACITY  :  rhs._size;

      // Start at the beginning by reseting the size and front
      _size = _front = 0;

      // copy the values while incrementing size
      for( std::size_t j = rhs._front;  _size < extent;  j = (j+1) % rhs.CAPACITY, ++_size )   _collection[_size] = rhs._collection[j];

      // clean up the leftovers
      for( std::size_t i = _size;  _size < CAPACITY;  ++i )   _collection[i] = T{};
    }

    return *this;
  }






  // Move assignment
  template<typename T, typename UnderlyingContainer>
  Queue_Over_Array<T, UnderlyingContainer> & Queue_Over_Array<T, UnderlyingContainer>::operator=( Queue_Over_Array && rhs ) noexcept
  {
    if (this != &rhs)
    {
      // Move as many values as possible.  Truncation occurs when rhs is bigger than *this
      std::size_t extent = (CAPACITY < rhs._size)  ?  CAPACITY  :  rhs._size;

      // Start ar the beginning by reseting the size and front
      _size = _front = 0;

      // copy the values while incrementing size
      for( std::size_t j = rhs._front;  _size < extent;  j = (j+1) % rhs.CAPACITY, ++_size )   _collection[_size] = std::move( rhs._collection[j] );

      // clean up the leftovers
      for( std::size_t i = _size;  _size < CAPACITY;  ++i )   _collection[i] = T{};
    }

    return *this;
  }






  // push()
  template<typename T, typename UnderlyingContainer>
  void Queue_Over_Array<T, UnderlyingContainer>::push( const T & value )              // must push and pop at opposite ends
  {
    // verify there is capacity for another value
    if( _size >= CAPACITY )    throw std::out_of_range( "ERROR:  Attempt to add to an already full queue of " + std::to_string( CAPACITY ) + " elements." );

    auto rear = ( _front + _size ) % CAPACITY;                                        // modulo arithmetic over the container's capacity

    _collection[rear] = value;
    ++_size;
  }






  // pop()
  template<typename T, typename UnderlyingContainer>
  void Queue_Over_Array<T, UnderlyingContainer>::pop()                                // must push and pop at opposite ends
  {
    // verify there is something to remove
    if( empty() )    throw std::out_of_range( "ERROR:  Attempt to remove an value from an empty queue" );

    _collection[_front] = T{};                                                        // We can't really delete the value, but we can free any held resources by setting it to the default value;
    _front              = ( _front + 1 ) % CAPACITY;                                  // move the front of the queue (modulo arithmetic over the container's capacity)

    --_size;
  }






  // front() const
  template<typename T, typename UnderlyingContainer>
  const T & Queue_Over_Array<T, UnderlyingContainer>::front() const
  { return const_cast<Queue_Over_Array<T, UnderlyingContainer> *>(this)->front(); }   // delegate to the read-write version of Queue_Over_Vector::front






  // front()
  template<typename T, typename UnderlyingContainer>
  T & Queue_Over_Array<T, UnderlyingContainer>::front()
  {
    if( empty() )   throw std::out_of_range( "ERROR:  Attempt to access a value from the front of an empty queue" );
    return _collection[_front];
  }






  // back() const
  template<typename T, typename UnderlyingContainer>
  const T & Queue_Over_Array<T, UnderlyingContainer>::back() const
  { return const_cast<Queue_Over_Array<T, UnderlyingContainer> *>(this)->back(); }    // delegate to the read-write version of Queue_Over_Vector::back






  // back()
  template<typename T, typename UnderlyingContainer>
  T & Queue_Over_Array<T, UnderlyingContainer>::back()
  {
    if( empty() )   throw std::out_of_range( "ERROR:  Attempt to access a value from the back of an empty queue" );
    auto back = ( _front + _size - 1 ) % CAPACITY;                                    // The back value is at size-1 then apply the modulo arithmetic
    return _collection[back];
  }






  // empty() const
  template<typename T, typename UnderlyingContainer>
  bool Queue_Over_Array<T, UnderlyingContainer>::empty() const noexcept
  { return size() == 0; }






  // size() const
  template<typename T, typename UnderlyingContainer>
  std::size_t Queue_Over_Array<T, UnderlyingContainer>::size() const noexcept
  { return _size; }






  // operator==  const
  template<typename T, typename UnderlyingContainer>
  bool Queue_Over_Array<T, UnderlyingContainer>::operator==( Queue_Over_Array const & rhs ) const
  {
    if( _size != rhs._size ) return false;

    for( std::size_t i = _front, j = rhs._front, s = 0;   s < _size;   ++s )
    {
      if( _collection[i] != rhs._collection[j] )   return false;
      i = ( i + 1 ) %     CAPACITY;
      j = ( j + 1 ) % rhs.CAPACITY;
    }

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
** Last modified:  27-JUL-2021
** Last Verified:  03-JAN-2022
** Verified with:  MS Visual Studio 2019 Version 16.11.8 (C++20)
**                 GCC version 11.2.1 20211124 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
