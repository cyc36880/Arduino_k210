/**
 * 设置 ai 视觉模块为二维码识别模式，并获取二维码信息
 * 草料二维码生成器: https://cli.im/
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
    ai_camrea_handle.set_sys_mode(AiCamera::AI_CAMERA_QRCODE);  // 设置模式为二维码识别模式
    delay(1000);                                                // 等待切换完成
}


void loop()
{
    // 获取二维码是否识别到
    if (ai_camrea_handle.get_identify_num(AiCamera::AI_CAMERA_QRCODE) > 0)
    {
        int16_t position[4] = {0};
        ai_camrea_handle.get_identify_position(AiCamera::AI_CAMERA_QRCODE, position); // 获取位置信息
        Serial.printf("QRcode: x->%d, y->%d, w->%d, h->%d\n", position[0], position[1], position[2], position[3]);

        std::string qrcode_content = ai_camrea_handle.get_qrcode_content(); // 获取二维码内容
        Serial.printf("QRcode content: %s\n", qrcode_content.c_str());
    }
    else
    {
        Serial.printf("No QRcode\n");
    }
    delay(400);
}

