/**
 * 设置 ai 视觉模块为颜色识别模式，颜色的rgb值
 */

#include <Arduino.h>
#include "ai_camera.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

void setup()
{
    Serial.begin(115200);                           // 初始化串口
    ai_camrea_handle.Init();                        // 初始化
    ai_camrea_handle.set_sys_mode(AI_CAMERA_COLOR); // 设置模式为颜色识别模式
    delay(1000);                                    // 等待切换完成
}

void loop()
{
    int rgb[3] = {0};
    ai_camrea_handle.get_color_rgb(rgb); // 获取颜色rgb值
    Serial.print("color rgb: ");
    Serial.print(rgb[0]);
    Serial.print(", ");
    Serial.print(rgb[1]);
    Serial.print(", ");
    Serial.println(rgb[2]);
    delay(400);
}
