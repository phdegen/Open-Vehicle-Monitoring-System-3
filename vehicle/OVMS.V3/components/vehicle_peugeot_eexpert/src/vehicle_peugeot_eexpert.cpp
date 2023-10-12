/*
;    Project:       Open Vehicle Monitor System
;    Date:          11th Oct 23
;
;    Changes:
;    0.1  Initial release
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
;
; Currently support only passive listening of the CAN bus. 
; 
; Testing status: "Works for me", meaning everything is tried just once.  
;
; Most data is reverse engineered from Mercedes Benz B250E (W242), year 2015. 
; vehicle_smarted.cpp was used as an example, and some MsgID's are the same. 
; Some MsgID's matched other Mercedes models. OpenDBC was helpfull here: 
; https://github.com/commaai/opendbc/blob/master/mercedes_benz_e350_2010.dbc
*/

#include "ovms_log.h"
static const char *TAG = "v-pgt-eexpert";

#define VERSION "0.0.1"

#include <stdio.h>
#include "ovms_metrics.h"


#include "vehicle_peugeot_eexpert.h"

OvmsVehiclePeugeotEexpert::OvmsVehiclePeugeotEexpert()
{
  ESP_LOGI(TAG, "Start Peugeot e-Expert vehicle module");

  mt_pgteex_not_sure        = MyMetrics.InitFloat("xpgteex.tbd", SM_STALE_MIN, 0, Kilometers);
  
  RegisterCanBus(1, CAN_MODE_ACTIVE, CAN_SPEED_500KBPS);
}

OvmsVehiclePeugeotEexpert::~OvmsVehiclePeugeotEexpert()
{
  ESP_LOGI(TAG, "Shutdown Peugeot e-Expert vehicle module");
}

class OvmsVehiclePeugeotEexpertInit
  {
  public: OvmsVehiclePeugeotEexpertInit();
} MyOvmsVehiclePeugeotEexpertInit  __attribute__ ((init_priority (9000)));

OvmsVehiclePeugeotEexpertInit::OvmsVehiclePeugeotEexpertInit()
{
  ESP_LOGI(TAG, "Registering Vehicle: MERCEDESB250E (9000)");

  MyVehicleFactory.RegisterVehicle<OvmsVehiclePeugeotEexpert>("PGTEEX", "Peugeot e-Expert");
}

void OvmsVehiclePeugeotEexpert::IncomingFrameCan1(CAN_frame_t* p_frame)
{
  uint8_t *d = p_frame->data.u8;

  static bool isCharging = false;
  static bool lastCharging = false;
   
  switch (p_frame->MsgID) {
  case 0x105: // Motor RPM
    {
      int rpm = ((d[0]&0x3f) << 8) + d[1]; 
      StandardMetrics.ms_v_mot_rpm->SetValue(rpm); // RPM
      StandardMetrics.ms_v_env_throttle->SetValue(d[4]/2.50); // Drive pedal state [%], raw values are from 0 to 250
      break;
    }

  default:
    //ESP_LOGD(TAG, "IFC %03x 8 %02x %02x %02x %02x %02x %02x %02x %02x",
    //p_frame->MsgID, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);
    break;
  }  
}
