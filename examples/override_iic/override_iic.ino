/**
 * 定义 m_ai_cmarea 类继承 AiCamera，重写 writeReg、 readReg 和 isOnline 函数，覆盖 AiCamera 的 iic 操作
 *
 */

#include <Arduino.h>
#include "ai_camera.h"

#include <Wire.h>

static uint8_t writeReg(TwoWire *wire, uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (NULL == wire)
        return 1;

    wire->beginTransmission(dev_addr);
    wire->write(reg);
    while (len--)
    {
        wire->write(*(data++));
    }
    wire->endTransmission(true);
    return 0;
}

static uint8_t readReg(TwoWire *wire, uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (NULL == wire)
        return 1;

    wire->beginTransmission(dev_addr);
    wire->write(reg);
    wire->endTransmission(false);

    wire->requestFrom((uint8_t)dev_addr, (uint8_t)len);
    while (wire->available())
    {
        *data = wire->read();
        data++;
    }
    return 0;
}

// 0:online, otherwise offline
static uint8_t isOnline(TwoWire *wire, uint8_t dev_addr)
{
    if (NULL == wire) return 0;
    wire->beginTransmission(dev_addr);
    return wire->endTransmission(true);
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
        if (this->isOnline(dev_addr) != 0)
            return 1;
        return ::writeReg(&Wire, dev_addr, reg, data, len);
    }
    uint8_t readReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len) override
    {
        if (this->isOnline(dev_addr) != 0)
            return 1;
        return ::readReg(&Wire, dev_addr, reg, data, len);
    }
    uint8_t isOnline(uint8_t dev_addr) override
    {
        return ::isOnline(&Wire, dev_addr);
    }
};

// 使用自定义的类设置 ai 视觉模块操作句柄
m_ai_cmarea ai_camrea_handle;

void setup()
{
    Serial.begin(115200); // 初始化串口
    // 使用自定义的iic操作对象初始化ai视觉模块，下方则不用使用
    //  ai_camrea_handle.Init();                             // 初始化
    ai_camrea_handle.set_sys_mode(AI_CAMERA_PATCH);         // 设置模式为色块识别模式
    delay(1000);                                            // 等待切换完成
    ai_camrea_handle.set_find_color(AI_CAMERA_COLOR_GREEN); // 设置识别颜色为绿色
}

void loop()
{
    int color_id = ai_camrea_handle.get_identify_id(AI_CAMERA_PATCH); // 获取设置的颜色id
    if (ai_camrea_handle.get_identify_num(AI_CAMERA_PATCH))           // 如果识别到
    {
        int position[4] = {0};
        ai_camrea_handle.get_identify_position(AI_CAMERA_PATCH, position); // 获取位置信息
        if (color_id == AI_CAMERA_COLOR_GREEN)                             // 如果识别到的是绿色
        {
            Serial.println("find green");
        }

        Serial.print("position: ");
        Serial.print(position[0]);
        Serial.print(", ");
        Serial.print(position[1]);
        Serial.print(", ");
        Serial.print(position[2]);
        Serial.print(", ");
        Serial.println(position[3]);
    }
    else
    {
        Serial.println("not find color");
    }
    delay(400);
}
