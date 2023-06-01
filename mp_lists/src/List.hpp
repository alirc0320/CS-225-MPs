/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */
#include <iostream>
template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0 ;
  
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  // @TODO: graded in mp_lists part 1
  //return List<T>::ListIterator(NULL);
   return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  
  return List<T>::ListIterator(NULL);
}


/**
 * Destroys all dynamically allocated memory associated with the current
 * List class.
 */
template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
  ListNode* current = head_; 
  ListNode* next; 
  while (current != NULL){
    next = current->next;
    delete current; 
    current = next; 
  }
  head_ = nullptr;
  tail_ = nullptr;
  length_ = 0;

  }
 


  
/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  newNode -> next = head_;
  newNode -> prev = NULL;
  
  if (head_ != NULL) {
    head_ -> prev = newNode;
  }
  if (tail_ == NULL) {
    tail_ = newNode;
  }
  head_ = newNode;

  length_++;

}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1
ListNode * newNode = new ListNode(ndata);
  newNode -> next = NULL;
  newNode -> prev = tail_;
  
  if (tail_ != NULL) {
    tail_ -> next = newNode;
  }
  if (tail_ == NULL) {
    head_ = newNode;
  }
  tail_ = newNode;

  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  /// @todo Graded in mp_lists part 1
  ListNode * curr = start;

  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }

  if (curr != NULL) {
    if(curr->prev != nullptr){
      tail_ = curr->prev;
      curr->prev->next = NULL;
      curr->prev = NULL;
    }
  }

  return curr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
//  */
template <typename T>
void List<T>::waterfall() {
    if (head_->next == tail_) {
        return;
    }
    int counter = 0;
    ListNode* current = head_->next;
    while (current != tail_) {
        if (counter % 2 == 0) {
            // Remove current node from its current position
            current->prev->next = current->next;
            if (current->next != nullptr) {
                current->next->prev = current->prev;
            }

            // Append current node to the tail
            ListNode* temp = current->next;
            current->prev = tail_;
            current->next = tail_->next;
            if (tail_->next != nullptr) {
                tail_->next->prev = current;
            }
            tail_->next = current;

            // Update the tail pointer
            tail_ = current;

            // Move to the next node
            current = temp;
        } else {
            // Move to the next node
            current = current->next;
        }
        counter++;
    }
}

/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
*/
template <typename T>
void List<T>::reverse(ListNode *&startPoint, ListNode *&endPoint) {
  // Check for empty list or a single node
  if (startPoint == nullptr || endPoint == nullptr || startPoint == endPoint) {
    return;
  }

  // Store pointers to the nodes before the start and after the end of the sublist to be reversed
  ListNode* prevStart = startPoint->prev;
  ListNode* nextEnd = endPoint->next;

  // Current node and its previous node
  ListNode* current = startPoint;
  ListNode* previous = nullptr;

  // Traverse the sublist and reverse the links between nodes
  while (current != nextEnd) {
    ListNode* next = current->next;
    current->next = previous;
    current->prev = next;
    previous = current;
    current = next;
  }

  // Update the links between the reversed sublist and the rest of the list
  startPoint->next = nextEnd;
  endPoint->prev = prevStart;

  // Update head_ and tail_ if necessary
  if (prevStart == nullptr) {
    head_ = endPoint;
  } else {
    prevStart->next = endPoint;
  }

  if (nextEnd == nullptr) {
    tail_ = startPoint;
  } else {
    nextEnd->prev = startPoint;
  }

  // Swap the startPoint and endPoint pointers
  ListNode* temp = startPoint;
  startPoint = endPoint;
  endPoint = temp;
}



    
    




/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */


template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  if (chainLength == 1 || chainLength == 0) return start;

  ListNode* secondSplit;
  ListNode* secondSplitSorted;
  ListNode* firstSplitSorted;
  int passedLength = chainLength - chainLength/2;

  secondSplit = split(start, chainLength/2);

  secondSplitSorted = mergesort(secondSplit, passedLength);
  firstSplitSorted = mergesort(start, chainLength/2);

  return merge(secondSplitSorted, firstSplitSorted);
}






/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
//  */

template <typename T>
void List<T>::reverseNth(int n) {
    if (length_ == 0 || length_ == 1) {
        return;
    }
    ListNode* start = head_;
    ListNode* end = head_;
    while (end != nullptr) {
        for (int i = 0; i < n - 1 && end->next != nullptr; i++) {
            end = end->next;
        }
        reverse(start, end);
        if (end->next == nullptr) {
            return;
        }
        start = end->next;
        end = start;
    }
}















/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
 


template <typename T>
typename List<T>::ListNode* List<T>::merge(ListNode* first, ListNode* second) {
    // Check for empty input lists
    if (first == nullptr) {
        return second;
    }
    if (second == nullptr) {
        return first;
    }

    // Create a new result list and tail pointer
    ListNode* result = nullptr;
    ListNode* tail = nullptr;

    // Choose the first node based on data value
    if (first->data < second->data) {
        result = first;
        first = first->next;
    } else {
        result = second;
        second = second->next;
    }
    tail = result;

    // Merge the rest of the nodes in the two input lists
    while (first != nullptr && second != nullptr) {
        if (first->data < second->data) {
            tail->next = first;
            first = first->next;
        } else {
            tail->next = second;
            second = second->next;
        }
        tail = tail->next;
    }

    // Append any remaining nodes to the result list
    if (first != nullptr) {
        tail->next = first;
    } else {
        tail->next = second;
    }

    return result;
}


//ALI 