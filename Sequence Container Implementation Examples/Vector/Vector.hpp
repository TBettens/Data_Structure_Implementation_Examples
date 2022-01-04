/***********************************************************************************************************************************
** Class Vector - a very basic example implementation of the Vector Abstract Data Type
**
**  The ADT's interface is a small subset of std::vector defined at https://en.cppreference.com/w/cpp/container/vector
**
**  The elements are stored contiguously, which means that elements can be accessed not only through iterators, but also using
**  offsets to regular pointers to elements. This means that a pointer to an element of a vector may be passed to any function that
**  expects a pointer to an element of an array.
**
**  The storage of the vector is handled automatically, being expanded and contracted as needed. Vectors usually occupy more space
**  than static arrays, because more memory is allocated to handle future growth. This way a vector does not need to reallocate each
**  time an element is inserted, but only when the additional memory is exhausted.
**
**  This example has both a fixed capacity vector and an expandable capacity vector.  Unlike arrays where both size and capacity are
**  constant, vectors (sometimes called adjustable arrays) always have variable size.  There are two flavors of vectors defined
**  here.  A Fixed capacity vector has constant capacity which must be specified at construction and throws an exception if an
**  attempt to exceed capacity is detected.  An extendable capacity vector will acquire additional capacity if an attempt to exceed
**  current capacity is detected making for an "unlimited" capacity.
**
**  A vector reserves space for "Capacity" elements.  Elements are constructed on that space when inserted into the
**  vector destructed when removed from the vector without releasing the space thus
**  maintaining capacity whist decreasing size.
***********************************************************************************************************************************/
#pragma once

#include <algorithm>                                                      // swap(), move(), move_backward(), copy()
#include <string>                                                         // string, to_string()
#include <compare>                                                        // weak_ordering
#include <cstddef>                                                        // size_t
#include <initializer_list>                                               // initializer_list
#include <memory>                                                         // unique_ptr, make_unique(), uninitialized_copy(), uninitialized_move(), uninitialized_move_n()uninitialized_value_construct_n(), construct_at()
#include <stdexcept>                                                      // out_of_range, overflow_error, underflow_error
#include <type_traits>                                                    // aligned_storage
#include <utility>                                                        // move()






namespace CSUF::CPSC131
{
  // Template Class Definition - Vector's Abstract Data Type Interface:
  enum class VectorPolicy {FIXED, EXTENDABLE};

  template<typename T, VectorPolicy POLICY = VectorPolicy::EXTENDABLE>
  class Vector
  {
    template<typename U, VectorPolicy P>
    friend void swap( Vector<U, P> & lhs, Vector<U, P> & rhs ) noexcept;  // The expected way to make a program-defined type swappable is to provide a non-member function swap in the same namespace as the type.
                                                                          // (https://en.cppreference.com/w/cpp/algorithm/swap)
    public :
      // Types
      using iterator       = T*;                                          // A vector's iterator is-a raw pointer-to-T
      using const_iterator = T const *;                                   // Modifiable pointer to constant T.  Not to be confused with "const iterator" which is a const pointer to a modifiable T


      // Constructors, destructor, and assignments
      Vector            ( std::size_t                size = 0,   std::size_t capacity = 0 );          // Default constructor  Capacity defaults to size
      Vector            ( Vector const             & original                             );          // Copy constructor
      Vector            ( Vector                  && original                             ) noexcept; // Move constructor
      Vector            ( std::initializer_list<T>   init_list                            );          // initialization list constructor
      Vector & operator=( Vector const             & rhs                                  );          // Copy assignment
      Vector & operator=( Vector                  && rhs                                  ) noexcept; // Move assignment
     ~Vector            (                                                                 ) noexcept;


      // Queries
      std::size_t size    () const noexcept;
      std::size_t capacity() const noexcept;
      bool        empty   () const noexcept;


      // Iterators
      iterator       begin ();                                            // Read-write access to the vector's first element.  Also enables Vector to be used in range-based for loops
      iterator       end   ();                                            // Read-write access to one past the vector's last element.  Never dereference end(), but you can decrement the returned iterator and then dereference
      const_iterator begin () const;                                      // Read-only access to the vector's first element
      const_iterator end   () const;                                      // Read-only access to one past the vector's last element.  Never dereference end(), but you can decrement the returned iterator and then dereference
      const_iterator cbegin() const;                                      // Read-only access to the vector's first element
      const_iterator cend  () const;                                      // Read-only access to one past the vector's last element.  Never dereference end(), but you can decrement the returned iterator and then dereference


      // Accessors
      T const & at        ( std::size_t index ) const;                    // Read-only access to the vector's contents.  Checks bounds, throws std::range_error
      T       & at        ( std::size_t index );                          // Read-write access to the vector's contents.  Checks bounds, throws std::out_of_range
      T const & operator[]( std::size_t index ) const;                    // Read-only access to the vector's contents.  No bounds checking
      T       & operator[]( std::size_t index );                          // Read-write access to the vector's contents.  No bounds checking

      T const & front() const;                                            // Read-only access to the vector's front value.  Checks bounds, throws std::range_error
      T       & front();                                                  // Read-write access to the vector's front value.  Checks bounds, throws std::range_error
      T const & back () const;                                            // Read-only access to the vector's back value.  Checks bounds, throws std::range_error
      T       & back ();                                                  // Read-write access to the vector's back value.  Checks bounds, throws std::range_error


      // Modifiers
      void push_back( T const & value );                                  // Checks capacity, throws std::overflow_error
      void pop_back (                 );                                  // Checks size, throws std::underflow_error

      iterator erase ( iterator       position );                         // Checks bounds, throws std::out_of_range
      iterator erase ( const_iterator position );                         // Checks bounds, throws std::out_of_range

      iterator insert( iterator       beforePosition,  T const &value );  // Checks capacity, throws std::overflow_error
      iterator insert( const_iterator beforePosition,  T const &value );  // Checks capacity, throws std::overflow_error

      void clear() noexcept;                                              // Sets size() to zero.  capacity() is unchanged


      // Relational Operators
      std::weak_ordering operator<=>( Vector const & rhs ) const;
      bool               operator== ( Vector const & rhs ) const;



    private:
      // Design choice:  Use raw memory.  Note, however, that when the smart pointer "_array" is destroyed or reset, the allocated
      // raw memory is released, but objects placed in that raw memory are NOT destroyed.
      //
      // Another design option is to use an array of std::optional<T>. This has the advantage having objects destroyed when "_array"
      // is destroyed and makes the implementing code simpler, but incurs 4 to 8 bytes (1 byte to hold the status, and the rest due
      // to alignment padding) of overhead added to every array element.  As the size increases, so does the wasted memory.  Since
      // we already have a way to know which slots contain objects (0 through _size-1), we can avoid the memory overhead but have to
      // do our placement construction and destruction.
      //   std::unique_ptr< std::std::optional<T>[] > _array  = nullptr;  // considered and discarded
      //
      // A third design option is to use an array of T.  This too has the advantage having objects destroyed when "_array" is
      // destroyed and makes the implementing code simpler, but forces every slot to be filled with a default constructed objects,
      // even if those objects are not logically part of the vector.  For example, a vector constructed with size = 0 and capacity =
      // 10,000, 10,000 objects are created but never used. These erroneous objects take time to construct, could potentially
      // consume resources, and must be destroyed when overwriting with a user inserted object.  We can avoid wasting this time and
      // resources by constructing objects only when needed.  The exception is when T is a trivial type (e.g. a native type).  In
      // this case it's possible to optimize some operations and gain some efficiencies.  Function clear()'s efficiency class, for
      // example, could change from O(n) to O(1).  Such optimization can be sprinkled through the implementing code, which I've done.
      //   std::unique_ptr< T[] > _array  = nullptr;                      // considered and discarded
      //
      using RawMemory = std::aligned_storage_t< sizeof(T), alignof(T) >;  // enough properly aligned uninitialized (raw) memory for one object of type T
                                                                          // See  https://en.cppreference.com/w/cpp/types/aligned_storage

      // Attributes
      // Note:  Physical ordering of _size, _capacity, and _array is required for construction and must be maintained
      std::size_t                   _size     = 0;                        // number of elements in the data structure
      std::size_t                   _capacity = 0;                        // length of the array
      std::unique_ptr<RawMemory[]>  _array    = nullptr;                  // smart pointer to dynamically allocated array

      // Helper functions
      void reserve( size_t newCapacity );                                 // helper function to change capacity (expandable vector only)
  };
}    // namespace CSUF::CPSC131















/***********************************************************************************************************************************
************************************************************************************************************************************
** Private Implementation
*
** Separating Interface from Implementation is an extremely important concept I hope students will come to appreciate.
************************************************************************************************************************************
***********************************************************************************************************************************/
namespace CSUF::CPSC131
{
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors, destructor, and assignments
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Default constructor (both parameters are defaulted in function's declaration)
  template <typename T, VectorPolicy POLICY>
  Vector<T, POLICY>::Vector( std::size_t size, std::size_t capacity )
    : _size    { size },
      _capacity{ size > capacity  ?  size  :  (POLICY == VectorPolicy::FIXED && capacity == 0  ? 64 : capacity) },    // Capacity can never be less than size, and a Fixed Capacity Vector can never be 0
      _array   { std::make_unique<RawMemory[]>( _capacity ) }             // Pre-allocate an array of raw memory
  {
    std::uninitialized_value_construct_n( begin(), _size );               // construct _size new elements placing them in _array's pre-allocated memory
  }                                                                       // See https://en.cppreference.com/w/cpp/memory/uninitialized_value_construct_n



  // Copy construction
  template <typename T, VectorPolicy POLICY>
  Vector<T, POLICY>::Vector( const Vector & original )
    : _size    { original._size },
      _capacity{ POLICY == VectorPolicy::EXTENDABLE ? original._size : original._capacity },    // let extendable vectors shrink-to-fit
      _array   { std::make_unique<RawMemory[]>( _capacity ) }                                   // Pre-allocate an array of raw memory
  {
    std::uninitialized_copy( original.begin(), original.end(), begin() ); // Deep copy - copy elements from original placing them in _array's pre-allocated memory
  }                                                                       // See https://en.cppreference.com/w/cpp/memory/uninitialized_copy




  // Move construction
  template <typename T, VectorPolicy POLICY>
  Vector<T, POLICY>::Vector( Vector && original ) noexcept
    : _size    { original._size               },                          // steal the resources from original
      _capacity{ original._capacity           },
      _array   { std::move( original._array ) }                           // Shallow copy - move the pointer, not what the pointer points to.
  {
    original._size = original._capacity = 0;                              // leave original in a valid state (original._array has already been set to nullptr)
  }




  // Initialization List construction
  template<typename T, VectorPolicy POLICY>
  Vector<T, POLICY>::Vector( std::initializer_list<T> init_list )
    : _size    { init_list.size()                           },
      _capacity{ init_list.size()                           },
      _array   { std::make_unique<RawMemory[]>( _capacity ) }             // Pre-allocate an array of raw memory
  {
    std::uninitialized_copy(init_list.begin(), init_list.end(), begin()); // Deep copy - move elements from initializer list placing them in _array's pre-allocated memory
  }                                                                       // See https://en.cppreference.com/w/cpp/memory/uninitialized_move_n




  // Copy assignment
  template<typename T, VectorPolicy POLICY>
  Vector<T, POLICY> & Vector<T, POLICY>::operator=( Vector const & rhs )
  {
    if( this != &rhs )                                                    // self assignment protection
    {
      clear();                                                            // destroy my objects before adopting the new ones
      for( auto && current : rhs )    push_back( current );               // copy elements - Fixed Capacity vectors will throw an exception if rhs.size() > this->_capacity
    }

    return *this;
  }




  // Move assignment
  template<typename T, VectorPolicy POLICY>
  Vector<T, POLICY> & Vector<T, POLICY>::operator=( Vector && rhs ) noexcept
  {
    if( this != &rhs )                                                    // self assignment protection
    {
      clear();                                                            // destroy my objects before adopting the new ones

      if( POLICY == VectorPolicy::EXTENDABLE  ||  _capacity == rhs._capacity )
      {
        _size     = rhs._size;
        _capacity = rhs._capacity;
        _array    = std::move( rhs._array );                              // Shallow copy - move the pointer, not what the pointer points to.
                                                                          // adopt the new objects and release my dynamic memory
        rhs._size = rhs._capacity = 0;                                    // leave rhs in a valid state (rhs._array has already been set to nullptr)
      }

      else
      {
        // Don't allow capacity to change.  Fixed capacity vectors of different capacities can be assigned to each other, but
        // trying to but too much stuff into a smaller vector is an error (i.e., this->capacity() < rhs.size())
        for( auto && current : rhs )    push_back( std::move (current) ); // move elements - will throw an exception if rhs.size() > this->_capacity
      }
    }
    return *this;
  }




  // Destruction
  template<typename T, VectorPolicy POLICY>
  Vector<T, POLICY>::~Vector() noexcept
  { clear(); }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Queries
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // size() const
  template<typename T, VectorPolicy POLICY>
  std::size_t   Vector<T, POLICY>::size() const noexcept
  { return _size; }




  // capacity() const
  template<typename T, VectorPolicy POLICY>
  std::size_t   Vector<T, POLICY>::capacity() const noexcept
  { return _capacity; }




  // empty() const
  template<typename T, VectorPolicy POLICY>
  bool   Vector<T, POLICY>::empty() const noexcept
  { return size() == 0; }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Iterators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // begin()
  template <typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::iterator    Vector<T, POLICY>::begin()
  {
    // access and return pointer-to-array-of-elements held by unique_ptr without changing ownership
    return reinterpret_cast<T *>( _array.get() );                         // cast pointer-to-uninitialized memory to a pointer-to-T
 }




  // end()
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::iterator    Vector<T, POLICY>::end()
  { return begin() + size(); }                                            // Note the pointer arithmetic used




  // begin() const
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::const_iterator    Vector<T, POLICY>::begin() const
  { return const_cast<Vector *>( this )->begin(); }                       // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version
                                                                          // Can't just return begin(), that'd be an infinite loop



  // end() const
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::const_iterator    Vector<T, POLICY>::end() const
  { return const_cast<Vector *>( this )->end(); }                         // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version
                                                                          // Can't just return end(), that'd be an infinite loop



  // cbegin() const
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::const_iterator    Vector<T, POLICY>::cbegin() const
  { return begin(); }                                                     // to ensure consistent behavior and to implement the logic in one place, delegate to begin()




  // cend() const
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::const_iterator    Vector<T, POLICY>::cend() const
  { return end(); }                                                       // to ensure consistent behavior and to implement the logic in one place, delegate to end()












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Accessors
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // at() const
  template<typename T, VectorPolicy POLICY>
  const T &   Vector<T, POLICY>::at( std::size_t index ) const
  { return const_cast<Vector *>( this )->at( index ); }                   // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // at()
  template <typename T, VectorPolicy POLICY>
  T &   Vector<T, POLICY>::at( std::size_t index )
  {
    if( index >= _size ) throw std::out_of_range( std::string("index out of bounds:  ") + __FILE__ + " @line " + std::to_string( __LINE__ ) + " in function \"" + __func__ + '"' );   // clang not yet ready for std::source_location

    return operator[]( index );                                           // to ensure consistent behavior and to implement the logic in one place, delegate to unchecked operator[]
    // could also be coded as:
    //  return (*this)[ index ];
  }




  // operator[] const
  template<typename T, VectorPolicy POLICY>
  const T &   Vector<T, POLICY>::operator[]( std::size_t index ) const
  { return const_cast<Vector &>( *this )[ index ]; }                      // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // operator[]
  template<typename T, VectorPolicy POLICY>
  T &   Vector<T, POLICY>::operator[]( std::size_t index )
  { return begin()[ index ]; }                                            // Note: array bounds intentionally not checked




  // front() const
  template<typename T, VectorPolicy POLICY>
  T const &   Vector<T, POLICY>::front() const
  { return at( 0 ); }




  // front()
  template<typename T, VectorPolicy POLICY>
  T & Vector<T, POLICY>::front()
  { return at( 0 ); }




  // back() const
  template<typename T, VectorPolicy POLICY>
  T const &   Vector<T, POLICY>::back() const
  { return at( size()-1 ); }




  // back()
  template<typename T, VectorPolicy POLICY>
  T & Vector<T, POLICY>::back()
  { return at( size()-1 ); }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Modifiers
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // push_back()
  template<typename T, VectorPolicy POLICY>
  void    Vector<T, POLICY>::push_back( T const & value )
  { insert( end(), value ); }                                             // delegate to insert() leveraging error checking - insert before the end




  // pop_back()
  template<typename T, VectorPolicy POLICY>
  void Vector<T, POLICY>::pop_back()
  { erase( end()-1 ); }                                                   // delegate to erase() leveraging error checking - erase the last element, which is one before the end, or begin() + (size-1)




  // erase( iterator )
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::iterator    Vector<T, POLICY>::erase( iterator position )
  {
    // Summary:
    // 1      Take the element at "position" out of the Vector by shifting everything to the left
    // 2      Destroy the leftover element at the end, if necessary
    // 3      decrement the vector's size


    if( position >= end() || position < begin() ) throw std::out_of_range( std::string("iterator out of bounds:  ") + __FILE__ + " @line " + std::to_string( __LINE__ ) + " in function \"" + __func__ + '"' );    // clang not yet ready for std::source_location

    // Removes element at "position". Elements from higher positions are shifted back to fill gap. Vector size is decremented.
    //
    // Remove and close the resulting gap by left shifting elements and decrement the number of elements in the container, for
    // example, remove element 2 (e2):
    //
    //         begin()  position                     size / end()
    //             v       v                           v
    //           +------+------+------+------+------+------+
    //  BEFORE   |  e1  |  e2  |  e3  |  e4  | e5   |      |   capacity=6
    //           +------+------+------+------+------+------+   .
    //              |             /      /      /              .
    //              |         (step 1)  /      /               .
    //              |           /   (step 2)  /                .
    //              |          /      /   (step 3)             .
    //              |         /      /      /                  .
    //              v        v      v      v                   .
    //           +------+------+------+------+------+------+   .
    //  AFTER    |  e1  |  e3  |  e4  |  e5  |      |      |  capacity=6
    //           +------+------+------+------+------+------+
    //               ^      ^                    ^
    //           begin()  position             size / end()

    // Pick one!  Several coding options to left shift elements are shown here, but only one is used at a time.  These options
    // provide alternatives for students to see and use in their own code.  Students should pick just one of these options, and
    // of course do not include the "if constexpr (...)" statement

    // Option 1:  Write your own loop using pointers
    if constexpr( false )
    {
      for( auto current = position + 1;   current != end();   ++current )     *( current-1 ) = std::move( *current );
    }

    // Option 2:  Use the standard move function providing the range of elements to be moved and where to put them
    else if constexpr( true )
    {
      std::move( position + 1, end(),                                     // move everything from "position + 1" up to but not including "end()"
                 position           );                                    // to a new place starting at "position"
    }

    // Option 3:  Write your own loop using indexes
    else
    {
      std::size_t erasurePoint = position - begin();                      // convert iterator to index.  Another valid technique:  auto erasurePoint = std::distance( begin(), position )
      for( auto current = erasurePoint + 1;   current != _size;   ++current )     _array[current-1] = std::move( _array[current] );
    }

    // All options then need to adjust the size and return the position of the element just after the one removed
    --_size;                                                              // changing _size also changes end()
    end()->~T();                                                          // Destroy the last object which is no longer part of the vector.  Avoid dereferencing end() beyond this point
    return position;
  }




  // erase( const_iterator )
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::iterator    Vector<T, POLICY>::erase( const_iterator position )
  { return erase( const_cast<iterator>(position) ); }                     // to ensure consistent behavior and to implement the logic in one place, delegate to the non-const iterator version




  // insert( iterator )
  template <typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::iterator    Vector<T, POLICY>::insert( iterator position,  T const & value )
  {
    // Summary:
    // 1      If there is insufficient capacity for an additional element
    // 1.1      If capacity is FIXED, then throw an exception.
    // 1.2      Otherwise get more capacity
    // 2      Make room (that is, open a gap) for the new element by shifting everything to the right
    // 3      Fill the gap with a copy of the element and increment the vector's size


    if( position > end() || position < begin() )   throw std::out_of_range( std::string( "iterator out of bounds:  " ) + __FILE__ + " @line " + std::to_string( __LINE__ ) + " in function \"" + __func__ + '"' );    // clang not yet ready for std::source_location

    //  if there is insufficient capacity for an additional element
    if( _size >= _capacity )
    {
      // One of the major differences between extendable and fixed capacity vectors is an extendable vector's ability to add
      // capacity when adding an element when no more space (capacity) is available.  Fixed capacity vectors can't do that.
      if constexpr( POLICY == VectorPolicy::FIXED ) throw std::overflow_error( std::string("insufficient capacity to add another element:  ") + __FILE__ + " @line " + std::to_string( __LINE__ ) + " in function \"" + __func__ + '"' );    // clang not yet ready for std::source_location
      else
      {
        // Reserving more capacity (allocating a larger array, moving elements, and then releasing the smaller array) invalidates
        // all iterators (pointers) that may point to an element inside the array.  To account for this, the "position" is converted
        // to an index before reserving more capacity, and then restored afterwards by converting the index to an iterator
        // (pointer).
        std::size_t index = position - begin();                           // Convert iterator to index
        reserve( _capacity == 0 ? 8 : 2 * _capacity );                    // double non-zero capacity
        position = begin() + index;                                       // Convert index to iterator
      }
    }



    // Make room by right shifting elements to create space starting from the back and working towards the front, for example,
    // insert "value" before element 3 (e3)
    //
    //          begin()  position             size / end()
    //              v      v                    v
    //           +------+------+------+------+------+------+
    //  BEFORE   |  e1  |  e3  |  e4  |  e5  |      |      |  capacity=6
    //           +------+------+------+------+------+------+  .
    //              |       \      \      \                   .
    //              |        \      \   (step 1)              .
    //              |         \   (step 2)  \                 .
    //              |       (step 3)  \      \                .
    //              |           \      \      \               .
    //              v            v      v      v              .
    //           +------+------+------+------+------+------+  .
    //  AFTER    |  e1  |value |  e3  |  e4  |  e5  |      |  capacity=6
    //           +------+------+------+------+------+------+
    //               ^      ^                          ^
    //           begin()  position                   size / end()

    if( position == end() )
    {                                                                     // nothing to move, so just place the value at the end. Because
      std::construct_at( end(),   value );                                // _array[_size] (i.e. *end()) contains uninitialized memory, the object
    }                                                                     // placed there must be constructed.

    else
    {
      std::construct_at( end(),   std::move( *(end()-1) ) );              // move the last element over 1 slot.  Because _array[_size] (i.e. *end())
                                                                          // contains uninitialized memory, the object placed there must be
                                                                          // constructed.

      // Working backwards, move elements to the right creating a gap at "position"

      // Pick one!  Several coding options to right shift elements are shown here, but only one is used at a time.  These options
      // provide alternatives for students to see and use in their own code.  Students should pick just one of these options, and
      // of course do not include the "if constexpr (...)" statement
      //
      // Option 1:  Write your own loop using pointers
      if constexpr( false )
      {
        // Make sure "current" never equals begin().  That would cause *(current-1) to break
        for( auto destination = end()-1;   destination != position;   --destination )    *destination = std::move( *( destination-1 ) );
      }

      // Option 2:  Use the standard move backwards function providing the range of elements to be moved and where to put them
      else if constexpr( true )
      {
        std::move_backward( position, end()-1,                            // move everything from "position" up to but not including "end()-1"
                                      end() );                            // to a new place ending at "end()"
      }

      // Option 3:  Write your own loop using indexes
      else
      {
        std::size_t position_index = position - begin();                  // convert iterator to index
        for( auto destination = _size - 1; destination != position_index; --destination )
        {
          begin()[destination] = std::move( begin()[destination - 1] );   // avoid _array[destination] - that is a raw memory (very shallow) bit copy
        }
      }


      // All options then need to actually insert the element and adjust the size
      *position = value;                                                // fill the empty slot with a copy of "value"
    }

    // Don't forget to adjust the vector's size
    ++_size;                                                              // changing _size also changes end()

    // Return an iterator to the element just inserted
    return position;
  }




  // insert( const_iterator )
  template<typename T, VectorPolicy POLICY>
  typename Vector<T, POLICY>::iterator    Vector<T, POLICY>::insert( const_iterator position,  T const & value )
  { return insert( const_cast<iterator>(position), value ); }             // to ensure consistent behavior and to implement the logic in one place, delegate to the non-const iterator version




  // clear()
  template<typename T, VectorPolicy POLICY>
  void    Vector<T, POLICY>::clear() noexcept
  {
    for( auto && current : *this ) current.~T();
    _size = 0;
  }









  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Private member functions
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // reserve()
  template<typename T, VectorPolicy POLICY>
  void   Vector<T, POLICY>::reserve( std::size_t newCapacity )
  {
    if( newCapacity > _capacity )
    {
      // More capacity requested.  Get a bigger array, move the stuff from the smaller to bigger array, and then adopt the bigger
      // array and capacity.  For example:
      //          begin()               size=3/end()
      //              v                    v
      //           +------+------+------+
      //  BEFORE   |  e1  |  e2  |  e3  |                       capacity=3
      //           +------+------+------+
      //              |      |      |
      //              |      |      |
      //              v      v      v
      //           +------+------+------+------+------+------+
      //  AFTER    |  e1  |  e2  |  e3  |      |      |      |  capacity=6
      //           +------+------+------+------+------+------+
      //              ^                    ^
      //           begin()              size=3/end()
      //
      auto newArray = std::make_unique<RawMemory[]>( newCapacity );       // get a bigger array

      // Move values from smaller array into the bigger new array
      std::uninitialized_move( begin(), end(), reinterpret_cast<T *>( newArray.get() ) );

      // Destroy the remnants of the old objects.
      for( auto && current : *this ) current.~T();                      // uses begin() and end() of original, smaller array

      // Release the smaller array and adopt the new bigger array.  Smart pointer assignment releases the smaller array's
      // dynamically allocated memory.  Once "_array" is set to the "newArray", begin() and end() will return iterators to the new,
      // bigger array.
      _array    = std::move( newArray );
      _capacity = newCapacity;
    }
  }













  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Relational Operators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // operator<=>
  template<typename T, VectorPolicy POLICY>
  std::weak_ordering Vector<T, POLICY>::operator<=>( Vector const & rhs ) const
  {
    // Find the first element that's different and you have your answer.  If the Vectors are different sizes but all the leading
    // elements match, then the Vector with the smallest size is less than the other
    auto const extent = _size < rhs._size  ?  _size  :  rhs._size;        //  min(size, rhs.size)
    for (auto p = begin(), end = p + extent, q = rhs.begin();   p != end;   ++p, ++q)
    {
      auto result = std::compare_weak_order_fallback( *p, *q );                // uses operator== and operator< if operator<=> is unavailable
      if( result != 0 ) return result;
    }

    return  _size <=> rhs._size;
  }




  // operator==
  template<typename T, VectorPolicy POLICY>
  bool Vector<T, POLICY>::operator==( Vector const & rhs ) const
  {
    if( _size != rhs._size )   return false;

    for( auto p = begin(), end = this->end(), q = rhs.begin();  p != end;  ++p, ++q )
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
  template<typename T, VectorPolicy POLICY>
  void swap( Vector<T, POLICY> & lhs, Vector<T, POLICY> & rhs ) noexcept
  {
    // Note that all std::swap() overload candidates are added to the overload resolution scope. This allows user defined versions
    // of swap() to be called instead of std::swap() for object types that define their own versions of swap.
    //    [[ Lippman Lajoie Moo, C++ Primer 5e, 2013, Section 13.3, Swap ]]
    //    [[ Stack Overflow, What is the copy-and-swap idiom?, http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom ]]
    using std::swap;
    swap( lhs._size,     rhs._size );
    swap( lhs._capacity, rhs._capacity );
    swap( lhs._array,    rhs._array );
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
** Last modified:  21-OCT-2021  (refactored C++20 workarounds)
** Last Verified:  03-JAN-2022
** Verified with:  MS Visual Studio 2019 Version 16.11.8 (C++20)
**                 GCC version 11.2.1 20211124 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
