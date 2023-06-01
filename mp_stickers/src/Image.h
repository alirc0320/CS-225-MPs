/**
 * @file Image.h
 * Contains your declaration of the interface for the Image class.
 */

#pragma once
//#include <Image.h>
#include "cs225/PNG.h"

using namespace cs225;

class Image: public PNG{

public:
 Image();
 Image(unsigned int width, unsigned int height );
void darken();
void darken(double 	ammount);
void desaturate(); 
void desaturate(double ammount);
void grayscale();
void illinify(); 
void lighten();
void lighten(double ammount);
void rotateColor(double degrees);
void saturate();
void saturate(double ammount);
void scale(unsigned w, unsigned h); 
void scale(double ammount);


unsigned width_;
unsigned height_;


};
