#include "atorch_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::atorch_dl24 {

ESPHOME_LOG_TAG(TAG, "atorch_dl24.button");

void AtorchButton::dump_config() { LOG_BUTTON("", "AtorchDL24 Button", this); }
void AtorchButton::press_action() {
  uint8_t device_type = this->parent_->get_device_type();
  if (device_type == 0x00) {
    device_type = 0x02;
    ESP_LOGW(TAG, "Device type not detected. Using device type 0x02 (DC meter) as fallback");
  }

  this->parent_->write_register(device_type, this->holding_register_, 0x00000000);
}

}  // namespace esphome::atorch_dl24
