/* New-Haven-Display library by Brian Beardmore
 */

#include "New-Haven-Display.h"

/**
 * Constructor.
 */
NewHavenDisplay::NewHavenDisplay()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Initialize the display - specify the number of rows and columns
 */
void NewHavenDisplay::begin()
{
  byte rows = 0x08;

  Wire.begin();                             // Initiate the Wire library and join the I2C bus as a master
  delay(10);                                // Waits 10 ms for stabilization purpose

  if (ROW_N == 2 || ROW_N == 4) rows = 0x08;                    // Display mode: 2/4 lines
  else rows = 0x00;                    // Display mode: 1/3 lines

  command(0x22 | rows); // Function set: extended command set (RE=1), lines #
  command(0x71);        // Function selection A:
  data(0x5C);           //  enable internal Vdd regulator at 5V I/O mode (def. value) (0x00 for disable, 2.8V I/O)
  command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
  command(0x08);        // Display ON/OFF control: display off, cursor off, blink off (default values)
  command(0x22 | rows); // Function set: extended command set (RE=1), lines #
  command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
  command(0xD5);        // Set display clock divide ratio/oscillator frequency:
  command(0x70);        //  divide ratio=1, frequency=7 (default values)
  command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)

  if (ROW_N > 2)
    command(0x09);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 3/4 lines
  else
    command(0x08);  // Extended function set (RE=1): 5-dot font, B/W inverting disabled (def. val.), 1/2 lines

  command(0x06);        // Entry Mode set - COM/SEG direction: COM0->COM31, SEG99->SEG0 (BDC=1, BDS=0)
  command(0x72);        // Function selection B:
  data(0x0A);           //  ROM/CGRAM selection: ROM C, CGROM=250, CGRAM=6 (ROM=10, OPR=10)
  command(0x79);        // OLED characterization: OLED command set enabled (SD=1)
  command(0xDA);        // Set SEG pins hardware configuration:
  command(0x10);        //  alternative odd/even SEG pin, disable SEG left/right remap (default values)
  command(0xDC);        // Function selection C:
  command(0x00);        //  internal VSL, GPIO input disable
  command(0x81);        // Set contrast control:
  command(0x7F);        //  contrast=127 (default value)
  command(0xD9);        // Set phase length:
  command(0xF1);        //  phase2=15, phase1=1 (default: 0x78)
  command(0xDB);        // Set VCOMH deselect level:
  command(0x40);        //  VCOMH deselect level=1 x Vcc (default: 0x20=0,77 x Vcc)
  command(0x78);        // OLED characterization: OLED command set disabled (SD=0) (exit from OLED command set)
  command(0x20 | rows); // Function set: fundamental command set (RE=0) (exit from extended command set), lines #
  command(0x01);        // Clear display
  delay(2);             // After a clear display, a minimum pause of 1-2 ms is required
  command(0x80);        // Set DDRAM address 0x00 in address counter (cursor home) (default value)
  command(0x0C);        // Display ON/OFF control: display ON, cursor off, blink off
  delay(250);           // Waits 250 ms for stabilization purpose after display on

  if (ROW_N == 2) new_line[1] = 0xC0;             // DDRAM address for each line of the display (only for 2-line mode)
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
* Example private method
*/
void NewHavenDisplay::doit()
{
    Serial.println("called doit");
}
