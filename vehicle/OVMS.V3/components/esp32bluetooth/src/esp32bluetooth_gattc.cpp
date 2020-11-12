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
#include "esp32bluetooth_gap.h"
#include "esp32bluetooth_gattc.h"
#include "ovms_config.h"
#include "ovms_events.h"

#include "ovms_log.h"
static const char *TAG = "bt-gatt-client";

esp32bluetoothGATTC MyBluetoothGATTC __attribute__ ((init_priority (8013)));

////////////////////////////////////////////////////////////////////////
// esp32bluetoothApp

esp32bluetoothClientApp::esp32bluetoothClientApp(const char* name)
: esp32bluetoothApp(name)
  {
    m_ble_connected = false;
    m_connected_to_server = false;
  }

esp32bluetoothClientApp::~esp32bluetoothClientApp()
  {
  }

void esp32bluetoothClientApp::EventRegistered(esp_ble_gattc_cb_param_t::gattc_reg_evt_param *reg)
  {
  }

void esp32bluetoothClientApp::EventConnect(esp_ble_gattc_cb_param_t::gattc_connect_evt_param *connect)
  {

  }

void esp32bluetoothClientApp::EventOpen(esp_ble_gattc_cb_param_t::gattc_open_evt_param *open)
  {
  }

void esp32bluetoothClientApp::EventConfigMTU(esp_ble_gattc_cb_param_t::gattc_cfg_mtu_evt_param *cfg_mtu)
  {
    esp_ble_gattc_search_service(m_gatt_if, m_conn_id, &m_service_id.id.uuid);
  }

void esp32bluetoothClientApp::EventSearchResult(esp_ble_gattc_cb_param_t::gattc_search_res_evt_param *search_res)
  {
    if(search_res->srvc_id.uuid.len == m_service_id.id.uuid.len && search_res->srvc_id.uuid.uuid.uuid16 == m_service_id.id.uuid.uuid.uuid16 ){
      ESP_LOGI(TAG, "ESP_GATTC_OPEN_EVT/%s Service found!",m_name);

      m_connected_to_server = true;
      m_service_handle = search_res->start_handle;
      m_service_end_handle = search_res->end_handle;
    }
  }

void esp32bluetoothClientApp::EventSearchComplete(esp_ble_gattc_cb_param_t::gattc_search_cmpl_evt_param *search_cmpl)
  {
    if(m_connected_to_server){
      for(auto characteristic : m_characteristics){
        uint16_t count = 0;
        esp_gatt_status_t status = esp_ble_gattc_get_attr_count( m_gatt_if,
                                                                 m_conn_id,
                                                                 ESP_GATT_DB_CHARACTERISTIC,
                                                                 m_service_handle,
                                                                 m_service_end_handle,
                                                                 0,
                                                                 &count);
        if (status != ESP_GATT_OK){
            ESP_LOGE(TAG, "esp_ble_gattc_get_attr_count error");
        }
        if (count > 0){
            esp_gattc_char_elem_t * char_elem_result = (esp_gattc_char_elem_t *)malloc(sizeof(esp_gattc_char_elem_t) * count);
            if (!char_elem_result){
                ESP_LOGE(TAG, "gattc no mem");
            }else{
                status = esp_ble_gattc_get_char_by_uuid( m_gatt_if,
                                                         m_conn_id,
                                                         m_service_handle,
                                                         m_service_end_handle,
                                                         characteristic->m_char_uuid,
                                                         char_elem_result,
                                                         &count);
                if (status != ESP_GATT_OK){
                    ESP_LOGE(TAG, "esp_ble_gattc_get_char_by_uuid error");
                }
                /*  Every service should have only one char per uuid so only the first one is taken */
                if (count > 0 ){
                  if(!characteristic->m_notifying || (char_elem_result[0].properties & ESP_GATT_CHAR_PROP_BIT_NOTIFY)){
                    ESP_LOGI(TAG, "got characteristic %d(uuid)'s char_handle! ",char_elem_result[0].char_handle);
                    characteristic->m_char_handle = char_elem_result[0].char_handle;
                    if(characteristic->m_notifying){
                      esp_ble_gattc_register_for_notify (m_gatt_if, characteristic->m_remote_bda, characteristic->m_char_handle);
                    }
                  }
                  else
                  {
                    ESP_LOGE(TAG, "BLE gattc characteristic %d(uuid) has no notification enabled even though it is expected by the config", characteristic->m_char_uuid.uuid.uuid16);
                  }
                  
                }
            }
            /* free char_elem_result */
            free(char_elem_result);
        }else{
            ESP_LOGE(TAG, "no characteristic for uuid %d found", characteristic->m_char_uuid.uuid.uuid16);
        }
      }
    }

  }

void esp32bluetoothClientApp::EventRegisterForNotify(esp_ble_gattc_cb_param_t::gattc_reg_for_notify_evt_param *reg_for_not)
  {
    uint16_t count = 0;
    uint16_t notify_en = 1;
    esp32bluetoothCharacteristic * respective_char = NULL;
    for( auto characteristic : m_characteristics){
      if(characteristic->m_char_handle == reg_for_not->handle){
        respective_char = characteristic;
      }
    }
    if(respective_char == NULL){
      ESP_LOGE(TAG, "Couldnt find respective Characteristic fitting char handle for notify reg: %d", reg_for_not->handle);
    }
    esp_gatt_status_t ret_status = esp_ble_gattc_get_attr_count( m_gatt_if,
                                                                 m_conn_id,
                                                                 ESP_GATT_DB_DESCRIPTOR,
                                                                 m_service_handle,
                                                                 m_service_end_handle,
                                                                 respective_char->m_char_handle,
                                                                 &count);
    if (ret_status != ESP_GATT_OK){
        ESP_LOGE(TAG, "esp_ble_gattc_get_attr_count error");
    }
    if (count > 0){
        esp_gattc_descr_elem_t * descr_elem_result = (esp_gattc_descr_elem_t *)malloc(sizeof(esp_gattc_descr_elem_t) * count);
        if (!descr_elem_result){
            ESP_LOGE(TAG, "malloc error, gattc no mem");
        }else{
            ret_status = esp_ble_gattc_get_descr_by_char_handle( m_gatt_if,
                                                                 m_conn_id,
                                                                 reg_for_not->handle,
                                                                 respective_char->m_descr_uuid,
                                                                 descr_elem_result,
                                                                 &count);
            if (ret_status != ESP_GATT_OK){
                ESP_LOGE(TAG, "esp_ble_gattc_get_descr_by_char_handle error");
            }
            esp_err_t err = ESP_OK;
            /* Every char has to have only one descriptor per char & uuid, so we use the first 'descr_elem_result' */
            if (count > 0 && descr_elem_result[0].uuid.len == ESP_UUID_LEN_16 && descr_elem_result[0].uuid.uuid.uuid16 == ESP_GATT_UUID_CHAR_CLIENT_CONFIG){
                respective_char->m_descr_handle = descr_elem_result[0].handle;
                err = esp_ble_gattc_write_char_descr( m_gatt_if,
                                                             m_conn_id,
                                                             descr_elem_result[0].handle,
                                                             sizeof(notify_en),
                                                             (uint8_t *)&notify_en,
                                                             ESP_GATT_WRITE_TYPE_RSP,
                                                             ESP_GATT_AUTH_REQ_NONE);
            }

            if (err != ESP_OK){
                ESP_LOGE(TAG, "esp_ble_gattc_write_char_descr error");
            }
            
            /* free descr_elem_result */
            free(descr_elem_result);
        }
    }
    else{
        ESP_LOGE(TAG, "decsr not found");
    }
  }

void esp32bluetoothClientApp::EventNotify(esp_ble_gattc_cb_param_t::gattc_notify_evt_param *notify)
  {
    esp32bluetoothCharacteristic * respective_char = NULL;
    int char_num = -1;
    for(int i=0; i<m_characteristics.size();i++){
      if(m_characteristics[i]->m_descr_handle == notify->handle){
        respective_char = m_characteristics[i];
        char_num = i;
      }
    }
    if(respective_char == NULL){
      ESP_LOGE(TAG, "Couldnt find respective Characteristic fitting notify handle: %d", notify->handle);
    }
    esp_log_buffer_hex(TAG, notify->value, notify->value_len);
    memcpy(respective_char->m_value_buffer, notify->value, notify->value_len);

    EventValueUpdate(char_num);
  }

void esp32bluetoothClientApp::EventWriteDescr(esp_ble_gattc_cb_param_t::gattc_write_evt_param *write)
  {
  }

void esp32bluetoothClientApp::EventServiceChange(esp_ble_gattc_cb_param_t::gattc_srvc_chg_evt_param *srvc_chg)
  {
    memcpy(m_remode_bd_addr, srvc_chg->remote_bda, sizeof(esp_bd_addr_t));
    ESP_LOGI(TAG, "new bd_addr:");
    esp_log_buffer_hex(TAG, m_remode_bd_addr, sizeof(esp_bd_addr_t));

  }

void esp32bluetoothClientApp::EventWriteChar(esp_ble_gattc_cb_param_t::gattc_write_evt_param *write)
  {
  }

void esp32bluetoothClientApp::EventReadChar(esp_ble_gattc_cb_param_t::gattc_read_char_evt_param *read)
  {
    for(auto characteristic : m_characteristics){
      if(read->handle == characteristic->m_char_handle){
        memcpy(characteristic->m_value_buffer,read->value,read->value_len);
      }
    }
  }

void esp32bluetoothClientApp::EventDisconnect(esp_ble_gattc_cb_param_t::gattc_disconnect_evt_param *disconnect)
  {
    m_ble_connected = false;
    m_connected_to_server = false;
  }

void esp32bluetoothClientApp::EventValueUpdate(int char_num)
  {
  }

void esp32bluetoothClientApp::WriteCharByte(int characteristic_idx, uint8_t data)
{
  m_characteristics[characteristic_idx]->m_value_buffer[0] = data;
  uint8_t write_char_data[2];
  write_char_data[0] = data;

  esp_ble_gattc_write_char(m_gatt_if, m_conn_id, m_characteristics[characteristic_idx]->m_char_handle,1,write_char_data,
                          ESP_GATT_WRITE_TYPE_RSP, ESP_GATT_AUTH_REQ_NONE);
}

void esp32bluetoothClientApp::TriggerReadChar(int characteristic_idx)
{
  esp_ble_gattc_read_char(m_gatt_if, m_conn_id, m_characteristics[characteristic_idx]->m_char_handle, ESP_GATT_AUTH_REQ_NONE);
}

////////////////////////////////////////////////////////////////////////
// esp32bluetoothGATTC

void ble_gattc_event_handler(esp_gattc_cb_event_t event,
                             esp_gatt_if_t gattc_if,
                             esp_ble_gattc_cb_param_t *param)
  {
  MyBluetoothGATTC.EventHandler(event, gattc_if, param);
  }

esp32bluetoothGATTC::esp32bluetoothGATTC()
  {
  ESP_LOGI(TAG, "Initialising Bluetooth GATTC (8013)");

  m_scan_params.scan_type = BLE_SCAN_TYPE_ACTIVE;
  m_scan_params.own_addr_type = BLE_ADDR_TYPE_PUBLIC;
  m_scan_params.scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL;
  m_scan_params.scan_interval = 0x50;
  m_scan_params.scan_window = 0x30;
  m_scan_params.scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE;

  }

esp32bluetoothGATTC::~esp32bluetoothGATTC()
  {
  }

void esp32bluetoothGATTC::RegisterForEvents()
  {
  esp_err_t ret = esp_ble_gattc_register_callback(ble_gattc_event_handler);
  if (ret)
    {
    ESP_LOGE(TAG, "gattc register error, error code = %x", ret);
    return;
    }
  }

void esp32bluetoothGATTC::EventHandler(esp_gattc_cb_event_t event,
                                       esp_gatt_if_t gattc_if,
                                       esp_ble_gattc_cb_param_t *param)
  {
  for (auto app_generic : m_apps)
    {
    esp32bluetoothClientApp* app = static_cast<esp32bluetoothClientApp*>(app_generic);
    if(!app->m_active)
    {
      continue;
    }
    if (event == ESP_GATTC_REG_EVT)
      {
      // Store the gattc_if for the registered app
      if (param->reg.app_id == app->m_app_id)
        {
        if (param->reg.status == ESP_GATT_OK)
          {
          ESP_LOGI(TAG,"ESP_GATTC_REG_EVT register app %s (0x%04x) ok with interface ID %d",
                   app->m_name, param->reg.app_id, gattc_if);
          app->m_gatt_if = gattc_if;
          }
        else
          {
          ESP_LOGE(TAG, "ESP_GATTC_REG_EVT register app %s (0x%04x) failed with status %d",
                   app->m_name,
                   param->reg.app_id,
                   param->reg.status);
          return;
          }
        }
      }
    if ((gattc_if == ESP_GATT_IF_NONE) ||
        (gattc_if == app->m_gatt_if))
      {
      switch (event)
        {
        case ESP_GATTC_REG_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_REG_EVT/%s",app->m_name);

          esp_err_t scan_ret = esp_ble_gap_set_scan_params(&m_scan_params);

          if(scan_ret){
            ESP_LOGE(TAG,"set scan params error, error code %x", scan_ret);
          }
          app->EventRegistered(&param->reg);
          break;
          }
        case ESP_GATTC_CONNECT_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_CONNECT_EVT/%s conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x", 
            app->m_name,
            param->connect.conn_id,
            param->connect.remote_bda[0],
            param->connect.remote_bda[1],
            param->connect.remote_bda[2],
            param->connect.remote_bda[3],
            param->connect.remote_bda[4],
            param->connect.remote_bda[5]);

          app->m_conn_id = param->connect.conn_id;
          memcpy(app->m_remode_bd_addr, param->connect.remote_bda, sizeof(esp_bd_addr_t));

          app->EventConnect(&param->connect);

          esp_err_t mtu_ret = esp_ble_gattc_send_mtu_req (gattc_if, param->connect.conn_id);
          if (mtu_ret){
            ESP_LOGE(TAG, "config MTU error, error code = %x", mtu_ret);
          }
          break;
          }
        case ESP_GATTC_OPEN_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_OPEN_EVT/%s",app->m_name);
          app->EventOpen(&param->open);
          break;
          }

        case ESP_GATTC_CFG_MTU_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_CFG_MTU_EVT/%s, Status %d, MTU %d",
          app->m_name, param->cfg_mtu.status, param->cfg_mtu.mtu);
          app->m_mtu = param->cfg_mtu.mtu;
          app->EventConfigMTU(&param->cfg_mtu);
          break;
          }
        
        case ESP_GATTC_SEARCH_RES_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_SEARCH_RES_EVT/%s, SEARCH RES: conn_id = %x is primary service %d", 
          app->m_name, 
          param->search_res.conn_id, param->search_res.is_primary);
          ESP_LOGI(TAG, "start handle %d end handle %d current handle value %d", param->search_res.start_handle, param->search_res.end_handle, param->search_res.srvc_id.inst_id);
           
          app->EventSearchResult(&param->search_res);
          break;
          }

        case ESP_GATTC_SEARCH_CMPL_EVT:
          {
          if (param->search_cmpl.status != ESP_GATT_OK){
            ESP_LOGE(TAG, "ESP_GATTC_SEARCH_CMPL_EVT/%s, search service failed, error status = %x", app->m_name, param->search_cmpl.status);
            break;
          }
          if(param->search_cmpl.searched_service_source == ESP_GATT_SERVICE_FROM_REMOTE_DEVICE) {
            ESP_LOGI(TAG, "ESP_GATTC_SEARCH_CMPL_EVT/%s, Get service information from remote device", app->m_name);
          } else if (param->search_cmpl.searched_service_source == ESP_GATT_SERVICE_FROM_NVS_FLASH) {
            ESP_LOGI(TAG, "ESP_GATTC_SEARCH_CMPL_EVT/%s, Get service information from flash", app->m_name);
          } else {
            ESP_LOGI(TAG, "ESP_GATTC_SEARCH_CMPL_EVT/%s, unknown service source", app->m_name);
          }

          app->EventSearchComplete(&param->search_cmpl);
          break;
          }

        case ESP_GATTC_REG_FOR_NOTIFY_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_REG_FOR_NOTIFY_EVT/%s", 
          app->m_name);
          if (param->reg_for_notify.status != ESP_GATT_OK){
            ESP_LOGE(TAG, "ESP_GATTC_REG_FOR_NOTIFY_EVT failed: error status = %d", param->reg_for_notify.status);
          }else{
            app->EventRegisterForNotify(&param->reg_for_notify);
          }
          break;
          }

        case ESP_GATTC_NOTIFY_EVT:
          {
          if (param->notify.is_notify){
            ESP_LOGI(TAG, "ESP_GATTC_NOTIFY_EVT/%s, receive notify value:", app->m_name);
          }else{
            ESP_LOGI(TAG, "ESP_GATTC_NOTIFY_EVT/%s, receive indicate value:",app->m_name);
          }

          app->EventNotify(&param->notify);

          break;
          }

        case ESP_GATTC_WRITE_DESCR_EVT:
          {
          if (param->write.status != ESP_GATT_OK){
            ESP_LOGE(TAG, "ESP_GATTC_WRITE_DESCR_EVT/%s, write descr failed, error status = %x", app->m_name, param->write.status);
            break;
          }
          ESP_LOGI(TAG, "ESP_GATTC_WRITE_DESCR_EVT/%s, write descr success ", app->m_name);
          app->EventWriteDescr(&param->write);

          break;
          }

        case ESP_GATTC_SRVC_CHG_EVT:
          {

          ESP_LOGI(TAG, "ESP_GATTC_SRVC_CHG_EVT/%s, ", app->m_name);
          app->EventServiceChange(&param->srvc_chg);

          break;
          }

        case ESP_GATTC_WRITE_CHAR_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_WRITE_CHAR_EVT/%s, ", app->m_name);
          if (param->write.status != ESP_GATT_OK){
            ESP_LOGE(TAG, "write char failed, error status = %x", param->write.status);
            break;
          }
          app->EventWriteChar(&param->write);

          break;
          }
        case ESP_GATTC_READ_CHAR_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_READ_CHAR_EVT/%s, ", app->m_name);
          if (param->read.status != ESP_GATT_OK){
            ESP_LOGE(TAG, "read char failed, error status = %x", param->write.status);
            break;
          }
          app->EventReadChar(&param->read);

          break;
          }

        case ESP_GATTC_DISCONNECT_EVT:
          {
          ESP_LOGI(TAG, "ESP_GATTC_DISCONNECT_EVT/%s, reason = %d", app->m_name, param->disconnect.reason);
          app->EventDisconnect(&param->disconnect);

          break;
          }
        default:
          break;
        }
      }
    }
  }

void esp32bluetoothGATTC::RegisterAllApps()
  {
  for (auto app : m_apps)
    {
    if(!app->m_active)
    {
      continue;
    }
    esp_err_t ret = esp_ble_gattc_app_register(app->m_app_id);
    if (ret)
      {
      ESP_LOGE(TAG, "App %s register error, error code = %x",
        app->m_name, ret);
      }
    else
      {
      ESP_LOGI(TAG,"App %s registered successfully",app->m_name);
      }
    }
  }

void esp32bluetoothGATTC::UnregisterAllApps()
  {
  for (auto app_generic : m_apps)
    {
    esp32bluetoothClientApp* app = static_cast<esp32bluetoothClientApp*>(app_generic);
    if(!app->m_active)
    {
      continue;
    }
    esp_err_t ret = esp_ble_gattc_app_unregister(app->m_gatt_if);
    if (ret)
      {
      ESP_LOGE(TAG, "App %s unregister error, error code = %x",
        app->m_name, ret);
      }
    else
      {
      ESP_LOGI(TAG,"App %s unregistered successfully",app->m_name);
      app->m_ble_connected = false;
      app->m_connected_to_server = false;
      }
    }
  }

void esp32bluetoothGATTC::RegisterApp(esp32bluetoothClientApp* app)
  {
  m_apps.push_back(static_cast<esp32bluetoothApp*>(app));
  app->m_app_slot = m_apps.size()-1;
  }
