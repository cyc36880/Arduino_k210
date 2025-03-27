/**
 * 设置 ai 视觉模块为人脸识别模式
 * 当检测到人脸时，学习一次人脸，并在之后识别他
 */

#include <Arduino.h>
#include "ai_camera.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

int face_study_flag = 0; // 人脸学习标志位

void setup()
{
    Serial.begin(115200); // 初始化串口
    ai_camrea_handle.Init();
    ai_camrea_handle.set_sys_mode(AI_CAMERA_FACE_RE); // 设置模式为人脸识别模式
    delay(1000);                                      // 等待切换完成
}

void loop()
{
    if (ai_camrea_handle.get_identify_num(AI_CAMERA_FACE_RE, 1) > 0) // 是否检测到人脸
    {
        if (0 == face_study_flag)
        {
            ai_camrea_handle.face_study(); // 学习人脸
            face_study_flag = 1;
        }
        if (ai_camrea_handle.get_identify_num(AI_CAMERA_FACE_RE))
        {
            int position[4] = {0};
            Serial.println("The learned face was detected..");                   // 检测到学习过的人脸
            ai_camrea_handle.get_identify_position(AI_CAMERA_FACE_RE, position); // 获取位置信息
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
            Serial.println("No studied faces.."); // 无学习过的人脸
        }
    }
    else
    {
        Serial.println("no face");
    }
    delay(400);
}
