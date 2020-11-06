/*
;    Project:       Open Vehicle Monitor System
;    Date:          14th March 2017
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2011       Michael Stegen / Stegen Electronics
;    (C) 2011-2017  Mark Webb-Johnson
;    (C) 2011        Sonny Chen @ EPRO/DX
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

#ifndef __ESP32BLUETOOTH_GAP_H__
#define __ESP32BLUETOOTH_GAP_H__

#include "esp32bluetooth.h"

class esp32bluetoothGAP
  {
  public:
    esp32bluetoothGAP();
    ~esp32bluetoothGAP();

  public:
    void StartAdvertising();
    void RegisterForEvents();
    void EventHandler(esp_gap_ble_cb_event_t event,
                      esp_ble_gap_cb_param_t *param);
    void GATTSEventHandler(esp_gap_ble_cb_event_t event,
                      esp_ble_gap_cb_param_t *param);
    void GATTCEventHandler(esp_gap_ble_cb_event_t event,
                      esp_ble_gap_cb_param_t *param);
    void RerunGATTCScan();

  public:
    bool m_gatts_active;
    bool m_gattc_active;
  
  private:
    esp_ble_adv_data_t m_adv_config;
    esp_ble_adv_data_t m_scan_rsp_config;
    esp_ble_adv_params_t m_adv_params;
    uint8_t m_adv_config_done;
  };

extern esp32bluetoothGAP MyBluetoothGAP;

#endif //#ifndef __ESP32BLUETOOTH_GAP_H__
