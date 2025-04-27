#pragma once

#include "Arduino.h"
#include "Wire.h"

#if defined(__AVR__) || defined(ARDUINO_ARCH_AVR)
  // AVR 平台（如 Arduino Nano）不支持 std::string
#else
  // 其他平台（如 ESP32、STM32）可能支持
  #include <string>
#endif

typedef enum 
{
    AI_CAMERA_SYS=0,
    AI_CAMERA_COLOR,
    AI_CAMERA_PATCH,
    AI_CAMERA_TAG,
    AI_CAMERA_LINE,
    AI_CAMERA_20_CLASS,
    AI_CAMERA_QRCODE,
    AI_CAMERA_FACE_ATTRIBUTE, // 人脸属性
    AI_CAMERA_FACE_RE,        // 人脸识别
    AI_CAMERA_DEEP_LEARN,
    AI_CAMERA_CARD,
    AI_CAMERA_WIFI_SERVER,           // 无线图传
    // AI_CAMERA_USER_IMAGE_DETECTION,  // 图像检测
    // AI_CAMERA_UR_IMAGE_CLASS,        // 图像分类

    AI_CAMERA_MAX,
} AI_CAMERA_REGISTER_t ;

enum AI_CAMERA_Color
{
    AI_CAMERA_COLOR_RED=1 , // 红色
    AI_CAMERA_COLOR_GREEN,  // 绿色
    AI_CAMERA_COLOR_BLUE,   // 蓝色
    AI_CAMERA_COLOR_YELLOW, // 黄色
    AI_CAMERA_COLOR_BLACK,  // 黑色
    AI_CAMERA_COLOR_WHITE,  // 白色
};


class AiCamera 
{
public:
    AiCamera(uint8_t addr=0x24);

public:
    void Init(TwoWire *wire) { _wire=wire;}
    void Init(int sda=-1, int scl=-1) 
    { 
        Wire.begin(sda, scl);
        _wire=&Wire;
    }
    void begin(TwoWire *wire) { Init(wire); }
    void begin(int sda=-1, int scl=-1)
    {
        Init(sda, scl);
    }
    void set_sys_mode(AI_CAMERA_REGISTER_t mode);
    uint8_t get_sys_mode(void);
    void get_color_rgb(int rgb[3]);
    void get_color_rgb(int &r, int &g, int &b);
    void set_find_color(uint8_t color_id);
    void face_study(void);
    void deep_learn_study(void);
    String get_qrcode_content(void);
    uint8_t get_identify_num(AI_CAMERA_REGISTER_t features, uint8_t total=0);
    uint8_t get_face_attributes(int &is_male, int &is_mouth_open, int &is_smail, int &is_glasses, uint8_t index=0);
    uint8_t get_identify_id(AI_CAMERA_REGISTER_t features, uint8_t index=0);
    int16_t get_identify_rotation(AI_CAMERA_REGISTER_t features, uint8_t index=0);
    void get_identify_position(AI_CAMERA_REGISTER_t features, int position[4], uint8_t index=0);
    void get_identify_position(AI_CAMERA_REGISTER_t features, int &x, int &y, int &w, int &h, uint8_t index=0);
    uint8_t get_identify_confidence(AI_CAMERA_REGISTER_t features, uint8_t id);

    uint8_t set_wifi_server_is_scan_qrcode(bool is_scan);
    uint8_t get_wifi_server_ssid_passward(String &ssid, String &password);
    uint8_t set_wifi_server_ssid_passward(const char *ssid, const char *password);
    String get_wifi_server_ip(void);

protected:
    virtual uint8_t writeReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len);
    virtual uint8_t readReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len);
    virtual uint8_t isOnline(uint8_t dev_addr);
    
private:
    const uint8_t DEV_ADDR;
    TwoWire *_wire=NULL;
};
