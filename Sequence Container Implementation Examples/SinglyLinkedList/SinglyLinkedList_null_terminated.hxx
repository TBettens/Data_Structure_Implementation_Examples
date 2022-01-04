/***********************************************************************************************************************************
** Class SinglyLinkedList - a very basic example implementation of the Singly Linked List Abstract Data Type
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
#pragma once

#include <cstddef>                                                            // size_t
#include <memory>                                                             // unique_ptr, make_unique()
#include <stdexcept>                                                          // length_error, invalid_argument

// A client may #include this file to explicitly select this null-terminated implementation, or the client may instead
// directly #include the *.hpp file to use a defaulted implementation
#define CSUF_CPSC131_SLL_SELECTION Null-terminated
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
    Node * _next = nullptr;                                                   // next item in the list
  };









  /*****************************************************************************
  ** A singly linked list's private data implementation
  *****************************************************************************/
  template<typename T>
  struct SinglyLinkedList<T>::PrivateMembers
  {
    Node *      _head = nullptr;                                              // first element in the list
    Node *      _tail = nullptr;                                              // last element in the list.  tail->next is always nullptr
    std::size_t _size = 0;                                                    // number of elements in the collection
  };









  /*******************************************************************************
  ** SinglyLinkedList<> Member Function Definitions
  *******************************************************************************/

  // insert_after()
  template <typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::insert_after( const_iterator position, T const & data )
  {
    auto & head = self->_head;                                                // an easier to read alias for the head of the list
    auto & tail = self->_tail;                                                // an easier to read alias for the tail of the list
    auto & size = self->_size;                                                // an easier to read alias for the size of the list

    Node * newNode     = new Node( data );                                    // Create and populate a new node with the provided data
    Node * currentNode = position._nodePtr;                                   // Convert iterator to pointer-to-Node

    // Special Case 1:  Inserting into an empty list?
    if( empty() )   head = tail = newNode;                                    // Both _head and _tail now point to the same, one and only node in the list


    // Special Case 2:  Inserting at the front of the list?
    else if( position == before_begin() )
    {
      // Add the new node to the front and then move the list's head
      newNode->_next = head;                                                  // newNode->_next and head now point to the same node
      head           = newNode;                                               // the new node becomes the head of the list
    }


    // Special Case 3:  Inserting at the back of the list?
    else if( currentNode == tail )
    {
      // Add the new node to the back and then move the list's tail
      tail->_next = newNode;                                                  // the new node now follows the tail node
      tail        = newNode;                                                  // the new node becomes the new tail of the list
    }


    // Normal Case:  Inserting somewhere in the middle of the list.
    else                                                                      // Insert in the middle of the list
    {
      // Relink the node into position
      newNode    ->_next = currentNode->_next;                                // the new node and the current node point to the same next node
      currentNode->_next = newNode;                                           // the current node now points to the new node
    }

    ++size;
    return newNode;
  }




  // erase_after()
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::erase_after( const_iterator position )
  {
    // Error Case: Removing from an empty list?
    if( empty() )   throw std::length_error( "attempt to remove from an empty list" );


    auto & head        = self->_head;                                         // an easier to read alias for the head of the list
    auto & tail        = self->_tail;                                         // an easier to read alias for the tail of the list
    auto & size        = self->_size;                                         // an easier to read alias for the size of the list
    Node * currentNode = position._nodePtr;                                   // Convert iterator to pointer-to-Node

    // Special Case 1:  Attempting to remove after the last node?             // There is no node after the tail - logic error?
    if( currentNode == tail )    return end();                                // removing after the tail intentionally does nothing



    Node * toBeRemoved = nullptr;                                             // toBeRemoved keeps a handle to the node taken out of the chain

    // Special Case 2:  Removing the node at the front of the list?
    if( position == before_begin() )                                          // remove the node at the head of the list
    {
      toBeRemoved = head;                                                     // keep a handle to the old head
      head        = head->_next;                                              // the second (next) node becomes the new head, or the head becomes nullptr if the only remaining node is being removed
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

      toBeRemoved        =currentNode->_next;                                 // toBeRemoved points to the node to taken out of the chain
      currentNode->_next =currentNode->_next->_next;                          // take the node out of the chain
    }

    --size;


    // Special Case 3: removing the back node in the list?
    //
    // Note: Don't dereference currentNode here, it may be null (e.g., before_begin())
    //       if( position()->next == end() ) ...
    if( toBeRemoved == tail )    tail = currentNode;                          // adjust tail to point to the new back node in the list

    iterator returnNode( toBeRemoved->_next );                                // return the node after the one removed
    delete toBeRemoved;                                                       // delete what used to be the old node
    return returnNode;
  }




  // before_begin()
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::before_begin()
  { return nullptr; }




  // end()
  template<typename T>
  typename SinglyLinkedList<T>::iterator SinglyLinkedList<T>::end()
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
