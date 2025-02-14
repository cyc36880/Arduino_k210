/**
 * 设置 ai 视觉模块为颜色识别模式，颜色的rgb值
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
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_COLOR);   // 设置模式为颜色识别模式
    delay(1000);                                                // 等待切换完成
}

void loop()
{
    uint8_t rgb[3] = {0};
    ai_camrea_handle.get_color_rgb(rgb); // 获取颜色rgb值
    Serial.printf("color rgb: %d %d %d\n", rgb[0], rgb[1], rgb[2]);
    delay(400);
}

 


