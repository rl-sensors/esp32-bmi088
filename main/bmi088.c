//
// Created by bohm on 2/8/26.
//
#include "bmi088.h"

extern char *TAG;

bool isCorrectAccId() {
    uint8_t readReg = 0;
    register_read(BMI088_ACC_SENSOR_ADDR, ACC_CHIP_ID_ADDR, &readReg, 1);
    ESP_LOGI(TAG, "Received chip id: %d", readReg);
    return readReg == ACC_CHIP_ID;
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