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


#include "ovms_log.h"

#include <stdio.h>
#include <string>

#include "vehicle_vw_id3.h"
#include "vw_id3_poller.h"

const char *OvmsVehicleVWID3::s_tag = "v-vw-id3";

OvmsVehicleVWID3::OvmsVehicleVWID3() {
  ESP_LOGI(TAG, "Start VW ID.3 vehicle module");

  //Init variables supressing push on boot up
  StandardMetrics.ms_v_type->SetValue("VWID3");
  StandardMetrics.ms_v_charge_inprogress->SetValue(false);
  StandardMetrics.ms_v_charge_pilot->SetValue(false);
  StandardMetrics.ms_v_charge_state->SetValue("stopped");
  StandardMetrics.ms_v_charge_substate->SetValue("stopped");
  StandardMetrics.ms_v_env_on->SetValue(false);
  
  MyConfig.RegisterParam("xvwid3", "VW ID.3 configuration", true, true);
  ConfigChanged(NULL);

  // Init VW ID3 Connection (CAN Gateway)
  RegisterCanBus(1, CAN_MODE_ACTIVE, CAN_SPEED_500KBPS);

  PollSetState(POLLSTATE_OFF);
  ESP_LOGI(TAG, "Pollstate switched to OFF");
  
  PollSetPidList(m_can1, vw_id3_polls);
  PollSetThrottling(10);
  PollSetResponseSeparationTime(20);
  pollstate_delay_ticker_s = 0;
  
  // VW ID.3 specific metrics
  mt_pos_odometer_start       = MyMetrics.InitFloat("zph2.v.pos.odometer.start", SM_STALE_MID, 0, Kilometers, true);

  // BMS configuration:
  BmsSetCellArrangementVoltage(96, 1);
  BmsSetCellArrangementTemperature(12, 1);
  BmsSetCellLimitsVoltage(2.0, 5.0);
  BmsSetCellLimitsTemperature(-39, 200);
  BmsSetCellDefaultThresholdsVoltage(0.030, 0.050);
  BmsSetCellDefaultThresholdsTemperature(4.0, 5.0);  

  PollSetState(POLLSTATE_ON);
  ESP_LOGI(TAG, "Pollstate switched to ON");
}

OvmsVehicleVWID3::~OvmsVehicleVWID3() {
  ESP_LOGI(TAG, "Stop VW ID.3 vehicle module");
}

void OvmsVehicleVWID3::PollerStateTicker() {
  float voltage_level_12V = StandardMetrics.ms_v_bat_12v_voltage->AsFloat();
  float active_threshold = MyConfig.GetParamValueFloat("vehicle", "12v.active_threshold", 13.0);

  if(m_poll_state == POLLSTATE_ON) {
    if(voltage_level_12V < active_threshold) {
      pollstate_delay_ticker_s++;
      if(pollstate_delay_ticker_s == 1) {
        ESP_LOGI(TAG, "12V not powered anymore -> Car is inactive");
      }
      if(pollstate_delay_ticker_s > 60* MyConfig.GetParamValueInt("vehicle", "poll.inactive_delay_min", 5)) {
        PollSetState(POLLSTATE_OFF);
        ESP_LOGI(TAG, "Inactivity pollstate delay passed: stop polling.");
      }
    }
    else {
      pollstate_delay_ticker_s = 0;
    }
  }
  else if(m_poll_state == POLLSTATE_OFF && voltage_level_12V > active_threshold){
    PollSetState(POLLSTATE_ON);
    pollstate_delay_ticker_s = 0;
    ESP_LOGI(TAG, "12V charge level up again -> Car active, start polling data.");
  }
}

/**
 * Handles incoming CAN-frames on bus 1
 */
void OvmsVehicleVWID3::IncomingFrameCan1(CAN_frame_t* p_frame) {
	uint8_t *data = p_frame->data.u8;
	//ESP_LOGI(TAG, "PID:%x DATA: %02x %02x %02x %02x %02x %02x %02x %02x", p_frame->MsgID, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
  //ESP_LOGD(TAG, "Status CAN Bus: %s", mt_bus_awake->AsBool() ? "true" : "false");

  // Poll reply gives 0x83 0xc0 that means zoe is sleeping and CAN gateway does not respond to anything
  //if (mt_bus_awake->AsBool() && data[0] == 0x83 && data[1] == 0xc0) {
  //  ESP_LOGI(TAG,"Zoe has gone to sleep (CAN Gateway NAK response)");
  //  mt_bus_awake->SetValue(false);
	//  StandardMetrics.ms_v_env_awake->SetValue(false);
  //  StandardMetrics.ms_v_env_charging12v->SetValue(false);
  //  StandardMetrics.ms_v_pos_speed->SetValue( 0 );
  //  StandardMetrics.ms_v_bat_12v_current->SetValue( 0 );
  //  StandardMetrics.ms_v_charge_12v_current->SetValue( 0 );
  //  StandardMetrics.ms_v_bat_current->SetValue( 0 );
  //  POLLSTATE_OFF;
  //  ESP_LOGI(TAG, "Pollstate switched to OFF");
  //  //Check if car is locked, if not send notify
  //  if (!StandardMetrics.ms_v_env_locked->AsBool()) {
  //    MyNotify.NotifyString("alert", "vehicle.lock", "Vehicle is not locked");
  //  }
  //} else if (!mt_bus_awake->AsBool()) {
  //  ZoeWakeUp();
  //}

}

/**
 * Handles incoming poll results
 */
void OvmsVehicleVWID3::IncomingPollReply(canbus* bus, uint16_t type, uint16_t pid, uint8_t* data, uint8_t length, uint16_t remain) {
	string& rxbuf = id3_obd_rxbuf;
  
  //ESP_LOGV(TAG, "pid: %04x length: %d m_poll_ml_remain: %d m_poll_ml_frame: %d", pid, length, m_poll_ml_remain, m_poll_ml_frame);

  // init / fill rx buffer:
  if (m_poll_ml_frame == 0) {
    rxbuf.clear();
    rxbuf.reserve(length + remain);
  }
  rxbuf.append((char*)data, length);
  
  if (remain)
    return;
  
	switch (m_poll_moduleid_low) {
    // ****** BMS *****
    case 0x17FE007B:
      IncomingBMS(type, pid, rxbuf.data(), rxbuf.size());
      break;
    // ****** ECU 1 *****
		case 0x17FE0076:
			IncomingECU1(type, pid, rxbuf.data(), rxbuf.size());
			break;
    // ****** ECU 2 *****
		case 0x777: //dummy value
			IncomingECU2(type, pid, rxbuf.data(), rxbuf.size());
			break;
    // ****** ECU 3 *****
		case 0x7B0:
			IncomingECU3(type, pid, rxbuf.data(), rxbuf.size());
			break;
    // ****** ECU 4 *****
		case 0x77A:
			IncomingECU4(type, pid, rxbuf.data(), rxbuf.size());
			break;
    
	}
}

class OvmsVehicleVWID3Init {
  public: OvmsVehicleVWID3Init();
} MyOvmsVehicleVWID3Init  __attribute__ ((init_priority (9000)));


OvmsVehicleVWID3Init::OvmsVehicleVWID3Init() 
{
  ESP_LOGI(TAG, "Registering Vehicle: VW ID.3 (9000)");
  MyVehicleFactory.RegisterVehicle<OvmsVehicleVWID3>("VWID3","VW ID.3");
}