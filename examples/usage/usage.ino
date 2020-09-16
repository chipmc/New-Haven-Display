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
}
