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

#ifndef __ESP32BLUETOOTH_GATTC_APP_EXAMPLE_H__
#define __ESP32BLUETOOTH_GATTC_APP_EXAMPLE_H__

#include "esp32bluetooth.h"
#include "esp32bluetooth_gattc.h"

#define GATTC_APP_UUID_OVMS_EXAMPLE       0x01
#define GATTC_SERVICE_UUID_OVMS_EXAMPLE   0x1234
#define GATTC_CHAR_UUID_OVMS_EXAMPLE_0    0x5555
#define GATTC_CHAR_UUID_OVMS_EXAMPLE_1    0x5556
#define GATTS_DESCR_UUID_OVMS_EXAMPLE_1   0x5768

void gattc_app_write_stuff(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv);
void gattc_app_read_stuff(int verbosity, OvmsWriter* writer, OvmsCommand* cmd, int argc, const char* const* argv);

class OvmsBluetoothAppGATTCExample : public esp32bluetoothClientApp
  {
  public:
    OvmsBluetoothAppGATTCExample();
    ~OvmsBluetoothAppGATTCExample();

  public:
    void EventValueUpdate(int char_num) override;
    void EventReadChar(esp_ble_gattc_cb_param_t::gattc_read_char_evt_param *read) override;
    void WriteToCharacteristics();
    void ReadFromCharacteristics();

  public:
    esp32bluetoothCharacteristic m_example_characteristic_0;
    esp32bluetoothCharacteristic m_example_characteristic_1;
  };

#endif //#ifndef __ESP32BLUETOOTH_GATTC_APP_EXAMPLE_H__
