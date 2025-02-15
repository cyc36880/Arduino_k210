#pragma once

#include "Arduino.h"
#include "Wire.h"

class AiCamera 
{
public:
    AiCamera(uint8_t addr=0x24):AI_CAMERA_ADDR(addr) {};
    virtual ~AiCamera(){}

public:
    enum Register
    {
        AI_CAMERA_SYS=0,
        AI_CAMERA_COLOR,
        AI_CAMERA_PATCH,
        AI_CAMERA_TAG,
        AI_CAMERA_LINE,
        AI_CAMERA_20_CLASS,
        AI_CAMERA_QRCODE,
        AI_CAMERA_FACE_DE, // 人脸检测
        AI_CAMERA_FACE_RE, // 人脸识别
        AI_CAMERA_DEEP_LEARN,
        AI_CAMERA_CARD,

        AI_CAMERA_MAX,
    };
    enum Color
    {
        RED=1 ,   // 红色
        GREEN,  // 绿色
        BLUE,   // 蓝色
        YELLOW, // 黄色
        BLACK,  // 黑色
        WHITE,// 白色
    };

public:
    void Init(TwoWire *wire) { _wire = wire; }
    void set_sys_mode(uint8_t mode);
    uint8_t get_sys_mode(void);
    void get_color_rgb(uint8_t rgb[3]);
    void set_find_color(uint8_t color_id);
    void face_study(void);
    void deep_learn_study(void);
    std::string get_qrcode_content(void);
    uint8_t get_identify_num(uint8_t features, uint8_t total=0);
    uint8_t get_identify_id(uint8_t features, uint8_t index=0);
    int16_t get_identify_rotation(uint8_t features, uint8_t index=0);
    void get_identify_position(uint8_t features, int16_t position[4], uint8_t index=0);
    uint8_t get_identify_confidence(uint8_t features, uint8_t id);
protected:
    virtual uint8_t writeReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len);
    virtual uint8_t readReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len);

private:
    const uint8_t AI_CAMERA_ADDR;
    TwoWire *_wire=NULL;
};
