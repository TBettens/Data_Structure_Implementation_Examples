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
module;                                                                               // Global fragment (not part of the module)
  // Empty









/***********************************************************************************************************************************
**  Module CSUF.CPSC131.SinglyLinkedList Interface
**
***********************************************************************************************************************************/
export module CSUF.CPSC131.SinglyLinkedList;
import std;
import CSUF.CPSC131.exceptionString;



namespace CSUF::CPSC131
{
  export enum class SllPolicy {CIRCULAR, NTL};

  template<typename, SllPolicy> struct Members;                                       // intentionally not exported

  // Template Class Definition
  export template<typename T, SllPolicy POLICY = SllPolicy::CIRCULAR>
  class SinglyLinkedList
  {
    template <typename U>
    friend void swap( SinglyLinkedList<U, POLICY> & lhs, SinglyLinkedList<U, POLICY> & rhs ); // The expected way to make a program-defined type swappable is to provide a
                                                                                      // non-member function swap in the same namespace as the type.
                                                                                      // (https://en.cppreference.com/w/cpp/algorithm/swap)
    private:
      // Types
      template <typename U>  class Iterator_type;                                     // Template class for iterator and const_iterator classes
      using Node              = Members<T, POLICY>::Node;                             // Specific implementations are responsible for defining their node structure
      using PrivateAttributes = Members<T, POLICY>;                                   // A specific implementation's private members (attributes, functions, etc)

      // Member instance attributes
      std::unique_ptr<PrivateAttributes> self = nullptr;                              // See Pimpl Idiom at https://cpppatterns.com/patterns/pimpl.html



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
      void forwardPrint( std::ostream & stream = std::cout ) const;



      // Add a function, named "backwardPrint", to the Singly Linked List that writes the elements to standard output from back to
      // front. The function must be implemented recursively. Assume operator+(lhs, rhs) is defined
      void backwardPrint( std::ostream & stream = std::cout ) const;



    private:
      // Extended interface helper functions to demonstrate recursion
      void     reverse      ( Node * current                        );
      iterator find         ( Node * current, const T & value       );
      T        add          ( Node * current                        ) const;
      void     forwardPrint ( Node * current, std::ostream & stream ) const;
      void     backwardPrint( Node * current, std::ostream & stream ) const;
  };  // class SinglyLinkedList









  /*********************************************************************************************************************************
  ** Class SinglyLinkedList<T, POLICY>::iterator - A singly linked list forward iterator
  **
  *********************************************************************************************************************************/
  template <typename T, SllPolicy POLICY>   template<typename U>
  class SinglyLinkedList<T, POLICY>::Iterator_type
  {
    friend class SinglyLinkedList<T, POLICY>;

    public:
      // Iterator Type Traits - Boilerplate stuff so the iterator can be used with the rest of the standard library
      using iterator_category = std::forward_iterator_tag;
      using value_type        = U;                                                    // U is expected to be either T or T const
      using difference_type   = std::ptrdiff_t;
      using pointer           = value_type *;
      using reference         = value_type &;


      // Compiler synthesized constructors and destructor are fine, just what we want (shallow copies, no ownership)

      // A default constructor should have Sentinel semantics, which is to say it indicates the end of a list.  An iterator default
      // constructed through the class (i.e., not through a list object) should indicate the end of any list.  This behavior is not
      // implemented here.  In particular, a default constructed iterator, this case, will not match list.end() if list is
      // implemented as a circular list.  The Sentinel Design Pattern could be implemented as an enhancement, but I choose to not go
      // that far primarily for simplicity.  To help prevent misuse of a default constructed iterator, I used to "delete" the
      // default constructor.  But that stops the list from being recognized by the std::ranges:range concept which in turn prevents
      // the list from using std::ranges based algorithms, including std::format.
      Iterator_type(                        );                                        // Default constructed Iterator_type constructs a pseudo Sentinel object (null pointer in this case)
      Iterator_type( iterator const & other );                                        // Copy constructor when T is non-const, Conversion constructor from non-const to const iterator when T is const

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
  };  // SinglyLinkedList<T, POLICY>::Iterator_type

}    // namespace CSUF::CPSC131















/***********************************************************************************************************************************
************************************************************************************************************************************
** Template Implementation
**
** Separating Interface from Implementation is an extremely important concept I hope students will come to appreciate.
************************************************************************************************************************************
***********************************************************************************************************************************/

// Not exported but reachable
// Organization:
//  o)  Circular Singly Linked List With One Dummy Node Specific
//  o)  Null-Terminated Singly Linked List With One Dummy Node Specific
//  0)  Common across all Singly Linked Lists



/***********************************************************************************************************************************
**  Circular Singly Linked List With One Dummy Node
**
**  This file provides a one-dummy node, circular implementation of the SinglyLinkedList interface
**
** Empty (size = 0):
**                           +---------------+
**                           |               |
**                           v               |
**                  +----------+------+      |
**                  | not used | next |------+
**                  +----------+------+
**             sentinel   ^     ^ ^  ^
**                        |     | |  |
**               beforeBegin()  | | tail
**                              | |
**                              | |
**                              | end()
**                              |
**                           begin()
**                         (aka head)
**
**
**  size = 3:
**                           +------------------------------------------------------------------------------+
**                           |                                                                              |
**                           v                                                                              |
**                  +----------+------+       +------+------+       +------+------+       +------+------+   |
**                  | not used | next |------>| data | next |------>| data | next |------>| data | next |---+
**                  +----------+------+       +------+------+       +------+------+       +------+------+
**             sentinel      ^    ^                 ^                                            ^
**                           |    |                 |                                            |
**                 beforeBegin() end()            begin()                                       tail
**                                              (aka head)
**
***********************************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A circular singly linked list's unique private type, attribute, and function members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace CSUF::CPSC131
{
  template<typename T>
  struct Members<T, SllPolicy::CIRCULAR>
  {
    // Types
    // A circular singly linked list's node
    struct Node
    {
      Node() = default;
      Node( T value ) : _data{ std::move(value) } {}

      T      _data = T{};                                                             // linked list element value
      Node * _next = this;                                                            // next item in the list
    };



    // Member instance attributes
    // A circular singly linked list's private data implementation
    Node        _sentinel;                                                            // dummy node.  The first node of the list (_head) is at *(_sentine._next);
    Node *&     _head = _sentinel._next;                                              // _head is just another name for _sentinel._next.  The head of the list is always at this location
    Node *      _tail = &_sentinel;                                                   // last element in the list.  tail->next always points to _sentinel
    std::size_t _size = 0;                                                            // number of elements in the collection



    //==============================================================================================================================
    // Specialized function implemented specifically for a circular singly linked list
    //
    //==============================================================================================================================

    // before_begin()
    Node * before_begin()
    { return &_sentinel; }



    // end()
    Node * end()
    { return &_sentinel; }



    // insert_after()
    Node * insert_after( Node * currentNode, T const & data )
    {
      Node * newNode     = new Node( data );                                            // Create and populate a new node with the provided data

      // Relink the node into position
      newNode    ->_next = currentNode->_next;                                          // the new node and the current node point to the same next node
      currentNode->_next = newNode;                                                     // the current node now points to the new node

      // If necessary, move the tail
      if( end() == newNode->_next )   _tail = newNode;

      ++_size;
      return newNode;
    }



    // erase_after()
    Node * erase_after( Node * currentNode )
    {
      // Error Case:  Removing from an empty list?
      if( _size == 0 )   throw std::length_error( exceptionString( "attempt to remove from an empty list" ) );

      // Error Case:  Removing after the last node?
      if( currentNode == _tail )   throw std::length_error( exceptionString( "attempt to remove after the last element" ) );


      Node * toBeRemoved = currentNode->_next;                                          // toBeRemoved points to the node taken out of the chain
      currentNode->_next = currentNode->_next->_next;                                   // take the node out of the chain

      --_size;

      if( toBeRemoved == _tail )    _tail = currentNode;                                // adjust tail to point to the new back node in the list

      delete toBeRemoved;                                                               // delete what used to be the old node
      return currentNode->_next;
    }

  }; // struct Members;
} // namespace CSUF::CPSC131















/***********************************************************************************************************************************
**  Null-Terminated Singly Linked
**
**  This file provides a null-terminated implementation of the SinglyLinkedList interface
**
**  Empty (size = 0):
**
**                      nullptr    nullptr    nullptr    nullptr    nullptr
**                         ^          ^          ^          ^          ^
**                         |          |          |          |          |
**                  before_begin()  head       begin()     tail      end()
**
**
**
**  size = 3:
**                                            +------+------+       +------+------+       +------+------+
**                        nullptr             | data | next |------>| data | next |------>| data | next |------>nullptr
**                           ^                +------+------+       +------+------+       +------+------+          ^
**                           |                  ^      ^                                         ^                 |
**                           |                  |      |                                         |                 |
**                    before_begin()          head   begin()                                   tail              end()
**
**
***********************************************************************************************************************************/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A null-terminated singly linked list's unique private type, attribute, and function members
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace CSUF::CPSC131
{
  template<typename T>
  struct Members<T, SllPolicy::NTL>
  {
    // Types
    // A null-terminated singly linked list's node
    struct Node
    {
      Node() = default;
      Node( T value ) : _data{ std::move(value) } {}

      T      _data = T{};                                                    // linked list element value
      Node * _next = nullptr;                                                // next item in the list
    };



    // Member instance attributes
    // A null-terminated linked list's private data implementation
    Node *      _head = nullptr;                                             // first element in the list
    Node *      _tail = nullptr;                                             // last element in the list.  tail->next is always nullptr
    std::size_t _size = 0;                                                   // number of elements in the collection



    //==============================================================================================================================
    // Specialized function implemented specifically for a null-terminated singly linked list
    //
    //==============================================================================================================================

    // before_begin()
    Node * before_begin()
    { return nullptr; }



    // end()
    Node * end()
    { return nullptr; }



    // insert_after()
    Node * insert_after( Node * currentNode, T const & data )
    {
      Node * newNode     = new Node( data );                                 // Create and populate a new node with the provided data

      // Special Case 1:  Inserting into an empty list?
      if( _size == 0 )   _head = _tail = newNode;                            // Both _head and _tail now point to the same, one and only node in the list


      // Special Case 2:  Inserting at the front of the list?
      else if( currentNode == before_begin() )
      {
        // Add the new node to the front and then move the list's head
        newNode->_next = _head;                                              // newNode->_next and head now point to the same node
        _head           = newNode;                                           // the new node becomes the head of the list
      }


      // Special Case 3:  Inserting at the back of the list?
      else if( currentNode == _tail )
      {
        // Add the new node to the back and then move the list's tail
        _tail->_next = newNode;                                              // the new node now follows the tail node
        _tail        = newNode;                                              // the new node becomes the new tail of the list
      }


      // Normal Case:  Inserting somewhere in the middle of the list.
      else                                                                   // Insert in the middle of the list
      {
        // Relink the node into position
        newNode    ->_next = currentNode->_next;                             // the new node and the current node point to the same next node
        currentNode->_next = newNode;                                        // the current node now points to the new node
      }

      ++_size;
      return newNode;
    }



    // erase_after()
    Node * erase_after( Node * currentNode )
    {
      // Error Case: Removing from an empty list?
      if( _size==0 )   throw std::length_error( exceptionString( "attempt to remove from an empty list" ) );

      // Special Case 1:  Attempting to remove after the last node?          // There is no node after the tail - logic error?
      if( currentNode == _tail )    return end();                            // removing after the tail intentionally does nothing



      Node * toBeRemoved = nullptr;                                          // toBeRemoved keeps a handle to the node taken out of the chain

      // Special Case 2:  Removing the node at the front of the list?
      if( currentNode == before_begin() )                                    // remove the node at the head of the list
      {
        toBeRemoved = _head;                                                 // keep a handle to the old head
        _head        = _head->_next;                                         // the second (next) node becomes the new head, or the head becomes nullptr if the only remaining node is being removed
      }


      // Normal Case:  Removing a node in the middle of the list.
      else
      {
        //  BEFORE:  ... --> (data|next) --> (data|next) --> (data|next) --> (data|next) --> ...
        //                                        ^               ^               ^
        //                                position/current   toBeRemoved       returnNode
        //
        //
        //                                                 _______________
        //  AFTER:   ... --> (data|next) --> (data|next) -/  (data|next)  \->(data|next) --> ...
        //                                        ^               ^               ^
        //                                position/current   toBeRemoved       returnNode

        toBeRemoved        =currentNode->_next;                              // toBeRemoved points to the node to taken out of the chain
        currentNode->_next =currentNode->_next->_next;                       // take the node out of the chain
      }

      --_size;


      // Special Case 3: removing the back node in the list?
      //
      // Note: Don't dereference currentNode here, it may be null (e.g., before_begin())
      //       if( position()->next == end() ) ...
      if( toBeRemoved == _tail )    _tail = currentNode;                     // adjust tail to point to the new back node in the list

      Node * returnNode( toBeRemoved->_next );                               // return the node after the one removed
      delete toBeRemoved;                                                    // delete what used to be the old node
      return returnNode;
    }
  };    // struct Members;
}  //namespace CSUF::CPSC131















/***********************************************************************************************************************************
** Common functions for all Singly Linked List implementations
**
** When designing a solution, first try real hard to write a function agnostic of implementation details.  For example, use end()
** instead of nullptr or &_sentinal.
***********************************************************************************************************************************/
namespace CSUF::CPSC131
{
  /*******************************************************************************
  ** A circular singly linked list's member function definitions
  *******************************************************************************/

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors, destructor, and assignments
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Default constructor
  template <typename T, SllPolicy POLICY>
  SinglyLinkedList<T, POLICY>::SinglyLinkedList()
    : self( std::make_unique<PrivateAttributes>() )                                   // construct the list's private members
  {}



  // Copy construction
  template <typename T, SllPolicy POLICY>
  SinglyLinkedList<T, POLICY>::SinglyLinkedList( const SinglyLinkedList & original )
    : SinglyLinkedList()                                                              // delegating constructor
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the original list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & current : original )    push_back( current );
  }



  // Move construction
  template <typename T, SllPolicy POLICY>
  SinglyLinkedList<T, POLICY>::SinglyLinkedList( SinglyLinkedList && original ) noexcept
    : self( std::move(original.self) )
  {}



  // Initializer List construction
  template <typename T, SllPolicy POLICY>
  SinglyLinkedList<T, POLICY>::SinglyLinkedList( std::initializer_list<T> init_list )
    : SinglyLinkedList()                                                              // delegating constructor
  {
    // This new list has already been constructed and initialized to be empty.  Now, walk the initializer list adding copies of the
    // elements to this (initially empty) list maintaining order (i.e,  perform a deep copy)
    for( const auto & current : init_list )    push_back( current );
  }



  // Copy assignment
  template <typename T, SllPolicy POLICY>
  SinglyLinkedList<T, POLICY> & SinglyLinkedList<T, POLICY>::operator=( SinglyLinkedList const & rhs )
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
  template <typename T, SllPolicy POLICY>
  SinglyLinkedList<T, POLICY> & SinglyLinkedList<T, POLICY>::operator=( SinglyLinkedList && rhs ) noexcept
  {
    if( &rhs != this )
    {
      clear();
      self = std::move( rhs.self );
    }
    return *this;
  }



  // Destructor
  template <typename T, SllPolicy POLICY>
  SinglyLinkedList<T, POLICY>::~SinglyLinkedList() noexcept
  { clear(); }



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Queries
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // empty() const
  template <typename T, SllPolicy POLICY>
  bool SinglyLinkedList<T, POLICY>::empty() const noexcept
  { return size() == 0; }



  // size() const
  template <typename T, SllPolicy POLICY>
  std::size_t SinglyLinkedList<T, POLICY>::size() const noexcept
  { return self->_size; }



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Iterators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // before_begin()
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::iterator SinglyLinkedList<T, POLICY>::before_begin()
  { return self->before_begin(); }                                                    // dispatch to policy specific implementation



  // before_begin() const
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::const_iterator SinglyLinkedList<T, POLICY>::before_begin() const
  { return const_cast<SinglyLinkedList<T, POLICY> *>(this)->before_begin(); }         // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // cbefore_begin() const
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::const_iterator SinglyLinkedList<T, POLICY>::cbefore_begin() const
  {  return const_cast<SinglyLinkedList<T, POLICY> *>(this)->before_begin(); }        // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // begin()
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::iterator SinglyLinkedList<T, POLICY>::begin()
  { return self->_head; }



  // begin() const
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::const_iterator SinglyLinkedList<T, POLICY>::begin() const
  { return const_cast<SinglyLinkedList<T, POLICY> *>(this)->begin(); }                // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // cbegin() const
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::const_iterator SinglyLinkedList<T, POLICY>::cbegin() const
  { return const_cast<SinglyLinkedList<T, POLICY> *>(this)->begin(); }                // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // end()
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::iterator SinglyLinkedList<T, POLICY>::end()
  { return self->end(); }                                                             // dispatch to policy specific implementation



  // end() const
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::const_iterator SinglyLinkedList<T, POLICY>::end() const
  { return const_cast<SinglyLinkedList<T, POLICY> *>(this)->end(); }                  // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  // cend() const
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::const_iterator SinglyLinkedList<T, POLICY>::cend() const
  { return const_cast<SinglyLinkedList<T, POLICY> *>(this)->end(); }                  // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Accessors
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // front()
  template <typename T, SllPolicy POLICY>
  T & SinglyLinkedList<T, POLICY>::front()
  {
    if( empty() )   throw std::length_error( exceptionString( "empty list" ) );

    return *begin();
  }



  // front() const
  template <typename T, SllPolicy POLICY>
  const T & SinglyLinkedList<T, POLICY>::front() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant SLL instead of a constant SLL. The
    // "const" at the end of the function signature above is what makes this object constant.  By casting away the const, we call
    // the front() function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only
    // one place, and then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of
    // spread through the code base.
    auto non_const_SLL = const_cast<SinglyLinkedList<T, POLICY> *>( this );
    return non_const_SLL->front();
  }



  // back()
  template <typename T, SllPolicy POLICY>
  T & SinglyLinkedList<T, POLICY>::back()
  {
    if( empty() )   throw std::length_error( exceptionString( "attempt to access data from an empty list" ) );

    return self->_tail->_data;
  }



  // back() const
  template <typename T, SllPolicy POLICY>
  const T & SinglyLinkedList<T, POLICY>::back() const
  {
    // delegate to the non-const overloaded version by temporarily pointing to a non-constant SLL instead of a constant SLL. The
    // "const" at the end of the function signature above is what makes this object constant.  By casting away the const, we call
    // the back() function below that doesn't have "const" at the end.  We do this to capture the logic and implementation in only
    // one place, and then reuse that logic elsewhere.  This way maintenance and bug fixes are localized to one place instead of
    // spread through the code base.
    auto non_const_SLL = const_cast<SinglyLinkedList<T, POLICY> *>( this );
    return non_const_SLL->back();
  }



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Modifiers
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // clear()
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::clear()
  { while( !empty() ) pop_front(); }



  // push_front()
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::push_front( const T & data )
  {
    // Since a singly linked list cannot look backwards, we can only insert after a given node.  So how can we insert a node before
    // the first one?  Use a special value, beforeBegin() in this case, that says insert at the front of the list.
    insert_after( before_begin(), data );
  }



  // push_back()
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::push_back( const T & data )
  { insert_after( self->_tail, data );}



  // pop_front()
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::pop_front()
  {
    // Since a singly linked list cannot look backwards, we can only remove after a given node.  So how can we remove the first one?
    // Use a special value, beforeBegin() in this case, that says remove the node at the front of the list.
    erase_after( before_begin() );
  }



  // insert_after()
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::iterator SinglyLinkedList<T, POLICY>::insert_after( const_iterator position, T const & data )
  { return self->insert_after( position._nodePtr, data ); }                           // dispatch to policy specific implementation



  // erase_after()
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::iterator SinglyLinkedList<T, POLICY>::erase_after( const_iterator position )
  { return self->erase_after( position._nodePtr ); }                                 // dispatch to policy specific implementation



  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Relational Operators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // operator<=>
  template <typename T, SllPolicy POLICY>
  std::weak_ordering SinglyLinkedList<T, POLICY>::operator<=>( SinglyLinkedList const & rhs ) const
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
  template <typename T, SllPolicy POLICY>
  bool SinglyLinkedList<T, POLICY>::operator==( SinglyLinkedList const & rhs ) const
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
  template <typename T, SllPolicy POLICY>
  void swap( SinglyLinkedList<T, POLICY> & lhs, SinglyLinkedList<T, POLICY> & rhs )
  {
    using std::swap;                                                                  // Let argument dependent lookup (ADL) find the right swap function
    swap( lhs.self, rhs.self );                                                       // and avoid a explicit call to std::swap()
  }






  /*********************************************************************************************************************************
  **********************************************************************************************************************************
  ** SinglyLinkedList<>::iterator Member Function Definitions
  **
  *********************************************************************************************************************************/
  // Default constructor - needed to support std::ranges::range concept, but should never be called.
  //
  // Let's move the error detection to a linker error instead of a run time error by not providing an definition
  #if 0
    template <typename T, SllPolicy POLICY>   template<typename U>
    SinglyLinkedList<T, POLICY>::Iterator_type<U>::Iterator_type()
    {
      throw std::logic_error( exceptionString( "CSUF::CPSC131::SinglyLinkedList<T, POLICY> default constructed (aka Sentinel) iterators not supported" ) );
    }
  #endif



  // Copy constructor when U is non-const iterator, Conversion constructor from non-const to const iterator when U is a const iterator
  // Type of parameter is intentionally a non-constant iterator
  template <typename T, SllPolicy POLICY>   template<typename U>
  SinglyLinkedList<T, POLICY>::Iterator_type<U>::Iterator_type( iterator const & other )      // Notice the parameter type is "iterator", not "Iterator_type"
    : _nodePtr{ other._nodePtr }
  {}



  // Conversion Constructor from pointer-to-Node to iterator
  template <typename T, SllPolicy POLICY>   template<typename U>
  SinglyLinkedList<T, POLICY>::Iterator_type<U>::Iterator_type( Node * p )
    : _nodePtr{ p }
  {}



  // operator++   pre-increment
  template <typename T, SllPolicy POLICY>   template<typename U>
  typename SinglyLinkedList<T, POLICY>::template Iterator_type<U> &   SinglyLinkedList<T, POLICY>::Iterator_type<U>::operator++()
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( exceptionString( "Attempt to increment null iterator" ) );

    _nodePtr = _nodePtr->_next;
    return *this;
  }



  // operator++   post-increment
  template <typename T, SllPolicy POLICY>   template<typename U>
  typename SinglyLinkedList<T, POLICY>::template Iterator_type<U>   SinglyLinkedList<T, POLICY>::Iterator_type<U>::operator++( int )
  {
    auto temp{ *this };
    operator++();                                                                     // Delegate to pre-increment leveraging error checking
    return temp;
  }



  // operator*
  template <typename T, SllPolicy POLICY>   template<typename U>
  typename SinglyLinkedList<T, POLICY>::template Iterator_type<U>::reference   SinglyLinkedList<T, POLICY>::Iterator_type<U>::operator*() const
  {
    if( _nodePtr == nullptr )   throw std::invalid_argument( exceptionString( "Attempt to dereference null iterator" ) );

    return _nodePtr->_data;
  }



  // operator->
  template <typename T, SllPolicy POLICY>   template<typename U>
  typename SinglyLinkedList<T, POLICY>::template Iterator_type<U>::pointer   SinglyLinkedList<T, POLICY>::Iterator_type<U>::operator->() const
  {
    if( _nodePtr == nullptr )  throw std::invalid_argument( exceptionString( "Attempt to dereference null iterator" ) );

    return &(_nodePtr->_data);
  }



  // operator==
  template <typename T, SllPolicy POLICY>   template<typename U>
  bool SinglyLinkedList<T, POLICY>::Iterator_type<U>::operator==( Iterator_type const & rhs ) const
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
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::reverse()
  {
    auto & head = self->_head;                                                        // an easier to read alias for the head of the list
    auto & tail = self->_tail;                                                        // an easier to read alias for the tail of the list

    reverse( head );                                                                  // Kick off the recursion starting at the head. Note the call to overloaded, private reverse function
    std::swap( head, tail );                                                          // Now swap the head and tail pointers
  }



  // The private helper function
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::reverse( Node * current )
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
  template <typename T, SllPolicy POLICY>
  T SinglyLinkedList<T, POLICY>::add() const
  {
    return add( self->_head );                                                        // Kick off the recursion starting at the head. Note the call to overloaded, private add function
  }



  // The private helper function
  template <typename T, SllPolicy POLICY>
  T SinglyLinkedList<T, POLICY>::add( Node * current ) const
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
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::iterator SinglyLinkedList<T, POLICY>::find( const T & data )
  {
    return find( self->_head, data );                                                 // Kick off the recursion starting at the head. Note the call to the overloaded, private find function
  }



  // The private helper function
  template <typename T, SllPolicy POLICY>
  typename SinglyLinkedList<T, POLICY>::iterator SinglyLinkedList<T, POLICY>::find( Node * current, const T & data )
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
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::forwardPrint( std::ostream & stream ) const
  {
    forwardPrint( self->_head, stream );                                              // Kick off the recursion starting at the head. Note the call to the overloaded, private forwardPrint function
  }



  // The private helper function
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::forwardPrint( Node * current, std::ostream & stream ) const
  {
    // Base Case - reached the end of the list
    if( end() == current )   return;

    // Visit - print the contents
    stream << current->_data;
    if( end() != current->_next )   stream << ", ";

    // Recurse - print a smaller list
    forwardPrint( current->_next, stream );
  }



  ///////////////////  Backward Print  ///////////////////////
  // Client facing public function
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::backwardPrint( std::ostream & stream ) const
  {
    backwardPrint( self->_head, stream );                                             // Kick off the recursion starting at the head. Note the call to the overloaded, private backwardPrint function
  }



  // The private helper function
  template <typename T, SllPolicy POLICY>
  void SinglyLinkedList<T, POLICY>::backwardPrint( Node * current, std::ostream & stream ) const
  {
    // Base Case - reached the end of the list
    if( end() == current )   return;

    // Recurse - print a smaller list
    backwardPrint( current->_next, stream );

    // Visit - print the contents
    if( end() != current->_next )   stream << ", ";
    stream << current->_data;
  }
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
