#include <WioLTEforArduino.h>
#include <GroveMiniMoto.h>  // Include the MiniMoto library

WioLTE Wio;

// Create two MiniMoto instances, with different address settings.
// Since Wire library takes a 7-bit i2c address, the original addresses are shifted to right by 1 bit.
MiniMoto motor0(0x65);  // 0x65 = (0xCA >> 1)
MiniMoto motor1(0x60);  // 0x60 = (0xC0 >> 1)
WioLTE Wio;


// Nothing terribly special in the setup() function- prep the
//  serial port, print a little greeting, and set up our fault
//  pin as an input.
void setup()
{
  SerialUSB.println("MiniMoto Test");
  Wio.Init(); // Don't forget this line or it won't work
  Wio.PowerSupplyGrove(true); // Don't forget this line or it won't work
  delay(100);
  
  motor0.begin();
  motor1.begin(); // technically you don't need the second begin().
  delay(100);
}

// The loop() function just spins the motors one way, then the
//  other, while constantly monitoring for any fault conditions
//  to occur. If a fault does occur, it will be reported over
//  the serial port, and then operation continues.
void loop()
{
  SerialUSB.println("Forward!");
  motor0.drive(100);
  motor1.drive(100);
  delayUntil(1000);
  SerialUSB.println("Stop!");
  motor0.stop();
  motor1.stop();
  delay(1000);
  SerialUSB.println("Reverse!");
  motor0.drive(-100);
  motor1.drive(-100);
  delayUntil(1000);
  SerialUSB.println("Brake!");
  motor0.brake();
  motor1.brake();
  delay(1000);
}

// delayUntil() is a little function to run the motor either for
//  a designated time OR until a fault occurs. Note that this is
//  a very simple demonstration; ideally, an interrupt would be
//  used to service faults rather than blocking the application
//  during motion and polling for faults.
void delayUntil(unsigned long elapsedTime)
{
  // See the "BlinkWithoutDelay" example for more details on how
  //  and why this loop works the way it does.
  unsigned long startTime = millis();
  delay(elapsedTime);

  // We're going to check both motors; the logic is the same
  //  for each...
  byte result = motor0.getFault();
  // If result masked by FAULT is non-zero, we've got a fault
  //  condition, and we should report it.
  if (result & FAULT)
  {
    SerialUSB.print("Motor 0 fault: ");
    if (result & OCP) SerialUSB.println("Chip overcurrent!");
    if (result & ILIMIT) SerialUSB.println("Load current limit!");
    if (result & UVLO) SerialUSB.println("Undervoltage!");
    if (result & OTS) SerialUSB.println("Over temp!");
  }
  result = motor1.getFault();
  if (result & FAULT)
  {
    SerialUSB.print("Motor 1 fault: ");
    if (result & OCP) SerialUSB.println("Chip overcurrent!");
    if (result & ILIMIT) SerialUSB.println("Load current limit!");
    if (result & UVLO) SerialUSB.println("Undervoltage!");
    if (result & OTS) SerialUSB.println("Over temp!");
  }
}
