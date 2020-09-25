// Example usage for New-Haven-Display library by Brian Beardmore.

#include "New-Haven-Display.h"

// Initialize objects from the lib
NewHavenDisplay newHavenDisplay;

void setup() {
    // Call functions on initialized library objects that require hardware
    newHavenDisplay.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    newHavenDisplay.splashScreen();
    delay(2000);
    newHavenDisplay.clear();
    delay(1000);
    newHavenDisplay.updateCell(1,5,'+');
    delay(2000);
}
