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

#include "esp32bluetooth_gattc_app_example.h"

#include "ovms_log.h"
static const char *TAG = "bt-gattc-app-example";


OvmsBluetoothAppGATTCExample MyBluetoothAppGATTCExample __attribute__ ((init_priority (8018)));

OvmsBluetoothAppGATTCExample::OvmsBluetoothAppGATTCExample()
  : esp32bluetoothClientApp("gattc_example")
  {
    ESP_LOGI(TAG, "Initialising Bluetooth GATTC Example App (8018)");

    /*OvmsCommand* cmd_bt_gattc_app = MyCommandApp.RegisterCommand("bt_gattc_example_app","gattc example app interaction", gattc_app_read_stuff);
    cmd_bt_gattc_app->RegisterCommand("write_stuff","write to gattc example app",gattc_app_write_stuff);
    cmd_bt_gattc_app->RegisterCommand("read_stuff","read from gattc example app",gattc_app_read_stuff);*/

    m_adv_name = "ExampleApp";

    m_service_id.id.uuid.len = ESP_UUID_LEN_16;
    m_service_id.id.uuid.uuid.uuid16 = GATTC_SERVICE_UUID_OVMS_EXAMPLE;

    //read / write characteristic
    m_example_characteristic_0.m_app = (esp32bluetoothApp *) this;
    m_example_characteristic_0.m_char_uuid.len = ESP_UUID_LEN_16;
    m_example_characteristic_0.m_char_uuid.uuid.uuid16 = GATTC_CHAR_UUID_OVMS_EXAMPLE_0;

    //characteristic 2 has notifications -> activation in descriptor ; read only
    m_example_characteristic_1.m_app = (esp32bluetoothApp *) this;
    m_example_characteristic_1.m_char_uuid.len = ESP_UUID_LEN_16;
    m_example_characteristic_1.m_char_uuid.uuid.uuid16 = GATTC_CHAR_UUID_OVMS_EXAMPLE_1;
    m_example_characteristic_1.m_descr_uuid.len = ESP_UUID_LEN_16;
    m_example_characteristic_1.m_descr_uuid.uuid.uuid16 = GATTS_DESCR_UUID_OVMS_EXAMPLE_1;
    m_example_characteristic_1.m_notifying = true;


    m_characteristics.push_back(&m_example_characteristic_0);
    m_characteristics.push_back(&m_example_characteristic_1);

    m_app_id = GATTC_APP_UUID_OVMS_EXAMPLE;
    m_active = true;
    MyBluetoothGATTC.RegisterApp(this);
  }

OvmsBluetoothAppGATTCExample::~OvmsBluetoothAppGATTCExample()
  {
  }

void OvmsBluetoothAppGATTCExample::EventValueUpdate(int char_num)
  {
    ESP_LOGI(TAG, "Got notification for new value of characteristic nr. %d: value: %d ",
    char_num, m_characteristics[char_num]->m_value_buffer[0]);
  }



void OvmsBluetoothAppGATTCExample::EventReadChar(esp_ble_gattc_cb_param_t::gattc_read_char_evt_param *read)
  {
    esp32bluetoothClientApp::EventReadChar(read);

    for(auto characteristic : m_characteristics){
      if(read->handle == characteristic->m_char_handle){
        ESP_LOGI(TAG, "Read value %x for characteristic %x ", characteristic->m_value_buffer[0], characteristic->m_char_uuid.uuid.uuid16);
      }
    }

  }

void OvmsBluetoothAppGATTCExample::WriteToCharacteristics()
  {
    ESP_LOGI(TAG, "writing stuff ...");

    WriteCharByte(0,0xAB);
  }

void OvmsBluetoothAppGATTCExample::ReadFromCharacteristics()
  {
    ESP_LOGI(TAG, "reading stuff ");

    TriggerReadChar(0);
    TriggerReadChar(1);
  }

void gattc_app_write_stuff(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
    MyBluetoothAppGATTCExample.WriteToCharacteristics();
  }

void gattc_app_read_stuff(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
    MyBluetoothAppGATTCExample.ReadFromCharacteristics();
  }
