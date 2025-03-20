#include "ai_camera.h"
// #include <exception>

/**
 * 所有地址为源地址+1，使用需要-1使用
 */

typedef struct ai_camera_sub
{
    uint8_t base_addr; // 基地址
    uint8_t id;
    uint8_t rot;
    uint8_t pos;
} ai_camera_sub_t;

typedef struct ai_element
{
    uint8_t num; // 识别的数量
    uint8_t id;  // 识别的id
    uint8_t pos; // 识别的位置

    const ai_camera_sub_t *sub; // 子属性
} ai_element_t;

static const uint8_t register_addr[] = {0, 15, 39, 63, 87, 111, 135, 159, 183, 207, 231};

static const ai_element_t obj_sys = {0};
static const ai_element_t obj_color = {0};

// 色块
static ai_element_t obj_patch = {0};
// 标签
static ai_camera_sub_t obj_tag_sub = {0};
static ai_element_t obj_tag = {0};

// 线条
static ai_element_t obj_line = {0};

// 20 类
static ai_camera_sub_t obj_20_class_sub = {0};
static ai_element_t obj_20_class = {0};
// 二维码------
static ai_element_t obj_qrcode = {0};

// 人脸检测
static ai_camera_sub_t obj_face_de_sub = {0};
static ai_element_t obj_face_de = {0};

// 人脸识别
static ai_camera_sub_t obj_face_re_sub = {0};
static ai_element_t obj_face_re = {0};

// 深度学习
static ai_element_t obj_deep_learn = {0};

// 卡片识别
static ai_camera_sub_t obj_card_sub = {0};
static ai_element_t obj_card = {0};

static const ai_element_t *obj_list[] = {
    &obj_sys,
    &obj_color,
    &obj_patch,
    &obj_tag,
    &obj_line,
    &obj_20_class,
    &obj_qrcode,
    &obj_face_de,
    &obj_face_re,
    &obj_deep_learn,
    &obj_card,
};

static uint8_t get_register_addr(uint8_t obj_id, uint8_t offset)
{
    return register_addr[obj_id] + offset;
}

static uint8_t get_obj_sub_size(const ai_camera_sub_t *sub)
{
    uint8_t info_size = 0;
    if (NULL == sub)
        return 0;
    if (sub->id)
        info_size += 1;
    if (sub->rot)
        info_size += 2;
    if (sub->pos)
        info_size += 8;
    return info_size;
}

static uint8_t writeReg(TwoWire *wire, uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (NULL == wire)
        return 1;
    wire->beginTransmission(dev_addr);
    wire->write(reg);
    while (len--)
    {
        wire->write(*(data++));
    }
    wire->endTransmission(true);

    return 0;
}

static uint8_t readReg(TwoWire *wire, uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (NULL == wire)
        return 1;
    wire->beginTransmission(dev_addr);
    wire->write(reg);
    wire->endTransmission(false);

    wire->requestFrom((uint8_t)dev_addr, (uint8_t)len);
    while (wire->available())
    {
        *data = wire->read();
        data++;
    }

    return 0;
}

// 0:online, otherwise offline
static uint8_t isOnline(TwoWire *wire, uint8_t dev_addr)
{
    if (NULL == wire)
        return 0;
    wire->beginTransmission(dev_addr);
    return wire->endTransmission(true);
}

uint8_t AiCamera::writeReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (this->isOnline(dev_addr) != 0)
        return 1;
    return ::writeReg(this->_wire, dev_addr, reg, data, len);
}

uint8_t AiCamera::readReg(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    if (this->isOnline(dev_addr) != 0)
        return 1;
    return ::readReg(this->_wire, dev_addr, reg, data, len);
}

uint8_t AiCamera::isOnline(uint8_t dev_addr)
{
    return ::isOnline(this->_wire, dev_addr);
}

AiCamera::AiCamera(uint8_t addr) : DEV_ADDR(addr)
{
    // 色块
    obj_patch.num = 0x02;
    obj_patch.id = 0x01;
    obj_patch.pos = 0x03;

    // 标签
    obj_tag_sub.base_addr = 0x02;
    obj_tag_sub.id = 1;
    obj_tag_sub.rot = 2;
    obj_tag_sub.pos = 4;

    obj_tag.num = 0x01;
    obj_tag.sub = &obj_tag_sub;

    // 线条
    obj_line.num = 0x01;
    obj_line.pos = 0x02;

    // 20 类
    obj_20_class_sub.base_addr = 0x02;
    obj_20_class_sub.id = 1;
    obj_20_class_sub.pos = 2;

    obj_20_class.num = 0x01;
    obj_20_class.sub = &obj_20_class_sub;

    // 二维码------
    obj_qrcode.num = 0x01;
    obj_qrcode.pos = 0x03;

    // 人脸检测
    obj_face_de_sub.base_addr = 0x02;
    obj_face_de_sub.pos = 1;

    obj_face_de.num = 0x01;
    obj_face_de.sub = &obj_face_de_sub;

    // 人脸识别
    obj_face_re_sub.base_addr = 0x03;
    obj_face_re_sub.id = 1;
    obj_face_re_sub.pos = 2;

    obj_face_re.num = 0x02;
    obj_face_re.sub = &obj_face_re_sub;

    // 深度学习
    obj_deep_learn.num = 0x02;
    obj_deep_learn.id = 0x03;

    // 卡片识别
    obj_card_sub.base_addr = 0x02;
    obj_card_sub.id = 1;
    obj_card_sub.pos = 2;

    obj_card.num = 0x01;
    obj_card.sub = &obj_card_sub;
}

void AiCamera::set_sys_mode(uint8_t mode)
{
    if (mode < 1)
        return;
    mode -= 1;
    this->writeReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_SYS, 0), &mode, 1);
}

uint8_t AiCamera::get_sys_mode(void)
{
    uint8_t mode = 0;
    this->readReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_SYS, 0), &mode, 1);
    return mode + 1;
}

void AiCamera::get_color_rgb(int rgb[3])
{
    uint8_t color[3] = {0};
    this->readReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_COLOR, 0), color, 3);
    for (int i = 0; i < 3; i++)
    {
        rgb[i] = color[i];
    }
}

void AiCamera::set_find_color(uint8_t color_id)
{
    this->writeReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_PATCH, 0), &color_id, 1);
}

void AiCamera::face_study(void)
{
    uint8_t study = 1;
    this->writeReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_FACE_RE, 0x06), &study, 1);
}

void AiCamera::deep_learn_study(void)
{
    uint8_t study = 1;
    this->writeReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_DEEP_LEARN, 0x00), &study, 1);
}

String AiCamera::get_qrcode_content(void)
{
    uint8_t len = 0;
    this->readReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_QRCODE, 0x01), &len, 1);
    char *qrcode = new char[len + 1];
    this->readReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_QRCODE, 0x03), (uint8_t *)qrcode, len);
    qrcode[len] = '\0';
    String str(qrcode);
    delete[] qrcode;
    return str;
}

uint8_t AiCamera::get_identify_num(uint8_t features, uint8_t total)
{
    uint8_t identify_num = 0;
    uint8_t targat_base_addr = get_register_addr(features, 0x00);
    if (AI_CAMERA_FACE_RE == features)
    {
        if (1 == total)
        {
            this->readReg(this->DEV_ADDR, targat_base_addr + 0, &identify_num, 1);
            return identify_num;
        }
    }
    const ai_element_t *obj = obj_list[features];
    uint8_t num_addr = obj->num;
    if (0 == num_addr)
        return 0;
    num_addr -= 1;
    this->readReg(this->DEV_ADDR, targat_base_addr + num_addr, &identify_num, 1);
    return identify_num;
}

uint8_t AiCamera::get_identify_id(uint8_t features, uint8_t index)
{
    uint8_t identify_id = 0;
    uint8_t target_base_addr = get_register_addr(features, 0);
    const ai_element_t *obj = obj_list[features];
    if (obj->id > 0)
    {
        uint8_t _offset = obj->id - 1;
        this->readReg(this->DEV_ADDR, target_base_addr + _offset, &identify_id, 1);
        return identify_id;
    }
    else if (NULL != obj->sub)
    {
        uint8_t id_offset = obj->sub->base_addr - 1;
        if (obj->sub->id > 0)
        {
            uint8_t _offset = obj->sub->id - 1;
            uint8_t sub_size = get_obj_sub_size(obj->sub);
            uint8_t *sub_info_buf = new uint8_t[sub_size];
            this->readReg(this->DEV_ADDR, target_base_addr + id_offset + index, sub_info_buf, sub_size);
            delete[] sub_info_buf;
            return sub_info_buf[_offset];
        }
        else
            return 0;
    }
    return 0;
}

int16_t AiCamera::get_identify_rotation(uint8_t features, uint8_t index)
{
    uint8_t rot[2] = {0};
    uint8_t target_base_addr = get_register_addr(features, 0);
    const ai_element_t *obj = obj_list[features];
    if (NULL == obj->sub)
        return 0;
    const ai_camera_sub_t *sub = obj->sub;

    if (sub->rot > 0)
    {
        uint8_t rot_offset = sub->base_addr - 1;
        uint8_t _offset = sub->rot - 1;
        uint8_t sub_size = get_obj_sub_size(sub);
        uint8_t *sub_info_buf = new uint8_t[sub_size];
        this->readReg(this->DEV_ADDR, target_base_addr + rot_offset + index, sub_info_buf, sub_size);
        rot[0] = sub_info_buf[_offset];
        rot[1] = sub_info_buf[_offset + 1];
        delete[] sub_info_buf;
    }
    return (int16_t)(((uint16_t)rot[0] << 8) | rot[1]);
}

void AiCamera::get_identify_position(uint8_t features, int position[4], uint8_t index)
{
    int16_t _position[4] = {0};
    uint8_t position_buf[8] = {0}; //
    uint8_t target_base_addr = get_register_addr(features, 0);
    const ai_element_t *obj = obj_list[features];
    if (obj->pos > 0)
    {
        uint8_t _offset = obj->pos - 1;
        this->readReg(this->DEV_ADDR, target_base_addr + _offset, position_buf, 8);
    }
    else if (NULL != obj->sub)
    {
        const ai_camera_sub_t *sub = obj->sub;
        uint8_t pos_offset = sub->base_addr - 1;
        uint8_t _offset = sub->pos - 1;
        uint8_t sub_size = get_obj_sub_size(sub);
        uint8_t *sub_info_buf = new uint8_t[sub_size];
        this->readReg(this->DEV_ADDR, target_base_addr + pos_offset + index, sub_info_buf, sub_size);
        for (uint8_t i = 0; i < 8; i++)
        {
            position_buf[i] = sub_info_buf[i + _offset];
        }
        delete[] sub_info_buf;
    }

    for (uint8_t i = 0; i < 4; i++)
    {
        _position[i] = (int16_t)(((uint16_t)position_buf[i * 2] << 8) | position_buf[i * 2 + 1]);
        position[i] = _position[i];
    }
}

uint8_t AiCamera::get_identify_confidence(uint8_t features, uint8_t id)
{
    uint8_t confidence_list[4] = {0};
    if (features != AI_CAMERA_DEEP_LEARN || id > 3)
        return 0;
    uint8_t target_base_addr = get_register_addr(features, 3);
    this->readReg(this->DEV_ADDR, target_base_addr, confidence_list, 4);
    return confidence_list[id];
}
