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

const char* ssid     = "xxxxx";
const char* password = "yyyyy";

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

uint32_t ms = 0;

#define HUE_A HUE_RED
#define HUE_B HUE_PURPLE
#define HUE_C HUE_ORANGE
#define HUE_D HUE_RED

CHSVPalette16 BLOOD(
                  CHSV(HUE_B + 0, 255, 128),
                  CHSV(HUE_A + 4, 255, 192),
                  CHSV(HUE_A + 8, 255, 192),
                  CHSV(HUE_A + 8, 64, 156),
                  CHSV(HUE_A,     96, 172),
                  CHSV(HUE_A,     255,255),
                  CHSV(HUE_A,     255,255),
                  CHSV(HUE_A,     255,128),
                  CHSV(HUE_A + 2, 255,128),
                  CHSV(HUE_A + 4, 255,255),
                  CHSV(HUE_A + 6, 255,255),
                  CHSV(HUE_A + 4, 255,128),
                  CHSV(HUE_A + 2, 255,128),
                  CHSV(HUE_A,     255,255),
                  CHSV(HUE_A,     255,255),
                  CHSV(HUE_A,     255,255));

CHSVPalette16 HARVEST(
                  CHSV(HUE_D + 0, 255, 128),
                  CHSV(HUE_D + 4, 255, 192),
                  CHSV(HUE_D + 8, 255, 192),
                  CHSV(HUE_D + 8, 64, 156),
                  CHSV(HUE_C,     96, 172),
                  CHSV(HUE_C,     255,255),
                  CHSV(HUE_C,     255,255),
                  CHSV(HUE_C,     255,128),
                  CHSV(HUE_C + 2, 255,128),
                  CHSV(HUE_C + 4, 255,255),
                  CHSV(HUE_C + 6, 255,255),
                  CHSV(HUE_C + 4, 255,128),
                  CHSV(HUE_C + 2, 255,128),
                  CHSV(HUE_C,     255,255),
                  CHSV(HUE_A,     255,255),
                  CHSV(HUE_A,     255,255));

uint8_t gHue = 0;
bool gReverseDirection = false;
CRGBPalette16 gPal = LavaColors_p;

void doWash_pattern(const CHSVPalette16& pal);

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
bool curMode = false;

void loop() {
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  EVERY_N_MILLISECONDS( 1000 ) { curIndex++;  curIndex = curIndex % 6; }
  EVERY_N_SECONDS( 600 ) { curMode = !curMode; }

  //fill_solid(ledsMain, NUM_LEDS_MAIN, CHSV(0, 255, 128));
  //for(int i = 0; i < sizeof(groupA); i++) { ledsMain[groupA[i]] = CHSV(49, 255, 128); }
  //for(int i = 0; i < sizeof(groupB); i++) { ledsMain[groupB[i]] = CHSV(49, 255, 128); }

  //if(curMode) {
  //  doWash_pattern(HARVEST);
  //} else {
  //  doWash_pattern(BLOOD);
  //}
  rainbow();
  addGlitter(1);

  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void rainbow() {
  fill_rainbow( ledsSide, NUM_LEDS_SIDE, gHue, 7);
  //fill_solid( ledsMain, NUM_LEDS_MAIN, CHSV(35, 80, 64));

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
}

void addGlitter(fract8 chanceOfGlitter) {
  if(random8() < chanceOfGlitter) {
    ledsMain[random16(NUM_LEDS_MAIN) ] += CRGB::White;
  }
}

void doWash_pattern(const CHSVPalette16& pal) {
    uint32_t ms = millis();
    
    int dtheta1 = 5300;
    uint16_t starttheta1 = ms * 15;
    uint16_t theta1 = starttheta1;

    int dtheta2 = 7300;
    uint16_t starttheta2 = ms * 17;
    uint16_t theta2 = starttheta2;

    int dtheta3 = 2300;
    uint16_t starttheta3 = ms * 3;
    uint16_t theta3 = starttheta3;

    int brightness = beatsin88(170, 96, 255);

    for(uint16_t i = 0; i < NUM_LEDS_MAIN; i++) {
        int s1 = sin16( theta1);
        theta1 += dtheta1;

        int s2 = sin16( theta2);
        theta2 += dtheta2;

        int s3 = sin16( theta3);
        theta3 += dtheta3;

        byte bri = ((s1 + 32768) >> 8);

        byte sat = ((s2 + 32768) >> 8);
        sat = (sat / 4) + 192;
        sat = brighten8_video(sat);
        byte desat = 255 - sat;
        desat = scale8( desat, 16);

        byte hue = ((s3 + 32768) >> 8);
        hue = scale8(hue, 240);
        
        CRGB rgb = ColorFromPalette( pal, hue, brightness);
        rgb += CRGB( desat, desat, desat);
        CRGB old = ledsMain[i];
        old.nscale8(192);
        rgb.nscale8_video(64);

        rgb += old;
        ledsMain[i] = rgb;
    }
}
