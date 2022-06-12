#include "atorch_dl24.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace atorch_dl24 {

static const char *const TAG = "atorch_dl24";

static const uint16_t DL24_SERVICE_UUID = 0xFFE0;
static const uint16_t DL24_CHARACTERISTIC_UUID = 0xFFE1;

static const uint8_t START_OF_FRAME_BYTE1 = 0xFF;
static const uint8_t START_OF_FRAME_BYTE2 = 0x55;

static const uint8_t MESSAGE_TYPE_REPORT = 0x01;
static const uint8_t MESSAGE_TYPE_REPLY = 0x02;
static const uint8_t MESSAGE_TYPE_COMMAND = 0x11;

static const uint8_t DEVICE_TYPE_AC_METER = 0x01;
static const uint8_t DEVICE_TYPE_DC_METER = 0x02;
static const uint8_t DEVICE_TYPE_USB_METER = 0x03;

uint8_t crc(const uint8_t data[], const uint16_t len) {
  uint8_t crc = 0;

  // skip header
  for (uint16_t i = 2; i < len; i++) {
    crc = crc + data[i];
  }
  return crc ^ 0x44;
}

void AtorchDL24::dump_config() {
  ESP_LOGCONFIG(TAG, "DL24");
  LOG_SENSOR(" ", "Voltage", this->voltage_sensor_);
  LOG_SENSOR(" ", "Current", this->current_sensor_);
  LOG_SENSOR(" ", "Power", this->power_sensor_);
  LOG_SENSOR(" ", "Capacity", this->capacity_sensor_);
  LOG_SENSOR(" ", "Energy", this->energy_sensor_);
  LOG_SENSOR(" ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR(" ", "Dim Backlight", this->dim_backlight_sensor_);
  LOG_SENSOR(" ", "Running", this->running_sensor_);
  LOG_SENSOR(" ", "USB Data Minus", this->usb_data_minus_sensor_);
  LOG_SENSOR(" ", "USB Data Plus", this->usb_data_plus_sensor_);
  LOG_SENSOR(" ", "Price per kWh", this->price_per_kwh_sensor_);
  LOG_SENSOR(" ", "Frequency ", this->frequency_sensor_);
  LOG_SENSOR(" ", "Power Factor", this->power_factor_sensor_);
  LOG_SENSOR(" ", "Runtime", this->runtime_sensor_);
  LOG_TEXT_SENSOR(" ", "Runtime Formatted", this->runtime_formatted_text_sensor_);
}

void AtorchDL24::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                     esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      this->node_state = espbt::ClientState::IDLE;

      this->publish_state_(this->voltage_sensor_, NAN);
      this->publish_state_(this->current_sensor_, NAN);
      this->publish_state_(this->power_sensor_, NAN);
      this->publish_state_(this->capacity_sensor_, NAN);
      this->publish_state_(this->energy_sensor_, NAN);
      this->publish_state_(this->temperature_sensor_, NAN);
      this->publish_state_(this->dim_backlight_sensor_, NAN);
      this->publish_state_(this->running_sensor_, NAN);
      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *chr = this->parent_->get_characteristic(DL24_SERVICE_UUID, DL24_CHARACTERISTIC_UUID);
      if (chr == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not an DL24..?", this->parent_->address_str().c_str());
        break;
      }
      this->char_handle_ = chr->handle;

      auto status =
          esp_ble_gattc_register_for_notify(this->parent()->gattc_if, this->parent()->remote_bda, chr->handle);
      if (status) {
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
      }
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = espbt::ClientState::ESTABLISHED;
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      if (param->notify.handle != this->char_handle_)
        break;

      ESP_LOGVV(TAG, "Notification received: %s",
                format_hex_pretty(param->notify.value, param->notify.value_len + 0).c_str());

      // Composite two short notifications into a complete one
      // FF.55.01.02.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00 (20)
      if (!this->incomplete_notify_value_received_ && param->notify.value_len == 20 &&
          param->notify.value[0] == START_OF_FRAME_BYTE1 && param->notify.value[1] == START_OF_FRAME_BYTE2) {
        memcpy(this->composite_notfiy_value_, param->notify.value, param->notify.value_len);
        this->incomplete_notify_value_received_ = true;
        break;
      }

      // 00.00.00.00.00.1E.00.00.00.00.3C.00.00.00.00.19 (16)
      if (this->incomplete_notify_value_received_ && param->notify.value_len == 16) {
        memcpy(this->composite_notfiy_value_ + 20, param->notify.value, param->notify.value_len);
        // FF.55.01.02.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.00.1E.00.00.00.00.3C.00.00.00.00.19
        this->decode(this->composite_notfiy_value_, 36);
        this->incomplete_notify_value_received_ = false;
        this->composite_notfiy_value_[0] = 0x00;
        break;
      }

      this->decode(param->notify.value, param->notify.value_len);

      break;
    }
    default:
      break;
  }
}

// AC Meter
//
// FF55010100090400000E0000040000000000006401F40085002F00000A093C0000000039
// FF5501010008EB000000000000000001FE00006401F40000002F003125143C0000000066
// FF5501010008FF0000270000210000000000006401F401740031000038083C0000000088
// https://github.com/NiceLabs/atorch-console/blob/master/src/service/atorch-packet/packet-meter-ac.spec.ts

// DC Meter
//
// FF55010200011A00003C0004D40000002000006400000000002600590D363C00000000F0
// FF55010200011A0000000004D40000002000006400000000002A00590E343C000000003F
// https://github.com/NiceLabs/atorch-console/blob/master/src/service/atorch-packet/packet-meter-dc.spec.ts

// USB Meter
//
// FF5501030001F3000000000638000003110007000A000000122E333C000000000000004E
// FF5501030001FB000000003CC70000554E00070007000000472F243C00000000000000CE
// FF5501030001CD00007F003CC80000554E0009000A00000047300D3C000000000000008F
// FF5501030001FB000001006C3F00006C4400070006001A00471C1A3C0000000000000078
// https://github.com/NiceLabs/atorch-console/blob/master/src/service/atorch-packet/packet-meter-usb.spec.ts
void AtorchDL24::decode(const uint8_t *data, uint16_t length) {
  if (length != 36) {
    ESP_LOGW(TAG, "Report skipped because of invalid length");
    return;
  }

  if (this->check_crc_ && crc(data, length - 1) != data[35]) {
    ESP_LOGW(TAG, "CRC check failed. Skipping frame");
    return;
  }

  if (data[0] != START_OF_FRAME_BYTE1 && data[1] != START_OF_FRAME_BYTE2) {
    ESP_LOGW(TAG, "Invalid header");
    return;
  }

  if (data[2] != MESSAGE_TYPE_REPORT) {
    ESP_LOGW(TAG, "Unsupported message type (%02X)", data[2]);
    return;
  }

  uint8_t device_type = data[3];
  switch (device_type) {
    case DEVICE_TYPE_AC_METER:
    case DEVICE_TYPE_DC_METER:
      this->decode_ac_and_dc_(data, length);
      break;
    case DEVICE_TYPE_USB_METER:
      this->decode_usb_(data, length);
      break;
    default:
      ESP_LOGW(TAG, "Unsupported device type (%02X)", device_type);
  }
}

void AtorchDL24::decode_ac_and_dc_(const uint8_t *data, uint16_t length) {
  auto dl24_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };
  auto dl24_get_24bit = [&](size_t i) -> uint32_t {
    return (uint32_t(data[i + 0]) << 16) | (uint32_t(data[i + 1]) << 8) | (uint32_t(data[i + 2]) << 0);
  };
  auto dl24_get_32bit = [&](size_t i) -> uint32_t {
    return (uint32_t(dl24_get_16bit(i + 0)) << 16) | (uint32_t(dl24_get_16bit(i + 2)) << 0);
  };

  // Example responses
  //
  // FF.55.01.02.00.00.20.00.4E.20.00.13.FA.00.00.00.11.00.00.00.00.00.00.00.00.25.00.02.21.1A.3C.00.00.00.00.09
  // FF.55.01.02.00.00.20.00.4E.1E.00.13.FA.00.00.00.11.00.00.00.00.00.00.00.00.25.00.02.21.1B.3C.00.00.00.00.08
  // FF.55.01.02.00.00.20.00.4E.21.00.13.FB.00.00.00.11.00.00.00.00.00.00.00.00.25.00.02.21.1C.3C.00.00.00.00.15
  // FF.55.01.02.00.00.20.00.4E.20.00.13.FC.00.00.00.11.00.00.00.00.00.00.00.00.25.00.02.21.1D.3C.00.00.00.00.16
  // FF.55.01.02.00.00.20.00.4E.1B.00.13.FC.00.00.00.11.00.00.00.00.00.00.00.00.25.00.02.21.1E.3C.00.00.00.00.0A
  // FF.55.01.02.00.00.20.00.4E.23.00.13.FD.00.00.00.11.00.00.00.00.00.00.00.00.25.00.02.21.1F.3C.00.00.00.00.1C
  //
  // 0xFF 0x55:            Magic header
  // 0x01:                 Message type           0x01: Report (32 byte), 0x02: Reply (4 byte), 0x11: Command (6 byte)
  // 0x02:                 Device type            0x01: AC meter, 0x02: DC meter, 0x03: USB meter
  // 0x00 0x00 0x20:       Voltage                32 * 0.1 = 3.2 V
  float voltage = dl24_get_24bit(4) * 0.1f;
  this->publish_state_(this->voltage_sensor_, voltage);

  // 0x00 0x4E 0x23:       Current                20003 * 0.001 = 20.003 A
  float current = dl24_get_24bit(7) * 0.001f;
  this->publish_state_(this->current_sensor_, current);
  this->publish_state_(this->power_sensor_, voltage * current);

  // 0x00 0x13 0xFD:       Capacity in Ah        5117 * 0.01 = 51.17 Ah
  this->publish_state_(this->capacity_sensor_, dl24_get_24bit(10) * 0.01f);

  // 0x00 0x00 0x00 0x11:  Energy in Wh           17 * 10.0 = 170.0
  this->publish_state_(this->energy_sensor_, dl24_get_32bit(13) * 10.0f);

  if (data[3] == DEVICE_TYPE_AC_METER) {
    // 0x00 0x00 0x00:       Price per kWh          value * 0.01
    this->publish_state_(this->price_per_kwh_sensor_, dl24_get_24bit(17) * 0.01f);

    // 0x00 0x00:            Frequency              value * 0.1 Hz
    this->publish_state_(this->frequency_sensor_, dl24_get_16bit(20) * 0.1f);

    // 0x00 0x00:            Power factor           value * 0.001
    this->publish_state_(this->power_factor_sensor_, dl24_get_16bit(22) * 0.001f);
  }

  // 0x00 0x25:            Temperature            37 °C
  this->publish_state_(this->temperature_sensor_, (float) dl24_get_16bit(24));

  // 0x00 0x02:            Hour                   2 h
  // 0x21:                 Minute                 33 min
  // 0x1F:                 Second                 31 sec
  uint32_t runtime = (dl24_get_16bit(26) * 3600) + (data[28] * 60) + data[29];
  this->publish_state_(this->runtime_sensor_, (float) runtime);
  if (this->runtime_formatted_text_sensor_ != nullptr) {
    this->publish_state_(this->runtime_formatted_text_sensor_, format_runtime_(runtime));
  }

  if (previous_value_ != 61)
    this->publish_state_(this->running_sensor_, (float) (previous_value_ != data[29]));
  previous_value_ = data[29];

  // 0x3C:                 Dim backlight          60 seconds
  this->publish_state_(this->dim_backlight_sensor_, (float) data[30]);

  // 0x00 0x00 0x00 0x00:  Unknown
  // 0x1C:                 Checksum
}

void AtorchDL24::decode_usb_(const uint8_t *data, uint16_t length) {
  auto dl24_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 8) | (uint16_t(data[i + 1]) << 0);
  };
  auto dl24_get_24bit = [&](size_t i) -> uint32_t {
    return (uint32_t(data[i + 0]) << 16) | (uint32_t(data[i + 1]) << 8) | (uint32_t(data[i + 2]) << 0);
  };
  auto dl24_get_32bit = [&](size_t i) -> uint32_t {
    return (uint32_t(dl24_get_16bit(i + 0)) << 16) | (uint32_t(dl24_get_16bit(i + 2)) << 0);
  };

  // Example responses
  //
  // FF.55.01.03.00.01.F3.00.00.00.00.06.38.00.00.03.11.00.07.00.0A.00.00.00.12.2E.33.3C.00.00.00.00.00.00.00.4E
  // FF.55.01.03.00.01.FB.00.00.00.00.3C.C7.00.00.55.4E.00.07.00.07.00.00.00.47.2F.24.3C.00.00.00.00.00.00.00.CE
  // FF.55.01.03.00.01.CD.00.00.7F.00.3C.C8.00.00.55.4E.00.09.00.0A.00.00.00.47.30.0D.3C.00.00.00.00.00.00.00.8F
  // FF.55.01.03.00.01.FB.00.00.01.00.6C.3F.00.00.6C.44.00.07.00.06.00.1A.00.47.1C.1A.3C.00.00.00.00.00.00.00.78
  //
  // 0xFF 0x55:            Magic header
  // 0x01:                 Message type           0x01: Report (32 byte), 0x02: Reply (4 byte), 0x11: Command (6 byte)
  // 0x03:                 Device type            0x01: AC meter, 0x02: DC meter, 0x03: USB meter
  // 0x00 0x01 0xF3:       Voltage                499 * 0.01 = 4.99 V
  float voltage = dl24_get_24bit(4) * 0.01f;
  this->publish_state_(this->voltage_sensor_, voltage);

  // 0x00 0x00 0x00:       Current                0 * 0.01 = 0.00 A
  float current = dl24_get_24bit(7) * 0.01f;
  this->publish_state_(this->current_sensor_, current);
  this->publish_state_(this->power_sensor_, voltage * current);

  // 0x00 0x06 0x38:       Capacity in Ah         1592 * 0.001 = 1.592 Ah
  this->publish_state_(this->capacity_sensor_, dl24_get_24bit(10) * 0.001f);

  // 0x00 0x00 0x03 0x11:  Energy in Wh           785 * 0.01 = 7.85 Wh
  this->publish_state_(this->energy_sensor_, dl24_get_32bit(13) * 0.01f);

  // 0x00 0x07:            USB D-                 7 * 0.01 = 0,07 V
  this->publish_state_(this->usb_data_minus_sensor_, (float) dl24_get_16bit(17) * 0.01f);

  // 0x00 0x0A:            USB D+                 10 * 0.01 = 0,10 V
  this->publish_state_(this->usb_data_plus_sensor_, (float) dl24_get_16bit(19) * 0.01f);

  // 0x00 0x00 0x00:       Temperature
  this->publish_state_(this->temperature_sensor_, (float) dl24_get_24bit(21));

  // 0x12 0x2E:            Hour                   4654 h
  // 0x33:                 Minute                 51 min
  // 0x3C:                 Second                 60 sec
  uint32_t runtime = (dl24_get_16bit(23) * 3600) + (data[25] * 60) + data[26];
  this->publish_state_(this->runtime_sensor_, (float) runtime);
  if (this->runtime_formatted_text_sensor_ != nullptr) {
    this->publish_state_(this->runtime_formatted_text_sensor_, format_runtime_(runtime));
  }

  if (previous_value_ != 61)
    this->publish_state_(this->running_sensor_, (float) (previous_value_ != data[26]));
  previous_value_ = data[26];

  // 0x00:                 Backlight
  this->publish_state_(this->dim_backlight_sensor_, (float) data[27]);

  // 0x00 0x00 0x00 0x00 0x00 0x00: Unknown
  // 0x4E:                 Checksum
}

void AtorchDL24::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void AtorchDL24::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

}  // namespace atorch_dl24
}  // namespace esphome

#endif
