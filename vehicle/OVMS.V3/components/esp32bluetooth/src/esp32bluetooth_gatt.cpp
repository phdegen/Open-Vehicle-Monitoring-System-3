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

#include "esp32bluetooth_gatt.h"

////////////////////////////////////////////////////////////////////////
// esp32bluetoothCharacteristic

esp32bluetoothCharacteristic::esp32bluetoothCharacteristic()
  {
  m_app = NULL;
  m_notifying = false;
  m_indicating = false;

  m_char_handle = 0;
  memset(&m_char_uuid, 0, sizeof(m_char_uuid));
  m_char_perm = 0;
  m_char_property = 0;
  m_descr_handle = 0;
  memset(&m_descr_uuid, 0, sizeof(m_descr_uuid));
  }

esp32bluetoothCharacteristic::~esp32bluetoothCharacteristic()
  {
  }

////////////////////////////////////////////////////////////////////////
// esp32bluetoothApp

esp32bluetoothApp::esp32bluetoothApp(const char* name)
  {
  m_name = name;
  m_gatt_if = ESP_GATT_IF_NONE;
  m_app_id = 0;
  m_conn_id = 0;
  m_service_handle = 0;
  memset(&m_service_id, 0, sizeof(m_service_id));
  m_mtu = 0;

  m_active = false;

  }

esp32bluetoothApp::~esp32bluetoothApp()
  {
  }


////////////////////////////////////////////////////////////////////////
// esp32bluetoothGATT


esp32bluetoothGATT::esp32bluetoothGATT()
  {
  }

esp32bluetoothGATT::~esp32bluetoothGATT()
  {
  }

void esp32bluetoothGATT::RegisterForEvents()
  {
  }

void esp32bluetoothGATT::RegisterAllApps()
  {
  }

void esp32bluetoothGATT::UnregisterAllApps()
  {
  }
