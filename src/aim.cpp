#include "aim.h"

#include "game_math.h"
#include "vars.h"
#include "trace_ray.h"
#include <cmath>

void c_aimbot::initialize() {
    config.key = VK_LSHIFT;
    m_target.reset();
}

void c_aimbot::update_config() {
    // 从全局配置读取
    config.enabled = g_vars.get_as<bool>("aim->enabled").value_or(false);
    config.fov = g_vars.get_as<float>("aim->fov").value_or(30.0f);
    config.smooth = g_vars.get_as<float>("aim->smooth").value_or(10.0f);
    config.target_bone = g_vars.get_as<int>("aim->bone").value_or(0);
    config.max_distance = g_vars.get_as<float>("aim->max_distance").value_or(2000.0f);
    config.lock_mode = g_vars.get_as<int>("aim->lock_mode").value_or(0);
    config.visibility_check = g_vars.get_as<bool>("aim->visibility_check").value_or(true);
}

special_infected_type c_aimbot::get_infected_type(int class_id) {
    switch (class_id) {
    case Boomer:  return special_infected_type::boomer;
    case Charger: return special_infected_type::charger;
    case Hunter:  return special_infected_type::hunter;
    case Jockey:  return special_infected_type::jockey;
    case Smoker:  return special_infected_type::smoker;
    case Spitter: return special_infected_type::spitter;
    case Tank:    return special_infected_type::tank;
    case Witch:   return special_infected_type::witch;
    default:      return special_infected_type::count;
    }
}

int c_aimbot::get_target_bone_index(special_infected_type type) {
    const infected_bones* bones = get_infected_bones(type);
    if (!bones)
        return -1;

    switch (config.target_bone) {
    case 0: return bones->head;   // 头部
    case 1: return bones->chest;  // 胸部
    case 2: return bones->waist;  // 腰部
    default: return bones->head;
    }
}

void c_aimbot::normalize_angles(vec3& angles) {
    while (angles.x > 89.0f)
        angles.x -= 180.0f;
    while (angles.x < -89.0f)
        angles.x += 180.0f;

    while (angles.y > 180.0f)
        angles.y -= 360.0f;
    while (angles.y < -180.0f)
        angles.y += 360.0f;

    angles.z = 0.0f;
}

void c_aimbot::clamp_angles(vec3& angles) {
    if (angles.x > 89.0f)
        angles.x = 89.0f;
    if (angles.x < -89.0f)
        angles.x = -89.0f;

    if (angles.y > 180.0f)
        angles.y = 180.0f;
    if (angles.y < -180.0f)
        angles.y = -180.0f;

    angles.z = 0.0f;
}

bool c_aimbot::is_visible(c_base_entity* entity, const vec3& target_pos, const vec3& eye_pos) {
    // 如果未启用可见性检测，直接返回可见
    if (!config.visibility_check)
        return true;

    // 如果没有射线检测接口，默认返回可见
    if (!g_l4d2.m_tracers)
        return true;

    // 创建射线（从眼睛位置到目标位置）
    ray_t ray(eye_pos, target_pos);

    // 创建过滤器，跳过本地玩家自身
    auto local = g_l4d2.get_local();
    if (!local)
        return false;

    trace_filter_t filter(local);
    trace_t trace_result;

    // 执行射线检测
    g_l4d2.m_tracers->trace(ray, MASK_SHOT, &filter, &trace_result);

    // 检查是否击中目标实体
    // m_fraction >= 1.0f 表示射线到达终点，没有碰到任何障碍物
    // 或者击中的实体就是我们想要瞄准的目标
    if (trace_result.m_fraction >= 1.0f)
        return true;

    // 如果射线击中了其他实体，检查是否是目标实体
    if (trace_result.m_entity == entity)
        return true;

    // 被其他障碍物遮挡
    return false;
}

float c_aimbot::calc_fov(const vec3& view_angles, const vec3& target_pos, const vec3& eye_pos) {
    vec3 aim_direction;
    float yaw = math::DEG2RAD(view_angles.y);
    float pitch = math::DEG2RAD(view_angles.x);

    aim_direction.x = std::cos(yaw) * std::cos(pitch);
    aim_direction.y = std::sin(yaw) * std::cos(pitch);
    aim_direction.z = -std::sin(pitch);

    vec3 target_direction = target_pos - eye_pos;
    float len = target_direction.length();
    if (len > 0.0f) {
        target_direction.x /= len;
        target_direction.y /= len;
        target_direction.z /= len;
    }

    float dot = aim_direction.dot(target_direction);
    dot = clamp_val(dot, -1.0f, 1.0f);

    return math::RAD2DEG(std::acos(dot));
}

vec3 c_aimbot::calc_angle(const vec3& src, const vec3& dst) {
    vec3 angles;
    vec3 delta = dst - src;

    float hyp = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    angles.x = math::RAD2DEG(std::atan2(-delta.z, hyp));
    angles.y = math::RAD2DEG(std::atan2(delta.y, delta.x));
    angles.z = 0.0f;

    return angles;
}

vec3 c_aimbot::smooth_aim(const vec3& current, const vec3& target, float factor) {
    vec3 delta = target - current;
    normalize_angles(delta);

    // 平滑因子：0-100，越高越平滑
    // 转换为 0-1 的系数
    float smooth_factor = clamp_val(factor / 100.0f, 0.0f, 1.0f);

    // 反转平滑因子：高smooth值 = 小的增量
    float speed = 1.0f - smooth_factor;
    speed = std::max(speed, 0.05f); // 最小速度保证

    vec3 smoothed = current + delta * speed;
    normalize_angles(smoothed);
    clamp_angles(smoothed);

    return smoothed;
}

bool c_aimbot::validate_target(const vec3& eye_pos) {
    if (!m_target.is_valid || !m_target.entity)
        return false;

    // 检查实体是否仍然有效
    auto local = g_l4d2.get_local();
    if (!local || local->get_health() <= 0)
        return false;

    // 获取实体索引验证
    for (int i = 0; i < g_l4d2.m_entity_list->get_highest_entity_index(); i++) {
        auto entity = g_l4d2.m_entity_list->get<c_base_entity*>(i);
        if (entity == m_target.entity) {
            // 验证实体类型是否匹配
            auto class_list = entity->get_client_class();
            if (!class_list)
                return false;

            auto type = get_infected_type(class_list->m_class_id);
            if (type != m_target.type)
                return false;

            // 可见性检测：检查目标是否被遮挡
            if (config.visibility_check) {
                int bone_idx = get_target_bone_index(type);
                if (bone_idx >= 0) {
                    vec3 bone_pos = entity->get_bone_position(bone_idx);
                    if (bone_pos.x != 0 || bone_pos.y != 0 || bone_pos.z != 0) {
                        if (!is_visible(entity, bone_pos, eye_pos))
                            return false;  // 目标被遮挡，无效
                    }
                }
            }

            return true;
        }
    }

    return false;
}

void c_aimbot::find_new_target(const vec3& view_angles, const vec3& eye_pos) {
    m_target.reset();

    auto local = g_l4d2.get_local();
    if (!local)
        return;

    float best_fov = config.fov;
    c_base_entity* best_target = nullptr;
    int best_index = -1;
    special_infected_type best_type = special_infected_type::count;

    for (int i = 0; i < g_l4d2.m_entity_list->get_highest_entity_index(); i++) {
        auto entity = g_l4d2.m_entity_list->get<c_base_entity*>(i);

        if (!entity || entity == local)
            continue;

        // 检查是否为特感
        auto class_list = entity->get_client_class();
        if (!class_list)
            continue;

        auto type = get_infected_type(class_list->m_class_id);
        if (type == special_infected_type::count)
            continue;

        // 获取目标骨骼点
        int bone_idx = get_target_bone_index(type);
        if (bone_idx < 0)
            continue;

        vec3 bone_pos = entity->get_bone_position(bone_idx);
        if (bone_pos.x == 0 && bone_pos.y == 0 && bone_pos.z == 0)
            continue;

        // 检查距离
        if (bone_pos.distance_to(eye_pos) > config.max_distance)
            continue;

        // 可见性检测：跳过被遮挡的目标
        if (config.visibility_check && !is_visible(entity, bone_pos, eye_pos))
            continue;

        // 计算FOV
        float fov = calc_fov(view_angles, bone_pos, eye_pos);
        if (fov < best_fov) {
            best_fov = fov;
            best_target = entity;
            best_index = i;
            best_type = type;
        }
    }

    if (best_target) {
        m_target.entity = best_target;
        m_target.entity_index = best_index;
        m_target.type = best_type;
        m_target.last_fov = best_fov;
        m_target.is_valid = true;
    }
}

void c_aimbot::run(user_cmd_t* cmd) {
    // 更新配置
    update_config();

    if (!config.enabled)
        return;

    // 检查触发键（左Shift）
    if (!(GetAsyncKeyState(config.key) & 0x8000))
        return;

    auto local = g_l4d2.get_local();
    if (!local || local->get_health() <= 0)
        return;

    // 获取本地玩家眼睛位置
    vec3 eye_pos = local->get_origin();
    // 添加眼睛高度偏移（约 64 单位）
    eye_pos.z += 64.0f;

    // 根据锁定模式处理目标选择
    if (config.lock_mode == 0) {
        // 模式0：始终锁定最近目标 - 每次都重新寻找
        find_new_target(cmd->viewangles, eye_pos);
    } else {
        // 模式1：锁定直到死亡 - 验证目标无效后才寻找新目标
        if (!validate_target(eye_pos)) {
            find_new_target(cmd->viewangles, eye_pos);
        }
    }

    // 如果没有有效目标，直接返回
    if (!m_target.is_valid || !m_target.entity)
        return;

    // 获取目标骨骼位置
    int bone_idx = get_target_bone_index(m_target.type);
    if (bone_idx < 0)
        return;

    vec3 target_pos = m_target.entity->get_bone_position(bone_idx);
    if (target_pos.x == 0 && target_pos.y == 0 && target_pos.z == 0)
        return;

    // 计算瞄准角度
    vec3 target_angles = calc_angle(eye_pos, target_pos);
    normalize_angles(target_angles);
    clamp_angles(target_angles);

    // 应用平滑
    vec3 final_angles = smooth_aim(cmd->viewangles, target_angles, config.smooth);

    // 设置视角
    cmd->viewangles = final_angles;
}
