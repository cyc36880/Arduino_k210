/**
 * 设置 ai 视觉模块为标签识别模式，并获取标签
 * 标签生成网址：https://chaitanyantr.github.io/apriltag
 * ai 视觉模块使用的族为TAG36H11，在选项中 Tag Family选择TAG36H11选项，
 * Tag ID选项可以自主填写，一般范围在 0~200 之间。
 */

#include <Arduino.h>
#include "ai_camera.h"

#include "Wire.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

void setup()
{
    Serial.begin(115200);                                   // 初始化串口
    Wire.begin();                                           // 初始化iic
    ai_camrea_handle.Init(&Wire);                           // 绑定IIC操作对象
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_TAG); // 设置模式为标签识别模式
    delay(1000);                                            // 等待切换完成
}

void loop()
{
    if (ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_TAG) > 0) // 判断是否有标签
    {
        uint16_t rot = 0;
        int16_t position[4] = {0};
        uint8_t tag_id = ai_camrea_handle.get_identify_id(AiCamera::AI_CAMERA_TAG); // 获取标签ID
        ai_camrea_handle.get_identify_position(AiCamera::AI_CAMERA_TAG, position);  // 获取位置信息
        rot = ai_camrea_handle.get_identify_rotation(AiCamera::AI_CAMERA_TAG);      // 获取旋转角度

        Serial.printf("Tag ID: %d, rot:%d, Position: (%d, %d, %d, %d)\n", tag_id, rot, position[0], position[1], position[2], position[3]);
    }
    else
    {
        Serial.println("No tag");
    }
    delay(400);
}
