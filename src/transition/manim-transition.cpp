#include "manim-transition.h"
#include "../math/rate_functions.hpp"

#include <obs-module.h>
#include <graphics/graphics.h>
#include <graphics/vec2.h>
#include <graphics/vec4.h>
#include <util/bmem.h>
#include <algorithm>

struct manim_transition_info {
    obs_source_t *source;
    gs_effect_t *effect;

    gs_eparam_t *param_tex_a;
    gs_eparam_t *param_tex_b;
    gs_eparam_t *param_phase;
    gs_eparam_t *param_exit_mode;
    gs_eparam_t *param_enter_mode;
    gs_eparam_t *param_progress;
    gs_eparam_t *param_border_color;
    gs_eparam_t *param_border_thickness;
    gs_eparam_t *param_glow_intensity;
    gs_eparam_t *param_canvas_size;
    gs_eparam_t *param_contour_mode;
    gs_eparam_t *param_contour_sensitivity;
    gs_eparam_t *param_fill_delay;
    gs_eparam_t *param_write_flow;
    gs_eparam_t *param_reverse_write;
    gs_eparam_t *param_lag_ratio;
    gs_eparam_t *param_tip_glow;

    // Transition parameters
    int exit_mode;
    int enter_mode;
    int contour_mode;
    int color_preset;
    int write_flow;
    bool reverse_write;
    float lag_ratio;
    float tip_glow;
    float split_t;
    float pause_t;
    struct vec4 border_color;
    float border_thickness;
    float glow_intensity;
    float contour_sensitivity;
    float fill_delay;
};

static const char *manim_get_name(void *type_data)
{
    UNUSED_PARAMETER(type_data);
    return obs_module_text("ManimMotionTransition");
}

static void manim_update(void *data, obs_data_t *settings)
{
    struct manim_transition_info *info = (struct manim_transition_info *)data;

    info->exit_mode = (int)obs_data_get_int(settings, "exit_mode");
    info->enter_mode = (int)obs_data_get_int(settings, "enter_mode");
    info->contour_mode = (int)obs_data_get_int(settings, "contour_mode");
    info->color_preset = (int)obs_data_get_int(settings, "color_preset");
    info->write_flow = (int)obs_data_get_int(settings, "write_flow");
    info->reverse_write = obs_data_get_bool(settings, "reverse_write");

    double lag = obs_data_get_double(settings, "lag_ratio");
    if (lag <= 0.0) lag = 0.20;
    info->lag_ratio = (float)lag;

    double tglow = obs_data_get_double(settings, "tip_glow");
    if (tglow < 0.0) tglow = 1.8;
    info->tip_glow = (float)tglow;

    double split_pct = obs_data_get_double(settings, "split_ratio");
    if (split_pct <= 0.0) split_pct = 42.0;
    info->split_t = (float)(split_pct / 100.0);

    double pause_pct = obs_data_get_double(settings, "pause_ratio");
    if (pause_pct < 0.0) pause_pct = 6.0;
    info->pause_t = (float)(pause_pct / 100.0);

    // Keep split + pause within 0.95
    if (info->split_t + info->pause_t > 0.95f) {
        info->split_t = 0.45f;
        info->pause_t = 0.05f;
    }

    uint32_t color_val = (uint32_t)obs_data_get_int(settings, "border_color");
    if (color_val == 0) {
        // Default Mizuiro Water Blue #7FA8E8 (ABGR: 0xFFE8A87F)
        color_val = 0xFFE8A87F;
    }
    vec4_from_rgba(&info->border_color, color_val);

    double thickness = obs_data_get_double(settings, "border_thickness");
    if (thickness <= 0.0) thickness = 0.008;
    info->border_thickness = (float)thickness;

    double glow = obs_data_get_double(settings, "glow_intensity");
    if (glow <= 0.0) glow = 1.6;
    info->glow_intensity = (float)glow;

    double sens = obs_data_get_double(settings, "contour_sensitivity");
    if (sens <= 0.0) sens = 0.22;
    info->contour_sensitivity = (float)sens;

    double fdelay = obs_data_get_double(settings, "fill_delay");
    if (fdelay <= 0.0) fdelay = 45.0;
    info->fill_delay = (float)(fdelay / 100.0);
}

static void *manim_create(obs_data_t *settings, obs_source_t *source)
{
    struct manim_transition_info *info = (struct manim_transition_info *)bzalloc(sizeof(struct manim_transition_info));
    info->source = source;

    char *effect_path = obs_module_file("manim_transition.effect");
    if (!effect_path) {
        effect_path = obs_module_file("effects/manim_transition.effect");
    }
    if (!effect_path) {
        blog(LOG_ERROR, "[obs-manim-motion] manim_transition.effect not found in module directory");
        bfree(info);
        return NULL;
    }

    obs_enter_graphics();
    info->effect = gs_effect_create_from_file(effect_path, NULL);
    obs_leave_graphics();
    bfree(effect_path);

    if (!info->effect) {
        blog(LOG_ERROR, "[obs-manim-motion] Failed to compile manim_transition.effect");
        bfree(info);
        return NULL;
    }

    info->param_tex_a = gs_effect_get_param_by_name(info->effect, "tex_a");
    info->param_tex_b = gs_effect_get_param_by_name(info->effect, "tex_b");
    info->param_phase = gs_effect_get_param_by_name(info->effect, "phase");
    info->param_exit_mode = gs_effect_get_param_by_name(info->effect, "exit_mode");
    info->param_enter_mode = gs_effect_get_param_by_name(info->effect, "enter_mode");
    info->param_progress = gs_effect_get_param_by_name(info->effect, "progress");
    info->param_border_color = gs_effect_get_param_by_name(info->effect, "border_color");
    info->param_border_thickness = gs_effect_get_param_by_name(info->effect, "border_thickness");
    info->param_glow_intensity = gs_effect_get_param_by_name(info->effect, "glow_intensity");
    info->param_canvas_size = gs_effect_get_param_by_name(info->effect, "canvas_size");
    info->param_contour_mode = gs_effect_get_param_by_name(info->effect, "contour_mode");
    info->param_contour_sensitivity = gs_effect_get_param_by_name(info->effect, "contour_sensitivity");
    info->param_fill_delay = gs_effect_get_param_by_name(info->effect, "fill_delay");
    info->param_write_flow = gs_effect_get_param_by_name(info->effect, "write_flow");
    info->param_reverse_write = gs_effect_get_param_by_name(info->effect, "reverse_write");
    info->param_lag_ratio = gs_effect_get_param_by_name(info->effect, "lag_ratio");
    info->param_tip_glow = gs_effect_get_param_by_name(info->effect, "tip_glow");

    manim_update(info, settings);
    return info;
}

static void manim_destroy(void *data)
{
    struct manim_transition_info *info = (struct manim_transition_info *)data;
    if (info) {
        if (info->effect) {
            obs_enter_graphics();
            gs_effect_destroy(info->effect);
            obs_leave_graphics();
        }
        bfree(info);
    }
}

static void manim_callback(void *data, gs_texture_t *a, gs_texture_t *b, float t, uint32_t cx, uint32_t cy)
{
    struct manim_transition_info *info = (struct manim_transition_info *)data;
    if (!info || !info->effect) return;

    int current_phase = 0;
    float rel_p = 0.0f;
    float eased_p = 0.0f;

    float t_exit_end = info->split_t;
    float t_enter_start = t_exit_end + info->pause_t;

    if (t < t_exit_end) {
        current_phase = 0; // Exit phase
        rel_p = t / std::max(t_exit_end, 0.001f);
        if (info->exit_mode == 0) {
            eased_p = manim::math::rush_into(rel_p);
        } else {
            eased_p = manim::math::double_smooth(rel_p);
        }
    } else if (t < t_enter_start) {
        current_phase = 1; // Pause phase
        eased_p = 0.0f;
    } else {
        current_phase = 2; // Entrance phase
        rel_p = (t - t_enter_start) / std::max(1.0f - t_enter_start, 0.001f);
        if (info->enter_mode == 0) {
            eased_p = manim::math::rush_from(rel_p);
        } else {
            eased_p = manim::math::double_smooth(rel_p);
        }
    }

    if (a) {
        gs_effect_set_texture_srgb(info->param_tex_a, a);
    }
    if (b) {
        gs_effect_set_texture_srgb(info->param_tex_b, b);
    }

    struct vec2 canvas_sz;
    vec2_set(&canvas_sz, (float)cx, (float)cy);

    gs_effect_set_int(info->param_phase, current_phase);
    gs_effect_set_int(info->param_exit_mode, info->exit_mode);
    gs_effect_set_int(info->param_enter_mode, info->enter_mode);
    gs_effect_set_float(info->param_progress, eased_p);
    gs_effect_set_vec4(info->param_border_color, &info->border_color);
    gs_effect_set_float(info->param_border_thickness, info->border_thickness);
    gs_effect_set_float(info->param_glow_intensity, info->glow_intensity);
    gs_effect_set_vec2(info->param_canvas_size, &canvas_sz);
    gs_effect_set_int(info->param_contour_mode, info->contour_mode);
    gs_effect_set_float(info->param_contour_sensitivity, info->contour_sensitivity);
    gs_effect_set_float(info->param_fill_delay, info->fill_delay);
    gs_effect_set_int(info->param_write_flow, info->write_flow);
    gs_effect_set_bool(info->param_reverse_write, info->reverse_write);
    gs_effect_set_float(info->param_lag_ratio, info->lag_ratio);
    gs_effect_set_float(info->param_tip_glow, info->tip_glow);

    while (gs_effect_loop(info->effect, "Transition")) {
        gs_draw_sprite(NULL, 0, cx, cy);
    }
}

static void manim_video_render(void *data, gs_effect_t *effect)
{
    UNUSED_PARAMETER(effect);
    struct manim_transition_info *info = (struct manim_transition_info *)data;
    obs_transition_video_render2(info->source, manim_callback, NULL);
}

static float mix_a(void *data, float t)
{
    UNUSED_PARAMETER(data);
    return 1.0f - t;
}

static float mix_b(void *data, float t)
{
    UNUSED_PARAMETER(data);
    return t;
}

static bool manim_audio_render(void *data, uint64_t *ts_out, struct obs_source_audio_mix *audio, uint32_t mixers,
                               size_t channels, size_t sample_rate)
{
    struct manim_transition_info *info = (struct manim_transition_info *)data;
    return obs_transition_audio_render(info->source, ts_out, audio, mixers, channels, sample_rate, mix_a, mix_b);
}

static void manim_defaults(obs_data_t *settings)
{
    obs_data_set_default_int(settings, "exit_mode", 0);       // FadeOut
    obs_data_set_default_int(settings, "enter_mode", 1);      // Create
    obs_data_set_default_int(settings, "contour_mode", 0);    // GPU XDoG
    obs_data_set_default_int(settings, "color_preset", 0);    // Mizuiro (#7FA8E8)
    obs_data_set_default_double(settings, "split_ratio", 42.0);
    obs_data_set_default_double(settings, "pause_ratio", 6.0);
    obs_data_set_default_int(settings, "border_color", 0xFFE8A87F); // Mizuiro Water Blue (#7FA8E8)
    obs_data_set_default_double(settings, "border_thickness", 0.008);
    obs_data_set_default_double(settings, "glow_intensity", 1.6);
    obs_data_set_default_double(settings, "contour_sensitivity", 0.22);
    obs_data_set_default_double(settings, "fill_delay", 45.0);
    obs_data_set_default_int(settings, "write_flow", 0);          // LTR Natural
    obs_data_set_default_bool(settings, "reverse_write", false);
    obs_data_set_default_double(settings, "lag_ratio", 0.20);     // 20% lag
    obs_data_set_default_double(settings, "tip_glow", 1.8);       // 1.8x glow
}

static bool color_preset_changed(obs_properties_t *props, obs_property_t *p, obs_data_t *settings)
{
    UNUSED_PARAMETER(props);
    UNUSED_PARAMETER(p);
    int preset = (int)obs_data_get_int(settings, "color_preset");
    uint32_t c = 0;
    switch (preset) {
    case 0: c = 0xFFE8A87F; break; // 水色雾蓝 (#7FA8E8)
    case 1: c = 0xFFFFEBDC; break; // 冰蓝微光 (#DCEBFF)
    case 2: c = 0xFFFFFFFF; break; // 经典粉笔白 (#FFFFFF)
    case 3: c = 0xFF00D7FF; break; // 黄金流光 (#FFD700)
    case 4: c = 0xFFD7C6F4; break; // 缎带柔粉 (#F4C6D7)
    case 5: c = 0xFFDEF3FF; break; // 奶油温白 (#FFF3DE)
    case 6: c = 0xFFFFFF00; break; // 赛博霓虹青 (#00FFFF)
    default: return true;          // 7: 自定义
    }
    obs_data_set_int(settings, "border_color", c);
    return true;
}

static obs_properties_t *manim_properties(void *data)
{
    UNUSED_PARAMETER(data);
    obs_properties_t *props = obs_properties_create();

    obs_property_t *prop_exit = obs_properties_add_list(
        props, "exit_mode", obs_module_text("ExitMode"),
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
    obs_property_list_add_int(prop_exit, obs_module_text("ExitMode.FadeOut"), 0);
    obs_property_list_add_int(prop_exit, obs_module_text("ExitMode.Uncreate"), 1);
    obs_property_list_add_int(prop_exit, obs_module_text("ExitMode.Unwrite"), 2);

    obs_property_t *prop_enter = obs_properties_add_list(
        props, "enter_mode", obs_module_text("EnterMode"),
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
    obs_property_list_add_int(prop_enter, obs_module_text("EnterMode.FadeIn"), 0);
    obs_property_list_add_int(prop_enter, obs_module_text("EnterMode.Create"), 1);
    obs_property_list_add_int(prop_enter, obs_module_text("EnterMode.Write"), 2);

    obs_property_t *prop_flow = obs_properties_add_list(
        props, "write_flow", obs_module_text("WriteFlow"),
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
    obs_property_list_add_int(prop_flow, obs_module_text("WriteFlow.Natural"), 0);
    obs_property_list_add_int(prop_flow, obs_module_text("WriteFlow.Boustrophedon"), 1);
    obs_property_list_add_int(prop_flow, obs_module_text("WriteFlow.Vertical"), 2);
    obs_property_list_add_int(prop_flow, obs_module_text("WriteFlow.Diagonal"), 3);
    obs_property_list_add_int(prop_flow, obs_module_text("WriteFlow.SpiralIn"), 4);

    obs_properties_add_bool(props, "reverse_write", obs_module_text("ReverseWrite"));
    obs_properties_add_float_slider(props, "lag_ratio", obs_module_text("LagRatio"), 0.02, 0.80, 0.02);
    obs_properties_add_float_slider(props, "tip_glow", obs_module_text("TipGlow"), 0.0, 4.0, 0.1);

    obs_property_t *prop_contour = obs_properties_add_list(
        props, "contour_mode", obs_module_text("ContourMode"),
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
    obs_property_list_add_int(prop_contour, obs_module_text("ContourMode.XDoG"), 0);
    obs_property_list_add_int(prop_contour, obs_module_text("ContourMode.Fourier"), 1);
    obs_property_list_add_int(prop_contour, obs_module_text("ContourMode.Disabled"), 2);

    obs_properties_add_float_slider(props, "contour_sensitivity", obs_module_text("ContourSensitivity"), 0.05, 0.50, 0.01);
    obs_properties_add_float_slider(props, "fill_delay", obs_module_text("FillDelay"), 10.0, 80.0, 1.0);

    obs_properties_add_float_slider(props, "split_ratio", obs_module_text("TimingSplit"), 10.0, 80.0, 1.0);
    obs_properties_add_float_slider(props, "pause_ratio", obs_module_text("PauseDuration"), 0.0, 20.0, 1.0);

    obs_property_t *prop_color_preset = obs_properties_add_list(
        props, "color_preset", obs_module_text("ColorPreset"),
        OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_INT);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.Mizuiro"), 0);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.IceBlue"), 1);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.ChalkWhite"), 2);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.MathGold"), 3);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.RibbonPink"), 4);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.CreamWhite"), 5);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.NeonCyan"), 6);
    obs_property_list_add_int(prop_color_preset, obs_module_text("ColorPreset.Custom"), 7);

    obs_property_set_modified_callback(prop_color_preset, color_preset_changed);

    obs_properties_add_color(props, "border_color", obs_module_text("BorderColor"));
    obs_properties_add_float_slider(props, "border_thickness", obs_module_text("BorderThickness"), 0.002, 0.03, 0.001);
    obs_properties_add_float_slider(props, "glow_intensity", obs_module_text("GlowIntensity"), 0.5, 4.0, 0.1);

    return props;
}

static enum gs_color_space manim_video_get_color_space(void *data, size_t count,
                                                      const enum gs_color_space *preferred_spaces)
{
    struct manim_transition_info *const info = (struct manim_transition_info *)data;
    const enum gs_color_space transition_space = obs_transition_video_get_color_space(info->source);

    enum gs_color_space space = transition_space;
    for (size_t i = 0; i < count; ++i) {
        if (preferred_spaces[i] == transition_space) {
            space = transition_space;
            break;
        }
    }
    return space;
}

struct obs_source_info manim_transition = {
    .id = "manim_motion_transition",
    .type = OBS_SOURCE_TYPE_TRANSITION,
    .get_name = manim_get_name,
    .create = manim_create,
    .destroy = manim_destroy,
    .get_defaults = manim_defaults,
    .get_properties = manim_properties,
    .update = manim_update,
    .video_render = manim_video_render,
    .audio_render = manim_audio_render,
    .video_get_color_space = manim_video_get_color_space,
};
