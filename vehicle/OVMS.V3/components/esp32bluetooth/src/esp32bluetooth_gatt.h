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

#ifndef __ESP32BLUETOOTH_GATT_H__
#define __ESP32BLUETOOTH_GATT_H__

#include <vector>
#include "esp_gattc_api.h"
#include "esp_gatts_api.h"
#include "esp32bluetooth.h"

#define GATT_CHAR_BUF_SIZE 8

class esp32bluetoothApp;

class esp32bluetoothCharacteristic
  {
  public:
    esp32bluetoothCharacteristic();
    ~esp32bluetoothCharacteristic();

  public:
    esp32bluetoothApp* m_app;

    esp_bt_uuid_t m_char_uuid;
    uint16_t m_char_handle;
    esp_gatt_perm_t m_char_perm;
    esp_gatt_char_prop_t m_char_property;

    uint16_t m_descr_handle;
    esp_bt_uuid_t m_descr_uuid;

    esp_bd_addr_t m_remote_bda;

    bool m_notifying;
    bool m_indicating;

    uint8_t m_value_buffer[GATT_CHAR_BUF_SIZE];
  };

class esp32bluetoothGATT
  {
  public:
    esp32bluetoothGATT();
    ~esp32bluetoothGATT();

  public:
    virtual void RegisterForEvents();
    virtual void RegisterAllApps();
    virtual void UnregisterAllApps();

  };

class esp32bluetoothApp
  {
  public:
    esp32bluetoothApp(const char* name);
    ~esp32bluetoothApp();

  public:
    const char* m_name;
    uint16_t m_app_slot;
    esp_gatt_if_t m_gatt_if;
    uint16_t m_app_id;
    uint16_t m_conn_id;
    uint16_t m_service_handle;
    esp_gatt_srvc_id_t m_service_id;
    uint16_t m_mtu;

    std::vector<esp32bluetoothCharacteristic*> m_characteristics;
  };

#endif //#ifndef __ESP32BLUETOOTH_GATT_H__
