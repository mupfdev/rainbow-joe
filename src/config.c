/** @file config.c
 * @ingroup   Config
 * @defgroup  Config
 * @brief     Configuration file manager.
 * @author    Michael Fitzmayer
 * @copyright "THE BEER-WARE LICENCE" (Revision 42)
 */

#include "config.h"

/**
 * @brief   
 * @param   cfg
 * @param   section
 * @param   name
 * @param   value
 * @return  
 * @ingroup Config
 */
static int32_t handler(void* cfg, const char *section, const char *name, const char *value)
{
    Config *config = (Config*)cfg;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    int32_t val = atoi(value);

    if      (MATCH("Audio", "enabled"))    config->audio.enabled    = val;
    else if (MATCH("Video", "fullscreen")) config->video.fullscreen = val;
    else if (MATCH("Video", "height"))     config->video.height     = val;
    else if (MATCH("Video", "width"))      config->video.width      = val;
    else if (MATCH("Video", "limitFPS"))   config->video.limitFPS   = val;
    else if (MATCH("Video", "fps"))        config->video.fps        = val;
    else
        return 0;

    return 1;
}

/**
 * @brief   
 * @param   filename
 * @return  
 * @ingroup Config
 */
Config configInit(const char *filename)
{
    static Config config;

    config.video.fullscreen =   0;
    config.video.height     = 600;
    config.video.width      = 800;
    config.video.limitFPS   =   1;
    config.video.fps        =  60;

    if (0 > ini_parse(filename, handler, &config))
        fprintf(stderr, "Couldn't load configuration file: %s\n", filename);

    return config;
}
