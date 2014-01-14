#include "GPIO.h"
#include <stdio.h>

#define short_delay 7000

// A short µS delay 
void usDelay(int d) { // Measurements taken 5000 = 134,4µS   100000 = 192  23000 = 315,8   18000=271
	unsigned int i;
	for ( i = 1 ; i <= d ; i++ )
	{
		asm("");	
	}
}

void digitalWrite(int led, int state) {
	FILE *f1;
	f1 = fopen ("/sys/class/gpio/gpio29/value", "w");
	fprintf (f1, "%d", state);
	fclose (f1);
}

// set pin to in/out
void pinMode(int pin, int state) {
	FILE *f1;
	f1 = fopen ("/sys/class/gpio/export", "w");
	fprintf (f1, "%d", pin);
	fclose (f1);

	f1 = fopen ("/sys/class/gpio/gpio29/direction", "w");
	fprintf (f1, "out");
	fclose (f1);

	digitalWrite(pin, 0);
}

// Send three pulses
void A(void) {
	digitalWrite(29, HIGH);
	usDelay(short_delay);
	digitalWrite(29, LOW);
	usDelay(short_delay);
	digitalWrite(29, HIGH);
	usDelay(short_delay);
	digitalWrite(29, LOW);
	usDelay(short_delay);
	digitalWrite(29, HIGH);
	usDelay(short_delay);
	digitalWrite(29, LOW);
	usDelay(51000);
}

// Send two pulses
void B(void) {
	digitalWrite(29, HIGH);
	usDelay(short_delay);
	digitalWrite(29, LOW);
	usDelay(short_delay);
	digitalWrite(29, HIGH);
	usDelay(short_delay);
	digitalWrite(29, LOW);
	usDelay(51000);
}

// send one pulse
void C(void) {
	digitalWrite(29, HIGH);
	usDelay(short_delay);
	digitalWrite(29, LOW);
	usDelay(51000);
}

// Send the init pulses
void sendInit(void) {
	digitalWrite(29, HIGH);
 	usDelay(short_delay);
	digitalWrite(29, LOW);
	usDelay(100200); // mål = 2635µS 100200= 2705, 100150=2590

	B();B();C();A();C();B();B();A();C();A();C();A();C();A();B();B();B();C();A();B();B();B();B();B();C();A();B();	
}

void setChannel(int k) { 
	if (k==1) {
		B();B();B();C(); 
	}
   
	if (k==2) {
		B();B();C();B(); 
	}
}

// Turn the device off
void turnOff(int k) {
	unsigned int i;
	for ( i = 1 ; i <= 5 ; i++ ) {	 // Send it 5 times so we are sure it turns off
		sendInit();
		B();B(); // means off
		setChannel(k);		

		usDelay(510000); // A long delay in the end
	}
}

// Turn the divice on
void turnOn(int k) {
	unsigned int i;
	for ( i = 1 ; i <= 5 ; i++ ) {	// Send it 5 times so we are sure it turns on
		sendInit();
		C();A(); // means on
		setChannel(k);		

		usDelay(510000);    // A long delay in the end
	}
}
