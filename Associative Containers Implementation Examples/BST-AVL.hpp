/***********************************************************************************************************************************
** Class BinarySearchTree - a very basic example implementation of the Binary Search Tree Abstract Data Type
**
**  The ADT's interface is a small subset of std::map defined at https://en.cppreference.com/w/cpp/container/map The tree does not
**  contain duplicate keys
**
**  Major differences between this example and std::map. Unlike std:map:
**  1)  end() and rend() cannot be decremented
***********************************************************************************************************************************/
#pragma once
#include <algorithm>                                                                  // max(), swap()
#include <cmath>                                                                      // abs()
#include <compare>                                                                    // week_ordering, strong_quality
#include <cstddef>                                                                    // size_t, ptrdiff_t
#include <initializer_list>                                                           // initializer_list
#include <iostream>
#include <iterator>                                                                   // next(), bidirectional_iterator_tag
#include <stdexcept>                                                                  // out_of_range, length_error
#include <utility>                                                                    // pair, move()





/*******************************************************************************
**  Binary Search Tree Abstract Data Type Definition (Duplicate keys allowed)
**
*******************************************************************************/
namespace CSUF::CPSC131
{
  // Template Class Definition
  template <typename Key, typename Value>
  class BinarySearchTree
  {
    template <typename T, typename U>
    friend void swap( BinarySearchTree<T, U> & lhs, BinarySearchTree<T, U> & rhs );   // The expected way to make a program-defined type swappable is to provide a
                                                                                      // non-member function swap in the same namespace as the type.
                                                                                      // (https://en.cppreference.com/w/cpp/algorithm/swap)

    private:
      // Types
      template <typename U> class  Iterator_type;                                     // Template class for iterator and const_iterator classes



    public:
      // Types
      using KeyValue_Pair  = std::pair     <Key const, Value   >;                     // An alias to the type of data held in the tree
      using iterator       = Iterator_type <KeyValue_Pair      >;                     // A bi-directional iterator to a read-write value in the tree
      using const_iterator = Iterator_type <KeyValue_Pair const>;                     // A bi-directional iterator to a read-only value in the tree


      // Constructors, destructor, and assignments
      BinarySearchTree(                                                   );          // Default constructor, creates an empty tree
      BinarySearchTree( BinarySearchTree const                & original  );          // Copy constructor, performs a deep copy
      BinarySearchTree( BinarySearchTree                     && original  ) noexcept; // Move constructor, takes ownership of the other tree
      BinarySearchTree( std::initializer_list<KeyValue_Pair>    init_list );          // initialization list constructor
     ~BinarySearchTree(                                                   ) noexcept; // Destructor, performs a deep Key-Value pair destruction

      BinarySearchTree & operator=( BinarySearchTree const  & rhs );                  // Copy assignment, performs a deep copy
      BinarySearchTree & operator=( BinarySearchTree       && rhs ) noexcept;         // Move assignment, takes ownership of the other tree

      // Queries
      std::size_t size    (                 ) const noexcept;                         // Returns the number of elements in the tree
      bool        empty   (                 ) const noexcept;                         // Returns true if the tree contains no elements, false otherwise
      bool        contains( Key const & key ) const;                                  // Returns true if there is such an element, false otherwise


      // Iterators
      iterator       begin ();                                                        // Returns a read-write iterator to the tree's first (least) element, end() if tree is empty
      iterator       end   ();                                                        // Returns a read-write iterator beyond the tree's last (greatest) element.  Do not dereference this Iterator

      const_iterator begin () const;                                                  // Returns a read-only iterator to the tree's first (least) element, end() if tree is empty
      const_iterator end   () const;                                                  // Returns a read-only iterator beyond the tree's last (greatest) element.  Do not dereference this Iterator

      const_iterator cbegin() const;                                                  // Returns a read-only iterator to the tree's first (least) element, end() if tree is empty
      const_iterator cend  () const;                                                  // Returns a read-only iterator beyond the tree's last (greatest) element.  Do not dereference this Iterator


      // Accessors
      Value const    & at        ( Key const & key ) const;                           // Returns the value associated with given key.  Throws std::out_of_range if key not found
      Value          & at        ( Key const & key );                                 // Returns the value associated with given key.  Throws std::out_of_range if key not found
      Value          & operator[]( Key const & key );                                 // Returns the value associated with given key, performing an insertion if such key does not already exist.
      const_iterator   find      ( Key const & key ) const;                           // Returns a read-only iterator to the key/value pair associated with the key, end() if key not found
      iterator         find      ( Key const & key );                                 // Returns a read-write iterator to the key/value pair associated with the key, end() if key not found


      // Modifiers
      std::pair<iterator, bool> insert( KeyValue_Pair const & pair     );             // Inserts a key-value pair into the container, if the container doesn't already contain an element with an equivalent key.
      std::size_t               erase ( Key           const & key      );             // Removes the matching node and returns the number of elements removed (0 or 1)
      iterator                  erase ( const_iterator        position );             // Removes the pointed-to node and returns the iterator following the removed element
      void                      clear (                                );             // Returns the tree to an empty state releasing all nodes


      // Relational Operators
      std::weak_ordering operator<=>( BinarySearchTree const & rhs ) const;
      bool               operator== ( BinarySearchTree const & rhs ) const;



    private:
      // Private Types
      struct Node;

      // Member instance attributes
      Node *      _root = nullptr;
      std::size_t _size = 0;

      // Helper functions
      bool   isBalanced  ( Node * current       ) const;
      Node * predecessor ( Node * current       ) const;
      Node * successor   ( Node * current       ) const;
      Node * reBalance   ( Node * offendingNode );
      Node * makeCopy    ( Node * current       );
      void   updateHeight( Node * current       );
      void   clear       ( Node * current       );




    /***************************************************************************
    ** Extended the BST Implementation Example with some additional examples of operations solved recursively.
    **
    ** These are not in a typical BST interface, but included here for educational discussion.  These types of exercises are often
    ** found in homework or exams.
    ***************************************************************************/
    public:
      // Client visible interface (the public function)
      long long int getHeight   () const;                                             // Returns the height of the tree, or -1 if tree is empty
      void          printInorder() const;                                             // Prints the contents of the tree in ascending sorted order
      Value         getMaxValue () const;                                             // more recursive algorithm examples
      Value         getSum      () const;


    private:
      // Private extended interface helper functions to demonstrate recursion
      long long int getHeight   ( Node * current ) const;
      void          printInorder( Node * current ) const;                             // valid only if type Value defines operator<<
      Value         getMaxValue ( Node * current ) const;                             // valid only if type Value defines operator<
      Value         getSum      ( Node * current ) const;                             // Valid only if type Value defines operator+
  };  // class BinarySearchTree










  /*******************************************************************************
  ** Class BinarySearchTree<Key, Value>::Iterator - A bi-directional iterator
  **
 *******************************************************************************/
  template<typename Key, typename Value>  template<typename U>
  class BinarySearchTree<Key, Value>::Iterator_type
  {
    friend class BinarySearchTree<Key, Value>;

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
      Iterator_type( iterator const & other ) noexcept;                               // Copy constructor when U is non-const, Conversion constructor from non-const to const iterator when U is const
                                                                                      // Note parameter type is intentionally "iterator", not "Iterator_type"
      // Pre and post Increment operators move the position to the next node in the list
      Iterator_type & operator++();                                                   // advance the iterator one node in-order (pre -increment)
      Iterator_type   operator++( int );                                              // advance the iterator one node in-order (post-increment)

      // Pre and post Decrement operators move the position to the previous node in the list
      Iterator_type & operator--();                                                   // retreat the iterator one node in-order (pre -decrement)
      Iterator_type   operator--( int );                                              // retreat the iterator one node in-order (post-decrement)

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
  };  // BinarySearchTree<U>::Iterator_type

}    // namespace CSUF::CPSC131















/***********************************************************************************************************************************
************************************************************************************************************************************
** Private Implementation
**
** Separating Interface from Implementation is an extremely important concept I hope students will come to appreciate.
************************************************************************************************************************************
***********************************************************************************************************************************/
/************************************************************************************************************************************************************************************************************
** Class BinarySearchTree - a very basic example implementation of the Binary Search Tree Abstract Data Type
**
**  Empty (size = 0):
**
**                   nullptr    nullptr     nullptr
**                      ^          ^           ^
**                      |          |           |
**                    root       begin()     end()
**
**  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
**
**   size = 3:
**                    EXAMPLE 1                                    |||                                EXAMPLE 2              |||                               EXAMPLE 3                           |||
**                                                                 |||                                                       |||                                                                   |||
**            +--------------------+                               |||                         +--------------------+        |||                        +--------------------+                     |||
**            |       parent       |                               |||                         |       parent       |        |||                        |       parent       |                     |||
**    root -> +------+------+------+                               |||                 root->  +------+------+------+        |||                 root-> +------+------+------+                     |||
**  begin()-> | left | data | right|                               |||                         | left | data | right|        |||                        | left | data | right|                     |||
**            +------+------+------+                               |||                         +------+------+------+        |||                        +------+------+------+                     |||
**               ^              ^                                  |||                            ^              ^           |||                           ^              ^                        |||
**              /                \                                 |||                           /                \          |||                          /                \                       |||
**             v                  v                                |||                          v                  v         |||                         v                  v                      |||
**        nullptr          +--------------------+                  |||             +--------------------+        nullptr     |||            +--------------------+    +--------------------+       |||
**                         |       parent       |                  |||             |       parent       |           ^        |||            |       parent       |    |       parent       |       |||
**                         +------+------+------+                  |||   begin()-> +------+------+------+           |        |||  begin()-> +------+------+------+    +------+------+------+       |||
**                         | left | data | right|                  |||             | left | data | right|         end()      |||            | left | data | right|    | left | data | right|       |||
**                         +------+------+------+                  |||             +------+------+------+                    |||            +------+------+------+    +------+------+------+       |||
**                            ^              ^                     |||                ^              ^                       |||               ^              ^            ^            ^          |||
**                           /                \                    |||               /                \                      |||              /                \          /              \         |||
**                          v                  v                   |||              v                  v                     |||             v                  v        v                v        |||
**                     nullptr          +--------------------+     |||         nullptr          +--------------------+       |||         nullptr              nullptr nullptr            nullptr   |||
**                                      |       parent       |     |||                          |       parent       |       |||                                                            ^      |||
**                                      +------+------+------+     |||                          +------+------+------+       |||                                                            |      |||
**                                      | left | data | right|     |||                          | left | data | right|       |||                                                          end()    |||
**                                      +------+------+------+     |||                          +------+------+------+       |||                                                                   |||
**                                         ^              ^        |||                             ^              ^          |||                                                                   |||
**                                        /                \       |||                            /                \         |||                                                                   |||
**                                       v                  v      |||                           v                  v        |||                                                                   |||
**                                  nullptr               nullptr  |||                      nullptr               nullptr    |||                                                                   |||
**                                                           ^     |||                                                       |||                                                                   |||
**                                                           |     |||                                                       |||                                                                   |||
**                                                         end()   |||                                                       |||                                                                   |||
**
************************************************************************************************************************************************************************************************************/

namespace CSUF::CPSC131
{
  /*****************************************************************************
  ** A Binary Search Tree node
  *****************************************************************************/
  template<typename Key, typename Value>
  struct BinarySearchTree<Key, Value>::Node
  {
    // Constructors  (Compiler synthesized copy and move constructors and assignments are fine)
    Node() = default;
    Node( KeyValue_Pair const & pair ) : _pair{ pair } {}

    // Node's Payload (content)
    KeyValue_Pair _pair = { Key{}, Value{} };

    // Adjacent node pointers
    Node *      _left   = nullptr;
    Node *      _right  = nullptr;
    Node *      _parent = nullptr;

    // Height of subtree rooted at this node.
    std::size_t _height = 0;                                                          // Added attribute used only for AVL trees

    // Convenience function used mainly as a tool to help create more readable code.  For some pointer-to-Node p, p->key() and
    // p->_pair.first can be used interchangeably but p->key() conveys more information to the reader.
    Key const & key  () { return _pair.first;  }
    Value     & value() { return _pair.second; }
  };









  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Constructors, destructor, and assignments
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // Default constructor
  template<typename Key, typename Value>
  BinarySearchTree<Key, Value>::BinarySearchTree() = default;




  // Copy constructor
  template<typename Key, typename Value>
  BinarySearchTree<Key, Value>::BinarySearchTree( const BinarySearchTree & original )
    : _root( makeCopy( original._root ) ), _size{ original._size }                    // performs a deep copy, recursively
  {}




  // Move constructor
  template<typename Key, typename Value>
  BinarySearchTree<Key, Value>::BinarySearchTree( BinarySearchTree && original ) noexcept
    : _root{ original._root }, _size{ original._size }                                // performs a shallow copy (takes ownership of the original tree)
  {
    original._root = nullptr;                                                         // set the original to an empty tree
    original._size = 0;
  }




  // Initialization list constructor
  template<typename Key, typename Value>
  BinarySearchTree<Key, Value>::BinarySearchTree( std::initializer_list<KeyValue_Pair> init_list )
    : BinarySearchTree()                                                              // delegate construction of an empty tree
  {
    for( auto && keyValue : init_list ) insert( keyValue );
  }




  // Destructor
  template<typename Key, typename Value>
  BinarySearchTree<Key, Value>::~BinarySearchTree() noexcept
  { clear(); }




  // Copy assignment
  template<typename Key, typename Value>
  BinarySearchTree<Key, Value> & BinarySearchTree<Key, Value>::operator=( BinarySearchTree const & rhs )
  {
    if( this != &rhs )    // self assignment guard
    {
      // to ensure consistent behavior and to implement the logic in one place, delegate to the copy constructor and then the move
      // assignment operator
      *this = BinarySearchTree{ rhs };                                                // Don't break this into two statements, or you may lose
    }                                                                                 // the rvalue and get into an infinite recursive loop

    return *this;
  }




  // Move assignment
  template<typename Key, typename Value>
  BinarySearchTree<Key, Value> & BinarySearchTree<Key, Value>::operator=( BinarySearchTree && rhs ) noexcept
  {
    if( this != &rhs )    // self assignment guard
    {
      clear();
      _root     = rhs._root;    // perform a shallow copy (takes ownership of the original tree)
      _size     = rhs._size;

      rhs._root = nullptr;    // set the original to an empty tree
      rhs._size = 0;
    }
    return *this;
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Queries
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // size()
  template<typename Key, typename Value>
  std::size_t BinarySearchTree<Key, Value>::size() const noexcept
  { return _size; }




  // empty()
  template<typename Key, typename Value>
  bool BinarySearchTree<Key, Value>::empty() const noexcept
  { return size() == 0; }




  // contains()
  template<typename Key, typename Value>
  bool BinarySearchTree<Key, Value>::contains( const Key & key ) const
  { return find( key ) != end(); }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Iterators - In-order traversal
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // begin()
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::begin()
  {
    if( _root == nullptr ) return end();

    auto current = _root;
    while( current->_left != nullptr ) current = current->_left;
    return current;
  }




  // end()
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::iterator BinarySearchTree<Key, Value>::end()
  { return nullptr; }




  // begin() const
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::const_iterator BinarySearchTree<Key, Value>::begin() const
  { return const_cast<BinarySearchTree &>( *this ).begin(); }                         // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // end() const
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::const_iterator BinarySearchTree<Key, Value>::end() const
  { return const_cast<BinarySearchTree &>( *this ).end(); }                           // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // cbegin() const
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::const_iterator BinarySearchTree<Key, Value>::cbegin() const
  { return const_cast<BinarySearchTree &>( *this ).begin(); }                         // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // cend() const
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::const_iterator BinarySearchTree<Key, Value>::cend() const
  { return const_cast<BinarySearchTree &>( *this ).end(); }                           // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Accessors
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // at() const
  template<typename Key, typename Value>
  const Value & BinarySearchTree<Key, Value>::at( const Key & key ) const
  { return const_cast<BinarySearchTree &>( *this ).at( key ); }                       // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // at()
  template<typename Key, typename Value>
  Value & BinarySearchTree<Key, Value>::at( const Key & key )
  {
    auto it = find( key );

    if( it == end() )   throw std::out_of_range( "Failure:  Attempted to access nonexistent element" );
    return it->second;
  }




  // operator[]
  template<typename Key, typename Value>
  Value & BinarySearchTree<Key, Value>::operator[]( const Key & key )
  {
    // Delegate to insert().  insert() will add a new {key, value} pair to the tree with a default constructed value if the key does
    // not exist and returns an iterator pointing to this new {key, value} pair. Otherwise, insert() locates the existing {key,
    // value} pair with a matching key and returns an iterator pointing to this existing {key, value} pair.   In either case
    // operator[] always return the value pointed to by insert()'s returned iterator.
    return insert( { key, Value{} } )                                                 // find the existing or insert a new {key, value} pair with a default constructed value, then
                                      .first                                          // use the returned iterator, which is in the "first" position of the returned {iterator, bool} pair, then
                                     ->second;                                        // return the value pointed to, which is in the "second" position of the KeyValue_Pair
  }




  // find() const
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::const_iterator    BinarySearchTree<Key, Value>::find( const Key & key ) const
  { return const_cast<BinarySearchTree &>( *this ).find( key ); }                     // to ensure consistent behavior and to implement the logic in one place, delegate to non-cost version




  // find()
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::iterator    BinarySearchTree<Key, Value>::find( const Key & key )
  {
    auto current = _root;

    while( current != nullptr )
    {
      auto comp = std::compare_weak_order_fallback( key, current->key() );            // uses operator== and operator< if operator<=> is unavailable
      if     ( comp == 0 )  return current;
      else if( comp  < 0 )  current = current->_left;
      else                  current = current->_right;
    }

    return end();
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Modifiers
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // insert()
  template<typename Key, typename Value>
  std::pair<typename BinarySearchTree<Key, Value>::iterator, bool>   BinarySearchTree<Key, Value>::insert( const KeyValue_Pair & pair )
  {
    Node *             current = _root;
    Node *             parent  = nullptr;
    std::weak_ordering comp    = std::weak_ordering::equivalent;


    // Work your way to the bottom of the tree ...
    while( current != nullptr )
    {                                                                                 // perform the comparison and remember the results.  if operator<=>() is not defined
      comp = std::compare_weak_order_fallback( pair.first, current->key() );          // for type Key, then fall back to using operator<() and operator==()

      if( comp == 0 ) return { current, false };                                      // duplicate key found;  return the node found and indicate nothing was added to the tree

      parent = current;                                                               // remember who my daddy is and then descend down to the child
      if( comp  < 0 ) current = current->_left ;                                      // if key to insert is less than the current key, go left
      else            current = current->_right;                                      //   otherwise, go right
    }

    // Insert the new node in place (i.e., as the root node, or the left or right child node)
    Node * newNode   = new Node( pair );                                              // construction of node initializes all its pointers to null (programming note: smart pointer opportunity here)
    newNode->_parent = parent;                                                        // let the child know who's its daddy

    if     ( parent == nullptr ) _root          = newNode;                            // let daddy (the parent) adopt the child
    else if( comp   <  0       ) parent->_left  = newNode;
    else                         parent->_right = newNode;

    ++_size;                                                                          // and of course increment the tree's size

    /************** AVL Tree Unique ************************************************************************************************
    ** Self balancing BSTs, like this AVL tree, balances the left and right subtrees as part of insertion.  Work your way from the
    ** node just inserted (which is at the bottom of the tree) all the way up to the tree's root updating height and looking for
    ** unbalanced subtrees
    **
    ** Balance Factor at this node calculated as Height(Left) - Height(Right)
    ** Note:  WikiBooks defines the balance factor to be Height(Right) - Height(Left)   https://en.wikipedia.org/wiki/AVL_tree
    **        VisuAlgo  defines the balance factor to be Height(Left)  - Height(Right)  https://visualgo.net/en/bst
    **        I tend to use Height(Left) - Height(Right) in classroom lectures, but it really doesn't matter. If abs( Height(Left) -
    **        Height(Right) ) >= 2, then it needs to be rebalanced
    *******************************************************************************************************************************/
    for( current = parent;  current != nullptr;  current = current->_parent)          // we know the node just inserted has no children, has height of 0, and is
    {                                                                                 // balanced, so we can start with it's parent and save looking at one node
      auto previousHeight = current->_height;
      updateHeight( current );

      if( !isBalanced( current ) )   current = reBalance( current );                  // if offending node found, let's fix it

      if( previousHeight == current->_height ) break;                                 // height has not changed, node insertion has been absorbed, and we can stop back tracing
    }
    /************** End AVL Tree Unique *******************************************************************************************/


    // return the added node and indicate something was added to the tree
    return { newNode, true };
  }







  // erase( key )
  template<typename Key, typename Value>
  std::size_t BinarySearchTree<Key, Value>::erase( const Key & key )                  // returns the number of nodes removed (0 or 1)
  {
    auto position = find( key );

    if( position == end() ) return 0;                                                 // if the key wasn't found, no nodes have been deleted

    erase( position );
    return 1;                                                                         // otherwise, one node (remember, no duplicates) has been removed
  }




  // erase( iterator )
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::iterator   BinarySearchTree<Key, Value>::erase( const_iterator position )
  {
    if( position == cend() ) return end();                                            // empty tree, do nothing

    Node * to_be_erased = position._nodePtr;                                          // convert the iterator to a pointer-to-Node for convenience

    if( to_be_erased->_left != nullptr  &&  to_be_erased->_right != nullptr )         // two children (programming note: a non-null pointer is "true")
    {
      Node * succ_ptr = successor( to_be_erased );
      // Move (don't copy) {key, value} pair from succ_ptr to to_be_erased
      //
      // Programming note:  The {key, value} pair's Key is constant to protect from (unintentionally ?) altering it and thus
      //   breaking the tree's key ordering invariant.  But in this specific case, we really do need to overwrite the key's content.
      //   So let's temporarily cast away its const-ness and assign the new content
      const_cast<Key &>( to_be_erased->key  () ) =  const_cast<Key &&>( succ_ptr->key  () );  // Programming note:  casting to a non-constant R-Value reference causes the move assignment operator to be called
                         to_be_erased->value()   =  std::move         ( succ_ptr->value() );
      erase( succ_ptr );
      return to_be_erased;                                                            // return the same node, but with updated content
    }

    else                                                                              // zero or one child
    {
      Node ** parent = to_be_erased->_parent        == nullptr      ? & _root         // "parent" is a pointer-to-pointer-to-Node; tells us what pointer to update
                   :   to_be_erased->_parent->_left == to_be_erased ? & to_be_erased->_parent->_left
                   :                                                  & to_be_erased->_parent->_right;

      Node * child = to_be_erased->_left != nullptr  ?  to_be_erased->_left  :  to_be_erased->_right;

      *parent = child;                                                                // remove me from the tree by making my parent point to my child
      if( child != nullptr )  child->_parent = to_be_erased->_parent;                 // and my child point to my parent


      /************** AVL Tree Unique **********************************************************************************************
      ** Self balancing BSTs, like this AVL tree, balances the left and right subtrees as part of erase.  Work your way from the
      ** node just erased (which is at the bottom of the tree) all the way up to the tree's root updating height and looking for
      ** unbalanced subtrees
      **
      ** Balance Factor at this node calculated as Height(Left) - Height(Right)
      ** Note:  WikiBooks defines the balance factor to be Height(Right) - Height(Left)   https://en.wikipedia.org/wiki/AVL_tree
      **        VisuAlgo  defines the balance factor to be Height(Left)  - Height(Right)  https://visualgo.net/en/bst
      **        I tend to use Height(Left) - Height(Right) in classroom lectures, but it really doesn't matter. If abs( Height(Left) -
      **        Height(Right) ) >= 2, then it needs to be rebalanced
      *****************************************************************************************************************************/
      for( auto current = to_be_erased->_parent;  current != nullptr;  current = current->_parent )
      {
        auto previousHeight = current->_height;
        updateHeight( current );

        if( !isBalanced( current ) )   current = reBalance( current );                // if offending node found, let's fix it

        if( previousHeight == current->_height ) break;                               // height has not changed, node removal has been absorbed, and we can stop back tracing
      }
      /************** End AVL Tree Unique ******************************************************************************************/

      iterator temp = std::next( position )._nodePtr;                                 // converts const_iterator to iterator
      delete to_be_erased;
      --_size;
      return temp;                                                                    // return the node after the one removed
    }
  }




  // clear() - public
  template<typename Key, typename Value>
  void BinarySearchTree<Key, Value>::clear()
  {
    clear( _root );
    _root = nullptr;
    _size = 0;
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Relational Operators
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // operator<=>
  template<typename Key, typename Value>
  std::weak_ordering BinarySearchTree<Key, Value>::operator<=>( BinarySearchTree const & rhs ) const
  {
    std::size_t i      = 0;
    std::size_t extent = size() < rhs.size()  ?  size()  :  rhs.size();               // min(size, rhs.size)

    for (auto p = begin(), q = rhs.begin();   i < extent;   ++i, ++p, ++q)
    {
      auto result = std::compare_weak_order_fallback( *p, *q );                       // uses operator== and operator< if operator<=> is unavailable
      if( result != 0 )   return result;                                              // compares both key and value
    }
    return size() <=> rhs.size();
  }




  // operator==
  template<typename Key, typename Value>
  bool BinarySearchTree<Key, Value>::operator==( BinarySearchTree const & rhs ) const
  {
    if( size() != rhs.size() ) return false;

    for (auto p = begin(), end = this->end(), q = rhs.begin();   p != end;   ++p, ++q)
    { if( *p != *q )   return false; }                                                // compares the pair (both key and value)

    return true;
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Private member functions
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // isBalanced()
  template<typename Key, typename Value>
  bool BinarySearchTree<Key, Value>::isBalanced( Node * p ) const
  {
    // Returns "true" if the tree rooted at "p" is balanced.  Calculate the balance factor of a node as
    //    balance factor = abs(height_left - height_right)
    // where the height of an empty tree is -1
    //
    // Definitions:
    //   1) An empty tree is balanced
    //   2) A node with no children is balanced
    //   3) A node with only one child and a height of 1 is balanced
    //
    // Assumptions:
    //   1) This node already has an accurate height
    //   2) The left and right subtrees already have an accurate height
    //
    // Programming note:  Be careful how you subtract the left and right subtree heights.  Height is an unsigned type and
    //                    subtracting a big number from a small number gives a really big positive number, not a negative number.
    if( p == nullptr ) return true;

    std::size_t balanceFactor = p->_height;                                                                         // A node without both children can only be 0, 1, or 2 high, otherwise it would have been rebalanced

    if( p->_left != nullptr  &&  p->_right != nullptr)                                                              // has two children
    {
      if( p->_right->_height  >  p->_left->_height )   balanceFactor = p->_right->_height  -  p->_left ->_height;   // right tree is taller
      else                                             balanceFactor = p->_left ->_height  -  p->_right->_height;   // left tree is taller (or the same height)
    }

    return balanceFactor <= 1;
  }




  // predecessor()
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::Node * BinarySearchTree<Key, Value>::predecessor( Node * current ) const
  {
    if( current == nullptr ) return nullptr;

    // Go left, then right, right, right, ... until you run out of tree
    current = current->_left;
    while( current->_right != nullptr ) current = current->_right;
    return current;
  }




  // successor()
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::Node * BinarySearchTree<Key, Value>::successor( Node * current ) const
  {
    if( current == nullptr ) return nullptr;

    // Go right, then left, left, left, ... until you run out of tree
    current = current->_right;
    while( current->_left != nullptr ) current = current->_left;
    return current;
  }




  // reBalance()
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::Node * BinarySearchTree<Key, Value>::reBalance( Node * offendingNode )
  {
    /***********************************************************************************************************************************
    **  Trinode Restructuring Each of the four patterns of out-of-balance nodes can be restructured to form the balanced pattern in the
    **  center.  Identify each of the nodes (a, b, c), their subtrees (T0, T1, T2, 73), and the parent (P).  Then tear apart the nodes
    **  and reassemble as shown.  The input parameter identifies the which node is out-of-balance, which is always the grandparent of
    **  the three nodes involved.
    **
    **             P                                                                                            P
    **             |                                                                                            |
    **           +-+-+                                                                                        +-+-+
    **          z| a |  (offendingNode)                                                                      z| c |  (offendingNode)
    **           +---+                                                                                        +---+
    **          /     \                                                                                      /     \
    **         T0      +---+                                                                            +---+      T3
    **                y| b |                                                                           y| b |
    **                 +---+                                                                            +---+
    **                /     \                                                                          /     \
    **               T1      +---+                                                                +---+      T2
    **                      x| c |                                                               x| a |
    **                       +---+                                                                +---+
    **                      /     \                              P                               /     \
    **                     T2     T3                             |                             T0      T1
    **                                    =======>>            +-+-+            <<=======
    **       (right-right)                                     | b |                                       (left-left)
    **                                                         +---+
    **                                                        /     \
    **                                                   +---+       +---+
    **                                                   | a |       | C |
    **             P              =======>>              +---+       +---+              <<=======               P
    **             |                                    /     \     /     \                                     |
    **           +-+-+                                 T0     T1   T2     T3                                  +-+-+
    **          z| a |  (offendingNode)                                                                      z| c |  (offendingNode)
    **           +---+                                                                                        +---+
    **          /     \                                                                                      /     \
    **         T0      +---+                                                                            +---+      T3
    **                y| c |                                                                           y| a |
    **                 +---+                                                                            +---+
    **                /     \                                                                          /     \
    **           +---+      T3                                                                        T0      +---+
    **          x| b |                                                                                       x| b |
    **           +---+                                                                                        +---+
    **          /     \                                                                                      /     \
    **         T1     T2                                                                                   T1      T2
    **
    **       (right-left)                                                                                  (left-right)
    **
    ***********************************************************************************************************************************/
    // Identify the three nodes involved in the restructure.  The first one is given as the offending node
    Node * z = offendingNode;                                                         // the node that is out of balance
    Node * y = nullptr;                                                               // the offendingNode's child with the tallest subtree
    Node * x = nullptr;                                                               // the offendingNode's grandchild with the tallest subtree

    // Identify the child with the tallest subtree
    if      ( z->_left  == nullptr )   y = z->_right;
    else if ( z->_right == nullptr )   y = z->_left;
    else                               y = (z->_left->_height < z->_right->_height)  ?  z->_right  :  z->_left;     // select the tallest

    // Identify the grandchild with the tallest subtree
    if      ( y->_left  == nullptr )   x = y->_right;
    else if ( y->_right == nullptr )   x = y->_left;
    else                               x = (y->_left->_height < y->_right->_height)  ?  y->_right  :  y->_left;     // select the tallest



    // Identify the ordering and subtrees such that "a" < "b" and "b" < "c".  "b" becomes the new subtree root with "a" as the left
    // child and "c" as a right child.  See picture above.
    Node * a  = nullptr;                                                              // b's left child after restructuring
    Node * b  = nullptr;                                                              // the new root of the restructure subtree
    Node * c  = nullptr;                                                              // b' right child
    Node * T0 = nullptr;                                                              // subtree to be reconnected in new structure
    Node * T1 = nullptr;                                                              // subtree to be reconnected in new structure
    Node * T2 = nullptr;                                                              // subtree to be reconnected in new structure
    Node * T3 = nullptr;                                                              // subtree to be reconnected in new structure

    if( z->_right == y )                                                              // right
    {
      if( y->_right == x )                                                            // right-right
      {
        a  = z;
        b  = y;
        c  = x;
        T0 = a->_left;
        T1 = b->_left;
        T2 = c->_left;
        T3 = c->_right;
      }
      else                                                                            // right-left
      {
        a  = z;
        b  = x;
        c  = y;
        T0 = a->_left;
        T1 = b->_left;
        T2 = b->_right;
        T3 = c->_right;
      }
    }
    else                                                                              // left
    {
      if(  y->_right == x )                                                           // left-right
      {
        a  = y;
        b  = x;
        c  = z;
        T0 = a->_left;
        T1 = b->_left;
        T2 = b->_right;
        T3 = c->_right;
      }
      else                                                                            // left-left
      {
        a  = x;
        b  = y;
        c  = z;
        T0 = a->_left;
        T1 = a->_right;
        T2 = b->_right;
        T3 = c->_right;
      }
    }


    // Now that all the parts have been identified, pull the tree apart and then put it back together like shown in the center of
    // the diagram.  This has the effect of a single rotation for the right-right and left-left patterns, and a double rotation for
    // the right-left and left-right patterns.
    b->_left   = a;
    b->_right  = c;
    b->_parent = offendingNode->_parent;

    if     ( b->_parent        == nullptr       )   _root              = b;           // the offending node won't have a parent if it's the tree's root
    else if( b->_parent->_left == offendingNode )   b->_parent->_left  = b;
    else                                            b->_parent->_right = b;

    a->_left   = T0;
    a->_right  = T1;
    a->_parent = b;
    if( a->_left  != nullptr )   a->_left ->_parent = a;
    if( a->_right != nullptr )   a->_right->_parent = a;


    c->_left   = T2;
    c->_right  = T3;
    c->_parent = b;
    if( c->_left  != nullptr )   c->_left ->_parent = c;
    if( c->_right != nullptr )   c->_right->_parent = c;


    // One last thing to do ... recalculate the height of the children.  Make sure to update the left child (a) and right child (c)
    // before the new subtree root (b)
    updateHeight( a );
    updateHeight( c );
    updateHeight( b );

    // Return the new subtree root
    return b;
  }




  // makeCopy() - private recursive helper
  template<typename Key, typename Value>
  typename BinarySearchTree<Key, Value>::Node * BinarySearchTree<Key, Value>::makeCopy( Node * current )
  {
    if( current == nullptr ) return nullptr;                                          // base case

    auto node     = new Node{ current->_pair };                                       // visit
    node->_height = current->_height;                                                 // topology is maintained, so copy vice recalculate height

    node->_left  = makeCopy( current->_left );                                        // recurse left
    node->_right = makeCopy( current->_right );                                       // recurse right

    if( node->_left  != nullptr ) node->_left->_parent  = node;                       // visit some more
    if( node->_right != nullptr ) node->_right->_parent = node;

    return node;
  }




  // updateHeight()
  template<typename Key, typename Value>
  void BinarySearchTree<Key, Value>::updateHeight( Node * p )
  {
    // Assumptions:
    //   1) The left and right subtrees already have an accurate height
    //
    if( p == nullptr ) return;

    // Set this node's height to one more than its tallest subtree
    if( p->_left != nullptr )
    {
      if( p->_right != nullptr )      p->_height = 1 + std::max( p->_left->_height, p->_right->_height );   // has two children
      else                            p->_height = 1 + p->_left->_height;                                   // has only left child
    }
    else if( p->_right != nullptr )   p->_height = 1 + p->_right->_height;                                  // has only right child
    else                              p->_height = 0;                                                       // no children
  }




  // clear() - private recursive helper
  template<typename Key, typename Value>
  void BinarySearchTree<Key, Value>::clear( Node * current )
  {
    if( current == nullptr ) return;                                                  // base case

    clear( current->_left );                                                          // recurse left
    clear( current->_right );                                                         // recurse right

    delete current;                                                                   // visit
    --_size;
  }












  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Non-member functions
  //
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // swap()
  template<typename Key, typename Value>
  void swap( BinarySearchTree<Key, Value> & lhs, BinarySearchTree<Key, Value> & rhs )
  {
    using std::swap;
    swap( lhs._root, rhs._root );
    swap( lhs._size, rhs._size );
  }












  /*********************************************************************************************************************************
  **********************************************************************************************************************************
  ** BinarySearchTree<>::iterator Member Function Definitions
  **
  *********************************************************************************************************************************/

  // Copy constructor when U is non-const iterator, Conversion constructor from non-const to const iterator when U is a const
  // iterator Type of parameter is intentionally a non-constant iterator
  template<typename Key, typename Value>  template<typename U>
  BinarySearchTree<Key, Value>::Iterator_type<U>::Iterator_type( iterator const & other ) noexcept    // Notice the parameter type is "iterator", not "Iterator_type"
    : _nodePtr{ other._nodePtr }
  {}




  // Conversion constructor
  template<typename Key, typename Value>  template<typename U>
  BinarySearchTree<Key, Value>::Iterator_type<U>::Iterator_type( Node * p ) noexcept
    : _nodePtr( p )
  {}




  // operator++ (pre-increment)
  template<typename Key, typename Value>  template<typename U>
  typename BinarySearchTree<Key, Value>::template Iterator_type<U> & BinarySearchTree<Key, Value>::Iterator_type<U>::operator++()    // pre-increment
  {
    if( _nodePtr == nullptr ) return *this;                                             // cannot increment past end(), should this be an error?


    // If there is a right subtree, move to this node's successor
    if (_nodePtr->_right != nullptr)
    {
      _nodePtr = _nodePtr->_right;
      while( _nodePtr->_left )   _nodePtr = _nodePtr->_left;
    }

    else
    {
      // unwind the stack
      while( _nodePtr->_parent  &&  _nodePtr->_parent->_right == _nodePtr )    _nodePtr = _nodePtr->_parent;
      _nodePtr = _nodePtr->_parent;
    }

    return *this;
  }




  // operator++   (post-increment)
  template<typename Key, typename Value>  template<typename U>
  typename BinarySearchTree<Key, Value>::template Iterator_type<U> BinarySearchTree<Key, Value>::Iterator_type<U>::operator++( int )    // post-increment
  {
    auto temp{ *this };                                                               // make a copy of the original iterator
    operator++();                                                                     // Delegate to pre-increment leveraging error checking
    return temp;                                                                      // return the copy
  }




  // operator--  (pre-decrement)
  template<typename Key, typename Value>  template<typename U>
  typename BinarySearchTree<Key, Value>::template Iterator_type<U> & BinarySearchTree<Key, Value>::Iterator_type<U>::operator--()    // pre-decrement
  {
    if( _nodePtr == nullptr ) return *this;                                             // decrement from end():  could return the right-most node, but the iterator, nor
                                                                                        // the underlying node, knows nothing about the tree or what the root of the tree is
    // If there is a left subtree, move to this node's predecessor
    if (_nodePtr->_left != nullptr)
    {
      _nodePtr = _nodePtr->_left;
      while( _nodePtr->_right )   _nodePtr = _nodePtr->_right;
    }

    else
    {
      // unwind the stack
      while( _nodePtr->_parent  &&  _nodePtr->_parent->_left == _nodePtr )    _nodePtr = _nodePtr->_parent;
      _nodePtr = _nodePtr->_parent;
    }

    return *this;
  }




  // operator--  (post-decrement)
  template<typename Key, typename Value>  template<typename U>
  typename BinarySearchTree<Key, Value>::template Iterator_type<U> BinarySearchTree<Key, Value>::Iterator_type<U>::operator--( int )    // post-decrement
  {
    auto temp{ *this };                                                               // make a copy of the original iterator
    operator--();                                                                     // Delegate to pre-decrement leveraging error checking
    return temp;                                                                      // return the copy
  }




  // operator*
  template<typename Key, typename Value>  template<typename U>
  typename BinarySearchTree<Key, Value>::template Iterator_type<U>::reference  BinarySearchTree<Key, Value>::Iterator_type<U>::operator*() const
  { return _nodePtr->_pair; }




  // operator->
  template<typename Key, typename Value>  template<typename U>
  typename BinarySearchTree<Key, Value>::template Iterator_type<U>::pointer  BinarySearchTree<Key, Value>::Iterator_type<U>::operator->() const
  { return &( _nodePtr->_pair ); }




  // operator==
  template<typename Key, typename Value>  template<typename U>
  bool BinarySearchTree<Key, Value>::Iterator_type<U>::operator==( const Iterator_type & rhs ) const
  { return _nodePtr == rhs._nodePtr; }












  /*********************************************************************************************************************************
  **********************************************************************************************************************************
  ** Extended the Binary Search Tree Private Members Implementation Example with some examples of operations solved recursively.
  **
  ** These are not in a typical Singly Linked List interface, but included here for educational discussion.  These types of
  ** exercises are often found in homework or exams.
  *********************************************************************************************************************************/

  ///////////////////  Get Height  ///////////////////////
  // Client facing public function
  template<typename Key, typename Value>
  long long int BinarySearchTree<Key, Value>::getHeight() const
  { return getHeight( _root ); }




  // The private helper function
  template<typename Key, typename Value>
  long long int BinarySearchTree<Key, Value>::getHeight( Node * current ) const
  {
    if( current == nullptr ) return -1;                                               // Base case

    auto leftHeight  = getHeight( current->_left );                                   // recurse left
    auto rightHeight = getHeight( current->_right );                                  // recurse right

    return 1 + std::max( leftHeight, rightHeight );                                   // visit
  }








  ///////////////////  Print Inorder  ///////////////////////
  // Client facing public function
  template<typename Key, typename Value>
  void BinarySearchTree<Key, Value>::printInorder() const
  { printInorder( _root ); }




  // The private helper function
  template<typename Key, typename Value>
  void BinarySearchTree<Key, Value>::printInorder( Node * current ) const
  {
    if( current == nullptr ) return;                                                             // Base case

    printInorder( current->_left );                                                              // Recurse left
    std::cout << "Key: \"" << current->key() << "\",  Value: \"" << current->value() << "\"\n";  // Visit
    printInorder( current->_right );                                                             // Recurse right
  }








  ///////////////////  Get Max Value  ///////////////////////
  // Client facing public function
  //
  // This one is slightly different in that we cannot assume a default constructed Value() is a member of the container and thus we
  // cannot consider this as a potential max value.  The solution is to identify the base case as a null left or null right subtree,
  // instead of a null tree as above. If the tree is empty, we must not return a value (whatever is returned may be interpreted by
  // the caller as "the" max value).
  template<typename Key, typename Value>
  Value BinarySearchTree<Key, Value>::getMaxValue() const
  {
    if( _root == nullptr ) throw std::length_error( "Oops!" );
    return getMaxValue( _root );
  }




  // The private helper function
  template<typename Key, typename Value>
  Value BinarySearchTree<Key, Value>::getMaxValue( Node * current ) const
  {
    // can assume current is not null - it's already been checked
    auto currentMaximum = current->value();

    //OPTION 1:  Use the standard max function
    {
      if( current->_left  != nullptr ) currentMaximum = std::max( currentMaximum, getMaxValue( current->_left ) );
      if( current->_right != nullptr ) currentMaximum = std::max( currentMaximum, getMaxValue( current->_right ) );
    }

    //OPTION 2:  Use the ternary operator
    {
      if( current->_left != nullptr )
      {
        auto maxLeftValue = getMaxValue( current->_left );                            // Save the answer and avoid traversing the subtree twice
        currentMaximum    = maxLeftValue > currentMaximum ? maxLeftValue : currentMaximum;
      }

      if( current->_right != nullptr )
      {
        auto maxRightValue = getMaxValue( current->_right );                          // Save the answer and avoid traversing the subtree twice
        currentMaximum     = maxRightValue > currentMaximum ? maxRightValue : currentMaximum;
      }
    }

    //OPTION 3:  Use normal if
    {
      if( current->_left != nullptr )
      {
        auto maxLeftValue = getMaxValue( current->_left );                            // Save the answer and avoid traversing the subtree twice
        if( maxLeftValue > currentMaximum ) currentMaximum = maxLeftValue;
      }

      if( current->_right != nullptr )
      {
        auto maxRightValue = getMaxValue( current->_right );                          // Save the answer and avoid traversing the subtree twice
        if( maxRightValue > currentMaximum ) currentMaximum = maxRightValue;
      }
    }

    // In all cases, return the max value found
    return currentMaximum;
  }








  ///////////////////  Get Sum  ///////////////////////
  // Client facing public function
  template<typename Key, typename Value>
  Value BinarySearchTree<Key, Value>::getSum() const
  { return getSum( _root ); }


  // The private helper function
  template<typename Key, typename Value>
  Value BinarySearchTree<Key, Value>::getSum( Node * current ) const
  {
    if( current == nullptr ) return Value();                                          // Base case
    return getSum( current->_left )                                                   // recurse left
           + current->value() +                                                       // Visit
           getSum( current->_right );                                                 // recurse right
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
** Last modified:  03-JAN-2022
** Last Verified:  03-JAN-2022
** Verified with:  MS Visual Studio 2019 Version 16.11.8 (C++20)
**                 GCC version 11.2.1 20211124 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
