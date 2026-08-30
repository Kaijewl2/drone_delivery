/**********************************************************************
 * This code is intended to support the command and control of the
 * Customizable Light-Duty Aerial Recovery Kit (CLARK-3), an open-source
 * 3D printable drone based winch capable of supporting payloads up to
 * 3kg.  Specifically, this code reads a hall effect sensor that is
 * triggered by a magnet embedded within the winch's shaft.  The amount
 * of line deployed can then be calculated based on the winch's
 * direction (determined by measuring the PWM signal sent to the winch)
 * as well as the shaft circumference.  The code also supports a limit
 * switch that resets the zero position and stops further retraction.
 * Currently, the winch speed is hard-coded here but could be
 * expanded to accept different input signals to allow for multiple
 * discreet speeds.
 *
 * The code leverages the MAVLink protocol's built-in winch status
 * message to report the line out to the operator (i.e. via ground
 * control software such as QGroundControl or MissionPlanner).
 *
 * This code is used in conjunction with the MAVLink protocol
 * whose source code is available here: https://github.com/mavlink/c_library_v2
 *
 * Additionally, this code was developed using a number of tutorials and
 * examples including:
 * https://github.com/patrickpoirier51/TFMINI-POC/blob/master/TFMINI_I2C_MAVLINK/TFMINI_I2C_MAVLINK.ino
 * (Patrick Poirier) https://discuss.ardupilot.org/t/mavlink-step-by-step/9629
 * (Pedro Albuquerque)
 *
 * The hardware used for the development of this control code was
 * an Arduino Pro Mini (5V 16MHz).  Please note that the serial lines
 * used for programming are also used to communicate to the autopilot.
 * It is important to disconnect the autopilot in order to program the
 * Arduino.
 *
 * Copyright 2024 Kyle Luthy
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 **********************************************************************/

/*****Includes*****/
#include <Arduino.h>
#include <Servo.h> // includes the standard Arduino servo library for control of the motor (needed to allow the Arduino to disable the motor when the limit switch is triggered - otherwise, the PWM signal could be used straight from the autopilot).
#include <ardupilotmega/mavlink.h> // includes the MAVLink library as specified here:  https: //github.com/mavlink/c_library_v2

/******************************CONSTANTS******************************/

// Communication constants
#define BRATE 115200 // Baudrate to communicate telemetry.
#define DATA_FREQ                                                              \
  1000 // The length of time, in milliseconds, at which the winch status is
       // relayed via MavLink.

// Define the positive and negative PWM values - here there is a deadband
// between the two - currently, the switch results in a fixed PWM/Speed for
// either direction
#define POSITIVE_PWM                                                           \
  1600 // The pulse width threshold for positive, or clockwise, servo rotation -
       // redefine per your servo specs
#define NEGATIVE_PWM                                                           \
  1400 // The pulse width threshold for negative, or counterclockwise, servo
       // rotation - redefine per your servo specs

// Winch measurement parameters
#define SPOOL_CIRCUMFERENCE                                                    \
  2 // This is the circumference, in cm, of the winch spool.  Currently, the
    // width change due to line is not accounted for.

// Define winch signal pins
#define HALL_EFFECT_PIN                                                        \
  2 // Defines the interrupt pin to which the Hall Effect sensor is connected.
#define SWITCH_PIN 3 // Defines the pin that the limit switch is connected to
#define PWM_PIN                                                                \
  9 // Defines the pin to which the PWM signal from the autopilot is connected.
#define SERVO_PWM_PIN                                                          \
  5 // Defines the pin which the PWM line of the winch's continuous servo is
    // connected.

/******************************VARIABLES******************************/

// Winch signal
volatile byte detected =
    LOW; // Whether or not the hall effect sensor has registered a rotation
long int rotationCount =
    0; // This is the cumulative count of positive rotations of the winch spool.
       // For every outward rotation, it is increased by 1.  It is decreased by
       // 1 for every retration rotation.
unsigned long pulseDuration =
    0; // This is a measure of the duration, in milliseconds of the PWM signal
       // coming from the controller.  Knowing the clockwise, stop, and
       // counterclockwise pulsewidths for the continuous rotation servo allows
       // us to determine the direction of travel.  The clockwise and
       // counterclockwise limits are defined as constants.

// Communication timing variables
unsigned long int lastHeartbeat =
    0; // The time, in milliseconds that the heartbeat was sent.
unsigned long int lastWinchStatus =
    0; // The time, in milliseconds when the data was sent.

// Winch motor variable
Servo winchMotor; // The winch's motor (a continuous servo for the CLARK-3).

/******************************SEND HEARTBEAT******************************/
/* This code provides a heartbeat signal to the autopilot.
 * See https://mavlink.io/en/services/heartbeat.html for more detail on
 * heartbeats in MAVLink.
 */
void sendHeartbeat() {
  int sysid = 1; // ID 1 for this system
  int compid =
      MAV_COMP_ID_WINCH; // The winch component ID is 169 - see
                         // https://mavlink.io/en/messages/minimal.html#MAV_COMP_ID_WINCH
  uint8_t system_type = MAV_TYPE_WINCH; // Specifies that this is a winch.
  uint8_t autopilot_type =
      MAV_AUTOPILOT_INVALID; // This component isn't an autopilot so it is
                             // registered as invalid
  uint8_t system_mode = 0; // system mode, custom mode, and system state can all
                           // be 0 for the heartbeat
  uint32_t custom_mode = 0;
  uint8_t system_state = 0;

  // Define the message data structures
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Pack the message
  mavlink_msg_heartbeat_pack(sysid, compid, &msg, system_type, autopilot_type,
                             system_mode, custom_mode, system_state);

  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

  // Send the message
  Serial.write(buf, len);
}

/******************************SEND WINCH STATUS******************************/
/* This code populates the line length in a winch status message and sends it
 * out to the system.
 */
void sendWinchStatus(uint16_t lineOut) {
  int sysid = 1;
  int compid =
      MAV_COMP_ID_WINCH; // The winch component ID is 169 - see
                         // https://mavlink.io/en/messages/minimal.html#MAV_COMP_ID_WINCH
  uint32_t timeBootms = millis(); // Time since boot in milliseconds

  // Define the message data structures
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Pack the message
  mavlink_msg_winch_status_pack(sysid, compid, &msg, timeBootms, lineOut, NULL,
                                NULL, NULL, NULL, INT16_MAX, 0);

  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

  // Send the message
  Serial.write(buf, len);
}

/******************************countRotation******************************/
/* This code increments or decrements the rotation count based on the
 * direction of rotation defined by the PWM signal.  To do so, the code
 * counts the pulse width and determines whether it is above the positive
 * direction threshold (+1) or below the negative direction threshold (-1).
 */
void countRotation() {
  detected = LOW; // Reset the detection variable

  // figure out the direction of travel based on pulse duration
  pulseDuration = pulseIn(
      PWM_PIN,
      HIGH); // pulseIn returns the high time of a pulse which, in this case, is
             // the PWM high time that defines the winch direction
  if (pulseDuration >=
      POSITIVE_PWM) { // if the pulseDuration is >= the positive direction
                      // defined by the continuous servo specs, it is releasing
                      // line and the rotationCount will increase
    rotationCount++;
  } else if (pulseDuration <=
             NEGATIVE_PWM) { // if the pulseDuration is <= the negative
                             // direction defined by the continuous servo specs,
                             // it is retracting line and the rotationCount will
                             // decrease
    rotationCount--;
  }
  if ((rotationCount < 0) ||
      (digitalRead(
          SWITCH_PIN))) { // if the rotationCount is < 0, meaning it attempted
                          // to retract further than the line available, then
                          // the rotationCount should remain 0 and the winch
                          // should stop.  Likewise, if the limit switch has
                          // been triggered, the rotation count should be set to
                          // 0.
    rotationCount = 0;
  }
}

/******************************hallEffectTriggered
 * ISR******************************/
/* To minimize the time spent in the ISR, this simply sets a detected flag,
 * which is checked in the main loop countRotation call.
 */
void hallEffectTriggered() { detected = HIGH; }

/******************************INITIAL SETUP******************************/
/* This code only runs at the beginning of code execution and configures
 * peripherals for the controller.
 */
void setup() {
  Serial.begin(
      BRATE); // Sets serial the communication baud rate between the winch and
              // the autopilot - this needs to be set the same on the autopilot!
              // For example, Telem2 on the Pixhawk 2 is mapped to Serial2 and
              // should be adjusted to 115.
  attachInterrupt(
      digitalPinToInterrupt(HALL_EFFECT_PIN), hallEffectTriggered,
      FALLING); // assigns an interrupt routine to the hall effect sensor
  pinMode(
      PWM_PIN,
      INPUT); // Sets the PWM_PIN to input in order to determine PWM high time.
  winchMotor.attach(SERVO_PWM_PIN);
  pinMode(LED_BUILTIN, OUTPUT); // Allows the built in LED in the Arduino to be
                                // used for debugging purposes.
  digitalWrite(LED_BUILTIN,
               LOW); // Ensures that the built in LED is initially off.
}

/******************************MAIN LOOP******************************/
/* The loop sends a heartbeat every second and winch status messages
 * at a defined rate.
 */
void loop() {

  // Heartbeats are sent every second (1000ms).
  if ((millis() - lastHeartbeat) >
      1000) {                 // Check to see if it is time to send a heartbeat.
    sendHeartbeat();          // Send a heartbeat.
    lastHeartbeat = millis(); // Keep track of last heartbeat time.
  }

  // Winch Status messages are sent every DATA_FREQ ms
  if ((millis() - lastWinchStatus) >
      (DATA_FREQ)) { // The lineOut is sent every DATA_FREQ ms.
    sendWinchStatus(
        rotationCount *
        SPOOL_CIRCUMFERENCE); // Send the winch status with the line out value
                              // detemined by the number of rotations and the
                              // winch spool circumference
    lastWinchStatus =
        millis(); // Keep track of the last winch status message time.
  }

  // Set the PWM to drive the winch motor
  pulseDuration = pulseIn(
      PWM_PIN, HIGH); // Determine the PWM pulse width from the autopilot.
  if (pulseDuration >
      POSITIVE_PWM) { // If the pulse width meets the positive criterion defined
                      // by POSITIVE_PWM then output a clockwise PWM.
    winchMotor.write(20);
  } else if ((pulseDuration < NEGATIVE_PWM) &&
             (!digitalRead(
                 SWITCH_PIN))) { // If the pulse width meets the negative
                                 // criterion defined by NEGATIVE_PWM and the
                                 // limit switch is not activated then output a
                                 // counter-clockwise PWM.
    winchMotor.write(160);
  } else { // If neither criterion for motion is met, then set the winch motor
           // to have no motion.
    winchMotor.write(90);
  }

  // Checks whether or not the hall effect sensor has registered a count (via
  // the hallEffectTriggered ISR)
  if (detected) {
    countRotation(); // Updates the rotation count depending on the direction
                     // the winch is spooling.
  }
}
