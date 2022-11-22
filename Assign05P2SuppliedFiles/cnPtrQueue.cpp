// Ezekiel Quintanilla
// Assignment 5 Pt 2
//
// CS3358 - Koh

#include "cnPtrQueue.h"
#include <cassert>
using namespace std;

namespace CS3358_FA2021_A5P2
{
   cnPtrQueue::cnPtrQueue() : numItems(0) {}

   bool cnPtrQueue::empty() const {
      // Queue is empty if and only if both stacks
      // are empty.
      return (inStack.empty() && outStack.empty());
   }

   cnPtrQueue::size_type cnPtrQueue::size() const
   {
      return numItems;
   }

   CNode* cnPtrQueue::front()
   {
      assert(!inStack.empty() || !outStack.empty());
      // Check if outstack is empty
      if (outStack.empty())
      {
         // Move anything in inStack
         // over to outStack.
         // Anything in outStack can stay for
         // future processing and to save resources.
         while (!inStack.empty())
         {
            outStack.push(inStack.top());
            inStack.pop();
         }
      }

      // Front of the queue
      return outStack.top();
   }

   void cnPtrQueue::push(CNode* cnPtr)
   {
      // Push to inStack.
      inStack.push(cnPtr);
      numItems++;
   }

   void cnPtrQueue::pop()
   {
      assert(!inStack.empty() || !outStack.empty());
      if(outStack.empty())
      {
         // Move anything in inStack
         // over to outStack.
         while(!inStack.empty())
         {
            // Move anything in inStack
            // over to outStack.
            outStack.push(inStack.top());
            inStack.pop();
         }
      }
      outStack.pop();
      numItems--;
   }
}
