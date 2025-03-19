/**
 * 设置 ai 视觉模块为色块识别模式，并获取识别信息
 *
 * 如果识别到色块，打印出色块的位置。
 * 如果识别的颜色ID为绿色，打印出 "find green"。
 * 可以通过拨轮切换识别的颜色。
 */

#include <Arduino.h>
#include "ai_camera.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

void setup()
{
    Serial.begin(115200);                                   // 初始化串口
    ai_camrea_handle.Init();                                // 初始化
    ai_camrea_handle.set_sys_mode(AI_CAMERA_PATCH);         // 设置模式为色块识别模式
    delay(1000);                                            // 等待切换完成
    ai_camrea_handle.set_find_color(AI_CAMERA_COLOR_GREEN); // 设置识别颜色为绿色
}

void loop()
{
    uint8_t color_id = ai_camrea_handle.get_identify_id(AI_CAMERA_PATCH); // 获取设置的颜色id
    if (ai_camrea_handle.get_identify_num(AI_CAMERA_PATCH))               // 如果识别到
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
