//
// Created by bohm on 2/8/26.
//

#ifndef ESP32_BMI088_BMI088_H
#define ESP32_BMI088_BMI088_H

#include "esp_log.h"
#include "driver/i2c.h"
#include "math.h"

#define I2C_MASTER_TIMEOUT_MS       1000

#define BMI088_ACC_SENSOR_ADDR      CONFIG_I2C_BMI088_ACC_ADDR // 0x18 (SDO1 grounded) or 0x19 (SDO1 pulled high)

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

#define G 9.807f
#define D2R (M_PI / 180.0f)


#define BMI088_GYRO_SENSOR_ADDR      CONFIG_I2C_BMI088_GYRO_ADDR //0x69 (default, SDO2 pulled high), or 0x68 (SDO2  grounded)



#define GYRO_CHIP_ID 0x0F
#define GYRO_RESET_CMD 0xB6
#define GYRO_ENABLE_DRDY_INT 0x80
#define GYRO_DIS_DRDY_INT 0x00
#define GYRO_INT_OPENDRAIN 0x02
#define GYRO_INT_PUSHPULL 0x00
#define GYRO_INT_LVL_HIGH 0x01
#define GYRO_INT_LVL_LOW 0x00
// registers
#define GYRO_CHIP_ID_ADDR 0x00
#define GYRO_CHIP_ID_MASK 0xFF
#define GYRO_CHIP_ID_POS 0
#define GYRO_DRDY_ADDR 0x0A
#define GYRO_DRDY_MASK 0x80
#define GYRO_DRDY_POS 7
#define GYRO_RANGE_ADDR 0x0F
#define GYRO_RANGE_MASK 0xFF
#define GYRO_RANGE_POS 0
#define GYRO_ODR_ADDR 0x10
#define GYRO_ODR_MASK 0xFF
#define GYRO_ODR_POS 0
#define GYRO_SOFT_RESET_ADDR 0x14
#define GYRO_SOFT_RESET_MASK 0xFF
#define GYRO_SOFT_RESET_POS 0
#define GYRO_INT_CNTRL_ADDR 0x15
#define GYRO_INT_CNTRL_MASK 0xFF
#define GYRO_INT_CNTRL_POS 0
#define GYRO_INT3_IO_CTRL_ADDR 0x16
#define GYRO_INT3_IO_CTRL_MASK 0x03
#define GYRO_INT3_IO_CTRL_POS 0
#define GYRO_INT4_IO_CTRL_ADDR 0x16
#define GYRO_INT4_IO_CTRL_MASK 0x0C
#define GYRO_INT4_IO_CTRL_POS 2
#define GYRO_INT3_DRDY_ADDR 0x18
#define GYRO_INT3_DRDY_MASK 0x01
#define GYRO_INT3_DRDY_POS 0
#define GYRO_INT4_DRDY_ADDR 0x18
#define GYRO_INT4_DRDY_MASK 0x80
#define GYRO_INT4_DRDY_POS 7
#define GYRO_DATA_ADDR 0x02

enum AccRange {
    RANGE_3G = 0x00,
    RANGE_6G = 0x01,
    RANGE_12G = 0x02,
    RANGE_24G = 0x03
};

enum GyroRange {
    GYRO_RANGE_2000DPS,
    GYRO_RANGE_1000DPS,
    GYRO_RANGE_500DPS,
    GYRO_RANGE_250DPS,
    GYRO_RANGE_125DPS
};

enum AccOdr {
    ODR_1600HZ_BW_280HZ,
    ODR_1600HZ_BW_234HZ,
    ODR_1600HZ_BW_145HZ,
    ODR_800HZ_BW_230HZ,
    ODR_800HZ_BW_140HZ,
    ODR_800HZ_BW_80HZ,
    ODR_400HZ_BW_145HZ,
    ODR_400HZ_BW_75HZ,
    ODR_400HZ_BW_40HZ,
    ODR_200HZ_BW_80HZ,
    ODR_200HZ_BW_38HZ,
    ODR_200HZ_BW_20HZ,
    ODR_100HZ_BW_40HZ,
    ODR_100HZ_BW_19HZ,
    ODR_100HZ_BW_10HZ,
    ODR_50HZ_BW_20HZ,
    ODR_50HZ_BW_9HZ,
    ODR_50HZ_BW_5HZ,
    ODR_25HZ_BW_10HZ,
    ODR_25HZ_BW_5HZ,
    ODR_25HZ_BW_3HZ,
    ODR_12_5HZ_BW_5HZ,
    ODR_12_5HZ_BW_2HZ,
    ODR_12_5HZ_BW_1HZ
};

enum GyroOdr {
    ODR_2000HZ_BW_532HZ = 0x80,
    ODR_2000HZ_BW_230HZ = 0x81,
    ODR_1000HZ_BW_116HZ = 0x82,
    ODR_400HZ_BW_47HZ = 0x83,
    ODR_200HZ_BW_23HZ = 0x84,
    ODR_100HZ_BW_12HZ = 0x85,
    ODR_200HZ_BW_64HZ = 0x86,
    ODR_100HZ_BW_32HZ = 0x87
};

typedef union {
    struct {
        float x;
        float y;
        float z;
    };

    float axis[3];
} Axis3f;

typedef struct {
    Axis3f acc;
    Axis3f gyro;
} BmiData;

/* Macros to get and set register fields */
#define GET_FIELD(regname,value) ((value & regname##_MASK) >> regname##_POS)
#define	SET_FIELD(regval,regname,value) ((regval & ~regname##_MASK) | ((value << regname##_POS) & regname##_MASK))

void bmi088_task(void *pvParams);

esp_err_t register_read(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, size_t len);

esp_err_t register_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data);

int8_t acc_begin();

bool is_correct_acc_id();

void acc_soft_reset();

bool acc_set_power(bool enable);

bool acc_set_mode(bool active);

bool acc_self_test();

bool acc_set_range(enum AccRange range);

bool acc_set_odr(enum AccOdr odr);

Axis3f acc_read_sensor();

bool acc_is_config_err();

bool acc_is_fatal_err();


bool is_correct_gyro_id();

void gyro_soft_reset();

bool gyro_set_odr(enum GyroOdr odr);

bool gyro_set_range(enum GyroRange range);

Axis3f gyro_read_sensor();

int gyro_begin();

#endif //ESP32_BMI088_BMI088_H
