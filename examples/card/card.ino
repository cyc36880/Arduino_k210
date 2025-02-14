/**
 * 设置 ai 视觉模块为卡片识别模式，并获取卡片信息
 */



#include <Arduino.h>
#include "ai_camera.h"

#include "Wire.h"
 
// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

void setup() 
{
    Serial.begin(115200);                                       // 初始化串口
    Wire.begin();                                               // 初始化iic
    ai_camrea_handle.Init(&Wire);                               // 绑定IIC操作对象
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_CARD);    // 设置模式为卡片识别模式
    delay(1000);                                                // 等待切换完成
}

void loop()
{
    if (ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_CARD) > 0) // 判读是否检测到卡片
    {
        int16_t position[4] = {0};
        uint8_t card_id = ai_camrea_handle.get_identify_id(AiCamera::AI_CAMERA_CARD); // 获取卡片id
        ai_camrea_handle.get_identify_position(AiCamera::AI_CAMERA_CARD, position);   // 获取位置信息

        Serial.printf("Card id: %d, position: (%d, %d, %d, %d)\n", card_id, position[0], position[1], position[2], position[3]);
    }
    else
    {
        Serial.printf("No card detected\n");
    }
}
