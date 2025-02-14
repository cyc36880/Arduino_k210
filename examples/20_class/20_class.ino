/**
 * 设置 ai 视觉模块为20类物体识别模式，并获取识别信息
 */


#include <Arduino.h>
#include "ai_camera.h"

#include "Wire.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

const char *card_name[] = {"飞机","自行车", "鸟", "船", "瓶", "公交车","车","猫","椅子","牛","餐桌", "狗","房子", "摩托车","人","盆栽", "羊","沙发", "船", "电视机"};

void setup() 
{
    Serial.begin(115200);                                           // 初始化串口
    Wire.begin();                                                   // 初始化iic
    ai_camrea_handle.Init(&Wire);                                   // 绑定IIC操作对象
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_20_CLASS);    // 设置模式为20类物体模式
    delay(1000);                                                    // 等待切换完成
}

void loop()
{
    if (ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_20_CLASS) > 0)    // 判断是否有物体被识别
    {
        int16_t position[4] = {0};
        uint8_t card_id = ai_camrea_handle.get_identify_id(AiCamera::AI_CAMERA_20_CLASS); // 获取物体识别id
        ai_camrea_handle.get_identify_position(AiCamera::AI_CAMERA_CARD, position);       // 获取位置信息

        Serial.printf("card: %s, position: %d, %d, %d, %d\n", card_name[card_id], position[0], position[1], position[2], position[3]);
    }
    else
    {
        Serial.println("No object detected!");
    }
    delay(400);
}
