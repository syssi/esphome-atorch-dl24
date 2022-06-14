#pragma once

#include "../atorch_dl24.h"
#include "esphome/core/component.h"
#include "esphome/components/button/button.h"

namespace esphome {
namespace atorch_dl24 {

class AtorchDL24;
class AtorchButton : public button::Button, public Component {
 public:
  void set_parent(AtorchDL24 *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };
  void dump_config() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void press_action() override;
  AtorchDL24 *parent_;
  uint8_t holding_register_;
};

}  // namespace atorch_dl24
}  // namespace esphome
