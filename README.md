# RFIDSafe
Simple RFID safe conversion project

I decided to convert my cheap Walmart keypad safe to work via RFID (either keycards or implants), by using an Arduino UNO and a PN532 breakout board (along with an N-channel MOSFET to drive the solenoid).

The code is incredibly simple, most of it is just the base example for UIDs from the Adafruit PN532 library. Figured this could still help some people who want to do similar projects.

Parts List:
Any safe with a simple solenoid unlock system, with a voltage directly matching that of your safe's power supply (6 volts in my case, through 4 AA batteries)
~210 ohm resistor (just has to be close in value, it stops the MOSFET from pulling too much current from the Uno)
Arduino Uno
PN532 NFC breakout board (mine is a cheap Chinese one from eBay)
N-channel MOSFET with less than 5V gate threshold (I used a SM3113NSU because I had one on hand)
A ton of wire. If you want to make your life easy, use differently colored wire if possible.
Connections/Wiring:
Power supply of safe positive-Arduino VIN (Ensure power supply/batteries for your safe outputs ~6-12 volts. Official documents say 7-12, but 6 works just fine. If you have a good 
Power supply of save negative-Arduino GND

PN532 VCC-Arduino 5V
PN532 GND-Arduino GND
PN532 SDA-Arduino A4
PN532 SCL-Arduino A5
PN532 RSTO-Arduino 3
PN532 IRQ-Arduino 2

MOSFET gate-220ohm resistor-Arduino 9
MOSFET source-Arduino GND/Power supply ground (should be the same in this case)
MOSFET drain-ground/negative input on solenoid

Solenoid positive-Power supply voltage/positive

After wiring everything, upload to your Arduino, then look in the serial monitor. Tap your card and copy the hex values. Then go to the code, and add them to an array of uint8_t. I've added leftHand, rightHand, and spareCard for implantee use, but you can use whatever fits the byte length of your card/chip. If you don't want to try adding your own array for your card, you can use the ones I've already added. For a 7 byte UID you can use either leftHand, rightHand, or vivoHand. If you need a 4 byte, you can add it to spareCard. Add it in a similar format as the example arrays, as comma seperated entries. I will likely make this more user friendly in the future, but for the moment, feel free to play around. It should be pretty easy to understand the way I'm verifying UIDs.

Full write-up available here: https://forum.dangerousthings.com/t/all-compromises-safe-conversion-project-for-less-than-9/

More info (including pictures) coming soon.
