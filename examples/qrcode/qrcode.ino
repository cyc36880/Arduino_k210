/**
 * 设置 ai 视觉模块为二维码识别模式，并获取二维码信息
 * 草料二维码生成器: https://cli.im/
 */

#include <Arduino.h>
#include "ai_camera.h"

// 设置 ai 视觉模块操作句柄
AiCamera ai_camrea_handle;

void setup()
{
    Serial.begin(115200);                            // 初始化串口
    ai_camrea_handle.Init();                         // 初始化
    ai_camrea_handle.set_sys_mode(AI_CAMERA_QRCODE); // 设置模式为二维码识别模式
    delay(1000);                                     // 等待切换完成
}

void loop()
{
    // 获取二维码是否识别到
    if (ai_camrea_handle.get_identify_num(AI_CAMERA_QRCODE) > 0)
    {
        int position[4] = {0};
        ai_camrea_handle.get_identify_position(AI_CAMERA_QRCODE, position); // 获取位置信息
        Serial.print("QRcode position: ");
        Serial.print(position[0]);
        Serial.print(", ");
        Serial.print(position[1]);
        Serial.print(", ");
        Serial.print(position[2]);
        Serial.print(", ");
        Serial.println(position[3]);

        String qrcode_content = ai_camrea_handle.get_qrcode_content(); // 获取二维码内容
        Serial.print("QRcode content: ");
        Serial.println(qrcode_content);
    }
    else
    {
        Serial.println("No QRcode\n");
    }
    delay(400);
}
