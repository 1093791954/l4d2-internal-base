#pragma once

#include "sdk.h"

// 特感类型枚举
enum class special_infected_type {
    boomer = 0,
    charger,
    hunter,
    jockey,
    smoker,
    spitter,
    tank,
    witch,
    count
};

// 骨骼点配置（来自骨骼点记录.md）
struct infected_bones {
    int head;
    int chest;
    int waist;
};

// 特感骨骼配置表
static const infected_bones g_infected_bones[] = {
    // Boomer: 头部=14, 胸部=12, 腰部=0
    { 14, 12, 0 },
    // Charger: 头部=16, 胸部=3, 腰部=0
    { 16, 3, 0 },
    // Hunter: 头部=14, 胸部=12, 腰部=0
    { 14, 12, 0 },
    // Jockey: 头部=7, 胸部=3, 腰部=0
    { 7, 3, 0 },
    // Smoker: 头部=14, 胸部=12, 腰部=0
    { 14, 12, 0 },
    // Spitter: 头部=7, 胸部=4, 腰部=0
    { 7, 4, 0 },
    // Tank: 头部=14, 胸部=12, 腰部=0
    { 14, 12, 0 },
    // Witch: 头部=-1 (没有头部骨骼记录), 胸部=13, 腰部=0
    { -1, 13, 0 },
};

// 获取特感骨骼索引
inline const infected_bones* get_infected_bones(special_infected_type type) {
    int idx = static_cast<int>(type);
    if (idx >= 0 && idx < static_cast<int>(special_infected_type::count))
        return &g_infected_bones[idx];
    return nullptr;
}

// 自瞄配置结构
struct aim_config {
    // 功能开关
    bool enabled = true;

    // 触发键：VK_LSHIFT (左Shift)
    int key = VK_LSHIFT;

    // 视野范围 (FOV) - 目标必须在当前视角的这个角度范围内才会被锁定
    float fov = 30.0f;

    // 平滑度 (0-100)，越高越平滑但越慢，0为即时瞄准
    float smooth = 10.0f;

    // 骨骼选择：0=头部, 1=胸部, 2=腰部
    int target_bone = 0; // 默认瞄准头部

    // 最大瞄准距离
    float max_distance = 2000.0f;

    // 锁定模式：0=始终锁定最近目标, 1=锁定直到死亡
    int lock_mode = 0;

    // 可见性检测：只瞄准没有被障碍物遮挡的目标
    bool visibility_check = true;
};

// 锁定目标信息
struct locked_target {
    c_base_entity* entity = nullptr;
    int entity_index = -1;
    special_infected_type type;
    float last_fov = 9999.0f;
    bool is_valid = false;

    void reset() {
        entity = nullptr;
        entity_index = -1;
        last_fov = 9999.0f;
        is_valid = false;
    }
};

// 自瞄管理器
class c_aimbot {
public:
    // 初始化
    void initialize();

    // 从配置更新设置
    void update_config();

    // 主运行函数，在CreateMove中调用
    void run(user_cmd_t* cmd);

    // 配置
    aim_config config;

private:
    // 当前锁定的目标
    locked_target m_target;

    // 根据class_id获取特感类型
    special_infected_type get_infected_type(int class_id);

    // 获取目标骨骼索引
    int get_target_bone_index(special_infected_type type);

    // 计算从当前视角到目标的角度差
    float calc_fov(const vec3& view_angles, const vec3& target_pos, const vec3& eye_pos);

    // 计算瞄准角度
    vec3 calc_angle(const vec3& src, const vec3& dst);

    // 平滑处理角度变化
    vec3 smooth_aim(const vec3& current, const vec3& target, float factor);

    // 寻找新目标（在FOV范围内最近的）
    void find_new_target(const vec3& view_angles, const vec3& eye_pos);

    // 验证锁定目标是否仍然有效
    bool validate_target(const vec3& eye_pos);

    // 检查目标是否可见（没有被障碍物遮挡）
    bool is_visible(c_base_entity* entity, const vec3& target_pos, const vec3& eye_pos);

    // 归一化角度（确保在 -180 到 180 范围内）
    void normalize_angles(vec3& angles);

    // 夹紧角度（防止非法值）
    void clamp_angles(vec3& angles);
};

inline c_aimbot g_aimbot;
