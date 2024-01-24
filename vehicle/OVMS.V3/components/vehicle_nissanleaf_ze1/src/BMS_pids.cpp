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

    case 0x02: {

      for(int i = 0; i< 96; i++) {
        StandardMetrics.ms_v_bat_cell_voltage->SetElemValue(i,(float)CAN_UINT(i*2)/1000, Volts);
        //ESP_LOGD(TAG, "0x02 ms_v_bat_cell_voltage nr %d : %f", i, CAN_UINT(i*2)/1000);
      }
      break;
    }

    case 0x04: {
      int bat_rack_temp_raw;
      float bat_rack_temp_f;
      for(int i = 0; i< 4; i++) {
        bat_rack_temp_raw = CAN_UINT(i*2);
        
        if(bat_rack_temp_raw == 1021) {
          bat_rack_temp_f = 1.0;
        }
        else if(bat_rack_temp_raw >= 589){
          bat_rack_temp_f = 162.0 - (bat_rack_temp_raw * 0.181);
        }
        else if(bat_rack_temp_raw >= 569){
          bat_rack_temp_f = 57.2 + ((579 - bat_rack_temp_raw) * 0.18);
        }
        else if(bat_rack_temp_raw >= 558){
          bat_rack_temp_f = 60.8 + ((558 - bat_rack_temp_raw) * 0.16363636363636364);
        }
        else if(bat_rack_temp_raw >= 548){
          bat_rack_temp_f = 62.6 + ((548 - bat_rack_temp_raw) * 0.18);
        }
        else if(bat_rack_temp_raw >= 537){
          bat_rack_temp_f = 64.4 + ((537 - bat_rack_temp_raw) * 0.16363636363636364);
        }
        else if(bat_rack_temp_raw >= 447){
          bat_rack_temp_f = 66.2 + ((527 - bat_rack_temp_raw) * 0.18);
        }
        else if(bat_rack_temp_raw >= 438){
          bat_rack_temp_f = 82.4 + ((438 - bat_rack_temp_raw) * 0.2);
        }
        else if(bat_rack_temp_raw >= 428){
          bat_rack_temp_f = 84.2 + ((428 - bat_rack_temp_raw) * 0.18);
        }
        else if(bat_rack_temp_raw >= 365){
          bat_rack_temp_f = 86.0 + ((419 - bat_rack_temp_raw) * 0.2);
        }
        else if(bat_rack_temp_raw >= 357){
          bat_rack_temp_f = 98.6 + ((357 - bat_rack_temp_raw) * 0.225);
        }
        else if(bat_rack_temp_raw >= 348){
          bat_rack_temp_f = 100.4 + ((348 - bat_rack_temp_raw) * 0.2);
        }
        else if(bat_rack_temp_raw >= 316){
          bat_rack_temp_f = 102.2 + ((340 - bat_rack_temp_raw) * 0.225);
        }
        else {
          bat_rack_temp_f = 109.4 + ((309 - bat_rack_temp_raw) * 0.2571428571428572);
        }


        StandardMetrics.ms_v_bat_cell_temp->SetElemValue(i,(bat_rack_temp_f-32.0)* 5.0 / 9.0, Celcius);
        //ESP_LOGD(TAG, "0x04 ms_v_bat_cell_temp nr %d : %f", i, (bat_rack_temp_f-32.0)* 5.0 / 9.0);
      }
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