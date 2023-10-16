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
static const OvmsVehicle::poll_pid_t vw_id3_polls[] = {
//***TX-ID, ***RX-ID, ***SID, ***PID, { Polltime (seconds) for Pollstate 0, 1, 2, 3}, ***CAN BUS Interface, ***FRAMETYPE


{ 0x17FC007B, 0x17FC7B00, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x028C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },
{ 0x17FC007B, 0x17FC7B00, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0xF40D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },
{ 0x17FC007B, 0x17FC7B00, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x7448, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },
{ 0x17FC007B, 0x17FC7B00, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x743B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },
{ 0x17FC007B, 0x17FC7B00, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1E33, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },
{ 0x17FC007B, 0x17FC7B00, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x1E34, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },

{ 0x17FC0076, 0x17FC7600, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x295A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },

POLL_LIST_END
};