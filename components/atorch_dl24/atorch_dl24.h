#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/hal.h"

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

  void set_throttle(uint32_t throttle) { this->throttle_ = throttle; }

  void set_running_binary_sensor(binary_sensor::BinarySensor *running_binary_sensor) {
    running_binary_sensor_ = running_binary_sensor;
  }

  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_capacity_sensor(sensor::Sensor *capacity_sensor) { capacity_sensor_ = capacity_sensor; }
  void set_energy_sensor(sensor::Sensor *energy_sensor) { energy_sensor_ = energy_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_dim_backlight_sensor(sensor::Sensor *dim_backlight_sensor) { dim_backlight_sensor_ = dim_backlight_sensor; }
  void set_usb_data_minus_sensor(sensor::Sensor *usb_data_minus_sensor) {
    usb_data_minus_sensor_ = usb_data_minus_sensor;
  }
  void set_usb_data_plus_sensor(sensor::Sensor *usb_data_plus_sensor) { usb_data_plus_sensor_ = usb_data_plus_sensor; }
  void set_price_per_kwh_sensor(sensor::Sensor *price_per_kwh_sensor) { price_per_kwh_sensor_ = price_per_kwh_sensor; }
  void set_frequency_sensor(sensor::Sensor *frequency_sensor) { frequency_sensor_ = frequency_sensor; }
  void set_power_factor_sensor(sensor::Sensor *power_factor_sensor) { power_factor_sensor_ = power_factor_sensor; }
  void set_runtime_sensor(sensor::Sensor *runtime_sensor) { runtime_sensor_ = runtime_sensor; }

  void set_runtime_formatted_text_sensor(text_sensor::TextSensor *runtime_formatted_text_sensor) {
    runtime_formatted_text_sensor_ = runtime_formatted_text_sensor;
  }

  void set_check_crc(bool check_crc) { check_crc_ = check_crc; }
  void decode(const uint8_t *data, uint16_t length);
  bool write_register(uint8_t device_type, uint8_t address, uint32_t value);
  uint8_t get_device_type() { return this->device_type_; }

 protected:
  void decode_ac_and_dc_(const uint8_t *data, uint16_t length);
  void decode_usb_(const uint8_t *data, uint16_t length);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);

  std::string format_runtime_(const uint32_t value) {
    int seconds = (int) value;
    int years = seconds / (24 * 3600 * 365);
    seconds = seconds % (24 * 3600 * 365);
    int days = seconds / (24 * 3600);
    seconds = seconds % (24 * 3600);
    int hours = seconds / 3600;
    seconds = seconds % 3600;
    int minutes = seconds / 60;
    seconds = seconds % 60;
    return (years ? to_string(years) + "y " : "") + (days ? to_string(days) + "d " : "") +
           (hours ? to_string(hours) + "h " : "") + (minutes ? to_string(minutes) + "m " : "") +
           (seconds ? to_string(seconds) + "s" : "");
  }

  binary_sensor::BinarySensor *running_binary_sensor_;

  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *current_sensor_{nullptr};
  sensor::Sensor *power_sensor_{nullptr};
  sensor::Sensor *capacity_sensor_{nullptr};
  sensor::Sensor *energy_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *dim_backlight_sensor_{nullptr};
  sensor::Sensor *usb_data_minus_sensor_{nullptr};
  sensor::Sensor *usb_data_plus_sensor_{nullptr};
  sensor::Sensor *price_per_kwh_sensor_{nullptr};
  sensor::Sensor *frequency_sensor_{nullptr};
  sensor::Sensor *power_factor_sensor_{nullptr};
  sensor::Sensor *runtime_sensor_{nullptr};

  text_sensor::TextSensor *runtime_formatted_text_sensor_;

  bool check_crc_;
  bool incomplete_notify_value_received_ = false;

  uint8_t previous_value_ = 61;
  uint8_t composite_notify_value_[36];
  uint8_t device_type_ = 0x00;
  uint16_t char_notify_handle_;
  uint16_t char_command_handle_;
  uint32_t last_publish_{0};
  uint32_t throttle_{0};
};

}  // namespace atorch_dl24
}  // namespace esphome

#endif
