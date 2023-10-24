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

void OvmsVehicleVWEUPEGolf::IncomingECU1(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {
    case 0x1DDA: {
      if(CAN_BYTE(1) > 0) {
        StandardMetrics.ms_v_door_chargeport->SetValue(true);
      }
      else {
        StandardMetrics.ms_v_door_chargeport->SetValue(false);
      }
      //ESP_LOGD(TAG, "0x1DDA ms_v_door_chargeport: %d", CAN_BYTE(1));
      break;
    }

    default: {
      char *buf = NULL;
      size_t rlen = len, offset = 0;
      do {
        rlen = FormatHexDump(&buf, data + offset, rlen, 16);
        offset += 16;
        ESP_LOGW(TAG, "OBD2: unhandled reply from EVC [%02x %02x]: %s", type, pid, buf ? buf : "-");
      } while (rlen);
      if (buf)
        free(buf);
      break;
    }
	}
}

void OvmsVehicleVWEUPEGolf::IncomingECU2(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {

    case 0x15D6: {
      float value = (float) CAN_BYTE(0);
        mt_chrg_eff_ecu->SetValue(value <= 250.0f ? value / 10.0f + 75.0f : 0.0f, Percentage);
      //ESP_LOGD(TAG, "0x15D6 ECU mt_chrg_eff_ecu: %d", CAN_BYTE(0));
      break;
    }

    case 0x15E1: {
      float value = (float) CAN_BYTE(0);
      mt_chrg_loss_ecu->SetValue((value * 20.0f) / 1000.0f, Percentage);
      //ESP_LOGD(TAG, "0x15E1 ECU mt_chrg_loss_ecu: %d", CAN_UINT24(0));
      break;
    }

    default: {
      char *buf = NULL;
      size_t rlen = len, offset = 0;
      do {
        rlen = FormatHexDump(&buf, data + offset, rlen, 16);
        offset += 16;
        ESP_LOGW(TAG, "OBD2: unhandled reply from EVC [%02x %02x]: %s", type, pid, buf ? buf : "-");
      } while (rlen);
      if (buf)
        free(buf);
      break;
    }
	}
}

