/**
 * 设置 ai 视觉模块为人脸识别模式
 * 当检测到人脸时，学习一次人脸，并在之后识别他
 */


#include <Arduino.h>
#include "ai_camera.h"

#include "Wire.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

uint8_t face_study_flag = 0; // 人脸学习标志位

void setup()
{
    Serial.begin(115200);                                       // 初始化串口
    Wire.begin();                                               // 初始化iic
    ai_camrea_handle.Init(&Wire);                               // 绑定IIC操作对象
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_FACE_RE); // 设置模式为二维码识别模式
    delay(1000);                                                // 等待切换完成
}

void loop()
{
    if (ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_FACE_RE, 1) > 0) // 是否检测到人脸
    {
        if (0 == face_study_flag)
        {
            ai_camrea_handle.face_study(); // 学习人脸
            face_study_flag = 1;
        }
        if(ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_FACE_RE))
        {
            int16_t position[4] = {0};
            Serial.println("The learned face was detected..");// 检测到学习过的人脸
            ai_camrea_handle.get_identify_position(AiCamera::AI_CAMERA_FACE_RE, position); // 获取位置信息
            Serial.printf("position: x->%d, y->%d, w->%d, h->%d\n", position[0], position[1], position[2], position[3]);
        }
        else
        {
            Serial.println("No studied faces..");// 无学习过的人脸
        }
    }
    else
    {
        Serial.println("no face");
    }
    delay(400);
}


