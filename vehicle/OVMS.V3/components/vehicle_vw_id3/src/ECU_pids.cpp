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

#include "vehicle_vw_id3.h"

void OvmsVehicleVWID3::IncomingECU1(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {
    case 0x295A: {
      StandardMetrics.ms_v_pos_odometer->SetValue((float) CAN_UINT24(0), Kilometers);
      //ESP_LOGD(TAG, "0x295A ECU ms_v_pos_odometer: %d", CAN_UINT24(0));
      break;
    }
    case 0xF802: {
      id3_vin[0] = CAN_BYTE(0);
      id3_vin[1] = CAN_BYTE(1);
      id3_vin[2] = CAN_BYTE(2);
      id3_vin[3] = CAN_BYTE(3);
      id3_vin[4] = CAN_BYTE(4);
      id3_vin[5] = CAN_BYTE(5);
      id3_vin[6] = CAN_BYTE(6);
      id3_vin[7] = CAN_BYTE(7);
      id3_vin[8] = CAN_BYTE(8);
      id3_vin[9] = CAN_BYTE(9);
      id3_vin[10] = CAN_BYTE(10);
      id3_vin[11] = CAN_BYTE(11);
      id3_vin[12] = CAN_BYTE(12);
      id3_vin[13] = CAN_BYTE(13);
      id3_vin[14] = CAN_BYTE(14);
      id3_vin[15] = 0;
      StandardMetrics.ms_v_vin->SetValue(id3_vin, Other);
      //ESP_LOGD(TAG, "0xF802 ECU ms_v_vin: %d", CAN_UINT32(4));
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

void OvmsVehicleVWID3::IncomingECU2(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {

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

void OvmsVehicleVWID3::IncomingECU3(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {
    case 0x2613: {  
      StandardMetrics.ms_v_env_cabintemp->SetValue((float) CAN_UINT(0)/5-40, Celcius);
      //ESP_LOGD(TAG, "0x2613 EVC ms_v_env_cabintemp: %f", CAN_UINT(0)/5-40);
      break;
    }
    case 0x2609: {  
      StandardMetrics.ms_v_env_temp->SetValue((float) CAN_BYTE(0)/2-50, Celcius);
      //ESP_LOGD(TAG, "0x2609 EVC ms_v_env_temp: %f", CAN_BYTE(0)/2-50);
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

void OvmsVehicleVWID3::IncomingECU4(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {
    case 0x084F: {  
      if(CAN_BYTE(2) > 0) {
        StandardMetrics.ms_v_door_chargeport->SetValue(true);
      }
      else {
        StandardMetrics.ms_v_door_chargeport->SetValue(false);
      }
      //ESP_LOGD(TAG, "0x084F EVC ms_v_door_chargeport: %d", CAN_BYTE(1));
      break;
    }
    case 0x2AB6: {  
      StandardMetrics.ms_v_bat_range_est->SetValue(CAN_UINT(0), Kilometers);
      //ESP_LOGD(TAG, "0x2AB6 EVC ms_v_bat_range_est: %d", CAN_UINT(0));
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
