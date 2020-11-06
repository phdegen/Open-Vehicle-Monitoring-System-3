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
#include "esp32bluetooth.h"
#include "esp32bluetooth_gap.h"
#include "esp32bluetooth_gatts.h"
#include "esp32bluetooth_gattc.h"
#include "esp_bt.h"
#include "ovms_peripherals.h"
#include "ovms_utils.h"

#include "ovms_log.h"
static const char *TAG = "bt";

esp32bluetooth::esp32bluetooth(const char* name)
  : pcp(name)
  {
  m_service_running = false;
  m_powermode = Off;

  MyConfig.RegisterParam("bt", "Bluetooth BLE Configs", true, true);
  ConfigChanged(NULL);
  }

esp32bluetooth::~esp32bluetooth()
  {
  }

void esp32bluetooth::StartService()
  {
  esp_err_t ret;

  if (m_service_running)
    {
    ESP_LOGE(TAG,"Bluetooth service cannot start (already running)");
    return;
    }

  ESP_LOGI(TAG,"Powering bluetooth on...");

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  ret = esp_bt_controller_init(&bt_cfg);
  if (ret)
    {
    ESP_LOGE(TAG, "init controller failed: %s", esp_err_to_name(ret));
    return;
    }

  ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
  if (ret)
    {
    ESP_LOGE(TAG, "enable controller failed: %s", esp_err_to_name(ret));
    return;
    }

  ret = esp_bluedroid_init();
  if (ret)
    {
    ESP_LOGE(TAG, "init bluetooth failed: %s", esp_err_to_name(ret));
    return;
    }

  ret = esp_bluedroid_enable();
  if (ret)
    {
    ESP_LOGE(TAG, "enable bluetooth failed: %s", esp_err_to_name(ret));
    return;
    }

  if(m_gatts_active)
  {    
    MyBluetoothGATTS.RegisterForEvents();
  }
  if(m_gattc_active)
  {   
    MyBluetoothGATTC.RegisterForEvents();
  }


  MyBluetoothGAP.RegisterForEvents();

  if(m_gatts_active)
  {
    MyBluetoothGATTS.RegisterAllApps();
  }
  if(m_gattc_active)
  {
    MyBluetoothGATTC.RegisterAllApps();
  }

  /* set the security iocap & auth_req & key size & init key response key parameters to the stack*/
  esp_ble_auth_req_t auth_req = ESP_LE_AUTH_BOND;     //bonding with peer device after authentication
  esp_ble_io_cap_t iocap = ESP_IO_CAP_OUT;           //set the IO capability to output but no input
  uint8_t key_size = 16;      //the key size should be 7~16 bytes
  uint8_t init_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  uint8_t rsp_key = ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK;
  esp_ble_gap_set_security_param(ESP_BLE_SM_AUTHEN_REQ_MODE, &auth_req, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_IOCAP_MODE, &iocap, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_MAX_KEY_SIZE, &key_size, sizeof(uint8_t));
  /* If your BLE device act as a Slave, the init_key means you hope which types of key of the master should distribut to you,
  and the response key means which key you can distribut to the Master;
  If your BLE device act as a master, the response key means you hope which types of key of the slave should distribut to you,
  and the init key means which key you can distribut to the slave. */
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_INIT_KEY, &init_key, sizeof(uint8_t));
  esp_ble_gap_set_security_param(ESP_BLE_SM_SET_RSP_KEY, &rsp_key, sizeof(uint8_t));

  m_service_running = true;
  }

void esp32bluetooth::StopService()
  {
  esp_err_t ret;

  if (!m_service_running)
    {
    ESP_LOGE(TAG,"Bluetooth service cannot stop (not running)");
    return;
    }

  ESP_LOGI(TAG,"Powering bluetooth off...");

  if(m_gatts_active)
  {
    MyBluetoothGATTS.UnregisterAllApps();
  }

  if(m_gattc_active)
  {
    MyBluetoothGATTC.UnregisterAllApps();
  }

  ret = esp_bluedroid_disable();
  if (ret)
    {
    ESP_LOGE(TAG, "disable bluetooth failed: %s", esp_err_to_name(ret));
    return;
    }

  ret = esp_bluedroid_deinit();
  if (ret)
    {
    ESP_LOGE(TAG, "deinit bluetooth failed: %s", esp_err_to_name(ret));
    return;
    }

  ret = esp_bt_controller_disable();
  if (ret)
    {
    ESP_LOGE(TAG, "disable controller failed: %s", esp_err_to_name(ret));
    return;
    }

  ret = esp_bt_controller_deinit();
  if (ret)
    {
    ESP_LOGE(TAG, "deinit controller failed: %s", esp_err_to_name(ret));
    return;
    }

  m_service_running = false;
  }

bool esp32bluetooth::IsServiceRunning()
  {
  return m_service_running;
  }

void esp32bluetooth::SetPowerMode(PowerMode powermode)
  {
  m_powermode = powermode;
  switch (powermode)
    {
    case On:
      if (!m_service_running) StartService();
      break;
    case Sleep:
    case DeepSleep:
    case Off:
      if (m_service_running) StopService();
      break;
    default:
      break;
    };
  }

void esp32bluetooth::ConfigChanged(OvmsConfigParam* param)
  {
  if (param && param->GetName() != "bt")
    return;
  ESP_LOGD(TAG, "load config");

  m_gatts_active = MyConfig.GetParamValueBool("bt", "gatts.active", false);
  m_gattc_active = MyConfig.GetParamValueBool("bt", "gattc.active", false);
  }

void bluetooth_status(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  esp32bluetooth *me = MyPeripherals->m_esp32bluetooth;

  if (!me->IsServiceRunning())
    {
    writer->puts("Bluetooth service is not running");
    return;
    }

  int nclients = esp_ble_get_bond_device_num();
  esp_ble_bond_dev_t *clist = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * nclients);
  esp_ble_get_bond_device_list(&nclients, clist);
  writer->printf("Bluetooth service is running (%d client(s) registered)\n",nclients);
  for (int k = 0; k < nclients; k++)
    {
    writer->printf("  %02x:%02x:%02x:%02x:%02x:%02x\n",
      clist[k].bd_addr[0],
      clist[k].bd_addr[1],
      clist[k].bd_addr[2],
      clist[k].bd_addr[3],
      clist[k].bd_addr[4],
      clist[k].bd_addr[5]);
    }

  free(clist);
  }

void bluetooth_clear(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  esp32bluetooth *me = MyPeripherals->m_esp32bluetooth;

  if (!me->IsServiceRunning())
    {
    writer->puts("Error: Bluetooth service is not running");
    return;
    }

  int nclients = esp_ble_get_bond_device_num();
  esp_ble_bond_dev_t *clist = (esp_ble_bond_dev_t *)malloc(sizeof(esp_ble_bond_dev_t) * nclients);
  esp_ble_get_bond_device_list(&nclients, clist);
  for (int k = 0; k < nclients; k++)
    {
    char bta[19];
    sprintf(bta,"%02x:%02x:%02x:%02x:%02x:%02x",
      clist[k].bd_addr[0],
      clist[k].bd_addr[1],
      clist[k].bd_addr[2],
      clist[k].bd_addr[3],
      clist[k].bd_addr[4],
      clist[k].bd_addr[5]);
    if ((argc==0)||(strcasecmp(bta,argv[0])==0))
      {
      writer->printf("Removing Registration: %s\n",bta);
      esp_ble_remove_bond_device(clist[k].bd_addr);
      }
    }

  free(clist);
  }

void bluetooth_set_gatts_active(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  esp32bluetooth *me = MyPeripherals->m_esp32bluetooth;
  bool gatts_active = me->m_gatts_active;
  if(argc>0){
    gatts_active = strtobool(std::string(argv[0]));
    me->m_gatts_active = gatts_active;
    MyConfig.SetParamValueBool("bt", "gatts.active", gatts_active);
  }
  MyBluetoothGAP.m_gatts_active = gatts_active;
  std::string return_str = me->m_gatts_active ? "gatts active\n" : "gatts inactive\n";
  writer->printf("%s",return_str.c_str());
  }

void bluetooth_set_gattc_active(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  esp32bluetooth *me = MyPeripherals->m_esp32bluetooth;
  bool gattc_active = me->m_gattc_active;
  if(argc>0){
    gattc_active = strtobool(std::string(argv[0]));
    me->m_gattc_active = gattc_active;
    MyConfig.SetParamValueBool("bt", "gattc.active", gattc_active);
  }
  MyBluetoothGAP.m_gattc_active = gattc_active;
  std::string return_str = me->m_gattc_active ? "gattc active\n" : "gattc inactive\n" ;
  writer->printf("%s",return_str.c_str());
  }

void bluetooth_list_gatt_apps(esp32bluetoothGATT* gatt_category, int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  std::string return_string = "";
  for(int i=0;i<gatt_category->m_apps.size();i++)
  {
    std::string app_str = "App Nr.";
    char int_str[10];
    sprintf(int_str, "%d", i);
    app_str.append(int_str);
    app_str.append(": ");
    app_str.append(gatt_category->m_apps[i]->m_name);
    app_str.append(" - ");
    std::string active_str = gatt_category->m_apps[i]->m_active ? "active" : "inactive";
    app_str.append(active_str);
    app_str.append("\n");
    return_string.append(app_str);
  }
  
  writer->printf("%s",return_string.c_str());

  }

void bluetooth_list_gatts_apps(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  std::string return_string = "BLE GATTS Apps: \n";
  writer->printf("%s",return_string.c_str());
  bluetooth_list_gatt_apps(static_cast<esp32bluetoothGATT*>(&MyBluetoothGATTS),verbosity,writer,cmd,argc,argv);
  }

void bluetooth_list_gattc_apps(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  std::string return_string = "BLE GATTC Apps: \n";
  writer->printf("%s",return_string.c_str());
  bluetooth_list_gatt_apps(static_cast<esp32bluetoothGATT*>(&MyBluetoothGATTC),verbosity,writer,cmd,argc,argv);
  }

void bluetooth_activeate_gatt_app(esp32bluetoothGATT* gatt_category, int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
    
    if(argc == 2)
    {
      int idx = atoi(argv[0]);
      bool active = strtobool(std::string(argv[1]));

      MyBluetoothGATTS.m_apps[idx]->m_active = active;
    }
    else
    {
      writer->printf("wrong amount of arguments: needs 2 (idx(int) and activeate/deactivate(bool)");
    }
  }

void bluetooth_activeate_gatts_app(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
    bluetooth_activeate_gatt_app(static_cast<esp32bluetoothGATT*>(&MyBluetoothGATTS),verbosity,writer,cmd,argc,argv);
  }

void bluetooth_activeate_gattc_app(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
    bluetooth_activeate_gatt_app(static_cast<esp32bluetoothGATT*>(&MyBluetoothGATTC),verbosity,writer,cmd,argc,argv);
  }

void bluetooth_gatt_app_info(esp32bluetoothGATT* gatt_category, int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
  if(argc == 1)
    {
      int idx = atoi(argv[0]);
      auto app = gatt_category->m_apps[idx];

      std::string name = app->m_name;
      int num_chars = app->m_characteristics.size();
      std::string chars_string = "";
      chars_string.append("App - ");
      chars_string.append(name);
      chars_string.append(":\n");
      for(int i=0;i<num_chars; i++)
      {
        chars_string.append("characteristic nr. ");
        char int_str[10];
        sprintf(int_str, "%d", i);
        chars_string.append(int_str);
        chars_string.append(" with uuid: ");
        auto uuid = app->m_characteristics[i]->m_char_uuid;
        uint8_t * uuid_p = NULL;
        switch (uuid.len)
        {
        case ESP_UUID_LEN_16:
          uuid_p = (uint8_t*) &(uuid.uuid.uuid16);
          break;
        case ESP_UUID_LEN_32:
          uuid_p = (uint8_t*) &(uuid.uuid.uuid32);
          break;
        case ESP_UUID_LEN_128:
          uuid_p = uuid.uuid.uuid128;
          break;
        default:
          break;
        }
        char uuid_str[40];
        const char * uuid_str_chararr = uuid_str;
        char * running_str_p = uuid_str;
        for(int k=1;k<=uuid.len;k++)
        {
          running_str_p = HexByte(running_str_p,*(uuid_p+uuid.len-k));
        }
        std::string hex_uuid_str = std::string(uuid_str_chararr,2*uuid.len);
        chars_string.append(hex_uuid_str);
        chars_string.append("\n");
      }
      writer->printf(chars_string.c_str());
    }
    else
    {
      writer->printf("wrong amount of arguments: needs idx(int)");
    }
  }

void bluetooth_gatts_app_info(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
    bluetooth_gatt_app_info(static_cast<esp32bluetoothGATT*>(&MyBluetoothGATTS),verbosity,writer,cmd,argc,argv);
  }

void bluetooth_gattc_app_info(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv)
  {
    bluetooth_gatt_app_info(static_cast<esp32bluetoothGATT*>(&MyBluetoothGATTC),verbosity,writer,cmd,argc,argv);
  }

class esp32bluetoothInit
  {
  public: esp32bluetoothInit();
  } esp32bluetoothInit  __attribute__ ((init_priority (8010)));

esp32bluetoothInit::esp32bluetoothInit()
  {
  ESP_LOGI(TAG, "Initialising ESP32 BLUETOOTH (8010)");

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

  OvmsCommand* cmd_bt = MyCommandApp.RegisterCommand("bt","BLUETOOTH framework", bluetooth_status);
  cmd_bt->RegisterCommand("status","Show bluetooth status",bluetooth_status);
  cmd_bt->RegisterCommand("clear","Clear bluetooth registrations",bluetooth_clear, "(<mac>)", 0, 1);
  
  OvmsCommand* cmd_ble_gatts = cmd_bt->RegisterCommand("gatts", "Set ble gatt server active configuration", bluetooth_set_gatts_active);
  OvmsCommand* cmd_ble_gattc = cmd_bt->RegisterCommand("gattc", "Set ble gatt client active configuration", bluetooth_set_gattc_active);
  cmd_ble_gatts->RegisterCommand("active", "Set ble gatt server active configuration", bluetooth_set_gatts_active, "true/false",0,1);
  cmd_ble_gattc->RegisterCommand("active", "Set ble gatt client active configuration", bluetooth_set_gattc_active, "true/false",0,1);
  
  OvmsCommand* cmd_gatts_apps = cmd_ble_gatts->RegisterCommand("app", "list gatts apps with status", bluetooth_list_gatts_apps);
  OvmsCommand* cmd_gattc_apps = cmd_ble_gattc->RegisterCommand("app", "list gattc apps with status", bluetooth_list_gattc_apps);
  cmd_gatts_apps->RegisterCommand("list", "list gatts apps with status", bluetooth_list_gatts_apps);
  cmd_gattc_apps->RegisterCommand("list", "list gattc apps with status", bluetooth_list_gattc_apps);
  cmd_gatts_apps->RegisterCommand("activate", "activate respective app with index", bluetooth_activeate_gatts_app,"app_idx[int] true/false",2,2);
  cmd_gattc_apps->RegisterCommand("activate", "activate respective app with index", bluetooth_activeate_gattc_app,"app_idx[int] true/false",2,2);
  cmd_gatts_apps->RegisterCommand("info", "get app info(characteristics) with index", bluetooth_gatts_app_info,"app_idx[int]",1,1);
  cmd_gattc_apps->RegisterCommand("info", "get app info(characteristics) with index", bluetooth_gattc_app_info,"app_idx[int]",1,1);

  }
