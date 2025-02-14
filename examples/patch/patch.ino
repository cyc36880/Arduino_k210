/**
 * 设置 ai 视觉模块为色块识别模式，并获取识别信息
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
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_QRCODE);  // 设置模式为色块识别模式
    delay(1000);                                                // 等待切换完成
    ai_camrea_handle.set_find_color(AiCamera::GREEN);           // 设置识别颜色为绿色
}
 
void loop()
{
    if (ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_COLOR))  // 如果识别到绿色
    {
        int16_t position[4] = {0};
        ai_camrea_handle.get_identify_position(AiCamera::AI_CAMERA_COLOR, position); // 获取位置信息
        Serial.printf("find green, position: %d %d %d %d\n", position[0], position[1], position[2], position[3]);
    }
    else
    {
        Serial.println("not find green");
    }
}
