// Ezekiel Quintanilla
// Assignment 5 Pt 2
//
// CS3358 - Koh

#include "nodes_LLoLL.h"
#include "cnPtrQueue.h"
#include <iostream>
using namespace std;

namespace CS3358_FA2021_A5P2
{
   void Destroy_cList(CNode*& cListHead)
   {
      int count = 0;
      CNode* cNodePtr = cListHead;
      while (cListHead != 0)
      {
         cListHead = cListHead->link;
         delete cNodePtr;
         cNodePtr = cListHead;
         ++count;
      }
      cout << "Dynamic memory for " << count << " CNodes freed"
           << endl;
   }

   void Destroy_pList(PNode*& pListHead)
   {
      int count = 0;
      PNode* pNodePtr = pListHead;
      while (pListHead != 0)
      {
         pListHead = pListHead->link;
         Destroy_cList(pNodePtr->data);
         delete pNodePtr;
         pNodePtr = pListHead;
         ++count;
      }
      cout << "Dynamic memory for " << count << " PNodes freed"
           << endl;
   }

   // do depth-first traversal and print data
   void ShowAll_DF(PNode* pListHead, ostream& outs)
   {
      while (pListHead != 0)
      {
         CNode* cListHead = pListHead->data;
         while (cListHead != 0)
         {
            outs << cListHead->data << "  ";
            cListHead = cListHead->link;
         }
         pListHead = pListHead->link;
      }
   }

   // do breadth-first (level) traversal and print data
   void ShowAll_BF(PNode* pListHead, ostream& outs)
   {
      if (pListHead == 0) return;

      // If the parent list is not empty
      // create a queue and cursor.
      cnPtrQueue nodeQueue;
      CNode* cursor;

      while (pListHead != 0)
      {
         if (pListHead->data != 0)
            nodeQueue.push(pListHead->data);
         pListHead = pListHead->link;
      }
      while (!nodeQueue.empty())
      {
         cursor = nodeQueue.front();
         nodeQueue.pop();
         std::cout << cursor->data << "  ";
         if (cursor->link != 0)
            nodeQueue.push(cursor->link);
      }
   }
}
