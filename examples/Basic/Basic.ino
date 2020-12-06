
//=======================================================================//

// --- Pretty tiny Scheduler ---

// Pretty tiny Scheduler is an Arduino library for writing non-blocking
// periodic tasks without using delay() or millis() routines.

// Author : Vishnu Mohanan (@vishnumaiea)
// Version : 1.1.0
// License : MIT
// Src : https://github.com/vishnumaiea/ptScheduler

// Last modified : +05:30 01:20:23 PM 06-12-2020, Sunday

//=======================================================================//
//description

//This is a basic usage example of ptScheduler. It creates four tasks.
//Two tasks, sayHello and sayName simply print to the serial monitor at
//specified intervals. The other two tasks, basicBlink and multiBlink
//blinks two LEDs. The basicBlink blinks an LED every second indefinitely.
//The multiBlink blinks the LED for three times in a burst manner. It is
//written to stop after three blinks. But it is enabled again to blink
//every three seconds. The multiBlink is added as a function you can invoke
//from the loop function.

//=======================================================================//
//includes

#include "ptScheduler.h"

//=======================================================================//
//defines

#define LED1 LED_BUILTIN
#define LED2 12

//=======================================================================//
//globals

//create tasks
ptScheduler sayHello = ptScheduler(1000);
ptScheduler sayName = ptScheduler(-3000);
ptScheduler basicBlink = ptScheduler(1000);
ptScheduler multiBlink = ptScheduler(100);

uint8_t ledOn = false;  //a var to toggle the LED state

//=======================================================================//
//setup function runs once

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  Serial.print("\n-- ptScheduler --\n\n");
}

//=======================================================================//
//infinite loop

void loop() {
  //executed every second
  if (sayHello.call()) {
    Serial.println("Hello World");
  }
  
  //skips first time and executed every 3 seconds
  if (sayName.call()) {
    Serial.println("I am ptScheduler");
    multiBlink.enable();
  }
  
  //toggles LED every second
  if (basicBlink.call()) {
    digitalWrite (LED2, !digitalRead(LED2));
  }
  
  //task as a function
  blinker();
}

//=======================================================================//
//blinks an LED three times when enabled and stops.
//enable again to blink. the same function can be easily implemented with
//an iterated task.

void blinker() {
  if (multiBlink.call()) {
    if (multiBlink.executionCounter == 1) {  //task counter starts at 1
      ledOn = true;
    }
    else {
      ledOn = !ledOn; //toggle state
    }

    if (ledOn) {
      digitalWrite (LED1, HIGH);
    }

    else {
      digitalWrite(LED1, LOW);
    }

    if (multiBlink.executionCounter == 6) {  //it requires 6 intervals for 3 blinks
      multiBlink.disable();
      multiBlink.executionCounter = 0; //reset task counter
      digitalWrite(LED1, LOW);  //bring the LED to idle state
    }
  }
}

//=======================================================================//

