#pragma once

#include "../atorch_dl24.h"
#include "esphome/components/button/button.h"

namespace esphome::atorch_dl24 {

class AtorchDL24;
class AtorchButton : public button::Button {
 public:
  void set_parent(AtorchDL24 *parent) { this->parent_ = parent; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };

 protected:
  void press_action() override;
  AtorchDL24 *parent_;
  uint8_t holding_register_;
};

}  // namespace esphome::atorch_dl24
