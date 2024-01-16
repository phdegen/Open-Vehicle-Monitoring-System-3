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

#include "vehicle_vw_eup_egolf.h"

void OvmsVehicleVWEUPEGolf::IncomingBMS(uint16_t type, uint16_t pid, const char* data, uint16_t len) {  

  int car_mode;
  
  switch (pid) {
    case 0x028C:  // SOC (BMS)
      StandardMetrics.ms_v_bat_soc->SetValue((float)CAN_BYTE(0) /2.5, Percentage);
      //ESP_LOGD(TAG, "0x028C ms_v_bat_soc: %f", CAN_BYTE(0) /2.5);
      break;

    case 0x02BD: {
      StandardMetrics.ms_v_pos_odometer->SetValue((float) CAN_UINT24(1), Kilometers);
      //ESP_LOGD(TAG, "0x02BD ECU ms_v_pos_odometer: %d", CAN_UINT24(1));
      break;
    }

    case 0x1E3B:
      StandardMetrics.ms_v_bat_voltage->SetValue((float)CAN_UINT(0) / 4, Volts);
      //ESP_LOGD(TAG, "0x1E3B ms_v_bat_voltage: %f", CAN_UINT(0) / 4);
      break;

    case 0x1E3D:
      StandardMetrics.ms_v_bat_current->SetValue(((float)(CAN_UINT(0) - 2044)) / 4, Amps);
      //ESP_LOGD(TAG, "0x1E3D ms_v_bat_current: %f", ((float)(CAN_UINT(0) - 2044)) / 4);
      break;

    case 0x2A0B:
      StandardMetrics.ms_v_bat_temp->SetValue((float)((int16_t)CAN_UINT(0))/64, Celcius);
      //ESP_LOGD(TAG, "0x2A0B ms_v_bat_temp: %f", (float)((int16_t)CAN_UINT(0))/64);
      break;

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