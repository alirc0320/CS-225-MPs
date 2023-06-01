#include "Image.h"

#include "StickerSheet.h"

 

int main() {


  Image alma, ali, ali_1, ali_2;

 

  alma.readFromFile("../data/alma.png");

  ali.readFromFile("../data/expected-5.png");

  ali_1.readFromFile("../data/expected-lighten.png");

  ali_2.readFromFile("../data/expected-saturate.png");

  StickerSheet ali_3(alma, 3);

  ali_3.addSticker(ali, 230, 230);

  ali_3.addSticker(ali_1, 400, 600);

  ali_3.addSticker(ali_2, 700, 700);

  Image result = ali_3.render();

  result.writeToFile("myImage.png");

  return 0;

}
