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
#pragma once

#include <cstddef>                                              // size_t
#include <stdexcept>                                            // length_error

// A client may #include this file to explicitly select this null-terminated implementation, or the client may instead
// directly #include the *.hpp file to use a defaulted implementation
#define CSUF_CPSC131_DLL_SELECTION Null-terminated
#include "DoublyLinkedList.hpp"




/*******************************************************************************
** Null Terminated DoublyLinkedList function definitions
*******************************************************************************/
namespace CSUF::CPSC131
{
  template<typename T>
  struct DoublyLinkedList<T>::Node
  {
    Node() = default;
    Node( const T value ) : _data( std::move(value) ) {}

    T      _data = T{};                                         // linked list element value
    Node * _next = nullptr;                                     // next item in the list
    Node * _prev = nullptr;                                     // previous item in the list
  };









  /*****************************************************************************
  ** A doubly linked list's private data implementation
  *****************************************************************************/
  template<typename T>
  struct DoublyLinkedList<T>::PrivateMembers               // A specific implementation's private members (attributes, functions, etc)
  {
    Node *      _head = nullptr;
    Node *      _tail = nullptr;
    std::size_t _size = 0;
  };










  // insert()
  template <typename T>
  typename DoublyLinkedList<T>::iterator DoublyLinkedList<T>::insert( const_iterator position, const T & element )
  {
    auto & head = self->_head;                                  // an easier to read alias for the head of the list
    auto & tail = self->_tail;                                  // an easier to read alias for the tail of the list
    auto & size = self->_size;                                  // an easier to read alias for the size of the list

    Node * newNode     = new Node( element );                   // Create and populate a new node with the provided data
    Node * currentNode = position._nodePtr;                     // Convert iterator to pointer-to-Node


    // Special Case 1:  Inserting into an empty list?
    if( empty() )   head = tail = newNode;                      // Both _head and _tail now point to the same, one and only node in the list


    // Special Case 2:  Inserting at the front of the list?
    else if( currentNode == head )                              // Insert at front of list
    {
      // Add the new node to the front and then move the list's head
      newNode->_next = head;                                    // newNode->_next and head now point to the same node
      head->_prev    = newNode;                                 // the head's previous node becomes the new node
      head           = newNode;                                 // the new node becomes the head of the list
    }


    // Special Case 3:  Inserting at the back of the list?
    else if( position == end() )                                // Insert at back of list
    {
      // Add the new node to the back and then move the list's tail
      tail->_next    = newNode;                                 // the new node now follows the tail node
      newNode->_prev = tail;                                    // the tail's previous and the new node's previous no point to the same node
      tail           = newNode;                                 // the new node becomes the new tail of the list
    }


    // Normal Case:  Inserting somewhere in the middle of the list.
    else                                                        // Insert in the middle of the list
    {
      newNode->_next = currentNode;                             // The new node's pointers is linked into place
      newNode->_prev = currentNode->_prev;

      currentNode->_prev->_next = newNode;                      // The new node becomes part of the chain
      currentNode->_prev        = newNode;
    }

    ++size;
    return newNode;
  }



  // erase()
  template <typename T>
  typename DoublyLinkedList<T>::iterator DoublyLinkedList<T>::erase( const_iterator position )
  {
    // Error Case: Removing from an empty list?
    if( empty() )   throw std::length_error( "attempt to erase from an empty list" );

    auto & head = self->_head;                                  // an easier to read alias for the head of the list
    auto & tail = self->_tail;                                  // an easier to read alias for the tail of the list
    auto & size = self->_size;                                  // an easier to read alias for the size of the list

    Node * currentNode = position._nodePtr;                     // Convert iterator to pointer-to-Node

    // Special Case 1:  Removing a node not in the list?
    if( position == end() )   throw std::length_error( "attempt to erase from past the end of list" );


    // Special Case 2:  Removing the node at the front of the list?
    if( currentNode == head )
    {
      // Special Case 3: removing the node at the back of the list?
      if( currentNode == tail )                                 // current node is both the head and the tail
      {
        head = tail = nullptr;
      }
      else                                                      // current node is the head but not the tail
      {
        head        = head->_next;
        head->_prev = nullptr;
      }
    }

    else if( currentNode == tail )                              // current node is the tail but not the head
    {
      tail        = tail->_prev;
      tail->_next = nullptr;
    }

    else                                                        // current node is between the head and the tail
    {
      currentNode->_next->_prev = currentNode->_prev;
      currentNode->_prev->_next = currentNode->_next;
    }

    --size;

    iterator returnNode( currentNode->_next );                  // return the node after the one removed
    delete currentNode;                                         // delete what used to be the old node
    return returnNode;
  }



  // end()
  template <typename T>
  typename DoublyLinkedList<T>::iterator DoublyLinkedList<T>::end()
  { return nullptr; }
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
