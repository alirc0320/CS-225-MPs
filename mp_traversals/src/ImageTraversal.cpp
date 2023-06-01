#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt( (h*h) + (s*s) + (l*l) );
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
     work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
     work_list.push_back(point);
  }

  /**
  * Removes and returns the current Point in the bfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    //return Point(0, 0);
    Point var_return = work_list.front();
      work_list.pop_front();
      return var_return;

  }

  /**
  * Removes and returns the current Point in the dfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    //return Point(0, 0);
     Point var_return = work_list.back();
      work_list.pop_back();
      return var_return ;
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param traversal_add a function pointer to an implemented function which takes in a reference to the work list
  * and the point to be added
  * @param traversal_pop a function pointer to an implemented function which takes in a reference to the work list
  * and returns the next point to be processed in the traversal
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) {  
    /** @todo [Part 1] */
    
    traversaladd = traversal_add;
    image = &png;
    tolerance_ = tolerance;
    pop_traversal = traversal_pop;
    start_point = &start;
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return  ImageTraversal::Iterator(*image, *start_point, tolerance_ ,traversaladd,  pop_traversal);
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
     return ImageTraversal::Iterator();
   
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
    //do not need to do this part
    //not  graded 
  }


  /**
  * Iterator increment opreator.
  *
  * Advances the traversal of the image.
  */


  bool ImageTraversal::Iterator::wasVisited(Point pt)
  {
    return visited[pt.x][pt.y];
  }
  void ImageTraversal::Iterator::visitPoint(Point pt)
  {
    visited[pt.x][pt.y] = true;
  }

ImageTraversal::Iterator & ImageTraversal::Iterator::operator++()  {
  // Get the dimensions of the image
  const unsigned int width = image-> width();
  const unsigned int height = image -> height();
  
  // Get the pixel value at the start point
  const HSLAPixel curPix = image->getPixel(start_point->x, start_point->y);
  
  // Mark the current point as visited
  visitPoint(cur);
  
  // Check the right neighbor
  if(cur.x + 1 < width && !wasVisited(Point(cur.x + 1, cur.y)) && calculateDelta(image->getPixel(cur.x + 1, cur.y), curPix) <= tolerance_)
  {
    // Add the right neighbor to the traversal if it has not been visited and the color difference is within the tolerance
    traversaladd(work_list, Point(cur.x + 1, cur.y));
  } 
  
  // Check the bottom neighbor
  if(cur.y + 1 < height && !wasVisited(Point(cur.x, cur.y + 1)) && calculateDelta(image->getPixel(cur.x, cur.y + 1), curPix) <= tolerance_)
  {
    // Add the bottom neighbor to the traversal if it has not been visited and the color difference is within the tolerance
    traversaladd (work_list, Point(cur.x, cur.y + 1));
  }
  
  // Check the left neighbor
  if(cur.x > 0 && !wasVisited(Point(cur.x - 1, cur.y)) && calculateDelta(image->getPixel(cur.x - 1, cur.y), curPix) <= tolerance_)
  {
    // Add the left neighbor to the traversal if it has not been visited and the color difference is within the tolerance
    traversaladd(work_list, Point(cur.x - 1, cur.y));
  } 
  
  // Check the top neighbor
  if(cur.y > 0 && !wasVisited(Point(cur.x, cur.y - 1)) && calculateDelta(image->getPixel(cur.x, cur.y - 1), curPix) <= tolerance_)
  {
    // Add the top neighbor to the traversal if it has not been visited and the color difference is within the tolerance
    traversaladd(work_list, Point(cur.x, cur.y - 1));
  }

  // Remove the current point from the traversal if it has been visited
  while (!work_list.empty() && wasVisited(cur)) {
    cur = pop_traversal(work_list);
  }
 
  // Return the iterator
  return *this;
}

  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return cur;
  }
  ImageTraversal::Iterator::Iterator(const Point *pt) {
    work_list.push_back(*pt);
  }


ImageTraversal::Iterator::Iterator(const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) {  
    /** @todo [Part 1] */

    image = &png;
    start_point = &start;
    cur = start;
    tolerance_  = tolerance;
    traversaladd = traversal_add;
    pop_traversal  = traversal_pop;
    visited.resize(image->width(), std::vector<bool>(image->height()));
    for (unsigned i = 0; i < image->width(); i++) {
        for (unsigned j = 0; j < image->height(); j++) {
            visited[i][j] = false;
        }
    }
    Point current = cur;
    unsigned int width = image-> width();
    unsigned int height = image -> height();
    
    bool is_inside_bound = (cur.x < width && cur.y < height && cur.x >=0 && cur.y >=0);
    if(is_inside_bound){
        const HSLAPixel  curPix = image->getPixel(start_point->x, start_point->y);
        visitPoint(cur);
        Point right(current.x + 1, current.y);
        Point down(current.x, current.y + 1);
        Point left(current.x - 1, current.y);
        Point up(current.x, current.y - 1);
        
        bool is_right_inside_bound = (current.x + 1 < width && current.x+1 >=0);
        bool is_down_inside_bound = (current.y +1  < height && current.y+1 >=0);
        bool is_left_inside_bound = (current.x - 1 < width && current.x-1 >=0);
        bool is_up_inside_bound = (current.y -1  < height && current.y-1 >=0);

        if(is_right_inside_bound){
            const HSLAPixel  pix = image->getPixel(current.x+1, current.y);
            double delta = calculateDelta(pix, curPix);

            if(!wasVisited(right) && delta <=tolerance_ )
               traversaladd(work_list, right);
        } 
        if(is_down_inside_bound){
            const HSLAPixel  pix = image->getPixel(current.x, current.y+1);
            double delta = calculateDelta(pix, curPix);

            if(!wasVisited(down) && delta <=tolerance_ )
              traversaladd(work_list, down);
        }
        if(is_left_inside_bound){
            const HSLAPixel  pix = image->getPixel(current.x-1, current.y);
            double delta = calculateDelta(pix, curPix);

            if(!wasVisited(left) && delta <= tolerance_ )
               traversaladd(work_list, left);
        } 
        
        if(is_up_inside_bound){
            const HSLAPixel  pix = image->getPixel(current.x, current.y-1);
            double delta = calculateDelta(pix, curPix);

            if(!wasVisited(up) && delta <= tolerance_ )
               traversaladd(work_list, up);
        }
    }
}

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  if(!(work_list == other.work_list)){
    return true;
  }
  return false;
}

}
//Ali