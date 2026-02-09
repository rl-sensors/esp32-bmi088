# ESP32 BMI088 Driver

A simple I2C driver for BMI088 IMU by Bosh that's meant to be suitable for high-vibrational applications such as drones.

Heavily (for now) based on https://github.com/bolderflight/bmi088-arduino/ but only include the I2C portion and uses C instead of C++.

#### Tested with ESP32S3.


### Config
Set I2C parameters (port, pins, data frequency, etc.) using `idf.py menuconfig`.

### Building

    idf.py build

### Deploy
Changed the `-p /dev/ttyACM0` to where your ESP32 is connected.

    idf.py flash monitor -p /dev/ttyACM0
    
### How to Use
The intended use is by receiving queue messages. See [main.c](main/main.c) for reference.

0) Initialize I2C master
1) reference the queue: `extern QueueHandle_t bmi_queue;`
2) receive the data: `if (xQueueReceive(bmi_queue, &bmi_data, portMAX_DELAY) == pdTRUE) {...}` 

Data is in the `BmiData` struct format that includes 2 `Axis3f` unions - one for accelerometer and one for gyro.