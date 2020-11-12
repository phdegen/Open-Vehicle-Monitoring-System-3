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

#include <string.h>
#include "esp_system.h"
#include "esp32bluetooth_gatts_app_metrics.h"

#include "ovms_log.h"
static const char *TAG = "bt-app-metrics";

// Demo property (just for testing)
static uint8_t char1_str[] = {'O','V','M','S'};
static esp_gatt_char_prop_t a_property = 0;

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40
static esp_attr_value_t gatts_demo_char1_val =
{
    .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
    .attr_len     = sizeof(char1_str),
    .attr_value   = char1_str,
};
// End of demo property

OvmsBluetoothAppMetrics MyBluetoothAppMetrics __attribute__ ((init_priority (8017)));

OvmsBluetoothAppMetrics::OvmsBluetoothAppMetrics()
  : esp32bluetoothServerApp("metrics")
  {
  ESP_LOGI(TAG, "Initialising Bluetooth METRICS App (8017)");

  m_metrics_characteristic.m_app = (esp32bluetoothApp *) this;
  m_characteristics.push_back(&m_metrics_characteristic);

  m_app_id = GATTS_APP_UUID_OVMS_METRICS;
  MyBluetoothGATTS.RegisterApp(this);
  }

OvmsBluetoothAppMetrics::~OvmsBluetoothAppMetrics()
  {
  }

void OvmsBluetoothAppMetrics::EventRegistered(esp_ble_gatts_cb_param_t::gatts_reg_evt_param *reg)
  {
  m_service_id.is_primary = true;
  m_service_id.id.inst_id = 0x00;
  m_service_id.id.uuid.len = ESP_UUID_LEN_16;
  m_service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_OVMS_METRICS;
  ESP_LOGI(TAG,"ESP_GATTS_REG_EVT Creating service %04x on interface %d",
    m_service_id.id.uuid.uuid.uuid16,
    m_gatt_if);
  esp_ble_gatts_create_service(m_gatt_if, &m_service_id, GATTS_NUM_HANDLE_OVMS_METRICS);
  }

void OvmsBluetoothAppMetrics::EventRead(esp_ble_gatts_cb_param_t::gatts_read_evt_param *read)
  {
  esp_gatt_rsp_t rsp;
  memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
  rsp.attr_value.handle = read->handle;
  rsp.attr_value.len = 4;
  rsp.attr_value.value[0] = 0xde;
  rsp.attr_value.value[1] = 0xed;
  rsp.attr_value.value[2] = 0xbe;
  rsp.attr_value.value[3] = 0xef;
  esp_ble_gatts_send_response(m_gatt_if,
                              read->conn_id,
                              read->trans_id,
                              ESP_GATT_OK, &rsp);
  }

void OvmsBluetoothAppMetrics::EventCreate(esp_ble_gatts_cb_param_t::gatts_add_attr_tab_evt_param *attrtab)
  {
  m_metrics_characteristic.m_char_uuid.len = ESP_UUID_LEN_16;
  m_metrics_characteristic.m_char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_OVMS_METRICS;
  a_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
  esp_err_t add_char_ret =
    esp_ble_gatts_add_char(m_service_handle,
                          &m_metrics_characteristic.m_char_uuid,
                          ESP_GATT_PERM_READ_ENC_MITM | ESP_GATT_PERM_WRITE_ENC_MITM,
                          a_property,
                          &gatts_demo_char1_val,
                          NULL);
  if (add_char_ret)
    {
    ESP_LOGE(TAG, "add char failed, error code =%x",add_char_ret);
    }

  esp_ble_gatts_start_service(m_service_handle);
  }

void OvmsBluetoothAppMetrics::EventAddChar(esp_ble_gatts_cb_param_t::gatts_add_char_evt_param *addchar)
  {
  uint16_t length = 0;
  const uint8_t *prf_char;

  m_metrics_characteristic.m_char_handle = addchar->attr_handle;
  m_metrics_characteristic.m_descr_uuid.len = ESP_UUID_LEN_16;
  m_metrics_characteristic.m_descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
  esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(addchar->attr_handle, &length, &prf_char);
  if (get_attr_ret == ESP_FAIL)
    {
    ESP_LOGE(TAG, "ILLEGAL HANDLE");
    }

  esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(
                         m_service_handle,
                         &m_metrics_characteristic.m_descr_uuid,
                         ESP_GATT_PERM_READ,
                         NULL,NULL);
  if (add_descr_ret)
    {
    ESP_LOGE(TAG, "add char descr failed, error code = %x", add_descr_ret);
    }
  }