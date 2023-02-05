#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// -----command lookup table-----
typedef enum {
    CMD_NULL,     /*empty command*/
    CMD_START,    /*start command*/
    CMD_STOP,     /*stop command*/
    CMD_NEW_FILE, /*new file command*/
    CMD_RESTART   /*restart command*/
} commands_t;

// -----board ID secret values-----
typedef enum {
    BOARD_01 = 0x7CE, /*1998*/
    BOARD_02 = 0x7CF, /*1998 + 1*/
    BOARD_03 = 0x7D0  /*1998 + 2*/
} boardID_t;

typedef enum {
    RPM,
    SPEEDOMETER,
    FUEL_LEVEL,
    FUEL_EMERGENCY,
    BATTERY,
    AMBIENT_TEMP,
    ROLLOVER,
    TILT_X,
    TILT_Y,
    TILT_Z
} sensors_t;

typedef struct {
    boardID_t id;
    float rpm;
    float speed;
    float fuel_level;
    float fuel_emer;
    float battery;
    float temp;
    float rollover;
    float tilt_x;
    float tilt_y;
    float tilt_z;
} system_t;

// // -----debug data type definition-----
// typedef struct {
//     boardID_t id;
//     char msg[ESPNOW_BUFFER_SIZE];
// } debug_t;

// -----espnow command data type
typedef struct {
    boardID_t id;
    commands_t command;
} cmd_t;

// -----sensor data type definition-----
typedef struct {
    boardID_t id;
    sensors_t type;
    float value;  // sensor data must be float due to a bug in writing to the sd card
} sensor_t;

// -----system run time data-----
system_t system_global = {
    .rpm = 0.0,
    .speed = 0.0,
    .fuel_level = 0.0,
    .fuel_emer = 0.0,
    .battery = 0.0,
    .temp = 0.0,
    .rollover = 0.0,
    .tilt_x = 0.0,
    .tilt_y = 0.0,
    .tilt_z = 0.0
};

// -----FreeRTOS objects-----
TaskHandle_t th_alive;            // LED alive task handler
TaskHandle_t th_rollover;
TaskHandle_t th_battery;          // sensor 3 task handler
TaskHandle_t th_display_control;  // display control task handler
TaskHandle_t th_SD;               // 
TaskHandle_t th_send_pack;        // 
SemaphoreHandle_t sh_SD;          //
SemaphoreHandle_t sh_global_vars; //
SemaphoreHandle_t sh_i2c;         //
QueueHandle_t qh_rpm;             //
QueueHandle_t qh_speed;           //
QueueHandle_t qh_fuel_level;      //
QueueHandle_t qh_fuel_emer;       //
QueueHandle_t qh_battery;         //
QueueHandle_t qh_temp;            //
QueueHandle_t qh_rollover;        //
QueueHandle_t qh_tilt_x;          //
QueueHandle_t qh_tilt_y;          //
QueueHandle_t qh_tilt_z;          //

#endif // DEFINITIONS_H