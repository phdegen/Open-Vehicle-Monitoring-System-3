/*
;    Project:       Open Vehicle Monitor System
;    Date:          11.10.23
;
;    (C) 2023       Philippe Degen
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/

// Pollstate 0 - POLLSTATE_OFF      - car is off
// Pollstate 1 - POLLSTATE_ON       - car is on
// Pollstate 2 - POLLSTATE_DRIVING  - car is driving
// Pollstate 3 - POLLSTATE_CHARGING - car is charging
static const OvmsVehicle::poll_pid_t nl_ze1_polls[] = {
//***TX-ID, ***RX-ID, ***SID, ***PID, { Polltime (seconds) for Pollstate 0, 1, 2, 3}, ***CAN BUS Interface, ***FRAMETYPE


// BMS / LBC (Lithium battery controller)
{ 0x79B, 0x7BB, VEHICLE_POLL_TYPE_OBDIIGROUP, 0x01, { 0, 30, 30, 30 }, 0, ISOTP_STD }, // Group 1: Health, SOC, HV current 1/2, HV voltage
{ 0x79B, 0x7BB, VEHICLE_POLL_TYPE_OBDIIGROUP, 0x02, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Group 2: Cell voltages
{ 0x79B, 0x7BB, VEHICLE_POLL_TYPE_OBDIIGROUP, 0x04, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Group 2: Packs temperatures


// ECU 1 / HVAC
//{ 0x744, 0x764, VEHICLE_POLL_TYPE_OBDIIGROUP, 0x01, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Heater temp. and fan speed

// ECU 2 / Meter
//{ 0x745, 0x765, VEHICLE_POLL_TYPE_OBDIIGROUP, 0x09, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Lever, wiper and light status

// ECU 3 / VCM
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x115D, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Ambient temperature, °C
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1234, { 0, 1, 10, 10 }, 0, ISOTP_STD }, // Plug state 
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x114E, { 0, 30, 30, 30 }, 0, ISOTP_STD }, // Charge mode
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1236, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Charging power (on-board charger) W
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1146, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Motor power W
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x121A, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Speed, km/h
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1106, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // AC status
{ 0x797, 0x79A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1151, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // HVAC power

// ECU 4 / BCM
{ 0x743, 0x763, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0E01, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Odometer, km
{ 0x743, 0x763, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0E24, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Range, km
{ 0x743, 0x763, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0E25, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Tire pressure FR, kPa
{ 0x743, 0x763, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0E26, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Tire pressure FL, kPa
{ 0x743, 0x763, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0E27, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Tire pressure RR, kPa
{ 0x743, 0x763, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0E28, { 0, 60, 60, 60 }, 0, ISOTP_STD }, // Tire pressure RL, kPa



POLL_LIST_END
};