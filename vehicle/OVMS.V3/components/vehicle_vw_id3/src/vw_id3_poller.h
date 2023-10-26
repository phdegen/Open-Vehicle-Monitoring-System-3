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


// DC-DC charger
//{ 0x17FC00B9, 0x17FCB900, VEHICLE_POLL_TYPE_READDATA, 0x465B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // DC-DC current
//{ 0x17FC00B9, 0x17FCB900, VEHICLE_POLL_TYPE_READDATA, 0x465D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // DC-DC voltage

// BMS
{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x028C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // SoC (BMS) ,%
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0xF40C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // motor speed, rpm
{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0xF40D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // Speed, km/h
{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x7448, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // Car operation mode, XX = 0 => standby, XX = 1 => driving, XX = 4 => AC charging, XX = 6 => DC charging
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x4219, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV bat on/off %
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x743B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // cirkulation pump HV battery - flow in %
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E33, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell # with highest voltage, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E34, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell # with lowest  voltage, V
{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E3B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery voltage, V
{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E3D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery current, A
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E0E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery max temp and temp point, °C and #
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E0F, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery min temp and temp point, °C and #
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1620, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // PTC heater battery current, A
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x189D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cooling liquid outlet, °C
////{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x189D?, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV battery cooling liquid inlet, °C  
{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x2A0B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp (main value), °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E32, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // Total accumulated charged and discharge
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x0500, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV battery serial
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E1B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // Dynamic limit for charging in ampere
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E1C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // Dynamic limit for discharging in ampere


//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EAE, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 1, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EAF, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 2, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB0, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 3, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB1, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 4, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB2, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 5, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB3, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 6, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB4, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 7, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB5, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 8, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB6, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 9, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB7, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 10, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB8, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 11, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EB9, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 12, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EBA, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 13, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EBB, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 14, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EBC, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 15, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EBD, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 16, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x7425, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 17, °C
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x7426, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery temp point 18, °C

//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E40, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 1, V
//{ 1x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E41, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 2, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E42, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 3, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E43, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 4, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E44, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 5, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E45, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 6, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E46, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 7, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E47, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 8, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E48, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 9, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E49, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 10, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E4A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 11, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E4B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 12, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E4C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 13, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E4D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 14, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E4E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 15, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E4F, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 16, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E50, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 17, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E51, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 18, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E52, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 19, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E53, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 20, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E54, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 21, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E55, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 22, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E56, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 23, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E57, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 24, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E58, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 25, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E59, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 26, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E5A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 27, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E5B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 28, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E5C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 29, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E5D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 30, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E5E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 31, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E5F, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 32, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E60, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 33, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E61, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 34, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E62, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 35, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E63, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 36, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E64, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 37, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E65, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 38, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E66, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 39, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E67, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 40, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E68, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 41, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E69, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 42, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E6A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 43, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E6B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 44, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E6C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 45, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E6D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 46, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E6E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 47, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E6F, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 48, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E70, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 49, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E71, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 50, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E72, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 51, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E73, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 52, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E74, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 53, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E75, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 54, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E76, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 55, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E77, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 56, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E78, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 57, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E79, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 58, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E7A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 59, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E7B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 60, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E7C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 61, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E7D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 62, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E7E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 63, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E7F, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 64, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E80, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 65, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E81, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 66, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E82, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 67, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E83, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 68, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E84, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 69, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E85, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 70, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E86, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 71, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E87, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 72, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E88, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 73, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E89, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 74, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E8A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 75, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E8B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 76, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E8C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 77, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E8D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 78, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E8E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 79, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E8F, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 80, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E90, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 81, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E91, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 82, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E92, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 83, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E93, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 84, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E94, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 85, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E95, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 86, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E96, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 87, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E97, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 88, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E98, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 89, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E99, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 90, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E9A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 91, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E9B, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 92, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E9C, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 93, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E9D, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 94, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E9E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 95, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1E9F, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 96, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA0, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 97, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA1, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 98, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA2, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 99, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA3, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 100, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA4, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 101, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA5, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 102, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA6, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 103, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA7, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 104, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA8, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 105, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EA9, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 106, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EAA, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 107, V
//{ 0x17FC007B, 0x17FE007B, VEHICLE_POLL_TYPE_READDATA, 0x1EAB, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME },  // HV Battery cell voltage - cell 108, V

// ECU 1
//{ 0x17FC0076, 0x17FE0076, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0364, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME }, // HV auxilary consumer power, kW
{ 0x17FC0076, 0x17FE0076, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x295A, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME }, // ODOMETER, km
//{ 0x17FC0076, 0x17FE0076, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x210E, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME }, // Driving mode position (P-N-D-B), YY=08->P,YY=05->D,YY=0c->B,YY=07->R,YY=06->N
{ 0x17FC0076, 0x17FE0076, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0xF802, { 10, 10, 10, 10 }, 0, ISOTP_EXTFRAME }, // VIN number

// ECU 2
//{ 0x767, 0x777, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x22B3, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // GPS time
//{ 0x767, 0x777, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2430, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // GPS multiframe data lat, long, init, height, quality
//{ 0x767, 0x777, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2431, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // GPS number of tracked and visual satellites

// ECU 3
{ 0x746, 0x7B0, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2613, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // Inside temperature, °C
{ 0x746, 0x7B0, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2609, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // Outside temperature,  °C
//{ 0x746, 0x7B0, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x263B, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // Recirculation of air, XX=00 -> fresh air, XX=04 -> manual recirculation
//{ 0x746, 0x7B0, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x42DB, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // CO2 content interior, ppm
//{ 0x746, 0x7B0, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0xF449, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // Accelerator pedal position, %
//{ 0x746, 0x7B0, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0801, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // PTC air heater inside, ampere
//{ 0x746, 0x7B0, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x0800, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // A/C compressor multiframe

// ECU 4 (/ gateway ?)
//{ 0x710, 0x77A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x15D5, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // Gateway charging active ?
{ 0x710, 0x77A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x084F, { 1, 1, 10, 10 }, 0, ISOTP_STD }, // Gateway charge plug locked 1= lock
//{ 0x710, 0x77A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2AB2, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // HV battery max energy content Wh
{ 0x710, 0x77A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2AB6, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // Gateway range display
////not confirmed{ 0x710, 0x77A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2AB8, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // HV battery energy content
//{ 0x710, 0x77A, VEHICLE_POLL_TYPE_OBDIIEXTENDED, 0x2AF7, { 10, 10, 10, 10 }, 0, ISOTP_STD }, // 12V multiframe



POLL_LIST_END
};