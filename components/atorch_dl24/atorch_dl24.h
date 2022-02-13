#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/sensor/sensor.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace atorch_dl24 {

namespace espbt = esphome::esp32_ble_tracker;

class AtorchDL24 : public esphome::ble_client::BLEClientNode, public Component {
 public:
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_capacity_sensor(sensor::Sensor *capacity_sensor) { capacity_sensor_ = capacity_sensor; }
  void set_energy_sensor(sensor::Sensor *energy_sensor) { energy_sensor_ = energy_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_dim_backlight_sensor(sensor::Sensor *dim_backlight_sensor) { dim_backlight_sensor_ = dim_backlight_sensor; }
  void set_running_sensor(sensor::Sensor *running_sensor) { running_sensor_ = running_sensor; }

  void set_check_crc(bool check_crc) { check_crc_ = check_crc; }

 protected:
  void decode_(const uint8_t *data, uint16_t length);
  void publish_state_(sensor::Sensor *sensor, float value);

  uint16_t char_handle_;
  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *power_sensor_{nullptr};
  sensor::Sensor *capacity_sensor_{nullptr};
  sensor::Sensor *energy_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *dim_backlight_sensor_{nullptr};
  sensor::Sensor *running_sensor_{nullptr};

  bool check_crc_;

  uint8_t previous_value_ = 61;
};

}  // namespace atorch_dl24
}  // namespace esphome

#endif
