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
#pragma once

#include <cstddef>                                                            // size_t
#include <stdexcept>                                                          // length_error, invalid_argument

// A client may #include this file to explicitly select this circular with one dummy node implementation, or the client may instead
// directly #include the *.hpp file to use a defaulted implementation
#define CSUF_CPSC131_DLL_SELECTION Circular with one dummy node
#include "DoublyLinkedList.hpp"






/*******************************************************************************
** DoublyLinkedList with one dummy node function definitions
*******************************************************************************/
namespace CSUF::CPSC131
{
  /*****************************************************************************
  ** A doubly linked list's node
  *****************************************************************************/
  template<typename T>
  struct DoublyLinkedList<T>::Node
  {
    Node() = default;
    Node( T value ) : _data( std::move(value) ) {}

    T      _data = T{};                                                         // linked list element value
    Node * _next = this;                                                        // next item in the list
    Node * _prev = this;                                                        // previous item in the list
  };









  /*****************************************************************************
  ** A doubly linked list's private data implementation
  *****************************************************************************/
  template<typename T>
  struct DoublyLinkedList<T>::PrivateMembers                                    // A specific implementation's private members (attributes, functions, etc)
  {
    Node        _sentinel;                                                      // dummy node.  The first node of the list (_head) is at *(_sentinel->next)
    Node *&     _head = _sentinel._next;                                        // An easier to read alias for the list's head, which is a pointer-to-Node
    Node *&     _tail = _sentinel._prev;                                        // last element in the list.  tail->next always points to _sentinel
    std::size_t _size = 0;                                                      // number of elements in the collection
  };









  // insert()
  template<typename T>
  typename DoublyLinkedList<T>::iterator DoublyLinkedList<T>::insert( const_iterator position, T const & value )
  {
    Node * newNode     = new Node( value );                                     // create and populate a new node
    Node * currentNode = position._nodePtr;                                     // Convert iterator to pointer-to-Node

    newNode->_next = currentNode;
    newNode->_prev = currentNode->_prev;

    currentNode->_prev->_next = newNode;
    currentNode->_prev        = newNode;

    ++self->_size;
    return newNode;
  }



  // erase()
  template<typename T>
  typename DoublyLinkedList<T>::iterator DoublyLinkedList<T>::erase( const_iterator position )
  {
    if( empty()           )   throw std::length_error    ( "Attempt to erase from an empty list"     );
    if( position == end() )   throw std::invalid_argument( "Attempt to erase at an invalid location" );

    Node * currentNode = position._nodePtr;                                     // Convert iterator to pointer-to-Node

    currentNode->_next->_prev = currentNode->_prev;
    currentNode->_prev->_next = currentNode->_next;

    --self->_size;

    iterator returnNode( currentNode->_next );                                  // return the node after the one removed
    delete currentNode;                                                         // delete what used to be the old node
    return returnNode;
  }



  // end()
  template<typename T>
  typename DoublyLinkedList<T>::iterator DoublyLinkedList<T>::end()
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
