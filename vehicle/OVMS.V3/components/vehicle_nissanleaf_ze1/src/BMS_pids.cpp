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

#include "vehicle_nissanleaf_ze1.h"

void OvmsVehicleNLZE1::IncomingBMS(uint16_t type, uint16_t pid, const char* data, uint16_t len) {  

  switch (pid) {
    case 0x01: {

      int32_t curr1_val = CAN_UINT32(0);
      if((curr1_val& 0x8000000) == 0x8000000) {
        StandardMetrics.ms_v_bat_current->SetValue((float)( curr1_val | -0x100000000 ) / 1024,Amps);
      }
      else {
        StandardMetrics.ms_v_bat_current->SetValue((float)curr1_val / 1024,Amps);
      }
      //ESP_LOGD(TAG, "0x01 ms_v_bat_current: %f", CAN_UINT32(0) /1024);

      int32_t curr2_val = CAN_UINT32(6);
      if((curr2_val& 0x8000000) == 0x8000000) {
        mt_b_current2->SetValue((float)( curr2_val | -0x100000000 ) / 1024, Amps);
      }
      else {
        mt_b_current2->SetValue((float)curr2_val / 1024, Amps);
      }
      //ESP_LOGD(TAG, "0x01 mt_b_current2: %f", CAN_UINT32(6) /1024);

      StandardMetrics.ms_v_bat_voltage->SetValue((float)CAN_UINT(18) /100, Volts);
      //ESP_LOGD(TAG, "0x01 ms_v_bat_voltage: %f", CAN_UINT(18) /100);

      StandardMetrics.ms_v_bat_soh->SetValue((float)CAN_UINT(28) /102.4, Percentage);
      //ESP_LOGD(TAG, "0x01 ms_v_bat_soh: %f", CAN_UINT(26) /102.4);

      StandardMetrics.ms_v_bat_soc->SetValue((float)CAN_UINT24(31) /10000, Percentage);
      //ESP_LOGD(TAG, "0x01 ms_v_bat_soc: %f", CAN_UINT24(29) /10000);

      break;
    }
    
    default: {
      char *buf = NULL;
      size_t rlen = len, offset = 0;
      do {
        rlen = FormatHexDump(&buf, data + offset, rlen, 16);
        offset += 16;
        ESP_LOGW(TAG, "OBD2: unhandled reply from BCM [%02x %02x]: %s", type, pid, buf ? buf : "-");
      } while (rlen);
      if (buf)
        free(buf);
      break;
    }
  }
}