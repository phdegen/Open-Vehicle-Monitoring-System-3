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

#ifndef __ESP32BLUETOOTH_GATTC_H__
#define __ESP32BLUETOOTH_GATTC_H__

#include "esp32bluetooth_gatt.h"

class esp32bluetoothClientApp : public esp32bluetoothApp
  {
  public:
    esp32bluetoothClientApp(const char* name);
    ~esp32bluetoothClientApp();

  public:
    virtual void EventRegistered(esp_ble_gattc_cb_param_t::gattc_reg_evt_param *reg);
    virtual void EventConnect(esp_ble_gattc_cb_param_t::gattc_connect_evt_param *connect);
    virtual void EventOpen(esp_ble_gattc_cb_param_t::gattc_open_evt_param *open);
    virtual void EventConfigMTU(esp_ble_gattc_cb_param_t::gattc_cfg_mtu_evt_param *cfg_mtu);
    virtual void EventSearchResult(esp_ble_gattc_cb_param_t::gattc_search_res_evt_param *search_res);
    virtual void EventSearchComplete(esp_ble_gattc_cb_param_t::gattc_search_cmpl_evt_param *search_cmpl);
    virtual void EventRegisterForNotify(esp_ble_gattc_cb_param_t::gattc_reg_for_notify_evt_param *reg_for_not);
    virtual void EventNotify(esp_ble_gattc_cb_param_t::gattc_notify_evt_param *notify);
    virtual void EventWriteDescr(esp_ble_gattc_cb_param_t::gattc_write_evt_param *write);
    virtual void EventServiceChange(esp_ble_gattc_cb_param_t::gattc_srvc_chg_evt_param *srvc_chg);
    virtual void EventWriteChar(esp_ble_gattc_cb_param_t::gattc_write_evt_param *write);
    virtual void EventReadChar(esp_ble_gattc_cb_param_t::gattc_read_char_evt_param *read);
    virtual void EventDisconnect(esp_ble_gattc_cb_param_t::gattc_disconnect_evt_param *disconnect);

    virtual void EventValueUpdate(int char_num);

    virtual void WriteCharByte(int characteristic_idx, uint8_t data);
    virtual void TriggerReadChar(int characteristic_idx);
    
  public:
    uint16_t m_service_end_handle;
    esp_bd_addr_t m_remode_bd_addr;

    bool m_ble_connected;
    bool m_connected_to_server;
    std::string m_adv_name;
    };

class esp32bluetoothGATTC : public esp32bluetoothGATT
  {
  public:
    esp32bluetoothGATTC();
    ~esp32bluetoothGATTC();
  public:
    void RegisterForEvents() override;
    void RegisterAllApps() override;
    void UnregisterAllApps() override;
    void EventHandler(esp_gattc_cb_event_t event,
                      esp_gatt_if_t gattc_if,
                      esp_ble_gattc_cb_param_t *param);
    
  public:
    void RegisterApp(esp32bluetoothClientApp* app);

  public:
    esp_ble_scan_params_t m_scan_params;
  
  };

extern esp32bluetoothGATTC MyBluetoothGATTC;

#endif //#ifndef __ESP32BLUETOOTH_GATTC_H__
