#pragma once

/*
 * from Demo_NHD0420CW-Ax3_I2C.in
 * Datasheet for display: https://www.newhavendisplay.com/specs/NHD-0420CW-AY3.pdf
 * Application notes for this display: https://www.newhavendisplay.com/resources_dataFiles/datasheets/OLEDs/US2066.pdf
 *
 * Tutorial sketch for use of character OLED slim display family by Newhaven with Arduino Uno (Brian Beardmore: modifying for Particle Photon), using
 * only Wire (I2C) library.  Models: NHD0420CW-Ax3, NHD0220CW-Ax3, NHD0216CW-Ax3. Controller: US2066
 * in this example, the display is connected to Photon via I2C interface.
 *
 * Displays on the OLED alternately a 4-line message and a sequence of character "block".
 * This pgm ssumes the use of a 4x20 display; if different, modify the values of the two variables
 * ROW_N e COLUMN_N.
 * The pgm uses the minimum possible of Photon pins; if you intend to use also /RES line,
 * the related instructions are already present, it's sufficient to remove the comment markers.
 *
 * The circuit modified by Brian Beardmore for Particle Photon I2C:
 * OLED pin 1 (Vss)          to VSS ground
 * OLED pin 2 (VDD)          to 3.3V
 * OLED pin 3 (REGVDD)       to GND (not using 5V, docs say take to GND)
 * OLED pin 4 (SA0)          to VSS ground should use 0x3C address (to assign I2C address 0x3D, connect to VDD)
 * OLED pin 5 and 6          to VSS ground
 * OLED pin 7 (SCL)          to Photon D1 (SCL); 10K pull-up resistor on OLED pin to 3.3V? Do I need this?
 * OLED pin 8 and 9 (SDAin,SDAout are jumpered) to Photon D0 (SDA); 10K pull-up resistor on each OLED pin 8 and 9 to 3.3V? Do I need this?
 * OLED pin 10 to 15         to VSS ground
 * OLED pin 16 (/RES)        to VDD 3.3V
 * OLED pin 17 (BS0)         to VSS ground  ** I2C config BSO to GND (low)
 * OLED pin 18 (BS1)         to VDD 3.3V    ** I2C condig BS1 to 3.3V (high)
 * OLED pin 19 (BS2)         to Vss ground  ** I2C config Bs2 to GND (low)
 * OLED pin 20 (Vss)         to Vss ground
 *
 * Original example created by Newhaven Display International Inc.
 * Modified and adapted to Arduino Uno 15 Mar 2015 by Pasquale D'Antini
 * Modified 19 May 2015 by Pasquale D'Antini
 * Modified 06 Feb 2017 by Brian Beardmore for Particle Photon
 *
 * This example code is in the public domain.
 */

// This will load the definition for common Particle variable types
#include <Particle.h>

// This is your main class that users will import into their application
class NewHavenDisplay
{
public:
  /**
   * Constructor
   */
  NewHavenDisplay();

  /**
   * Initialize the display - specify the number of rows and columns
   */
  void begin();

  /**
   * Prepares for the transmission of a command
   */
  void command( byte c);

  /**
  * Clear the Display
  */
  void clear();

  /**
   * Prepares the tranmission of a byte of data
   */
  void data( byte d);

  /**
   * Send to the display the x bytes stored in tx_packet
   */
  void send_packet( byte x);

  /**
   * Displays the splash screen for startup
   */
  void splashScreen();

  /**
   * Takes in a row of text and displays it
   */
  void updateRow(byte targetROW, const char *newRow);

  /**
  * Displays a single character at a specific row and column on the screen
  */
  void updateCell(byte targetROW, byte targetCOL, const char targetCHAR);

private:
  /**
   * Define certain variables
   */
  void doit();

protected:                              // These variables must match your display's row / column and i2c address specifics
  const byte SLAVE2W = 0x3C;            // Display I2C address, in 7-bit form: 0x3C if SA0=LOW, 0x3D if SA0=HIGH
  const byte ROW_N = 4;                 // Number of display rows
  const byte COLUMN_N = 20;             // Number of display columns
  byte rows = 0x08;                     // Display mode: 1/3 lines or 2/4 lines; default 2/4 (0x08)

  byte new_line[4] = {0x80, 0xA0, 0xC0, 0xE0};   // DDRAM address for each line of the display
  byte tx_packet[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  // Packet to be transmitted (max 20 bytes)
  byte TEXT[4][21] = {"1-Newhaven Display--",
                      "2--This is a test---",
                      "3-16/20-Characters--",
                      "4!@#$%^&*()_+{}[]<>?"};         // Strings to be displayed

};
