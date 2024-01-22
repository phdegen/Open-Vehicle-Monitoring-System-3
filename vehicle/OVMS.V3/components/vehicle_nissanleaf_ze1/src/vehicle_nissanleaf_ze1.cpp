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

#include "vehicle_nissanleaf_ze1.h"
#include "nissanleaf_ze1_poller.h"

const char *OvmsVehicleNLZE1::s_tag = "v-nl-ze1";

OvmsVehicleNLZE1::OvmsVehicleNLZE1() {
  ESP_LOGI(TAG, "Start Nissan Leaf ZE1 vehicle module");

  //Init variables supressing push on boot up
  StandardMetrics.ms_v_type->SetValue("NLZE1");
  StandardMetrics.ms_v_charge_inprogress->SetValue(false);
  StandardMetrics.ms_v_charge_pilot->SetValue(false);
  StandardMetrics.ms_v_charge_state->SetValue("stopped");
  StandardMetrics.ms_v_charge_substate->SetValue("stopped");
  StandardMetrics.ms_v_env_on->SetValue(false);
  
  MyConfig.RegisterParam("xnlze1", "Nissan Leaf ZE1 configuration", true, true);
  ConfigChanged(NULL);

  // Init Nissan Leaf ZE1 Connection (CAN Gateway)
  RegisterCanBus(1, CAN_MODE_ACTIVE, CAN_SPEED_500KBPS);

  POLLSTATE_OFF;
  ESP_LOGI(TAG, "Pollstate switched to OFF");
  
  PollSetPidList(m_can1, nl_ze1_polls);
  PollSetThrottling(10);
  PollSetResponseSeparationTime(20);
  
  // NL ze1 specific metrics
  mt_b_current2       = MyMetrics.InitFloat("nlze1.b.current2", SM_STALE_MID, 0, Amps, false);

  // BMS configuration:
  BmsSetCellArrangementVoltage(96, 1);
  BmsSetCellArrangementTemperature(12, 1);
  BmsSetCellLimitsVoltage(2.0, 5.0);
  BmsSetCellLimitsTemperature(-39, 200);
  BmsSetCellDefaultThresholdsVoltage(0.030, 0.050);
  BmsSetCellDefaultThresholdsTemperature(4.0, 5.0);  

  POLLSTATE_ON;
}

OvmsVehicleNLZE1::~OvmsVehicleNLZE1() {
  ESP_LOGI(TAG, "Stop Nissan Leaf ZE1 vehicle module");
}

/**
 * Handles incoming CAN-frames on bus 1
 */
void OvmsVehicleNLZE1::IncomingFrameCan1(CAN_frame_t* p_frame) {
	uint8_t *data = p_frame->data.u8;
	ESP_LOGW(TAG, "PID:%x DATA: %02x %02x %02x %02x %02x %02x %02x %02x", p_frame->MsgID, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
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
void OvmsVehicleNLZE1::IncomingPollReply(canbus* bus, uint16_t type, uint16_t pid, uint8_t* data, uint8_t length, uint16_t remain) {
	string& rxbuf = nl_ze1_obd_rxbuf;
  
  ESP_LOGW(TAG, "pid: %04x length: %d m_poll_ml_remain: %d m_poll_ml_frame: %d", pid, length, m_poll_ml_remain, m_poll_ml_frame);
  ESP_LOGW(TAG, "m_poll_moduleid_low is : 0x%08x",m_poll_moduleid_low);
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
    case 0x7BB:
      IncomingBMS(type, pid, rxbuf.data(), rxbuf.size());
      break;
    // ****** ECU 1 *****
		case 0x764:
			IncomingECU1(type, pid, rxbuf.data(), rxbuf.size());
			break;
    // ****** ECU 2 *****
		case 0x765: //dummy value
			IncomingECU2(type, pid, rxbuf.data(), rxbuf.size());
			break;
    // ****** ECU 3 *****
		case 0x79A:
			IncomingECU3(type, pid, rxbuf.data(), rxbuf.size());
			break;
    // ****** ECU 4 *****
		case 0x763:
			IncomingECU4(type, pid, rxbuf.data(), rxbuf.size());
			break;
    
	}
}

class OvmsVehicleNLZE1Init {
  public: OvmsVehicleNLZE1Init();
} MyOvmsVehicleNLZE1Init  __attribute__ ((init_priority (9000)));


OvmsVehicleNLZE1Init::OvmsVehicleNLZE1Init() 
{
  ESP_LOGI(TAG, "Registering Vehicle: Nissan Leaf ZE1 (9000)");
  MyVehicleFactory.RegisterVehicle<OvmsVehicleNLZE1>("NLZE1","Nissan Leaf ZE1");
}