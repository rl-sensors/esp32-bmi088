//
// Created by bohm on 2/8/26.
//
#include "bmi088.h"

extern char *TAG;

static float accel_range_mss = 0.0f;
static float gyro_range_rads = 0.0f;
// transformation from sensor frame to right hand coordinate system
const int16_t tX[3] = {1, 0, 0};
const int16_t tY[3] = {0, -1, 0};
const int16_t tZ[3] = {0, 0, -1};

int8_t acc_begin() {
    if (!is_correct_acc_id()) {
        ESP_LOGE(TAG, "Cannot find BMI088.");
        return -1;
    }
    ESP_LOGI(TAG, "Found BMI088 ID");

    acc_soft_reset();

    if (!acc_set_power(true)) {
        ESP_LOGE(TAG, "Cannot set power on BMI088.");
        return -2;
    }
    ESP_LOGI(TAG, "BMI088 POWER ON");

    if (!acc_set_mode(false)) {
        ESP_LOGE(TAG, "Cannot set active mode BMI088.");
        return -3;
    }
    ESP_LOGI(TAG, "BMI088 ACTIVE");

    if (!acc_self_test()) {
        ESP_LOGE(TAG, "Cannot self test BMI088.");
        return -4;
    }
    ESP_LOGI(TAG, "Self test successful");

    acc_soft_reset();
    acc_set_power(true);
    acc_set_mode(true);

    vTaskDelay(pdMS_TO_TICKS(50));

    acc_set_range(RANGE_24G);
    acc_set_odr(ODR_1600HZ_BW_280HZ);

    if (acc_is_config_err()) {
        ESP_LOGE(TAG, "Config error!");
        return -9;
    }

    if (acc_is_fatal_err()) {
        ESP_LOGE(TAG, "Fatal error!");
        return -10;
    }

    return 1;
}


bool is_correct_acc_id() {
    uint8_t read_reg = 0;
    register_read(BMI088_ACC_SENSOR_ADDR, ACC_CHIP_ID_ADDR, &read_reg, 1);
    ESP_LOGI(TAG, "Received chip id: %d", read_reg);
    return read_reg == ACC_CHIP_ID;
}

void acc_soft_reset() {
    uint8_t reg = 0;
    reg = SET_FIELD(reg, ACC_SOFT_RESET, ACC_RESET_CMD);
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_SOFT_RESET_ADDR, reg);
    vTaskDelay(pdMS_TO_TICKS(50));
}

bool acc_set_power(bool enable) {
    uint8_t write_reg = 0;
    uint8_t read_reg = 0;

    uint8_t value = enable ? ACC_ENABLE_CMD : ACC_DISABLE_CMD;
    write_reg = SET_FIELD(write_reg, ACC_PWR_CNTRL, value);
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_PWR_CNTRL_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(5)); // 5 ms wait after power mode changes

    register_read(BMI088_ACC_SENSOR_ADDR, ACC_PWR_CNTRL_ADDR, &read_reg, 1);

    return read_reg == write_reg;
}

bool acc_set_mode(bool active) {
    uint8_t write_reg = 0;
    uint8_t read_reg = 0;

    uint8_t value = (active) ? ACC_ACTIVE_MODE_CMD : ACC_SUSPEND_MODE_CMD;
    write_reg = SET_FIELD(write_reg, ACC_PWR_CONF, value);
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_PWR_CONF_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(5)); // 5 ms wait after power mode changes

    register_read(BMI088_ACC_SENSOR_ADDR, ACC_PWR_CONF_ADDR, &read_reg, 1);

    return read_reg == write_reg;
}

bool acc_set_range(enum AccRange range) {
    uint8_t write_reg = 0;
    uint8_t read_reg = 0;
    register_read(BMI088_ACC_SENSOR_ADDR, ACC_RANGE_ADDR, &read_reg, 1);
    write_reg = SET_FIELD(read_reg, ACC_RANGE, range);
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_RANGE_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(1));

    register_read(BMI088_ACC_SENSOR_ADDR, ACC_RANGE_ADDR, &read_reg, 1);
    if (read_reg == write_reg) {
        switch (range) {
            case RANGE_3G: {
                accel_range_mss = 3.0f * G;
                break;
            }
            case RANGE_6G: {
                accel_range_mss = 6.0f * G;
                break;
            }
            case RANGE_12G: {
                accel_range_mss = 12.0f * G;
                break;
            }
            case RANGE_24G: {
                accel_range_mss = 24.0f * G;
                break;
            }
        }
        return true;
    }

    return false;
}

bool acc_set_odr(enum AccOdr odr) {
    uint8_t write_reg = 0;
    uint8_t read_reg = 0;
    uint8_t value = 0;

    switch (odr) {
        case ODR_1600HZ_BW_280HZ: {
            value = (0x0A << 4) | 0x0C;
            break;
        }
        case ODR_1600HZ_BW_234HZ: {
            value = (0x09 << 4) | 0x0C;
            break;
        }
        case ODR_1600HZ_BW_145HZ: {
            value = (0x08 << 4) | 0x0C;
            break;
        }
        case ODR_800HZ_BW_230HZ: {
            value = (0x0A << 4) | 0x0B;
            break;
        }
        case ODR_800HZ_BW_140HZ: {
            value = (0x09 << 4) | 0x0B;
            break;
        }
        case ODR_800HZ_BW_80HZ: {
            value = (0x08 << 4) | 0x0B;
            break;
        }
        case ODR_400HZ_BW_145HZ: {
            value = (0x0A << 4) | 0x0A;
            break;
        }
        case ODR_400HZ_BW_75HZ: {
            value = (0x09 << 4) | 0x0A;
            break;
        }
        case ODR_400HZ_BW_40HZ: {
            value = (0x08 << 4) | 0x0A;
            break;
        }
        case ODR_200HZ_BW_80HZ: {
            value = (0x0A << 4) | 0x09;
            break;
        }
        case ODR_200HZ_BW_38HZ: {
            value = (0x09 << 4) | 0x09;
            break;
        }
        case ODR_200HZ_BW_20HZ: {
            value = (0x08 << 4) | 0x09;
            break;
        }
        case ODR_100HZ_BW_40HZ: {
            value = (0x0A << 4) | 0x08;
            break;
        }
        case ODR_100HZ_BW_19HZ: {
            value = (0x09 << 4) | 0x08;
            break;
        }
        case ODR_100HZ_BW_10HZ: {
            value = (0x08 << 4) | 0x08;
            break;
        }
        case ODR_50HZ_BW_20HZ: {
            value = (0x0A << 4) | 0x07;
            break;
        }
        case ODR_50HZ_BW_9HZ: {
            value = (0x09 << 4) | 0x07;
            break;
        }
        case ODR_50HZ_BW_5HZ: {
            value = (0x08 << 4) | 0x07;
            break;
        }
        case ODR_25HZ_BW_10HZ: {
            value = (0x0A << 4) | 0x06;
            break;
        }
        case ODR_25HZ_BW_5HZ: {
            value = (0x09 << 4) | 0x06;
            break;
        }
        case ODR_25HZ_BW_3HZ: {
            value = (0x08 << 4) | 0x06;
            break;
        }
        case ODR_12_5HZ_BW_5HZ: {
            value = (0x0A << 4) | 0x05;
            break;
        }
        case ODR_12_5HZ_BW_2HZ: {
            value = (0x09 << 4) | 0x05;
            break;
        }
        case ODR_12_5HZ_BW_1HZ: {
            value = (0x08 << 4) | 0x05;
            break;
        }
        default: {
            value = (0x0A << 4) | 0x0C;
            break;
        }
    }

    write_reg = SET_FIELD(write_reg, ACC_ODR, value);
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_ODR_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(1));

    register_read(BMI088_ACC_SENSOR_ADDR, ACC_ODR_ADDR, &read_reg, 1);

    return read_reg == write_reg;
}

bool acc_self_test() {
    uint8_t write_reg = 0;
    float accel_pos_mg[3];
    float accel_neg_mg[3];
    Axis3f acc_mss;

    acc_set_range(RANGE_24G);
    acc_set_odr(ODR_1600HZ_BW_145HZ); // set 1.6 kHz ODR, 4x oversampling
    vTaskDelay(pdMS_TO_TICKS(3));

    write_reg = SET_FIELD(write_reg, ACC_SELF_TEST, ACC_POS_SELF_TEST); // enable self test, positive polarity
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_SELF_TEST_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(51));

    acc_mss = acc_read_sensor();
    for (uint8_t i = 0; i < 3; ++i) {
        accel_pos_mg[i] = acc_mss.axis[i] / G * 1000.0f;
    }

    write_reg = SET_FIELD(write_reg, ACC_SELF_TEST, ACC_NEG_SELF_TEST); // enable self test, negative polarity
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_SELF_TEST_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(51));

    acc_mss = acc_read_sensor();
    for (uint8_t i = 0; i < 3; ++i) {
        accel_neg_mg[i] = acc_mss.axis[i] / G * 1000.0f;
    }

    /* disable self test */
    write_reg = SET_FIELD(write_reg, ACC_SELF_TEST, ACC_DIS_SELF_TEST);
    register_write_byte(BMI088_ACC_SENSOR_ADDR, ACC_SELF_TEST_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(51));

    if ((fabsf(accel_pos_mg[0] - accel_neg_mg[0]) >= 1000) && (fabsf(accel_pos_mg[1] - accel_neg_mg[1]) >= 1000) && (
            fabsf(accel_pos_mg[2] - accel_neg_mg[2]) >= 500)) {
        return true;
    }

    return false;
}

Axis3f acc_read_sensor() {
    int16_t accel[3];
    uint8_t _buffer[9];
    Axis3f acc_mss;

    register_read(BMI088_ACC_SENSOR_ADDR, ACC_ACCEL_DATA_ADDR, _buffer, 9);
    accel[0] = (_buffer[1] << 8) | _buffer[0];
    accel[1] = (_buffer[3] << 8) | _buffer[2];
    accel[2] = (_buffer[5] << 8) | _buffer[4];

    acc_mss.axis[0] = (float) (accel[0] * tX[0] + accel[1] * tX[1] + accel[2] * tX[2]) / 32768.0f * accel_range_mss;
    acc_mss.axis[1] = (float) (accel[0] * tY[0] + accel[1] * tY[1] + accel[2] * tY[2]) / 32768.0f * accel_range_mss;
    acc_mss.axis[2] = (float) (accel[0] * tZ[0] + accel[1] * tZ[1] + accel[2] * tZ[2]) / 32768.0f * accel_range_mss;

    /*
    // time data - do we need this?
    int16_t temp_int11;
    current_time_counter = (_buffer[8] << 16) | (_buffer[7] << 8) | _buffer[6];
    time_counter = current_time_counter - prev_time_counter;
    prev_time_counter = current_time_counter;

    // temperature data - do we need this?
    register_read(BMI088_ACC_SENSOR_ADDR, ACC_TEMP_DATA_ADDR,2,_buffer);
    uint16_t temp_uint11 = (_buffer[0] * 8) + (_buffer[1] / 32);
    if (temp_uint11 > 1023) {
        temp_int11 = temp_uint11 - 2048;
    } else {
        temp_int11 = temp_uint11;
    }
    temp_c = (float) temp_int11 * 0.125f + 23.0f;
    */

    return acc_mss;
}

bool acc_is_config_err() {
    uint8_t read_reg = 0;
    register_read(BMI088_ACC_SENSOR_ADDR, ACC_ERR_CODE_ADDR, &read_reg, 1);
    return GET_FIELD(ACC_ERR_CODE, read_reg);
}

bool acc_is_fatal_err() {
    uint8_t read_reg = 0;
    register_read(BMI088_ACC_SENSOR_ADDR, ACC_FATAL_ERR_ADDR, &read_reg, 1);
    return GET_FIELD(ACC_FATAL_ERR, read_reg);
}


bool is_correct_gyro_id() {
    uint8_t read_reg = 0;
    register_read(BMI088_GYRO_SENSOR_ADDR, GYRO_CHIP_ID_ADDR, &read_reg, 1);
    ESP_LOGI(TAG, "Received gyro chip id: %d", read_reg);
    return read_reg == GYRO_CHIP_ID;
}

void gyro_soft_reset() {
    uint8_t reg = 0;
    reg = SET_FIELD(reg, GYRO_SOFT_RESET, GYRO_RESET_CMD);
    register_write_byte(GYRO_SOFT_RESET_ADDR, reg, 1);
    vTaskDelay(pdMS_TO_TICKS(50));
}

bool gyro_set_odr(enum GyroOdr odr) {
    uint8_t write_reg = 0;
    uint8_t read_reg = 0;

    write_reg = SET_FIELD(write_reg, GYRO_ODR, odr);
    register_write_byte(BMI088_GYRO_SENSOR_ADDR, GYRO_ODR_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(1));

    register_read(BMI088_GYRO_SENSOR_ADDR, GYRO_ODR_ADDR, &read_reg, 1);

    return read_reg == write_reg;
}

bool gyro_set_range(enum GyroRange range) {
    uint8_t write_reg = 0;
    uint8_t read_reg = 0;
    write_reg = SET_FIELD(write_reg, GYRO_RANGE, range);
    register_write_byte(BMI088_GYRO_SENSOR_ADDR, GYRO_RANGE_ADDR, write_reg);
    vTaskDelay(pdMS_TO_TICKS(1));

    register_read(BMI088_GYRO_SENSOR_ADDR, GYRO_RANGE_ADDR, &read_reg, 1);
    if (read_reg == write_reg) {
        switch (range) {
            case GYRO_RANGE_125DPS: {
                gyro_range_rads = 125.0f * D2R;
                break;
            }
            case GYRO_RANGE_250DPS: {
                gyro_range_rads = 250.0f * D2R;
                break;
            }
            case GYRO_RANGE_500DPS: {
                gyro_range_rads = 500.0f * D2R;
                break;
            }
            case GYRO_RANGE_1000DPS: {
                gyro_range_rads = 1000.0f * D2R;
                break;
            }
            case GYRO_RANGE_2000DPS: {
                gyro_range_rads = 2000.0f * D2R;
                break;
            }
        }
        return true;
    }

    return false;
}

Axis3f gyro_read_sensor() {
    uint8_t _buffer[6];
    int16_t gyro[3];
    Axis3f gyro_rads;
    register_read(BMI088_GYRO_SENSOR_ADDR, GYRO_DATA_ADDR, _buffer, 6);
    gyro[0] = (_buffer[1] << 8) | _buffer[0];
    gyro[1] = (_buffer[3] << 8) | _buffer[2];
    gyro[2] = (_buffer[5] << 8) | _buffer[4];
    gyro_rads.axis[0] = (float) (gyro[0] * tX[0] + gyro[1] * tX[1] + gyro[2] * tX[2]) / 32767.0f * gyro_range_rads;
    gyro_rads.axis[1] = (float) (gyro[0] * tY[0] + gyro[1] * tY[1] + gyro[2] * tY[2]) / 32767.0f * gyro_range_rads;
    gyro_rads.axis[2] = (float) (gyro[0] * tZ[0] + gyro[1] * tZ[1] + gyro[2] * tZ[2]) / 32767.0f * gyro_range_rads;

    return gyro_rads;
}

int gyro_begin() {
    if (!is_correct_gyro_id()) {
        ESP_LOGE(TAG, "Gyro ID not found");
        return -1;
    }

    gyro_soft_reset();

    if (!gyro_set_range(GYRO_RANGE_2000DPS)) {
        ESP_LOGE(TAG, "Gyro set range 2000DPS failed");
        return -2;
    }

    if (!gyro_set_odr(ODR_2000HZ_BW_532HZ)) {
        ESP_LOGE(TAG, "Gyro set odr 2000HZ_BW_532HZ failed");
        return -4;
    }

    return 1;
}


esp_err_t register_read(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, size_t len) {
    return i2c_master_write_read_device(CONFIG_I2C_MASTER_NUM, device_addr, &reg_addr, 1, data, len,
                                        pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));
}

esp_err_t register_write_byte(uint8_t device_addr, uint8_t reg_addr, uint8_t data) {
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(CONFIG_I2C_MASTER_NUM, device_addr, write_buf, sizeof(write_buf),
                                     pdMS_TO_TICKS(I2C_MASTER_TIMEOUT_MS));

    return ret;
}

