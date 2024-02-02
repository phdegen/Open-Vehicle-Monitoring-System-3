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

#ifndef __VEHICLE_NL_ZE1_H__
#define __VEHICLE_NL_ZE1_H__

#include <atomic>

#include "can.h"
#include "vehicle.h"


#define CAN_BYTE(b)     data[b]
#define CAN_UINT(b)     (((UINT)CAN_BYTE(b) << 8) | CAN_BYTE(b+1))
#define CAN_UINT24(b)   (((uint32_t)CAN_BYTE(b) << 16) | ((UINT)CAN_BYTE(b+1) << 8) | CAN_BYTE(b+2))
#define CAN_UINT32(b)   (((uint32_t)CAN_BYTE(b) << 24) | ((uint32_t)CAN_BYTE(b+1) << 16)  | ((UINT)CAN_BYTE(b+2) << 8) | CAN_BYTE(b+3))
#define CAN_NIBL(b)     (data[b] & 0x0f)
#define CAN_NIBH(b)     (data[b] >> 4)
#define CAN_NIB(n)      (((n)&1) ? CAN_NIBL((n)>>1) : CAN_NIBH((n)>>1))

enum poll_states
  {
  POLLSTATE_OFF,      //- car is off
  POLLSTATE_ON,       //- car is on
  POLLSTATE_RUNNING,  //- car is in drive/reverse
  POLLSTATE_CHARGING  //- car is charging
  };

using namespace std;

#define TAG (OvmsVehicleNLZE1::s_tag)

class OvmsVehicleNLZE1 : public OvmsVehicle {
  public:
    static const char *s_tag;

  public:
    OvmsVehicleNLZE1();
    ~OvmsVehicleNLZE1();
		void IncomingFrameCan1(CAN_frame_t* p_frame);
		void IncomingPollReply(canbus* bus, uint16_t type, uint16_t pid, uint8_t* data, uint8_t length, uint16_t remain);


	protected:
    void PollerStateTicker();
    void IncomingBMS(uint16_t type, uint16_t pid, const char* data, uint16_t len);
    void IncomingECU1(uint16_t type, uint16_t pid, const char* data, uint16_t len);
    void IncomingECU2(uint16_t type, uint16_t pid, const char* data, uint16_t len);
    void IncomingECU3(uint16_t type, uint16_t pid, const char* data, uint16_t len);
    void IncomingECU4(uint16_t type, uint16_t pid, const char* data, uint16_t len);
    
    		
    // NL ZE1 specific metrics
    OvmsMetricFloat  *mt_b_current2;      //HV Battery current 2
    OvmsMetricInt  *mt_e_hvac_power;      //HV Battery current 2
    
  protected:
    int pollstate_delay_ticker_s;
    string nl_ze1_obd_rxbuf;
    char nl_ze1_vin[15] = "";
};

#endif //#ifndef __VEHICLE_NL_ZE1_H__
