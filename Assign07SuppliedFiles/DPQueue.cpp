// FILE: DPQueue.cpp
// IMPLEMENTS: p_queue (see DPQueue.h for documentation.)
//
// INVARIANT for the p_queue class:
//   1. The number of items in the p_queue is stored in the member
//      variable used.
//   2. The items themselves are stored in a dynamic array (partially
//      filled in general) organized to follow the usual heap storage
//      rules.
//      2.1 The member variable heap stores the starting address
//          of the array (i.e., heap is the array's name). Thus,
//          the items in the p_queue are stored in the elements
//          heap[0] through heap[used - 1].
//      2.2 The member variable capacity stores the current size of
//          the dynamic array (i.e., capacity is the maximum number
//          of items the array currently can accommodate).
//          NOTE: The size of the dynamic array (thus capacity) can
//                be resized up or down where needed or appropriate
//                by calling resize(...).
// NOTE: Private helper functions are implemented at the bottom of
// this file along with their precondition/postcondition contracts.

#include <cassert>   // provides assert function
#include <iostream>  // provides cin, cout
#include <iomanip>   // provides setw
#include <cmath>     // provides log2
#include "DPQueue.h"

using namespace std;

namespace CS3358_FA2021_A7
{
   // EXTRA MEMBER FUNCTIONS FOR DEBUG PRINTING
   void p_queue::print_tree(const char message[], size_type i) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the portion of the heap with root at
   //       node i has been written to the screen. Each node's data
   //       is indented 4*d, where d is the depth of the node.
   //       NOTE: The default argument for message is the empty string,
   //             and the default argument for i is zero. For example,
   //             to print the entire tree of a p_queue p, with a
   //             message of "The tree:", you can call:
   //                p.print_tree("The tree:");
   //             This call uses the default argument i=0, which prints
   //             the whole tree.
   {
      const char NO_MESSAGE[] = "";
      size_type depth;

      if (message[0] != '\0')
         cout << message << endl;

      if (i >= used)
         cout << "(EMPTY)" << endl;
      else
      {
         depth = size_type( log( double(i+1) ) / log(2.0) + 0.1 );
         if (2*i + 2 < used)
            print_tree(NO_MESSAGE, 2*i + 2);
         cout << setw(depth*3) << "";
         cout << heap[i].data;
         cout << '(' << heap[i].priority << ')' << endl;
         if (2*i + 1 < used)
            print_tree(NO_MESSAGE, 2*i + 1);
      }
   }

   void p_queue::print_array(const char message[]) const
   // Pre:  (none)
   // Post: If the message is non-empty, it has first been written to
   //       cout. After that, the contents of the array representing
   //       the current heap has been written to cout in one line with
   //       values separated one from another with a space.
   //       NOTE: The default argument for message is the empty string.
   {
      if (message[0] != '\0')
         cout << message << endl;

      if (used == 0)
         cout << "(EMPTY)" << endl;
      else
         for (size_type i = 0; i < used; i++)
            cout << heap[i].data << ' ';
   }

   // CONSTRUCTORS AND DESTRUCTOR

   p_queue::p_queue(size_type initial_capacity) : capacity(initial_capacity), used(0)
   {
      if (initial_capacity < 1)
         capacity = DEFAULT_CAPACITY;
      heap = new ItemType[capacity];
   }

   p_queue::p_queue(const p_queue& src) : capacity(src.capacity), used(src.used)
   {
      // Copy Constructor
      heap = new ItemType[capacity];
      for (size_type i = 0; i < used; ++i)
         heap[i] = src.heap[i];
   }

   p_queue::~p_queue()
   {
      delete [] heap;
      heap = 0;
   }

   // MODIFICATION MEMBER FUNCTIONS
   p_queue& p_queue::operator=(const p_queue& rhs)
   {
      if (this != &rhs)
      {
         // If the priority queues are different build a new array
         // in new memory to assign to the invoking set
         ItemType* newHeap = new ItemType[rhs.capacity];
         for (size_type i = 0; i < rhs.used; ++i)
            newHeap[i] = rhs.heap[i];
         delete [] heap;
         heap = newHeap;
         capacity = rhs.capacity;
         used = rhs.used;
      }
      return *this;
   }

   void p_queue::push(const value_type& entry, size_type priority)
   {
      if (used == capacity)
         resize(size_type(1.5 * capacity) + 1);
      
      heap[used].priority = priority;
      heap[used].data = entry;
      size_type i = used;
      used++;

      // Check integrity of heap structure.
      // If the parent has lower priority,
      // swap the parent with the child
      // and heapify up to the correct location.
      while (i > 0 && parent_priority(i) < heap[i].priority)
      {
         swap_with_parent(i);
         i = parent_index(i);
      }

   }

   void p_queue::pop()
   {
      // Heap cannot be empty
      assert( size() > 0 );

      // Indices to keep track of children and parent
      size_type pIndex = 0, cIndex = 0;  // p = parent, c = child

      // Heap has only one element
      if (used == 1)
      {
         used--;
         return;
      }

      // 1. Copy last entry to root
      heap[0].priority = heap[used - 1].priority;
      heap[0].data = heap[used - 1].data;
      used--; // Removal of last item

      // 2. Swap parent with child while parent > child
      // If no child exists, do nothing
      // Perform swap with greatest child if two exist
      while (!is_leaf(pIndex) && heap[pIndex].priority <= big_child_priority(pIndex))
      {
         // heapify downwards
         cIndex = big_child_index(pIndex);            // Get index of greatest child
         swap_with_parent(big_child_index(pIndex));   // Swap greatest child with parent
         pIndex = cIndex;                             // Child is now the parent
      }


   }

   // CONSTANT MEMBER FUNCTIONS

   p_queue::size_type p_queue::size() const { return used; }

   bool p_queue::empty() const { return (used < 1); }

   p_queue::value_type p_queue::front() const
   // Pre:  size() > 0.
   // Post: The return value is the data of the highest priority
   //       item in the p_queue, but the p_queue is unchanged.
   //       (If several items have equal priority, then the
   //       implementation may decide which one to return.)
   {
      assert( size() > 0);
      return heap[0].data;
   }

   // PRIVATE HELPER FUNCTIONS
   void p_queue::resize(size_type new_capacity)
   // Pre:  (none)
   // Post: The size of the dynamic array pointed to by heap (thus
   //       the capacity of the p_queue) has been resized up or down
   //       to new_capacity, but never less than used (to prevent
   //       loss of existing data).
   //       NOTE: All existing items in the p_queue are preserved and
   //             used remains unchanged.
   {
      // Prevent loss of data
      if (new_capacity < used)
         new_capacity = used;
      
      // Capacity cannot be less than the default
      if (new_capacity < 1)
         new_capacity = DEFAULT_CAPACITY;

      // If the above is satisfied set capacity
      // then create a new array with the new capacity
      // and transfer data
      capacity = new_capacity;
      ItemType* newHeap = new ItemType[capacity];

      for (size_type i = 0; i < used; ++i)
         newHeap[i] = heap[i];

      // Free up memory of old array
      delete [] heap;
      heap = newHeap;
   }

   bool p_queue::is_leaf(size_type i) const
   // Pre:  (i < used)
   // Post: If the item at heap[i] has no children, true has been
   //       returned, otherwise false has been returned.
   {
      assert(i < used);
      return (((2 * i) + 1) >= used);
   }

   p_queue::size_type
   p_queue::parent_index(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The index of "the parent of the item at heap[i]" has
   //       been returned.
   {
      assert(i > 0 && i < used);
      return ((i-1) / 2);
   }

   p_queue::size_type
   p_queue::parent_priority(size_type i) const
   // Pre:  (i > 0) && (i < used)
   // Post: The priority of "the parent of the item at heap[i]" has
   //       been returned.
   {
      assert(i > 0 && i < used);
      return heap[parent_index(i)].priority;
   }

   p_queue::size_type
   p_queue::big_child_index(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The index of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      assert( !(is_leaf(i)) );

      // Indices for left and right children
      size_type lcIndex = (2 * i) + 1;
      size_type rcIndex = (2 * i) + 2;


      if (i == 0 && heap[1].priority >= heap[2].priority)
      {
         // Case 1: Parent is root, left child is greater
         return 1;
      }
      else if (i == 0 && heap[1].priority < heap[2].priority)
      {
         // Case 2: Parent is root, right child is greater
         return 2;
      }
      else
      {
         if (rcIndex < used && heap[rcIndex].priority > heap[lcIndex].priority)
         {
            // Case 3: Parent isn't root, right child exists
            return rcIndex;
         } else {
            // Case 4: Parent isn't root, only left child is present
            return lcIndex;
         }
      }
   }

   p_queue::size_type
   p_queue::big_child_priority(size_type i) const
   // Pre:  is_leaf(i) returns false
   // Post: The priority of "the bigger child of the item at heap[i]"
   //       has been returned.
   //       (The bigger child is the one whose priority is no smaller
   //       than that of the other child, if there is one.)
   {
      assert( !(is_leaf(i)) );
      return heap[big_child_index(i)].priority;
   }

   void p_queue::swap_with_parent(size_type i)
   // Pre:  (i > 0) && (i < used)
   // Post: The item at heap[i] has been swapped with its parent.
   {
      assert(i > 0 && i < used);

      // Item for swapping
      ItemType swapItem;

      // Get parent
      size_type pIndex = parent_index(i);
      swapItem = heap[pIndex];

      // Swap parent with child
      heap[pIndex] = heap[i];
      heap[i] = swapItem;
   }
}

