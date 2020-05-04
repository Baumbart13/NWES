#include <Adafruit_NeoPixel.h>

#define RGB(r,g,b) (((uint32_t)r << 16) | ((uint32_t)g <<  8) | b)
#define string String

//#define DEBUG

#define PIN_LED 6
#define PIXELS 30

// LED-Stripe
Adafruit_NeoPixel stripe(PIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);

// Serial communication
bool Serial_executeOrder = false;
int Serial_value = 0;

void Serial_setPixels();
void Serial_update();


void Serial_setPixels(){
	stripe.clear();

#ifdef WHILE
	uint32_t i = 0;
	while(true){
		if(i >= Serial_value || i >= stripe.numPixels()){
			break;
		}else{
			stripe.setPixelColor(i, RGB(0,128,0));
		}
		++i;
	}
#else
	for(uint32_t i = 0; i < Serial_value; ++i){
		stripe.setPixelColor(i, RGB(0,128,0));
	}
#endif
	
	if(Serial_value > stripe.numPixels()){
		Serial.print("Input is higher than "); Serial.println(stripe.numPixels());
	}else if(Serial_value < 0){
		Serial.println("Input is too low");
	}

	stripe.show();
	return;
}

void Serial_update(){
	static string command = "";
	Serial_executeOrder = false;
	char currentChar;

	if(Serial.available()){
		currentChar = Serial.read();

		if(currentChar >= '0' and currentChar <= '9'){
#ifdef DEBUG
			Serial.write(currentChar);Serial.write('\n');
#endif
			command += currentChar;

		}else if(currentChar == '\n'){
			Serial_value = command.toInt();
#ifdef DEBUG
			Serial.print("Serial_value: "); Serial.println(Serial_value);
			Serial.print("command: "); Serial.println(command);
#endif
			command = "";
			Serial_executeOrder = true;
		}
	}
}

void setup() {
	stripe.begin();
	stripe.clear();
	stripe.show();
	Serial.begin(9600);
}

void loop() {
	Serial_update();
	if(Serial_executeOrder){
		Serial_setPixels();
	}
}
