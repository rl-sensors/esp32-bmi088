//
// Created by bohm on 2/8/26.
//

#ifndef ESP32_BMI088_BMI088_H
#define ESP32_BMI088_BMI088_H

#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_MASTER_TIMEOUT_MS       1000

#define BMI088_ACC_SENSOR_ADDR              0x19 // 0x18 (SDO1 grounded) or 0x19 (SDO1 pulled high)

#define ACC_CHIP_ID 0x1E
#define ACC_RESET_CMD 0xB6
#define ACC_ENABLE_CMD 0x04
#define ACC_DISABLE_CMD 0x00
#define ACC_SUSPEND_MODE_CMD 0x03
#define ACC_ACTIVE_MODE_CMD 0x00
#define ACC_INT_INPUT 0x11
#define ACC_INT_OUTPUT 0x08
#define ACC_INT_OPENDRAIN 0x04
#define ACC_INT_PUSHPULL 0x00
#define ACC_INT_LVL_HIGH 0x02
#define ACC_INT_LVL_LOW 0x00
#define ACC_POS_SELF_TEST 0x0D
#define ACC_NEG_SELF_TEST 0x09
#define ACC_DIS_SELF_TEST 0x00
// registers
#define ACC_CHIP_ID_ADDR 0x00
#define ACC_CHIP_ID_MASK 0xFF
#define ACC_CHIP_ID_POS 0
#define ACC_FATAL_ERR_ADDR 0x02
#define ACC_FATAL_ERR_MASK 0x01
#define ACC_FATAL_ERR_POS 0
#define ACC_ERR_CODE_ADDR 0x02
#define ACC_ERR_CODE_MASK 0x1C
#define ACC_ERR_CODE_POS 2
#define ACC_DRDY_ADDR 0x03
#define ACC_DRDY_MASK 0x80
#define ACC_DRDY_POS 7
#define ACC_ODR_ADDR 0x40
#define ACC_ODR_MASK 0xFF
#define ACC_ODR_POS 0
#define ACC_RANGE_ADDR 0x41
#define ACC_RANGE_MASK 0x03
#define ACC_RANGE_POS 0
#define ACC_INT1_IO_CTRL_ADDR 0x53
#define ACC_INT1_IO_CTRL_MASK 0x1F
#define ACC_INT1_IO_CTRL_POS 0
#define ACC_INT2_IO_CTRL_ADDR 0x54
#define ACC_INT2_IO_CTRL_MASK 0x1F
#define ACC_INT2_IO_CTRL_POS 0
#define ACC_INT1_DRDY_ADDR 0x58
#define ACC_INT1_DRDY_MASK 0x04
#define ACC_INT1_DRDY_POS 2
#define ACC_INT2_DRDY_ADDR 0x58
#define ACC_INT2_DRDY_MASK 0x40
#define ACC_INT2_DRDY_POS 6
#define ACC_SELF_TEST_ADDR 0x6D
#define ACC_SELF_TEST_MASK 0xFF
#define ACC_SELF_TEST_POS 0
#define ACC_PWR_CONF_ADDR 0x7C
#define ACC_PWR_CONF_MASK 0xFF
#define ACC_PWR_CONF_POS 0
#define ACC_PWR_CNTRL_ADDR 0x7D
#define ACC_PWR_CNTRL_MASK 0xFF
#define ACC_PWR_CNTRL_POS 0
#define ACC_SOFT_RESET_ADDR 0x7E
#define ACC_SOFT_RESET_MASK 0xFF
#define ACC_SOFT_RESET_POS 0
#define ACC_ACCEL_DATA_ADDR 0x12
#define ACC_TEMP_DATA_ADDR 0x22

esp_err_t register_read(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, size_t len);

esp_err_t register_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data);

bool isCorrectAccId();

#endif //ESP32_BMI088_BMI088_H
