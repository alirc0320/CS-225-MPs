#include "StickerSheet.h"



StickerSheet::StickerSheet(const Image &picture, unsigned max) {
    max_ = max;
    picture_ = Image(picture);
    for(unsigned int i = 0; i < max_ ; i++) {
        layer myLayer = {0,0,NULL};
        layers_.push_back(myLayer);
    }
}


StickerSheet::StickerSheet(const StickerSheet & other){
    copy(other);
}


void StickerSheet::copy(const StickerSheet & other) {
 max_ = other.max_;
 picture_ = other.picture_;
 layers_= other.layers_;
}




void StickerSheet::changeMaxStickers(unsigned max){
    max_  = max;
    layers_.resize(max);
}


int StickerSheet::layers ()const{
    return max_; 
}


Image* StickerSheet::getSticker(unsigned int index) const {
    if (index < layers_.size() && layers_[index].sticker != nullptr) {
        return layers_[index].sticker;
    }
    return nullptr;
}


const StickerSheet& StickerSheet:: operator=(const StickerSheet& other){
    if(this != &other){
    copy(other);
    return *this;
    }
return *this;
}




void StickerSheet:: removeSticker(unsigned index){
    if( (index < max_ && index >= 0) ){
        layers_.at(index).sticker = NULL;
    }

}







Image StickerSheet::render () const{

    if(layers_.size() == 0)
    return picture_;
    Image image_base = picture_;

     int Xmax = image_base.width(); 
     int Ymax = image_base.height();

    int Xneg = 0; 
    int Yneg = 0;
    int xOffset = 0; 
    int yOffset = 0;

    for(unsigned w = 0; w<layers_.size(); w++){
        if(layers_.at(w).sticker!=NULL){
        if (layers_.at(w).x < Xneg)

        {

            Xneg = layers_.at(w).x;

        }

         if (layers_.at(w).y < Yneg)

        {

            Yneg = layers_.at(w).y;

        }

 
        if((layers_.at(w).x + (int)layers_.at(w).sticker->width())>Xmax){

            Xmax = (layers_.at(w).x + layers_.at(w).sticker->width());

        }

             

        if((layers_.at(w).y  + (int)layers_.at(w).sticker->height())>Ymax){

            Ymax = (layers_.at(w).y + layers_.at(w).sticker->height());

        }

        }

    }

    xOffset = 0 - Xneg;

    yOffset = 0 - Yneg;

    Image blank_canvas((unsigned)(Xmax - Xneg),(unsigned)( Ymax - Yneg));  

    for(unsigned i = 0; i<image_base.width(); i++){
        for(unsigned j = 0; j<image_base.height(); j++){
            blank_canvas.getPixel(i+xOffset, j + yOffset) = picture_.getPixel(i,j);
        }
    }
    for (unsigned int w = 0; w<max_; w++){
        if(layers_.at(w).sticker!=NULL){
        for(unsigned int i = 0; i< layers_.at(w).sticker->width(); i++){
            for(unsigned int j = 0; j<layers_.at(w).sticker->height(); j++){
                        HSLAPixel & p = blank_canvas.getPixel(i + layers_.at(w).x+xOffset,j + layers_.at(w).y+yOffset);
                        if(layers_.at(w).sticker->getPixel(i, j).a!=0){
                        blank_canvas.getPixel(i+layers_.at(w).x-Xneg, j+ layers_.at(w).y - Yneg)  = (layers_.at(w).sticker)->getPixel(i,j);
                    }
                }
            }
        }
    }
    return blank_canvas;
}


          




int StickerSheet::setStickerAtLayer(Image &sticker, unsigned layer, int x, int y){
 if(layer < max_ && layer >= 0){
    layers_.at(layer).x = x ;
    layers_.at(layer).y = y ;
    layers_.at(layer).sticker = &sticker;
    return layer; 
}
return -1;
}


bool StickerSheet::translate(unsigned index, int x, int y){
    if((index >= 0 && index< layers_.size()) && (layers_.at(index).sticker != NULL)){
        layers_.at(index).x = x ; 
          layers_.at(index).y = y ; 
        return true;
    }
return false;
}
















int StickerSheet::addSticker(Image &sticker, int x, int y) {
    unsigned int index = 0;
    for (index = 0; index < layers_.size(); index++) {
        if (layers_[index].sticker == nullptr) {
            break;
        }
    }
    if (index == layers_.size()) {
        layer myLayer = { x, y, &sticker };
        layers_.push_back(myLayer);
        index = max_;
        max_++;
    } else {
        layers_[index].sticker = &sticker;
        layers_[index].x = x;
        layers_[index].y = y;
    }
    return index;
}










