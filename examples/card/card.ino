/**
 * 设置 ai 视觉模块为卡片识别模式，并获取卡片信息
 */

#include <Arduino.h>
#include "ai_camera.h"

/**
 * id 对应的物体名称
 * 0: ["绿灯", "green"],
 * 1: ["左转", "left"],
 * 2: ["停车", "park"],
 * 3: ["红灯", "red"],
 * 4: ["右转", "right"],
 * 5: ["鸣笛", "whistle"]
 */


// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

void setup()
{
    Serial.begin(115200);                          // 初始化串口
    ai_camrea_handle.Init();                       // 初始化
    ai_camrea_handle.set_sys_mode(AI_CAMERA_CARD); // 设置模式为卡片识别模式
    delay(1000);                                   // 等待切换完成
}

void loop()
{
    if (ai_camrea_handle.get_identify_num(AI_CAMERA_CARD) > 0) // 判读是否检测到卡片
    {
        int position[4] = {0};
        int card_id = ai_camrea_handle.get_identify_id(AI_CAMERA_CARD);   // 获取卡片id
        ai_camrea_handle.get_identify_position(AI_CAMERA_CARD, position); // 获取位置信息

        Serial.print("Card id: ");
        Serial.print(card_id);
        Serial.print(", position: (");
        Serial.print(position[0]);
        Serial.print(", ");
        Serial.print(position[1]);
        Serial.print(", ");
        Serial.print(position[2]);
        Serial.print(", ");
        Serial.print(position[3]);
        Serial.println(")");
    }
    else
    {
        Serial.println("No card detected");
    }
    delay(400);
}
