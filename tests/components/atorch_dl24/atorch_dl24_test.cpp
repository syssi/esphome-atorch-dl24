#include <gtest/gtest.h>
#include "common.h"

namespace esphome::atorch_dl24::testing {

// ── DC meter frame ────────────────────────────────────────────────────────────

TEST(AtorchDL24DcMeterTest, Voltage) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_NEAR(voltage.state, 3.2f, 0.001f);
}

TEST(AtorchDL24DcMeterTest, Current) {
  TestableAtorchDL24 atorch;
  sensor::Sensor current;
  atorch.set_current_sensor(&current);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_NEAR(current.state, 19.968f, 0.001f);
}

TEST(AtorchDL24DcMeterTest, Power) {
  TestableAtorchDL24 atorch;
  sensor::Sensor power;
  atorch.set_power_sensor(&power);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_NEAR(power.state, 3.2f * 19.968f, 0.01f);
}

TEST(AtorchDL24DcMeterTest, Capacity) {
  TestableAtorchDL24 atorch;
  sensor::Sensor capacity;
  atorch.set_capacity_sensor(&capacity);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_NEAR(capacity.state, 51.14f, 0.001f);
}

TEST(AtorchDL24DcMeterTest, Energy) {
  TestableAtorchDL24 atorch;
  sensor::Sensor energy;
  atorch.set_energy_sensor(&energy);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_NEAR(energy.state, 170.0f, 0.01f);
}

TEST(AtorchDL24DcMeterTest, Temperature) {
  TestableAtorchDL24 atorch;
  sensor::Sensor temperature;
  atorch.set_temperature_sensor(&temperature);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_FLOAT_EQ(temperature.state, 37.0f);
}

TEST(AtorchDL24DcMeterTest, Runtime) {
  TestableAtorchDL24 atorch;
  sensor::Sensor runtime;
  atorch.set_runtime_sensor(&runtime);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  // 2h 33m 26s = 2*3600 + 33*60 + 26 = 9206 s
  EXPECT_FLOAT_EQ(runtime.state, 9206.0f);
}

TEST(AtorchDL24DcMeterTest, RuntimeFormatted) {
  TestableAtorchDL24 atorch;
  text_sensor::TextSensor runtime_formatted;
  atorch.set_runtime_formatted_text_sensor(&runtime_formatted);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_EQ(runtime_formatted.state, "2h 33m 26s");
}

TEST(AtorchDL24DcMeterTest, DimBacklight) {
  TestableAtorchDL24 atorch;
  sensor::Sensor dim_backlight;
  atorch.set_dim_backlight_sensor(&dim_backlight);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_FLOAT_EQ(dim_backlight.state, 60.0f);
}

TEST(AtorchDL24DcMeterTest, NullSensorsDoNotCrash) {
  TestableAtorchDL24 atorch;
  atorch.decode_ac_and_dc_(DC_FRAME_1);
}

TEST(AtorchDL24DcMeterTest, DispatchedViaDecode) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  atorch.decode(DC_FRAME_1);

  EXPECT_NEAR(voltage.state, 3.2f, 0.001f);
}

// ── AC meter frame ────────────────────────────────────────────────────────────

TEST(AtorchDL24AcMeterTest, Voltage) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_NEAR(voltage.state, 230.8f, 0.001f);
}

TEST(AtorchDL24AcMeterTest, Current) {
  TestableAtorchDL24 atorch;
  sensor::Sensor current;
  atorch.set_current_sensor(&current);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_NEAR(current.state, 0.014f, 0.0001f);
}

TEST(AtorchDL24AcMeterTest, Capacity) {
  TestableAtorchDL24 atorch;
  sensor::Sensor capacity;
  atorch.set_capacity_sensor(&capacity);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_NEAR(capacity.state, 0.04f, 0.001f);
}

TEST(AtorchDL24AcMeterTest, PricePerKwh) {
  TestableAtorchDL24 atorch;
  sensor::Sensor price;
  atorch.set_price_per_kwh_sensor(&price);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_NEAR(price.state, 1.0f, 0.001f);
}

TEST(AtorchDL24AcMeterTest, Frequency) {
  TestableAtorchDL24 atorch;
  sensor::Sensor frequency;
  atorch.set_frequency_sensor(&frequency);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_NEAR(frequency.state, 50.0f, 0.001f);
}

TEST(AtorchDL24AcMeterTest, PowerFactor) {
  TestableAtorchDL24 atorch;
  sensor::Sensor power_factor;
  atorch.set_power_factor_sensor(&power_factor);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_NEAR(power_factor.state, 0.133f, 0.001f);
}

TEST(AtorchDL24AcMeterTest, Temperature) {
  TestableAtorchDL24 atorch;
  sensor::Sensor temperature;
  atorch.set_temperature_sensor(&temperature);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_FLOAT_EQ(temperature.state, 47.0f);
}

TEST(AtorchDL24AcMeterTest, Runtime) {
  TestableAtorchDL24 atorch;
  sensor::Sensor runtime;
  atorch.set_runtime_sensor(&runtime);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  // 0h 10m 9s = 10*60 + 9 = 609 s
  EXPECT_FLOAT_EQ(runtime.state, 609.0f);
}

TEST(AtorchDL24AcMeterTest, RuntimeFormatted) {
  TestableAtorchDL24 atorch;
  text_sensor::TextSensor runtime_formatted;
  atorch.set_runtime_formatted_text_sensor(&runtime_formatted);

  atorch.decode_ac_and_dc_(AC_FRAME_1);

  EXPECT_EQ(runtime_formatted.state, "10m 9s");
}

TEST(AtorchDL24AcMeterTest, NullSensorsDoNotCrash) {
  TestableAtorchDL24 atorch;
  atorch.decode_ac_and_dc_(AC_FRAME_1);
}

TEST(AtorchDL24AcMeterTest, DispatchedViaDecode) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  atorch.decode(AC_FRAME_1);

  EXPECT_NEAR(voltage.state, 230.8f, 0.001f);
}

// ── USB meter frame ───────────────────────────────────────────────────────────

TEST(AtorchDL24UsbMeterTest, Voltage) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_NEAR(voltage.state, 4.99f, 0.001f);
}

TEST(AtorchDL24UsbMeterTest, Current) {
  TestableAtorchDL24 atorch;
  sensor::Sensor current;
  atorch.set_current_sensor(&current);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_FLOAT_EQ(current.state, 0.0f);
}

TEST(AtorchDL24UsbMeterTest, Capacity) {
  TestableAtorchDL24 atorch;
  sensor::Sensor capacity;
  atorch.set_capacity_sensor(&capacity);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_NEAR(capacity.state, 1.592f, 0.001f);
}

TEST(AtorchDL24UsbMeterTest, Energy) {
  TestableAtorchDL24 atorch;
  sensor::Sensor energy;
  atorch.set_energy_sensor(&energy);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_NEAR(energy.state, 7.85f, 0.001f);
}

TEST(AtorchDL24UsbMeterTest, UsbDataMinus) {
  TestableAtorchDL24 atorch;
  sensor::Sensor usb_data_minus;
  atorch.set_usb_data_minus_sensor(&usb_data_minus);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_NEAR(usb_data_minus.state, 0.07f, 0.001f);
}

TEST(AtorchDL24UsbMeterTest, UsbDataPlus) {
  TestableAtorchDL24 atorch;
  sensor::Sensor usb_data_plus;
  atorch.set_usb_data_plus_sensor(&usb_data_plus);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_NEAR(usb_data_plus.state, 0.10f, 0.001f);
}

TEST(AtorchDL24UsbMeterTest, Runtime) {
  TestableAtorchDL24 atorch;
  sensor::Sensor runtime;
  atorch.set_runtime_sensor(&runtime);

  atorch.decode_usb_(USB_FRAME_1);

  // 18h 46m 51s = 18*3600 + 46*60 + 51 = 67611 s
  EXPECT_FLOAT_EQ(runtime.state, 67611.0f);
}

TEST(AtorchDL24UsbMeterTest, RuntimeFormatted) {
  TestableAtorchDL24 atorch;
  text_sensor::TextSensor runtime_formatted;
  atorch.set_runtime_formatted_text_sensor(&runtime_formatted);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_EQ(runtime_formatted.state, "18h 46m 51s");
}

TEST(AtorchDL24UsbMeterTest, DimBacklight) {
  TestableAtorchDL24 atorch;
  sensor::Sensor dim_backlight;
  atorch.set_dim_backlight_sensor(&dim_backlight);

  atorch.decode_usb_(USB_FRAME_1);

  EXPECT_FLOAT_EQ(dim_backlight.state, 60.0f);
}

TEST(AtorchDL24UsbMeterTest, NullSensorsDoNotCrash) {
  TestableAtorchDL24 atorch;
  atorch.decode_usb_(USB_FRAME_1);
}

TEST(AtorchDL24UsbMeterTest, DispatchedViaDecode) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  atorch.decode(USB_FRAME_1);

  EXPECT_NEAR(voltage.state, 4.99f, 0.001f);
}

// ── Device type detection ─────────────────────────────────────────────────────

TEST(AtorchDL24DeviceTypeTest, DetectedFromDcFrame) {
  TestableAtorchDL24 atorch;

  atorch.decode(DC_FRAME_1);

  EXPECT_EQ(atorch.get_device_type(), 0x02);
}

TEST(AtorchDL24DeviceTypeTest, DetectedFromAcFrame) {
  TestableAtorchDL24 atorch;

  atorch.decode(AC_FRAME_1);

  EXPECT_EQ(atorch.get_device_type(), 0x01);
}

TEST(AtorchDL24DeviceTypeTest, DetectedFromUsbFrame) {
  TestableAtorchDL24 atorch;

  atorch.decode(USB_FRAME_1);

  EXPECT_EQ(atorch.get_device_type(), 0x03);
}

TEST(AtorchDL24DeviceTypeTest, LockedAfterFirstFrame) {
  TestableAtorchDL24 atorch;

  atorch.decode(DC_FRAME_1);
  atorch.decode(AC_FRAME_1);  // second frame with different type

  EXPECT_EQ(atorch.get_device_type(), 0x02);  // locked to first detected type
}

// ── Running state detection ───────────────────────────────────────────────────

TEST(AtorchDL24RunningTest, NoPublishOnFirstDecode) {
  TestableAtorchDL24 atorch;
  binary_sensor::BinarySensor running;
  atorch.set_running_binary_sensor(&running);

  atorch.decode_ac_and_dc_(DC_FRAME_1);

  EXPECT_FALSE(running.state);  // state not updated on first decode
}

TEST(AtorchDL24RunningTest, RunningWhenSecondsChange) {
  TestableAtorchDL24 atorch;
  binary_sensor::BinarySensor running;
  atorch.set_running_binary_sensor(&running);

  atorch.decode_ac_and_dc_(DC_FRAME_1);  // second=26, sets previous_value
  atorch.decode_ac_and_dc_(DC_FRAME_2);  // second=27, detects change

  EXPECT_TRUE(running.state);
}

TEST(AtorchDL24RunningTest, NotRunningWhenSecondsStatic) {
  TestableAtorchDL24 atorch;
  binary_sensor::BinarySensor running;
  atorch.set_running_binary_sensor(&running);

  atorch.decode_ac_and_dc_(DC_FRAME_1);  // sets previous_value=26
  atorch.decode_ac_and_dc_(DC_FRAME_1);  // same second=26, not running

  EXPECT_FALSE(running.state);
}

// ── Assemble (chunked BLE reassembly) ─────────────────────────────────────────

TEST(AtorchDL24AssembleTest, CompleteFrameInOneChunk) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  atorch.assemble(DC_FRAME_1.data(), DC_FRAME_1.size());

  EXPECT_NEAR(voltage.state, 3.2f, 0.001f);
}

TEST(AtorchDL24AssembleTest, CompleteFrameIn5Chunks) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  // DC_FRAME_1 split across 5 chunks (from esp32-dc-meter-example-faker-5chunks.yaml)
  const uint8_t chunk1[8] = {0xFF, 0x55, 0x01, 0x02, 0x00, 0x00, 0x20, 0x00};
  const uint8_t chunk2[7] = {0x4E, 0x20, 0x00, 0x13, 0xFA, 0x00, 0x00};
  const uint8_t chunk3[8] = {0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  const uint8_t chunk4[8] = {0x00, 0x00, 0x00, 0x25, 0x00, 0x02, 0x21, 0x1A};
  const uint8_t chunk5[5] = {0x3C, 0x00, 0x00, 0x00, 0x09};

  atorch.assemble(chunk1, sizeof(chunk1));
  atorch.assemble(chunk2, sizeof(chunk2));
  atorch.assemble(chunk3, sizeof(chunk3));
  atorch.assemble(chunk4, sizeof(chunk4));
  atorch.assemble(chunk5, sizeof(chunk5));

  EXPECT_NEAR(voltage.state, 3.2f, 0.001f);
}

TEST(AtorchDL24AssembleTest, ReplyFrameIn3Chunks) {
  TestableAtorchDL24 atorch;

  // REPLY_FRAME_SUCCESS split across 3 chunks (from esp32-dc-meter-example-faker-5chunks.yaml)
  const uint8_t reply_chunk1[4] = {0xFF, 0x55, 0x02, 0x01};
  const uint8_t reply_chunk2[3] = {0x01, 0x00, 0x00};
  const uint8_t reply_chunk3[1] = {0x40};

  atorch.assemble(reply_chunk1, sizeof(reply_chunk1));
  atorch.assemble(reply_chunk2, sizeof(reply_chunk2));
  atorch.assemble(reply_chunk3, sizeof(reply_chunk3));
}

TEST(AtorchDL24AssembleTest, InterruptedFrameClearedByNewFrame) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);

  // Send incomplete frame (from esp32-dc-meter-example-faker-5chunks.yaml test 3)
  const uint8_t incomplete[4] = {0xFF, 0x55, 0x01, 0x02};
  atorch.assemble(incomplete, sizeof(incomplete));

  // New frame start clears the buffer and processes correctly
  atorch.assemble(DC_FRAME_1.data(), DC_FRAME_1.size());

  EXPECT_NEAR(voltage.state, 3.2f, 0.001f);
}

// ── Reply frame handling ──────────────────────────────────────────────────────

TEST(AtorchDL24ReplyTest, DoesNotUpdateSensors) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  voltage.state = -1.0f;
  atorch.set_voltage_sensor(&voltage);

  atorch.decode(REPLY_FRAME_SUCCESS);

  EXPECT_FLOAT_EQ(voltage.state, -1.0f);
}

TEST(AtorchDL24ReplyTest, InvalidLengthDoesNotUpdateSensors) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  voltage.state = -1.0f;
  atorch.set_voltage_sensor(&voltage);

  const std::vector<uint8_t> short_frame = {0xFF, 0x55, 0x01, 0x02};
  atorch.decode(short_frame);

  EXPECT_FLOAT_EQ(voltage.state, -1.0f);
}

// ── CRC validation ────────────────────────────────────────────────────────────

TEST(AtorchDL24CrcTest, ValidCrcPassesWhenEnabled) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  atorch.set_voltage_sensor(&voltage);
  atorch.set_check_crc(true);

  // DC_FRAME_1 has valid CRC (0x09)
  atorch.assemble(DC_FRAME_1.data(), DC_FRAME_1.size());

  EXPECT_NEAR(voltage.state, 3.2f, 0.001f);
}

TEST(AtorchDL24CrcTest, InvalidCrcRejectedWhenEnabled) {
  TestableAtorchDL24 atorch;
  sensor::Sensor voltage;
  voltage.state = -1.0f;
  atorch.set_voltage_sensor(&voltage);
  atorch.set_check_crc(true);

  // Corrupt the last byte (CRC)
  std::vector<uint8_t> corrupted = DC_FRAME_1;
  corrupted[35] ^= 0xFF;
  atorch.assemble(corrupted.data(), corrupted.size());

  EXPECT_FLOAT_EQ(voltage.state, -1.0f);
}

}  // namespace esphome::atorch_dl24::testing
