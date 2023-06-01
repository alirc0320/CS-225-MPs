#include "cs225/PNG.h"
#include <list>
#include <iostream>

#include "colorPicker/ColorPicker.h"
#include "ImageTraversal.h"

#include "Point.h"
#include "Animation.h"
#include "FloodFilledImage.h"

using namespace cs225;

/**
 * Constructs a new instance of a FloodFilledImage with a image `png`.
 * 
 * @param png The starting image of a FloodFilledImage
 */
FloodFilledImage::FloodFilledImage(const PNG & png) {
  /** @todo [Part 2] */
  picture = png;
}

/**
 * Adds a FloodFill operation to the FloodFillImage.  This function must store the operation,
 * which will be used by `animate`.
 * 
 * @param traversal ImageTraversal used for this FloodFill operation.
 * @param colorPicker ColorPicker used for this FloodFill operation.
 */
void FloodFilledImage::addFloodFill(Traversals::ImageTraversal & traversal, ColorPicker & colorPicker) {
  /** @todo [Part 2] */
  traverse.push_back(&traversal);
  picker.push_back(&colorPicker);
}


Animation FloodFilledImage::animate(unsigned frameInterval) const {
  Animation animation;
  PNG copy = picture;
  animation.addFrame(picture);
  unsigned frame = 1;
  for (const auto& traversal : traverse) {
    for (const auto& pt : *traversal) {
      copy.getPixel(pt.x, pt.y) = picker[frame % picker.size()]->getColor(pt.x, pt.y);
      if (frame % frameInterval == 0) {
        animation.addFrame(copy);
      }
      ++frame;
    }
  }
  animation.addFrame(copy);
  return animation;
}
//Ali