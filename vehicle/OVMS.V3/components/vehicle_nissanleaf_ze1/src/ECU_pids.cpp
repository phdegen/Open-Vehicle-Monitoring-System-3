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

void OvmsVehicleNLZE1::IncomingECU1(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {

    default: {
      char *buf = NULL;
      size_t rlen = len, offset = 0;
      do {
        rlen = FormatHexDump(&buf, data + offset, rlen, 16);
        offset += 16;
        ESP_LOGW(TAG, "OBD2: unhandled reply from ECU1 [%02x %02x]: %s", type, pid, buf ? buf : "-");
      } while (rlen);
      if (buf)
        free(buf);
      break;
    }
	}
}

void OvmsVehicleNLZE1::IncomingECU2(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {

    default: {
      char *buf = NULL;
      size_t rlen = len, offset = 0;
      do {
        rlen = FormatHexDump(&buf, data + offset, rlen, 16);
        offset += 16;
        ESP_LOGW(TAG, "OBD2: unhandled reply from ECU2 [%02x %02x]: %s", type, pid, buf ? buf : "-");
      } while (rlen);
      if (buf)
        free(buf);
      break;
    }
	}
}

void OvmsVehicleNLZE1::IncomingECU3(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {
    case 0x115D: {  
      StandardMetrics.ms_v_env_temp->SetValue((((float) CAN_BYTE(0))*0.9-40.9-32)*5.0/9.0, Celcius);
      //ESP_LOGD(TAG, "0x115D EVC ms_v_env_temp: %f", (((float) CAN_BYTE(0))*0.9-40.9-32)*5.0/9.0);
      break;
    }
    case 0x1234: {  
      if(CAN_BYTE(0) > 0) {
        StandardMetrics.ms_v_door_chargeport->SetValue(true);
      }
      else {
        StandardMetrics.ms_v_door_chargeport->SetValue(false);
      }
      //ESP_LOGD(TAG, "0x1234 EVC ms_v_door_chargeport: %f", CAN_BYTE(0));
      break;
    }
    case 0x114E: {  
      if(CAN_BYTE(0) > 0) {
        StandardMetrics.ms_v_charge_inprogress->SetValue(true);
      }
      else {
        StandardMetrics.ms_v_charge_inprogress->SetValue(false);
      }
      //ESP_LOGD(TAG, "0x114E EVC ms_v_charge_inprogress: %f", CAN_BYTE(0));
      break;
    }
    case 0x1236: {  
      StandardMetrics.ms_v_charge_power->SetValue((float) CAN_UINT(0)*0.1, kW);
      //ESP_LOGD(TAG, "0x1236 EVC ms_v_charge_power: %f", CAN_UINT(0)*0.1);
      break;
    }
    case 0x1146: {  
      StandardMetrics.ms_v_inv_power->SetValue((float) CAN_UINT(0)*0.04, kW);
      //ESP_LOGD(TAG, "0x1146 EVC ms_v_inv_power: %f", CAN_UINT(0)*0.04);
      break;
    }
    case 0x121A: {  
      StandardMetrics.ms_v_pos_speed->SetValue((float) CAN_UINT(0)/10, Kph);
      //ESP_LOGD(TAG, "0x121A EVC ms_v_pos_speed: %f", CAN_UINT(0)/10);
      break;
    }
    case 0x1106: {  
      //AC on/off
      if(CAN_BYTE(0) > 0) {
        StandardMetrics.ms_v_env_cooling->SetValue(true);
      }
      else {
        StandardMetrics.ms_v_env_cooling->SetValue(false);
      }
      //ESP_LOGD(TAG, "0x1106 EVC ms_v_env_cooling: %f", CAN_BYTE(0));
      break;
    }
    case 0x1151: {  
      //HVAC Power
      mt_e_hvac_power->SetValue(CAN_BYTE(0)*250, Watts);
      //ESP_LOGD(TAG, "0x1151 EVC mt_e_hvac_power: %d", CAN_BYTE(0)*250);
      break;
    }

    default: {
      char *buf = NULL;
      size_t rlen = len, offset = 0;
      do {
        rlen = FormatHexDump(&buf, data + offset, rlen, 16);
        offset += 16;
        ESP_LOGW(TAG, "OBD2: unhandled reply from ECU3 [%02x %02x]: %s", type, pid, buf ? buf : "-");
      } while (rlen);
      if (buf)
        free(buf);
      break;
    }
	}
}

void OvmsVehicleNLZE1::IncomingECU4(uint16_t type, uint16_t pid, const char* data, uint16_t len) {
	switch (pid) {
    case 0x0E01: {
      StandardMetrics.ms_v_pos_odometer->SetValue(CAN_UINT24(0), Kilometers);
      //ESP_LOGD(TAG, "0x0E01 EVC ms_v_pos_odometer: %d", CAN_UINT24(0));
      break;
    }
    case 0x0E24: {  
      StandardMetrics.ms_v_bat_range_est->SetValue(CAN_UINT(0)/10, Kilometers);
      //ESP_LOGD(TAG, "0x0E24 EVC ms_v_bat_range_est: %d", CAN_UINT(0)/10);
      break;
    }
    case 0x0E25: {  
      StandardMetrics.ms_v_tpms_pressure->SetElemValue(MS_V_TPMS_IDX_FR, (float)CAN_BYTE(0)*0.068947576*25, kPa);
      //ESP_LOGD(TAG, "0x0E25 EVC ms_v_tpms_pressure FR: %f", CAN_BYTE(0)*0.068947576*25);
      break;
    }
    case 0x0E26: {  
      StandardMetrics.ms_v_tpms_pressure->SetElemValue(MS_V_TPMS_IDX_FL, (float)CAN_BYTE(0)*0.068947576*25, kPa);
      //ESP_LOGD(TAG, "0x0E26 EVC ms_v_tpms_pressure FL: %f", CAN_BYTE(0)*0.068947576*25);
      break;
    }
    case 0x0E27: {  
      StandardMetrics.ms_v_tpms_pressure->SetElemValue(MS_V_TPMS_IDX_RR, (float)CAN_BYTE(0)*0.068947576*25, kPa);
      //ESP_LOGD(TAG, "0x0E27 EVC ms_v_tpms_pressure RR: %f", CAN_BYTE(0)*0.068947576*25);
      break;
    }
    case 0x0E28: {  
      StandardMetrics.ms_v_tpms_pressure->SetElemValue(MS_V_TPMS_IDX_RL, (float)CAN_BYTE(0)*0.068947576*25, kPa);
      //ESP_LOGD(TAG, "0x0E28 EVC ms_v_tpms_pressure RL: %f", CAN_BYTE(0)*0.068947576*25);
      break;
    }
    default: {
      char *buf = NULL;
      size_t rlen = len, offset = 0;
      do {
        rlen = FormatHexDump(&buf, data + offset, rlen, 16);
        offset += 16;
        ESP_LOGW(TAG, "OBD2: unhandled reply from ECU4 [%02x %02x]: %s", type, pid, buf ? buf : "-");
      } while (rlen);
      if (buf)
        free(buf);
      break;
    }
	}
}
