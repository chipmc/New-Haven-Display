/* New-Haven-Display library adapted from NHD sample code by Chip McClelland
* This library is specific to the New Haven Display NHD-0402CW-AY3 which is a 4x20 OLED
* This library is specific to controlling the display on i2c but other options are available
* See https://github.com/newhavendisplay for more examples
* This display uses a NHD_US2066 controller, this repo may have more good bits: https://github.com/NewhavenDisplay/NHD_US2066
* I am not using the reset pin in this library
* This libary assumes 2.8-3.3V operation - see the Vdd regulator line in initialization if you want to power at 5V
 */

#include "New-Haven-Display-4x20-i2c.h"


NewHavenDisplay::NewHavenDisplay()      // Constructor
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

// Initialize the display - This code is specific to teh 4 row / 20 column I2C display
void NewHavenDisplay::begin()
{

  Wire.begin();                             // Initiate the Wire library and join the I2C bus as a master
  delay(10);                                // Waits 10 ms for stabilization purpose

  command(0x2A);        // Function set: extended command set (RE=1)
    command(0x71);      // Function selection A:
    data(0x00);         // disable internal Vdd regulator (0x00 for disable, 2.8V I/O)
  command(0x28);        // Function set: fundamental command set (RE=0) (exit from extended command set)
  command(0x08);        // Display ON/OFF control: display off, cursor off, blink off (default values)
  command(0x2A);        // Function set: extended command set (RE=1)
  command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
  command(0xD5);        // Set display clock divide ratio/oscillator frequency:
  command(0x70);        //  divide ratio=1, frequency=7 (default values)
  command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
    command(0x09);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 3/4 lines
  //  command(0x08);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 1/2 lines
  command(0x06);        // Entry Mode set - COM/SEG direction: COM0->COM31, SEG99->SEG0 (BDC=1, BDS=0)
    command(0x72);        // Function selection B:
    data(0x00);           //  ROM/CGRAM selection
  command(0x2A);  //function set (extended command set)
  command(0x79);  //OLED command set enabled
  command(0xDA);  //set SEG pins hardware configuration
  command(0x10);  //set SEG pins hardware configuration
  command(0xDC);  //function selection C
  command(0x00);  //function selection C
  command(0x81);  //set contrast control
  command(0x7F);  //set contrast control
  command(0xD9);  //set phase length
  command(0xF1);  //set phase length
  command(0xDB);  //set VCOMH deselect level
  command(0x40);  //set VCOMH deselect level
  command(0x78);  //OLED command set disabled
  command(0x28);  //function set (fundamental command set)
  command(0x01);  //clear display
  command(0x80);  //set DDRAM address to 0x00
  command(0x0C);  //display ON
  delay(100);
}

/**
* Clear the Display
*/
void NewHavenDisplay::clear() {
   command(0x01);        // Clear display
}

/**
 * Prepares the transmission of a command
 */
void NewHavenDisplay::command(byte c)
{
   tx_packet[0] = 0x00;               // Control Byte; C0_bit=0, D/C_bit=0 -> following Data Byte contains command
   tx_packet[1] = c;                  // Data Byte: the command to be executed by the display
   send_packet(2);                    // Transmits the two bytes
}

/**
 * Prepares the transmission of data
 */
void NewHavenDisplay::data( byte d)
{
   tx_packet[0] = 0x40;               // Control Byte; C0_bit=0, D/C_bit=1 -> following Data Byte contains data
   tx_packet[1] = d;                  // Data Byte: the character to be displayed
   send_packet(2);                    // Transmits the two bytes
}

/**
 * Send to the display the x bytes stored in tx_packet
 */
void NewHavenDisplay::send_packet( byte x)
{
   byte ix = 0;                       // Bytes index

   Wire.beginTransmission(SLAVE2W);   // Begin the transmission via I2C to the display with the given address
   for(ix=0; ix<x; ix++)              // One byte at a time,
   {
      Wire.write(tx_packet[ix]);      //  queue bytes for transmission
   }
   Wire.endTransmission();            // Transmits the bytes that were queued
}

/**
 * Send to the display the x bytes stored in tx_packet
 */
void NewHavenDisplay::splashScreen()
{
   byte r = 0;                        // Row index
   byte c = 0;                        // Column index

   command(0x01);                     // Clears display (and cursor home)
   delay(2);                          // After a clear display, a minimum pause of 1-2 ms is required

   for (r=0; r<ROW_N; r++)            // One row at a time,
   {
      command(new_line[r]);           //  moves the cursor to the first column of that line
      for (c=0; c<COLUMN_N; c++)      // One character at a time,
      {
         data(TEXT[r][c]);            //  displays the correspondig string
      }
   }
}

/**
 * Send to the display the x bytes stored in tx_packet
 */
void NewHavenDisplay::updateRow( byte targetRow, const char *newRow)
{
   byte r = 0;                        // Row index
   byte c = 0;                        // Column index

   command(0x01);                     // Clears display (and cursor home)
   delay(2);                          // After a clear display, a minimum pause of 1-2 ms is required

  for (c = 0; c<COLUMN_N; c++) {
    TEXT[targetRow][c] = newRow[c];
  }

   for (r=0; r<ROW_N; r++)            // One row at a time,
   {
      command(new_line[r]);           //  moves the cursor to the first column of that line
      for (c=0; c<COLUMN_N; c++)      // One character at a time,
      {
         data(TEXT[r][c]);            //  displays the correspondig string
      }
   }
}


/**
 * Takes in a row of text and displays it
*/
void NewHavenDisplay::updateCell(byte targetROW, byte targetCOL, const char targetCHAR) {
   command(0x80 | (targetCOL + new_line[targetROW]));
   delay(2);
   data(targetCHAR);
}


/**
* Example private method
*/
void NewHavenDisplay::doit()
{
    Serial.println("called doit");
}
