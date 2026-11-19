#include <obs-module.h>
#include "transition/manim-transition.h"
#include "version.h"

OBS_DECLARE_MODULE()
OBS_MODULE_AUTHOR("VincentZyu233")
OBS_MODULE_USE_DEFAULT_LOCALE("obs-manim-motion", "en-US")

MODULE_EXPORT const char *obs_module_name(void)
{
    return obs_module_text("ManimMotionTransition");
}

MODULE_EXPORT const char *obs_module_description(void)
{
    return "Manim-inspired kinetic transitions and animation effects for OBS Studio";
}

bool obs_module_load(void)
{
    blog(LOG_INFO, "[obs-manim-motion] version %s loaded successfully", PROJECT_VERSION);
    obs_register_source(&manim_transition);
    return true;
}

void obs_module_unload(void)
{
    blog(LOG_INFO, "[obs-manim-motion] unloaded");
}
