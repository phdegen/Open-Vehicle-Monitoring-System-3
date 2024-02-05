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
static const OvmsVehicle::poll_pid_t vw_eupegolf_polls[] = {
//***TX-ID, ***RX-ID, ***SID, ***PID, { Polltime (seconds) for Pollstate 0, 1, 2, 3}, ***CAN BUS Interface, ***FRAMETYPE

//Battery Management
{ 0x7E5, 0x7ED, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1E3B, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // High-Volt battery voltage
{ 0x7E5, 0x7ED, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1E3D, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // High-Volt battery current
{ 0x7E5, 0x7ED, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x028C, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // SoC 
{ 0x7E5, 0x7ED, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x02BD, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Odometer
{ 0x7E5, 0x7ED, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2A0B, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // High-Volt battery temp
//{ 0x7E5, 0x7ED, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1E40, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Cell voltage base
//{ 0x7E5, 0x7ED, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1EAE, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Cell temperature base

//Battery Charge Management (/Charger)
{ 0x765, 0x7CF, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1DDA, { 0, 1, 10, 10 }, 0, ISOTP_STD }, // Steckdose A für Batterieladung

//Battery Charge
{ 0x744, 0x7AE, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x15D6, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Charging power efficiency
{ 0x744, 0x7AE, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x15E1, { 0, 10, 10, 10 }, 0, ISOTP_STD }, // Charging power loss


POLL_LIST_END
};