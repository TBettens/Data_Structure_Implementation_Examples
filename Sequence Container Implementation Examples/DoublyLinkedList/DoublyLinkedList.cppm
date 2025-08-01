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
module;                                                                               // Global fragment (not part of the module)
  // Empty









/***********************************************************************************************************************************
**  Module CSUF.CPSC131.DoublyLinkedList Interface
**
***********************************************************************************************************************************/
export module CSUF.CPSC131.DoublyLinkedList;
import std;
import CSUF.CPSC131.exceptionString;




namespace CSUF::CPSC131
{
  export enum class DllPolicy {CIRCULAR, NTL};

  template<typename, DllPolicy> struct Members;                                       // intentionally not exported

  // Template Class Definition
  export template<typename T, DllPolicy POLICY = DllPolicy::CIRCULAR>
  class DoublyLinkedList
  {
    template <typename U>
    friend void swap( DoublyLinkedList<U> & lhs, DoublyLinkedList<U> & rhs );         // The expected way to make a program-defined type swappable is to provide a
                                                                                      // non-member function swap in the same namespace as the type.
                                                                                      // (https://en.cppreference.com/w/cpp/algorithm/swap)
    private:
      // Types
      template <typename U> class  Iterator_type;                                     // Template class for iterator and const_iterator classes
      using Node              = Members<T, POLICY>::Node;                             // Specific implementations are responsible for defining their node structure
      using PrivateAttributes = Members<T, POLICY>;                                   // A specific implementation's private members (attributes, functions, etc)

      // Member attributes
      std::unique_ptr<PrivateAttributes> self;                                        // See Pimpl Idiom at https://en.cppreference.com/w/cpp/language/pimpl and https://cpppatterns.com/patterns/pimpl.html



    public:
      // Types
      using iterator       = Iterator_type<T      >;                                  // A bi-directional iterator to a read-write value in the list
      using const_iterator = Iterator_type<T const>;                                  // A bi-directional iterator to a read-only value in the list

      using reverse_iterator       = std::reverse_iterator<iterator>;                 // A bi-directional iterator to a read-write value in the list
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;           // A bi-directional iterator to a read-only value in the list



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
      iterator               begin ()       noexcept;                                 // Returns a read-write iterator to the list's front element, end() if list is empty
      iterator               end   ()       noexcept;                                 // Returns a read-write iterator beyond the list's back element.  Do not dereference this iterator

      const_iterator         begin () const noexcept;                                 // Returns a read-only iterator to the list's front element, end() if list is empty
      const_iterator         end   () const noexcept;                                 // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator

      const_iterator         cbegin() const noexcept;                                 // Returns a read-only iterator to the list's front element, end() if list is empty
      const_iterator         cend  () const noexcept;                                 // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator


      reverse_iterator       rbegin ()       noexcept;                                // Returns a read-write iterator to the list's front element, end() if list is empty
      reverse_iterator       rend   ()       noexcept;                                // Returns a read-write iterator beyond the list's back element.  Do not dereference this iterator

      const_reverse_iterator rbegin () const noexcept;                                // Returns a read-only iterator to the list's front element, end() if list is empty
      const_reverse_iterator rend   () const noexcept;                                // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator

      const_reverse_iterator crbegin() const noexcept;                                // Returns a read-only iterator to the list's front element, end() if list is empty
      const_reverse_iterator crend  () const noexcept;                                // Returns a read-only iterator beyond the list's back element.  Do not dereference this iterator



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






  /*********************************************************************************************************************************
  ** Class DoublyLinkedList<T, POLICY>::iterator - A doubly linked bi-directional iterator
  **
  *********************************************************************************************************************************/
  template<typename T, DllPolicy POLICY>   template<typename U>
  class DoublyLinkedList<T, POLICY>::Iterator_type
  {
    friend class DoublyLinkedList<T, POLICY>;

    public:
      // Iterator Type Traits - Boilerplate stuff so the iterator can be used with the rest of the standard library
      using iterator_category = std::bidirectional_iterator_tag;
      using value_type        = U;
      using difference_type   = std::ptrdiff_t;
      using pointer           = value_type *;
      using reference         = value_type &;



      // Compiler synthesized constructors and destructor are fine, just what we want (shallow copies, no ownership) but needed to
      // explicitly say that because there is also a user defined constructor

      // A default constructor should have Sentinel semantics, which is to say it indicates the end of a list.  An iterator default
      // constructed through the class (i.e., not through a list object) should indicate the end of any list.  This behavior is not
      // implemented here.  In particular, a default constructed iterator, this case, will not match list.end() if list is
      // implemented as a circular list.  The Sentinel Design Pattern could be implemented as an enhancement, but I choose to not go
      // that far primarily for simplicity.  To help prevent misuse of a default constructed iterator, I used to "delete" the
      // default constructor.  But that stops the list from being recognized by the std::ranges:range concept which in turn prevents
      // the list from using std::ranges based algorithms, including std::format.
      Iterator_type(                        );                                        // Default constructed Iterator_type returns a pseudo Sentinel (null pointer in this case)
      Iterator_type( iterator const & other ) noexcept;                               // Copy constructor when T is non-const, Conversion constructor from non-const to const iterator when T is const



      // Pre and post Increment operators move the position to the next node in the list
      Iterator_type & operator++();                                                   // advance the iterator one node (pre -increment)
      Iterator_type   operator++( int );                                              // advance the iterator one node (post-increment)



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
      Iterator_type( Node * position ) noexcept;                                      // Implicit conversion constructor from pointer-to-Node to iterator-to-Node
  };  // DoublyLinkedList<T, DllPolicy POLICY>::Iterator_type

}    // namespace CSUF::CPSC131
























/***********************************************************************************************************************************
************************************************************************************************************************************
** Private Implementation
**
** Separating Interface from Implementation is an extremely important concept I hope students will come to appreciate.
************************************************************************************************************************************
***********************************************************************************************************************************/

// Not exported but reachable
// Organization:
//  o)  Circular Doubly Linked List With One Dummy Node Specific
//  o)  Null-Terminated Doubly Linked List With One Dummy Node Specific
//  0)  Common across all Doubly Linked Lists



/***********************************************************************************************************************************
** Class DoublyLinkedList - a very basic example implementation of the Doubly Linked List Abstract Data Type
**
**  This file provides a one-dummy node, circular implementation of the DoublyLinkedList interface
**
** Empty (size = 0):
**                               +---------------+      +---------------+
**                               |               |      |               |
**                               |               v      v               |
**                               |      +------+----------+------+      |
**                               +------| prev | not used | next |------+
**                                      +------+----------+------+
**                                    sentinel    ^     ^     ^
**                                                |     |     |
**                                             begin()  |    tail
**                           (aka head, sentinel.next)  |    (aka sentinel.prev)
**                                                      |
**                                                      end()
**
**
**
**
**
**  size = 3:
**                           +---------------------------------------------------------------------------------------------------+
**                           |                                                                                                   |
**                           v                                                                                                   |
**              +------+----------+------+      +------+------+------+      +------+------+------+     +------+------+------+    |
**         +----| prev | not used | next |<---->| prev | data | next |<---->| prev | data | next |<--->| prev | data | next |----+
**         |    +------+----------+------+      +------+------+------+      +------+------+------+     +------+------+------+
**         | sentinel            ^                         ^                                                     ^       ^
**         |                     |                         |                                                     |       |
**         |                    end()                    begin()                                                 |      tail
**         |                                    (aka head, sentinel.next)                                        |  (aka sentinel.prev)
**         +-----------------------------------------------------------------------------------------------------+
**
***********************************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A circular doubly linked list's unique private type, attribute, and function members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace CSUF::CPSC131
{
  template<typename T>
  struct Members<T, DllPolicy::CIRCULAR>
  {
    // Types
    // A circular doubly linked list's node
    struct Node
    {
      Node() = default;
      Node( T value ) : _data( std::move(value) ) {}

      T      _data = T{};                                                             // linked list element value
      Node * _next = this;                                                            // next item in the list
      Node * _prev = this;                                                            // previous item in the list
    };



    // Member instance attributes
    // A circular doubly linked list's private data implementation
    Node        _sentinel;                                                            // dummy node.  The first node of the list (_head) is at *(_sentinel->next)
    Node *&     _head = _sentinel._next;                                              // An easier to read alias for the list's head, which is a pointer-to-Node
    Node *&     _tail = _sentinel._prev;                                              // last element in the list.  tail->next always points to _sentinel
    std::size_t _size = 0;                                                            // number of elements in the collection



    //==============================================================================================================================
    // Specialized function implemented specifically for a circular doubly linked list
    //
    //==============================================================================================================================

    // end()
    Node * end() noexcept
    { return &_sentinel; }



    // insert()
    Node * insert( Node * currentNode, T const & value )
    {
      Node * newNode     = new Node( value );                                         // create and populate a new node

      newNode->_next = currentNode;                                                   // Link the node to the list
      newNode->_prev = currentNode->_prev;

      currentNode->_prev->_next = newNode;                                            // Link the list to the node
      currentNode->_prev        = newNode;

      ++_size;                                                                        // Increase the list's size and return
      return newNode;
    }



    // erase()
    Node * erase( Node * currentNode )
    {
      if( _size == 0           )   throw std::length_error    ( exceptionString( "Attempt to erase from an empty list"     ) );
      if( currentNode == end() )   throw std::invalid_argument( exceptionString( "Attempt to erase at an invalid location" ) );

      currentNode->_next->_prev = currentNode->_prev;                                 // Take the node out of the list
      currentNode->_prev->_next = currentNode->_next;

      --_size;                                                                        // decrease the list's size

      Node * returnNode{ currentNode->_next };                                        // return the node after the one removed
      delete currentNode;                                                             // delete the old, now removed, node
      return returnNode;
    }

  }; // struct Members;
} // namespace CSUF::CPSC131















/***********************************************************************************************************************************
** Class DoublyLinkedList - a very basic example implementation of the Doubly Linked List Abstract Data Type
**
**  This file provides a null-terminated implementation of the DoublyLinkedList interface
**
**  Empty (size = 0):
**
**                   nullptr    nullptr    nullptr    nullptr
**                      ^          ^          ^          ^
**                      |          |          |          |
**                    head       begin()     tail      end()
**
**
**
**  size = 3:
**                              +------+------+------+      +------+------+------+     +------+------+------+
**                nullptr<------| prev | data | next |<---->| prev | data | next |<--->| prev | data | next |------>nullptr
**                              +------+------+------+      +------+------+------+     +------+------+------+          ^
**                                   ^      ^                                                     ^                    |
**                                   |      |                                                     |                    |
**                                 head   begin()                                                tail                end()
**
**
***********************************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A null-terminated doubly linked list's unique private type, attribute, and function members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace CSUF::CPSC131
{
  template<typename T>
  struct Members<T, DllPolicy::NTL>
  {
    // Types
    // A null-terminate doubly linked list's node
    struct Node
    {
      Node() = default;
      Node( const T value ) : _data( std::move(value) ) {}

      T      _data = T{};                                                             // linked list element value
      Node * _next = nullptr;                                                         // next item in the list
      Node * _prev = nullptr;                                                         // previous item in the list
    };



    // Member instance attributes
    // A circular doubly linked list's private data implementation
    Node *      _head = nullptr;
    Node *      _tail = nullptr;
    std::size_t _size = 0;



    //==============================================================================================================================
    // Specialized function implemented specifically for a null-terminate circular doubly linked list
    //
    //==============================================================================================================================

    // end()
    Node * end() noexcept
    { return nullptr; }



    // insert()
    Node * insert( Node * currentNode, T const & value )
    {
      Node * newNode     = new Node( value );                                         // Create and populate a new node with the provided data


      // Special Case 1:  Inserting into an empty list?
      if( _size == 0 )   _head = _tail = newNode;                                     // Both _head and _tail now point to the same, one and only node in the list


      // Checking head for a null pointer is not logically necessary because we know head should be null if the list is empty. But a
      // little defensive programming here is worth while.
      else if( _head == nullptr )   throw std::logic_error( exceptionString( "Error:  DoublyLinkedList insert() size and head inconsistency" ) );


      // Special Case 2:  Inserting at the front of the list?
      else if( currentNode == _head )                                                 // Insert at front of list
      {
        // Add the new node to the front and then move the list's head
        newNode->_next = _head;                                                       // newNode->_next and head now point to the same node
        _head->_prev   = newNode;                                                     // the head's previous node becomes the new node
        _head          = newNode;                                                     // the new node becomes the head of the list
      }


      // Special Case 3:  Inserting at the back of the list?
      else if( currentNode == end() )                                                 // Insert at back of list
      {
        // Add the new node to the back and then move the list's tail
        _tail->_next    = newNode;                                                    // the new node now follows the tail node
        newNode->_prev = _tail;                                                       // the tail's previous and the new node's previous no point to the same node
        _tail           = newNode;                                                    // the new node becomes the new tail of the list
      }


      // Normal Case:  Inserting somewhere in the middle of the list.
      else                                                                            // Insert in the middle of the list
      {
        newNode->_next = currentNode;                                                 // The new node's pointers is linked into place
        newNode->_prev = currentNode->_prev;

        currentNode->_prev->_next = newNode;                                          // The new node becomes part of the chain
        currentNode->_prev        = newNode;
      }

      ++_size;
      return newNode;
    }



    // erase()
    Node * erase( Node * currentNode )
    {
      // Error Case: Removing from an empty list?
      if( _size == 0 )   throw std::length_error( exceptionString( "attempt to erase from an empty list" ) );

      // Special Case 1:  Removing a node not in the list?
      if( currentNode == end() )   throw std::length_error( exceptionString( "attempt to erase from past the end of list" ) );


      // Special Case 2:  Removing the node at the front of the list?
      if( currentNode == _head )
      {
        // Special Case 3: removing the node at the back of the list?
        if( currentNode == _tail )                                                    // current node is both the head and the tail
        {
          _head = _tail = nullptr;
        }
        else                                                                          // current node is the head but not the tail
        {
          _head        = _head->_next;
          _head->_prev = nullptr;
        }
      }

      else if( currentNode == _tail )                                                  // current node is the tail but not the head
      {
        _tail        = _tail->_prev;
        _tail->_next = nullptr;
      }

      else                                                                            // current node is between the head and the tail
      {
        currentNode->_next->_prev = currentNode->_prev;
        currentNode->_prev->_next = currentNode->_next;
      }

      --_size;

      Node *  returnNode{ currentNode->_next };                                       // return the node after the one removed
      delete currentNode;                                                             // delete what used to be the old node
      return returnNode;
    }

  }; // struct Members;
} // namespace CSUF::CPSC131















/***********************************************************************************************************************************
** Common functions for all Doubly Linked List implementations
**
** When designing a solution, first try real hard to write a function agnostic of implementation details.  For example, use end()
** instead of nullptr or &_sentinal.
***********************************************************************************************************************************/
namespace CSUF::CPSC131
{
  /*******************************************************************************
  ** A circular doubly linked list's member function definitions
  *******************************************************************************/

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors, destructor, and assignments
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Default constructor
  template<typename T, DllPolicy POLICY>
  DoublyLinkedList<T, POLICY>::DoublyLinkedList()
    : self( std::make_unique<PrivateAttributes>() )
  {}



  // Copy constructor
  template<typename T, DllPolicy POLICY>
  DoublyLinkedList<T, POLICY>::DoublyLinkedList( const DoublyLinkedList & original )
    : DoublyLinkedList()
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the original list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & element : original )    push_back( element );
  }



  // Move constructor
  template<typename T, DllPolicy POLICY>
  DoublyLinkedList<T, POLICY>::DoublyLinkedList( DoublyLinkedList && original ) noexcept
    : self{ std::move( original.self ) }
  {}



  // Initializer list constructor
  template<typename T, DllPolicy POLICY>
  DoublyLinkedList<T, POLICY>::DoublyLinkedList( std::initializer_list<T> init_list )
    : DoublyLinkedList()
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the initializer list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & element : init_list )    push_back( element );
  }



  // Copy assignment
  template<typename T, DllPolicy POLICY>
  DoublyLinkedList<T, POLICY> & DoublyLinkedList<T, POLICY>::operator=( DoublyLinkedList const & rhs )
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
  template<typename T, DllPolicy POLICY>
  DoublyLinkedList<T, POLICY> & DoublyLinkedList<T, POLICY>::operator=( DoublyLinkedList && rhs ) noexcept
  {
    if (&rhs != this)
    {
      clear();
      self = std::move( rhs.self );
    }
    return *this;
  }



  // Destructor
  template<typename T, DllPolicy POLICY>
  DoublyLinkedList<T, POLICY>::~DoublyLinkedList() noexcept
  { clear(); }



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Queries
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // empty() const
  template<typename T, DllPolicy POLICY>
  bool DoublyLinkedList<T, POLICY>::empty() const noexcept
  {
    return size() == 0;
  }



  // size() const
  template<typename T, DllPolicy POLICY>
  std::size_t DoublyLinkedList<T, POLICY>::size() const noexcept
  { return self->_size; }



  // front() const
  template<typename T, DllPolicy POLICY>
  const T & DoublyLinkedList<T, POLICY>::front() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant DLL instead of a constant DLL. The "const"
    // at the end of the function signature above is what makes this object constant.  By casting away the const, we call the front()
    // function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only one place, and
    // then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of spread through the
    // code base.
    auto non_const_DLL = const_cast<DoublyLinkedList<T, POLICY> *>( this );
    return non_const_DLL->front();
  }



  // back() const
  template<typename T, DllPolicy POLICY>
  const T & DoublyLinkedList<T, POLICY>::back() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant DLL instead of a constant DLL. The
    // "const" at the end of the function signature above is what makes this object constant.  By casting away the const, we call
    // the back() function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only
    // one place, and then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of
    // spread through the code base.
    auto non_const_DLL = const_cast<DoublyLinkedList<T, POLICY> *>( this );
    return non_const_DLL->back();
  }



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Iterators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // begin()
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::iterator DoublyLinkedList<T, POLICY>::begin() noexcept
  { return self->_head; }



  // begin() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_iterator DoublyLinkedList<T, POLICY>::begin() const noexcept
  { return const_cast<DoublyLinkedList<T, POLICY> *>(this)->begin(); }                        // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // cbegin() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_iterator DoublyLinkedList<T, POLICY>::cbegin() const noexcept
  { return const_cast<DoublyLinkedList<T, POLICY> *>(this)->begin(); }                        // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // end()
  template <typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::iterator DoublyLinkedList<T, POLICY>::end() noexcept
  { return self->end(); }                                                             // dispatch to policy specific implementation



  // end() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_iterator DoublyLinkedList<T, POLICY>::end() const noexcept
  { return const_cast<DoublyLinkedList<T, POLICY> *>(this)->end(); }                          // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // cend() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_iterator DoublyLinkedList<T, POLICY>::cend() const noexcept
  { return const_cast<DoublyLinkedList<T, POLICY> *>(this)->end(); }                          // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // rbegin()
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::reverse_iterator DoublyLinkedList<T, POLICY>::rbegin() noexcept
  { return std::make_reverse_iterator( end() ); }



  // rbegin() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_reverse_iterator DoublyLinkedList<T, POLICY>::rbegin() const noexcept
  { return std::make_reverse_iterator( end() ); }



  // crbegin() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_reverse_iterator DoublyLinkedList<T, POLICY>::crbegin() const noexcept
  { return std::make_reverse_iterator( end() ); }



  // rend()
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::reverse_iterator DoublyLinkedList<T, POLICY>::rend() noexcept
  { return std::make_reverse_iterator( begin() ); }



  // rend() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_reverse_iterator DoublyLinkedList<T, POLICY>::rend() const noexcept
  { return std::make_reverse_iterator( begin() ); }



  // crend() const
  template<typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::const_reverse_iterator DoublyLinkedList<T, POLICY>::crend() const noexcept
  { return std::make_reverse_iterator( begin() ); }



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Accessors
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // front()
  template<typename T, DllPolicy POLICY>
  T & DoublyLinkedList<T, POLICY>::front()
  {
    if( empty() )   throw std::length_error( exceptionString( "Attempt to access the front of an empty list" ) );

    return self->_head->_data;
  }



  // back()
  template<typename T, DllPolicy POLICY>
  T & DoublyLinkedList<T, POLICY>::back()
  {
    if( empty() )    throw std::length_error( exceptionString( "Attempt to access the back of an empty list" ) );
    return self->_tail->_data;
  }



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Modifiers
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // clear()
  template<typename T, DllPolicy POLICY>
  void DoublyLinkedList<T, POLICY>::clear()
  { while( !empty() ) pop_front(); }



  // push_front()
  template<typename T, DllPolicy POLICY>
  void DoublyLinkedList<T, POLICY>::push_front( T const & value )
  { insert( self->_head, value ); }



  // pop_front
  template<typename T, DllPolicy POLICY>
  void DoublyLinkedList<T, POLICY>::pop_front()
  { erase( self->_head ); }



  // push_back()
  template<typename T, DllPolicy POLICY>
  void DoublyLinkedList<T, POLICY>::push_back( T const & value )
  { insert( end(), value  ); }



  // pop_back()
  template<typename T, DllPolicy POLICY>
  void DoublyLinkedList<T, POLICY>::pop_back()
  { erase( self->_tail ); }



  // insert()
  template <typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::iterator DoublyLinkedList<T, POLICY>::insert( const_iterator position, T const & data )
  { return self->insert( position._nodePtr, data ); }                                 // dispatch to policy specific implementation



  // erase()
  template <typename T, DllPolicy POLICY>
  typename DoublyLinkedList<T, POLICY>::iterator DoublyLinkedList<T, POLICY>::erase( const_iterator position )
  { return self->erase( position._nodePtr ); }                                        // dispatch to policy specific implementation



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Relational Operators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // operator<=>
  template<typename T, DllPolicy POLICY>
  std::weak_ordering DoublyLinkedList<T, POLICY>::operator<=>( DoublyLinkedList const & rhs ) const
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
  template<typename T, DllPolicy POLICY>
  bool DoublyLinkedList<T, POLICY>::operator==( DoublyLinkedList const & rhs ) const
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
  template<typename T, DllPolicy POLICY>
  void swap( DoublyLinkedList<T, POLICY> & lhs, DoublyLinkedList<T, POLICY> & rhs )
  {
    using std::swap;                                                                  // Let argument dependent lookup (ADL) find the right swap function
    swap( lhs.self, rhs.self );                                                       // and avoid a explicit call to std::swap()
  }






  /*********************************************************************************************************************************
  **********************************************************************************************************************************
  ** DoublyLinkedList<>::iterator Member Function Definitions
  **
  *********************************************************************************************************************************/
  // Default constructor - needed to support std::ranges::range concept, but should never be called.
  //
  // Let's move the error detection to a linker error instead of a run time error by not providing an definition
  #if 0
    template<typename T, DllPolicy POLICY>   template<typename U>
    DoublyLinkedList<T, POLICY>::Iterator_type<U>::Iterator_type()
    {
      throw std::logic_error( exceptionString( "CSUF::CPSC131::DoublyLinkedList<T, POLICY> default constructed (aka Sentinel) iterators not supported" ) );
    }
  #endif



  // Copy constructor when U is non-const iterator, Conversion constructor from non-const to const iterator when U is a const iterator
  // Type of parameter is intentionally a non-constant iterator
  template<typename T, DllPolicy POLICY>   template<typename U>
  DoublyLinkedList<T, POLICY>::Iterator_type<U>::Iterator_type( iterator const & other )  noexcept    // Notice the parameter type is "iterator", not "Iterator_type"
    : _nodePtr{ other._nodePtr }
  {}



  // Conversion Constructor from pointer-to-Node to iterator
  template<typename T, DllPolicy POLICY>   template<typename U>
  DoublyLinkedList<T, POLICY>::Iterator_type<U>::Iterator_type( Node * p ) noexcept
    : _nodePtr{ p }
  {}



  // operator++   pre-increment
  template<typename T, DllPolicy POLICY>   template<typename U>
  typename DoublyLinkedList<T, POLICY>::template Iterator_type<U> &   DoublyLinkedList<T, POLICY>::Iterator_type<U>::operator++()
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( exceptionString( "Attempt to increment null iterator.  Cannot increment end() for a null-terminated list" ) );

    _nodePtr = _nodePtr->_next;
    return *this;
  }



  // operator++   post-increment
  template<typename T, DllPolicy POLICY>   template<typename U>
  typename DoublyLinkedList<T, POLICY>::template Iterator_type<U>   DoublyLinkedList<T, POLICY>::Iterator_type<U>::operator++( int )
  {
    auto temp{ *this };
    operator++();                                                                     // Delegate to pre-increment leveraging error checking
    return temp;
  }



  // operator--   pre-decrement
  template<typename T, DllPolicy POLICY>   template<typename U>
  typename DoublyLinkedList<T, POLICY>::template Iterator_type<U> &   DoublyLinkedList<T, POLICY>::Iterator_type<U>::operator--()
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( exceptionString( "Attempt to decrement null iterator.  Cannot decrement end() for a null-terminated list" ) );

    _nodePtr = _nodePtr->_prev;
    return *this;
  }



  // operator--   post-decrement
  template<typename T, DllPolicy POLICY>   template<typename U>
  typename DoublyLinkedList<T, POLICY>::template Iterator_type<U>    DoublyLinkedList<T, POLICY>::Iterator_type<U>::operator--( int )
  {
    auto temp( *this );
    operator--();                                                                     // Delegate to pre-decrement leveraging error checking
    return temp;
  }



  // operator*
  template<typename T, DllPolicy POLICY>   template<typename U>
  typename DoublyLinkedList<T, POLICY>::template Iterator_type<U>::reference   DoublyLinkedList<T, POLICY>::Iterator_type<U>::operator*() const
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( exceptionString( "Attempt to dereference null iterator" ) );

    return _nodePtr->_data;
  }



  // operator->
  template<typename T, DllPolicy POLICY>   template<typename U>
  typename DoublyLinkedList<T, POLICY>::template Iterator_type<U>::pointer   DoublyLinkedList<T, POLICY>::Iterator_type<U>::operator->() const
  {
    if( _nodePtr == nullptr )  throw std::invalid_argument( exceptionString( "Attempt to dereference null iterator" ) );

    return &(_nodePtr->_data);
  }



  // operator==
  template<typename T, DllPolicy POLICY>   template<typename U>
  bool DoublyLinkedList<T, POLICY>::Iterator_type<U>::operator==( Iterator_type const & rhs ) const
  { return _nodePtr == rhs._nodePtr; }
}    // namespace CSUF::CPSC131















/***********************************************************************************************************************************
** (C) Copyright 2025 by Thomas Bettens. All Rights Reserved.
**
** DISCLAIMER: The participating authors at California State University's Computer Science Department have used their best efforts
** in preparing this code. These efforts include the development, research, and testing of the theories and programs to determine
** their effectiveness. The authors make no warranty of any kind, expressed or implied, with regard to these programs or to the
** documentation contained within. The authors shall not be liable in any event for incidental or consequential damages in
** connection with, or arising out of, the furnishing, performance, or use of these libraries and programs.  Distribution without
** written consent from the authors is prohibited.
***********************************************************************************************************************************/

/**************************************************
** Last modified:  27-JUL-2025 (Converted to C++ Modules)
** Last Verified:  27-JUL-2025
** Verified with:  MS Visual Studio 2022 Version 17.14.9,  Compiler Version 19.44.35213 (/std:c++latest)
**                 GCC version 15.1.0 (-std=c++23 )
**                 Clang version 21.0.0 (-std=c++23 -stdlib=libc++)
***************************************************/
