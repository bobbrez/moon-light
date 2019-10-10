#include <WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define NUM_LEDS_MAIN     250
#define NUM_LEDS_SIDE     150
#define MAIN_LED_PIN      23
#define SIDE_LED_PIN      22
#define LED_TYPE          WS2812B

#define FRAMES_PER_SECOND  120

const char* ssid     = "xxxx";
const char* password = "yyyy";

CRGB ledsMain[NUM_LEDS_MAIN];
CRGB ledsSide[NUM_LEDS_SIDE];

uint8_t groupA[] = {   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,
                      97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108,                     
                     169, 170, 171, 172, 173, 174, 175, 176,                                                        
                     217, 218, 219, 220, 221, 222                                                   };
uint8_t groupB[] = {  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,
                     109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,                     
                     177, 178, 179, 180, 181, 182, 183, 184,
                     223, 224, 225, 226                                                             };
uint8_t groupC[] = {  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
                     121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
                     185, 186, 187, 188, 189, 190, 191, 192,
                     227, 228, 229, 230, 231, 232                                                   };
uint8_t groupD[] = {  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,
                     133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
                     193, 194, 195, 196, 197, 198, 199, 200,
                     233, 234, 235, 236, 237, 238                                                   };
uint8_t groupE[] = {  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,
                     145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
                     201, 202, 203, 204, 205, 206, 207, 208,
                     239, 240, 241, 242, 243, 244                                                   };
uint8_t groupF[] = {  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,
                     157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,       
                     209, 210, 211, 212, 213, 214, 215, 216,
                     245, 246, 247, 248, 249, 250                                                   };

uint8_t gHue = 0;
bool gReverseDirection = false;
CRGBPalette16 gPal = LavaColors_p;

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  
  Serial.println("Connected");
  Serial.print("IP address: ");
  
  Serial.println(WiFi.localIP());

  LEDS.addLeds<LED_TYPE, MAIN_LED_PIN, GRB>(ledsMain, NUM_LEDS_MAIN);
  LEDS.addLeds<LED_TYPE, SIDE_LED_PIN, GRB>(ledsSide, NUM_LEDS_SIDE);
}

int curIndex = 0;

void loop() {
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_MILLISECONDS( 1000 ) { curIndex++;  curIndex = curIndex % 6; }
  //fill_rainbow( ledsMain, NUM_LEDS_MAIN, gHue, 7);

  //Fire2012WithPalette();

  fill_rainbow( ledsSide, NUM_LEDS_SIDE, gHue, 7);
  fill_solid( ledsMain, NUM_LEDS_MAIN, CHSV(35, 80, 64));
/*
  for(int j = 0; j < NUM_LEDS_MAIN; j++) {
    ledsMain[j].fadeToBlackBy( 10 );  
  }

  switch(curIndex) {
    case 0:
      for(int i = 0; i < sizeof(groupA); i++) { ledsMain[groupA[i]] = CHSV(gHue, 255, 255); }
      break;
    case 1:
      for(int i = 0; i < sizeof(groupB); i++) { ledsMain[groupB[i]] = CHSV(gHue, 255, 255); }
      break;
    case 2:
      for(int i = 0; i < sizeof(groupC); i++) { ledsMain[groupC[i]] = CHSV(gHue, 255, 255); }
      break;
    case 3:
      for(int i = 0; i < sizeof(groupD); i++) { ledsMain[groupD[i]] = CHSV(gHue, 255, 255); }
      break;
    case 4:
      for(int i = 0; i < sizeof(groupE); i++) { ledsMain[groupE[i]] = CHSV(gHue, 255, 255); }
      break;
    default:
      for(int i = 0; i < sizeof(groupF); i++) { ledsMain[groupF[i]] = CHSV(gHue, 255, 255); }
      break;
  }
  */
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}
