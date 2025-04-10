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

static const uint8_t register_addr[] = {0,15,30,45,60,75,90,105,120,135,150, 165, 180, 195};

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

// 无线图传
static const ai_element_t obj_wifi_server={};

// 自定义 图形检测
static const ai_camera_sub_t obj_ur_image_det_sub = {0};
static const ai_element_t obj_ur_image_det = {0};

// 自定义 图像分类
static const ai_element_t obj_image_class = {0};


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
    &obj_wifi_server,
    &obj_ur_image_det,
    &obj_image_class,
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
        return 1;
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

    // 无线图传
}

void AiCamera::set_sys_mode(AI_CAMERA_REGISTER_t mode)
{
    uint8_t _mode = mode;
    if (_mode < 1)
        return;
        _mode -= 1;
    this->writeReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_SYS, 0), &_mode, 1);
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
void AiCamera::get_color_rgb(int &r, int &g, int &b)
{
    int rgb[3] = {0};
    this->get_color_rgb(rgb);
    r = rgb[0];
    g = rgb[1];
    b = rgb[2];
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

uint8_t AiCamera::get_identify_num(AI_CAMERA_REGISTER_t features, uint8_t total)
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

uint8_t AiCamera::get_face_attributes(int &is_male, int &is_mouth_open, int &is_smail, int &is_glasses, uint8_t index)
{
    uint8_t ret = 0;
    uint8_t attr_buf[4] = {0};
    ret =  this->readReg(this->DEV_ADDR, get_register_addr(AI_CAMERA_FACE_ATTRIBUTE, 0x05+index), attr_buf, 4);
    is_male = attr_buf[0];
    is_mouth_open = attr_buf[1];
    is_smail = attr_buf[2];
    is_glasses = attr_buf[3];
    return ret;
}

uint8_t AiCamera::get_identify_id(AI_CAMERA_REGISTER_t features, uint8_t index)
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
            identify_id = sub_info_buf[_offset];
            delete[] sub_info_buf;
            return identify_id;
        }
        else
            return 0;
    }
    return 0;
}

int16_t AiCamera::get_identify_rotation(AI_CAMERA_REGISTER_t features, uint8_t index)
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

void AiCamera::get_identify_position(AI_CAMERA_REGISTER_t features, int position[4], uint8_t index)
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

void AiCamera::get_identify_position(AI_CAMERA_REGISTER_t features, int &x, int &y, int &w, int &h, uint8_t index)
{
    int position[4] = {0};
    this->get_identify_position(features, position, index);
    x = position[0];
    y = position[1];
    w = position[2];
    h = position[3];
}

uint8_t AiCamera::get_identify_confidence(AI_CAMERA_REGISTER_t features, uint8_t id)
{
    uint8_t confidence_list[4] = {0};
    if (features != AI_CAMERA_DEEP_LEARN || id > 3)
        return 0;
    uint8_t target_base_addr = get_register_addr(features, 3);
    this->readReg(this->DEV_ADDR, target_base_addr, confidence_list, 4);
    return confidence_list[id];
}



uint8_t AiCamera::set_wifi_server_is_scan_qrcode(bool is_scan)
{
    uint8_t ret=0;
    uint8_t is_scan_qrcode_addr = get_register_addr(AI_CAMERA_WIFI_SERVER, 3);
    uint8_t is_scan_value = is_scan ? 1 : 0;
    ret += this->writeReg(this->DEV_ADDR, is_scan_qrcode_addr, &is_scan_value, 1);
    return ret;
}

uint8_t AiCamera::get_wifi_server_ssid_passward(String &ssid, String &password)
{
    uint8_t ret=0;
    uint8_t ssid_len = 0;
    uint8_t password_len = 0;
    uint8_t ssid_addr = get_register_addr(AI_CAMERA_WIFI_SERVER, 0);
    uint8_t password_addr = ssid_addr+1;
    
    ret+=this->readReg(this->DEV_ADDR, ssid_addr, &ssid_len, 1);
    char *ssid_content = new char[ssid_len+2];
    ret+=this->readReg(this->DEV_ADDR, ssid_addr, (uint8_t *)ssid_content, ssid_len+1);
    ssid_content[ssid_len+1] = '\0';
    ssid = String(ssid_content+1);
    delete[] ssid_content;

    ret+=this->readReg(this->DEV_ADDR, password_addr, &password_len, 1);
    char *password_content = new char[password_len+2];
    ret+=this->readReg(this->DEV_ADDR, password_addr, (uint8_t *)password_content, password_len+1);
    password_content[password_len+1] = '\0';
    password = String(password_content+1);
    delete[] password_content;

    return ret;
}


uint8_t AiCamera::set_wifi_server_ssid_passward(const char *ssid, const char *password)
{
    uint8_t ret=0;
    uint8_t ssid_addr = get_register_addr(AI_CAMERA_WIFI_SERVER, 0);
    uint8_t password_addr = ssid_addr+1;

    const uint8_t ssid_len = strlen(ssid);
    const uint8_t password_len = strlen(password);

    char *ssid_content = new char[ssid_len+1];
    char *password_content = new char[password_len+1];

    ssid_content[0] = ssid_len;
    memcpy(ssid_content+1, ssid, ssid_len);
    
    password_content[0] = password_len;
    memcpy(password_content+1, password, password_len);

    ret += this->writeReg(this->DEV_ADDR, ssid_addr,     (uint8_t *)ssid_content,     ssid_len+1);
    ret += this->writeReg(this->DEV_ADDR, password_addr, (uint8_t *)password_content, password_len+1);

    delete[] ssid_content;
    delete[] password_content;

    this->set_wifi_server_is_scan_qrcode(0); // 关闭二维码扫描

    return ret;
}


String AiCamera::get_wifi_server_ip(void)
{
    String ip;
    uint8_t ret=0;
    uint8_t ip_len = 0;
    uint8_t ip_addr = get_register_addr(AI_CAMERA_WIFI_SERVER, 2);
    
    ret+=this->readReg(this->DEV_ADDR, ip_addr, &ip_len, 1);
    char *ip_content = new char[ip_len+2];
    ret+=this->readReg(this->DEV_ADDR, ip_addr, (uint8_t *)ip_content, ip_len+1);
    ip_content[ip_len+1] = '\0';
    ip = String(ip_content+1);
    delete[] ip_content;
    if (0!=ret)
    {
        return String();
    }
    return ip;
}

#if 0
uint8_t AiCamera::set_user_image_detection_model_info(uint32_t model_size, const char *anchors, uint8_t identify_num)
{
    uint8_t ret = 0;
    uint8_t target_base_addr = get_register_addr(AI_CAMERA_USER_IMAGE_DETECTION, 0);
    uint8_t model_size_addr = target_base_addr+0;
    uint8_t anchors_addr = target_base_addr+1;
    uint8_t identify_num_addr = target_base_addr+2;

    String model_size_str;
    String _model_size_str = String(model_size);
    model_size_str += _model_size_str.length();
    model_size_str += _model_size_str;
    ret += this->writeReg(this->DEV_ADDR, model_size_addr, (uint8_t *)model_size_str.c_str(), model_size_str.length());

    String anchors_str;
    String _anchors_str = String(anchors);
    anchors_str += _anchors_str.length();
    anchors_str += _anchors_str;
    ret += this->writeReg(this->DEV_ADDR, anchors_addr, (uint8_t *)anchors_str.c_str(), anchors_str.length());

    ret += this->writeReg(this->DEV_ADDR, identify_num_addr, (uint8_t *)&identify_num, 1);

    return ret;
}

uint8_t AiCamera::set_user_image_class_model_info(uint32_t model_size)
{
    uint8_t ret = 0;
    uint8_t target_base_addr = get_register_addr(AI_CAMERA_UR_IMAGE_CLASS, 0);
    uint8_t model_size_addr = target_base_addr+0;

    String model_size_str;
    // String _model_size_str = std::to_string(model_size);
    // model_size_str.push_back(_model_size_str.length());
    // model_size_str += _model_size_str;
    model_size_str += String(model_size);
    ret += this->writeReg(this->DEV_ADDR, model_size_addr, (uint8_t *)model_size_str.c_str(), model_size_str.length());
    return ret;
}

#endif