#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/sensor/sensor.h"

// @FIXME: This define should be used as soon as esphome 2021.10 is released
// #ifdef USE_ESP32
#ifdef ARDUINO_ARCH_ESP32  // NOLINT

#include <esp_gattc_api.h>

namespace esphome {
namespace atorch_dl24 {

namespace espbt = esphome::esp32_ble_tracker;

class AtorchDL24 : public esphome::ble_client::BLEClientNode, public PollingComponent {
 public:
  void setup() override;
  void update() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_voltage(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void set_current(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_power(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_energy(sensor::Sensor *energy_sensor) { energy_sensor_ = energy_sensor; }
  void set_temperature(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }

 protected:
  void decode(const uint8_t *data, uint16_t length);
  void publish_state_(sensor::Sensor *sensor, float value);

  uint16_t char_handle_;
  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *power_sensor_{nullptr};
  sensor::Sensor *energy_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
};

}  // namespace atorch_dl24
}  // namespace esphome

#endif
