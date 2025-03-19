/**
 * 设置 ai 视觉模块为20类物体识别模式，并获取识别信息
 */

/**
 * id 对应的物体名称
 * 0:飞机 plane
 * 1:自行车 bicycle
 * 2:鸟 bird
 * 3:船 boat
 * 4:瓶子 bottle
 * 5:公交车 bus
 * 6:汽车 car
 * 7:猫 cat
 * 8:椅子 chair
 * 9:牛 cow
 * 10:餐桌 diningtable
 * 11:狗 dog
 * 12:房子 house
 * 13:摩托车 motorbike
 * 14:人 person
 * 15:盆栽 pottedplant
 * 16:羊 sheep
 * 17:沙发 sofa
 * 18:船 ship
 * 19:电视机 tvmonitor
 */

#include <Arduino.h>
#include "ai_camera.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

void setup()
{
    Serial.begin(115200);    // 初始化串口
    ai_camrea_handle.Init(); // 初始化
    ai_camrea_handle.set_sys_mode(AI_CAMERA_20_CLASS); // 设置模式为20类物体模式
    delay(1000);             // 等待切换完成
}

void loop()
{
    if (ai_camrea_handle.get_identify_num(AI_CAMERA_20_CLASS) > 0) // 判断是否有物体被识别
    {
        int position[4] = {0};
        int card_id = ai_camrea_handle.get_identify_id(AI_CAMERA_20_CLASS);   // 获取物体识别id
        ai_camrea_handle.get_identify_position(AI_CAMERA_20_CLASS, position); // 获取位置信息

        Serial.print("card_id: ");
        Serial.print(card_id);
        Serial.print(", ");
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
        Serial.println("No object detected!");
    }
    delay(400);
}
