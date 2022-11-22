// Ezekiel Quintanilla
// Assignment 6 Pt 2
//
// CS3358 - Koh

#include "btNode.h"

// write definition for bst_insert here
void bst_insert(btNode*& bst_root, const int insInt) 
{
   btNode *cursor = bst_root;
   if (bst_root == 0) 
   {
      btNode *newLeaf = new btNode;
      newLeaf->data = insInt;
      newLeaf->left = 0;
      newLeaf->right = 0;
      bst_root = newLeaf;
   }
   else
   {
      while (cursor) {
         if (insInt > cursor->data && cursor->right != 0)
         {
            cursor = cursor->right;
         }
         else if (insInt < cursor->data && cursor->left != 0)
         {
            cursor = cursor->left;
         }
         else if (insInt > cursor->data && cursor->right == 0)
         {
            btNode *newLeaf = new btNode;
            newLeaf->data = insInt;
            newLeaf->left = 0;
            newLeaf->right = 0;
            cursor->right = newLeaf;
         }
         else if (insInt < cursor->data && cursor->left == 0)
         {
            btNode *newLeaf = new btNode;
            newLeaf->data = insInt;
            newLeaf->left = 0;
            newLeaf->right = 0;
            cursor->left = newLeaf;
         }
         else
         {
            cursor = 0;
         }
      }
   }
} // End bst_insert()

// write definition for bst_remove here
bool bst_remove(btNode*& bst_root, const int remInt)
{
   btNode* old_bst_root = bst_root;
   if (bst_root == 0) return false;
   
   // Traverse tree dependent on value of remInt
   if (remInt > bst_root->data)
   {
      bst_remove(bst_root->right, remInt);
   }
   else if (remInt < bst_root->data)
   {
      bst_remove(bst_root->left, remInt);
   }
   else if (remInt == bst_root->data)
   {
      // Root equals remInt
      // Check for child nodes
      if (bst_root->left && bst_root->right)
      {
         // Case 1: Root has two children, use remove max
         // to replace root and maintain BST structure
         bst_remove_max(bst_root->left, bst_root->data);
      }
      else
      {
         if (bst_root->left && !bst_root->right)
         {
            // Case 2: Root has left child only
            bst_root = bst_root->left;
         }
         else if (!bst_root->left && bst_root->right)
         {
            // Case 3: Root has right child only
            bst_root = bst_root->right;
         }
         else
         {
            // Case 4: Root has no children
            bst_root = 0;
         }
         delete old_bst_root;
      }
      return true;
   }
   else
   {
      return false;
   }
   // return false;
} // End bst_remove()

// write definition for bst_remove_max here
void bst_remove_max(btNode*& bst_root, int& data)
{
   // Empty tree, nothing to remove
   if (!bst_root) return;

   // Save root for deletion
   btNode* targetNode = bst_root;

   if (bst_root->right)
   {
      // Continue down right subtree
      bst_remove_max(bst_root->right, data);
   }
   else
   {
      // Change root to successor
      // Then delete target
      data = bst_root->data;
      bst_root = bst_root->left;
      delete targetNode;
   }
} // End bst_remove_max()

void dumpToArrayInOrder(btNode* bst_root, int* dumpArray)
{
   if (bst_root == 0) return;
   int dumpIndex = 0;
   dumpToArrayInOrderAux(bst_root, dumpArray, dumpIndex);
}

void dumpToArrayInOrderAux(btNode* bst_root, int* dumpArray, int& dumpIndex)
{
   if (bst_root == 0) return;
   dumpToArrayInOrderAux(bst_root->left, dumpArray, dumpIndex);
   dumpArray[dumpIndex++] = bst_root->data;
   dumpToArrayInOrderAux(bst_root->right, dumpArray, dumpIndex);
}

void tree_clear(btNode*& root)
{
   if (root == 0) return;
   tree_clear(root->left);
   tree_clear(root->right);
   delete root;
   root = 0;
}

int bst_size(btNode* bst_root)
{
   if (bst_root == 0) return 0;
   return 1 + bst_size(bst_root->left) + bst_size(bst_root->right);
}
