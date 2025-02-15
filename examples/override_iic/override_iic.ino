/**
 * 定义 m_ai_cmarea 类继承 AiCamera，重写 writeReg 和 readReg 函数覆盖 AiCamera 的 iic 操作
 *
 */

#include <Arduino.h>
#include "ai_camera.h"

#include "Wire.h"

static uint8_t writeReg(TwoWire *wire, uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (NULL == wire)
        return 1;
    Serial.println("writeReg");
    try
    {
        wire->beginTransmission(dev_addr);
        wire->write(reg);
        while (len--)
        {
            wire->write(*(data++));
        }
        wire->endTransmission(true);
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}

static uint8_t readReg(TwoWire *wire, uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (NULL == wire)
        return 1;
    try
    {
        wire->beginTransmission(dev_addr);
        wire->write(reg);
        wire->endTransmission(false);

        wire->requestFrom((uint8_t)dev_addr, (uint8_t)len);
        while (wire->available())
        {
            *data = wire->read();
            data++;
        }
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}

class m_ai_cmarea : public AiCamera
{
public:
    m_ai_cmarea()
    {
        Wire.begin();
    }

protected:
    uint8_t writeReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len) override
    {
        return ::writeReg(&Wire, dev_addr, reg, data, len);
    }
    uint8_t readReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len) override
    {
        return ::readReg(&Wire, dev_addr, reg, data, len);
    }
};

// 使用自定义的类设置 ai 视觉模块操作句柄
m_ai_cmarea ai_camrea_handle;

void setup()
{
    Serial.begin(115200);                                     // 初始化串口
    // 使用自定义的iic操作对象初始化ai视觉模块，下方则不用使用
    // Wire.begin();                                             // 初始化iic
    // ai_camrea_handle.Init(&Wire);                             // 绑定IIC操作对象
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_PATCH); // 设置模式为色块识别模式
    delay(1000);                                              // 等待切换完成
    ai_camrea_handle.set_find_color(AiCamera::GREEN);         // 设置识别颜色为绿色
}

void loop()
{
    uint8_t color_id = ai_camrea_handle.get_identify_id(AiCamera::AI_CAMERA_PATCH); // 获取设置的颜色id
    if (ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_PATCH))               // 如果识别到
    {
        int16_t position[4] = {0};
        ai_camrea_handle.get_identify_position(AiCamera::AI_CAMERA_PATCH, position); // 获取位置信息
        if (color_id == AiCamera::GREEN)                                             // 如果识别到的是绿色
        {
            Serial.println("find green");
        }
        Serial.printf("position: %d %d %d %d\n", position[0], position[1], position[2], position[3]);
    }
    else
    {
        Serial.println("not find color");
    }
    delay(400);
}
