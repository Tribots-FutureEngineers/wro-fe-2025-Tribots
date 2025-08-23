#include <Pixy2.h>

Pixy2 pixy;
#define RED_SIGNATURE 2
#define GREEN_SIGNATURE 1

void setup() {
  Serial.begin(9600);
  pixy.init();
  Serial.println("Pixy2 Test Started");
}

void loop() {
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks) {
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      uint8_t sig = pixy.ccc.blocks[i].m_signature;
      if (sig == RED_SIGNATURE) Serial.println("RED block detected!");
      else if (sig == GREEN_SIGNATURE) Serial.println("GREEN block detected!");
    }
  }
  delay(100);
}
