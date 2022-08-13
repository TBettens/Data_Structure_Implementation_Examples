/***********************************************************************************************************************************
** Class SinglyLinkedList - a very basic example implementation of the Singly Linked List Abstract Data Type
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
#pragma once

#include <cstddef>                                                            // size_t
#include <stdexcept>                                                          // length_error, invalid_argument

// A client may #include this file to explicitly select this circular with one dummy node implementation, or the client may instead
// directly #include the *.hpp file to use a defaulted implementation
#define CSUF_CPSC131_SLL_SELECTION Circular with one dummy node
#include "SinglyLinkedList.hpp"





namespace CSUF::CPSC131
{
  /*****************************************************************************
  ** A singly linked list's node
  *****************************************************************************/
  template<typename T>
  struct SinglyLinkedList<T>::Node
  {
    Node() = default;
    Node( T value ) : _data{ std::move(value) } {}

    T      _data = T{};                                                       // linked list element value
    Node * _next = this;                                                      // next item in the list
  };









  /*****************************************************************************
  ** A singly linked list's private data implementation
  *****************************************************************************/
  template<typename T>
  struct SinglyLinkedList<T>::PrivateMembers
  {
    Node        _sentinel;                                                    // dummy node.  The first node of the list (_head) is at *(_sentine._next);
    Node *&     _head = _sentinel._next;                                      // _head is just another name for _sentinel._next.  The head of the list is always at this location
    Node *      _tail = &_sentinel;                                           // last element in the list.  tail->next always points to _sentinel
    std::size_t _size = 0;                                                    // number of elements in the collection
  };
  // Example usage:
  //  To access the node at the front of the list, use either of the equivalent methods:
  //      Node * front_node = self->_head;
  //      Node * front_node = self->_sentinel._next;
  //  To access the last node in the list, use either of the equivalent methods:
  //      Node * back_node = self->_tail;
  //      Node * back_node = &(self->_sentinel)









  /*******************************************************************************
  ** SinglyLinkedList<> Member Function Definitions
  *******************************************************************************/

  // insert_after()
  template <typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::insert_after( const_iterator position, T const & data )
  {
    auto & tail = self->_tail;                                                // an easier to read alias for the tail of the list
    auto & size = self->_size;                                                // an easier to read alias for the size of the list

    Node * newNode     = new Node( data );                                    // Create and populate a new node with the provided data
    Node * currentNode = position._nodePtr;                                   // Convert iterator to pointer-to-Node

    // Relink the node into position
    newNode    ->_next = currentNode->_next;                                  // the new node and the current node point to the same next node
    currentNode->_next = newNode;                                             // the current node now points to the new node

    // If necessary, move the tail
    if( end() == newNode->_next )   tail = newNode;

    ++size;
    return newNode;
  }




  // erase_after()
  template <typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::erase_after( const_iterator position )
  {
    // Error Case: Removing from an empty list?
    if( empty() )   throw std::length_error( "attempt to remove from an empty list" );

    auto & tail        = self->_tail;                                         // an easier to read alias for the tail of the list
    auto & size        = self->_size;                                         // an easier to read alias for the size of the list
    Node * currentNode = position._nodePtr;                                   // Convert iterator to pointer-to-Node

    // Special Case 1:  Removing after the last node?
    if( currentNode == tail )   return end();                                 // removing after the tail intentionally does nothing


    Node * toBeRemoved = currentNode->_next;                                  // toBeRemoved points to the node taken out of the chain
    currentNode->_next = currentNode->_next->_next;                           // take the node out of the chain

    --size;

    if( toBeRemoved == tail )    tail = currentNode;                          // adjust tail to point to the new back node in the list

    delete toBeRemoved;                                                       // delete what used to be the old node
    return currentNode->_next;
  }




  // before_begin()
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::before_begin()
  { return &self->_sentinel; }




  // end()
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::end()
  { return &self->_sentinel; }
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
** Last modified:  14-JUL-2021
** Last Verified:  03-JAN-2022
** Verified with:  MS Visual Studio 2019 Version 16.11.8 (C++20)
**                 GCC version 11.2.1 20211124 (-std=c++20 ),
**                 Clang version 13.0.0 (-std=c++20 -stdlib=libc++)
***************************************************/
