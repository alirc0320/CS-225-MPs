/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second, int curDim)
{
if (first[curDim] == second[curDim]) {
for (int i = 0; i < Dim; i++) {
if (i != curDim && first[i] != second[i]) {
return first[i] < second[i];
}
}
return false;
} else {
return first[curDim] < second[curDim];
}
}



template <int Dim>
bool shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential)
{
    double tp_dist = 0.0;
    double tc_dist = 0.0;
    for (int i = 0; i < Dim; i++) {
        double diff_p = potential[i] - target[i];
        double diff_c = currentBest[i] - target[i];
        tp_dist += diff_p * diff_p;
        tc_dist += diff_c * diff_c;
    }
    double eps = std::numeric_limits<double>::epsilon();
    if (std::abs(tp_dist - tc_dist) <= eps) {
        return potential < currentBest;
    } else {
        return tp_dist < tc_dist;
    }
}








//*************************************************************************************************************//
//helper

template <int Dim>
typename KDTree<Dim>::KDTreeNode * KDTree<Dim>::Newtree(vector<Point<Dim>>& newP,int left, int right, int dimensions)
{
  auto cmp=[dimensions](Point<Dim>left2, Point<Dim>right2){return smallerDimVal(left2,right2,dimensions);};
  if (left > right) {
    return NULL;
  }
 
  double mid = (left + right ) / 2 ;
  
  auto iterbegin = newP.begin() + left;
  auto iterfinish = newP.begin() + right + 1 ; 
  auto itermid = newP.begin() + mid;

  
  select(iterbegin, iterfinish, itermid,cmp);

  typename KDTree<Dim>::KDTreeNode* nodes = new KDTreeNode(*itermid);
  
  dimensions++;
  nodes->left = Newtree(newP, left, mid - 1, (dimensions) % Dim);
  nodes->right = Newtree(newP, mid + 1, right , (dimensions) % Dim);
  size++;
  return nodes;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
  if (newPoints.size() == 0) {
    // create an empty tree
    root = NULL;
    size = 0;
    return;
  }
  else { 
    vector<Point<Dim>> copypoints = newPoints;
    root = Newtree(copypoints,0, copypoints.size()-1,0);
    
  }
  printTree(cout);
}




//************************************************************************************
















//*****************************************************

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
}


//*****************************************************

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
 if(this != &rhs){
      delete *this;
      this = new KDTree(rhs);
    }
    return *this;
  
}
//*****************************************************
template <int Dim>
void KDTree<Dim>::destroy(KDTreeNode *sr) {
    if (!sr) return;
    destroy(sr->left);
    destroy(sr->right);
    delete sr;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
   destroy(root);
}
//*****************************************************














template <typename RandIter, typename Comparator>
RandIter pivot_partition(RandIter begin, RandIter end, RandIter pivot, Comparator comp) {
    auto pivot_val = *pivot;
    auto temp_val = *pivot;
    *pivot = *(end - 1);
    *(end - 1) = temp_val;
    RandIter store_index = begin;
    for (RandIter i = begin; i != end - 1; ++i) {
        if (comp(*i, pivot_val)) {
            auto temp_val2 = *store_index;
            *store_index = *i;
            *i = temp_val2;
            ++store_index;
        }
    }
    auto temp_val3 = *(end - 1);
    *(end - 1) = *store_index;
    *store_index = temp_val3;
    return store_index;
}




template <typename RandIter, typename Comparator>
void select(RandIter begin, RandIter end, RandIter k, Comparator cmp) {
    while (true) {
        if (begin == end - 1) {
            return;
        }
        RandIter pivot = begin + std::rand() % std::distance(begin, end);
        pivot = pivot_partition(begin, end, pivot, cmp);
        if (k == pivot) {
            return;
        } else if (k < pivot) {
            end = pivot;
        } else {
            begin = pivot + 1;
        }
    }
}




template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    return helper(query, root, 0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::helper(const Point<Dim>& query, KDTreeNode * node, int depth) const
{
    if(node -> left == nullptr && node->right == nullptr)
        return node->point;
    Point<Dim> great = (node->point);
    Point<Dim> bar = great;
    bool leftSide = smallerDimVal(query, node->point,depth);
    if(leftSide && node-> left != nullptr)
        bar = helper(query, node->left, (depth+1)%Dim);
    else if(!leftSide && node -> right != nullptr)
        bar = helper(query, node->right, (depth+1)%Dim);
    if(!shouldReplace(query, bar, great))
        great = bar;
    double radius = 0;
    for(int i =0; i < Dim; i++)
        radius += (query[i] - great[i]) * (query[i] - great[i]);
    double splitDist = 0;

    splitDist = ((node -> point)[depth] - query[depth]) * ((node -> point)[depth] - query[depth]);
   
    if(radius >= splitDist)
    {
        if(leftSide && node -> right !=  nullptr){
            bar = helper(query, node->right, (depth+1)%Dim);

        }
        else if (!leftSide && node -> left != nullptr){
            bar = helper(query, node->left, (depth+1)%Dim);
           
        }
        if(shouldReplace(query, great, bar))
            great = bar;
    }
    return great;
  
}





//ALI