#include "Image.h"
//#include "HSLAPixel.h"

using namespace cs225;


Image::Image()
{}


Image::Image(unsigned int width, unsigned int height) : PNG(width, height)
{
    width_ = width;
    height_ = height;
}


void Image::lighten(){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j< this->height(); j++){
          HSLAPixel & pixel = getPixel(i, j);   
          if(pixel.l<.9){
                pixel.l += .1;
                }   else{                                   
            pixel.l = 1;
                }
        }
    }
}


void Image::lighten(double ammount){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j< this->height(); j++){
            HSLAPixel & pixel = getPixel(i, j);   
            if(pixel.l + ammount < 1){
                pixel.l += ammount;      
            }            
            else{                          
                pixel.l = 1;
            }
        }
    }

}


void Image::darken(){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j< this->height(); j++){
            HSLAPixel & pixel = getPixel(i, j);   
            pixel.l -= .1 ;
            if(pixel.l<0){
                
                pixel.l = 0;
        
            }                                 
        }
    }
}


void Image::darken(double ammount){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j< this->height(); j++){   
            HSLAPixel & pixel = getPixel(i, j);   
            pixel.l -= ammount ;
            if(pixel.l<0){
                pixel.l = 0;
            }                          
        }
    }
}


void Image::desaturate(){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j< this->height(); j++){
            HSLAPixel & pixel = getPixel(i, j);   
            pixel.s -= .1 ;
            if(pixel.s<0){
                pixel.s = 0;
            }                                
        }
    }
}


void Image::desaturate(double ammount){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j< this->height(); j++){  
            HSLAPixel & pixel = getPixel(i, j);   
            pixel.s -= ammount;
            if(pixel.s<0){
                pixel.s = 0;
            }                          
        }
    }
}

void Image::saturate(){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j<this->height(); j++){   
            HSLAPixel & pixel = getPixel(i, j);   
            if(pixel.s<.9){
                pixel.s += .1;      
            }    
            
            else{                             
        pixel.s = 1;
            }
            
        }
    }
}



void Image::saturate(double ammount){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j<this->height(); j++){
            HSLAPixel & pixel = getPixel(i, j);   
            if(pixel.s + ammount < 1){
                pixel.s += ammount;      
            }      
            else{                               
        pixel.s = 1;
            }
        }
    }
}




void Image::grayscale(){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j< this->height(); j++){
            HSLAPixel & pixel = getPixel(i, j);   
           pixel.s = 0;              
            }                                     
        }
    }

//Check hue
//If hue is closer to orange then change to orange hue 
//If hue is closer to blue then change to blue hue


void Image::illinify(){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j<this->height(); j++){
             HSLAPixel & pixel = getPixel(i, j);
             
             if((pixel.h >= 0 && pixel.h <= 11) || (pixel.h >11 && pixel.h <= 113.5) || (pixel.h > 293.5 && pixel.h <= 360) ){
                pixel.h = 11;
            }
            else 
            {
                pixel.h = 216;
            }
             
            
            
                
            }                                     
        }
    }



void Image::rotateColor(double degrees){
    for (unsigned i = 0; i<this->width(); i++){
        for(unsigned j = 0; j<this->height(); j++){
             HSLAPixel & pixel = getPixel(i, j);
            pixel.h += degrees;
            
            
            while(pixel.h<0){
                pixel.h+=360;
            }
            
            while(pixel.h>360){
                pixel.h-=360;
            }
            
            // if(pixel.h<0){
            //     pixel.h += 360;
            // }
            // if(pixel.h>360){
            //     pixel.h -= 360;
            // }
            
            
            }                                     
        }
    }




void Image::scale(double factor){
   if(factor == 0){
       return;

   }
    PNG newImage = *this;
    double previousHeight = this->height();
    double previousWidth =  this->width();
    this->resize(previousWidth * factor, previousHeight * factor);
    for(unsigned int i = 0; i<this->width(); i++){
        for(unsigned int j = 0; j<this->height(); j++){
            this->getPixel(i, j) = newImage.getPixel(i/factor, j/factor);  
            }
    }
}



void Image::scale(unsigned w,unsigned h ){
    double width = w/this->width();
    double height = h/this->height();
    if (width<height){
        scale(width);
    }
    else{
        scale (height);
    }
}

