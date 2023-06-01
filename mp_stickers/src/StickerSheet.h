

#pragma once
#include<vector>
#include "Image.h"

using namespace std;

struct layer {
  int x;
  int y;
  Image* sticker;
};

class StickerSheet {
public:
  StickerSheet(const Image &picture, unsigned max );
  StickerSheet(const StickerSheet & other );

  int addSticker(Image & sticker, int x, int y);
  void changeMaxStickers(unsigned max);

  Image* getSticker(unsigned int index) const;
  int layers() const;

  const StickerSheet & operator=(const StickerSheet & other);
  void removeSticker(unsigned index);

  // void destroy();
  // ~StickerSheet();


  void copy(const StickerSheet & obj);

  Image render() const;

  int setStickerAtLayer(Image & sticker, unsigned layer, int x, int y);

  bool translate(unsigned index, int x, int y);

private:
  Image picture_;
  unsigned max_;
  vector<layer> layers_;
};



