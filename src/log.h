#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

enum log_control {
    LOG_CONTROL_ON = 0,
    LOG_CONTROL_OFF
};

enum log_level {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR
};

/*
 * LOG_X(abcd, LOG_SAVE_FILE_ON, ....)
 * log_module_define(abcd, level, LOG_SAVE_FILE_ON)
 * 
 * 对于保存文件只要LOG_X()和log_module_define()其一设置
 * 为LOG_SAVE_FILE_ON，则文件就会保存,两者是"或"的关系.但是
 * 前提都是需要输出的log等级符合要求.
 * 比如当前log等级为WARNING，那么LOG_W()和LOG_E()会保存文件,
 * LOG_D()和LOG_I()不会保存文件.
 */
enum log_save_file {
    LOG_SAVE_FILE_OFF = 0,
    LOG_SAVE_FILE_ON,
};

typedef void (*log_out_t)(void *handle, const char *func, int line, enum log_level level, const char *message, ...);

struct log_context {
    FILE *fp;
};

struct log_control_block {
    const char *module_name;
    enum log_control control;
    enum log_level level;
    enum log_save_file golbal_save_file;
    log_out_t system;
    log_out_t file;
    void *private_data;
};

extern struct log_context g_log_ctx;
void log_system_out(void *handle, const char *func, int line, enum log_level level, const char *message, ...);
void log_file_out(void *handle, const char *func_name, int line, enum log_level level, const char *message, ...);

#define log_module_define(module, level, golbal_save_file) \
struct log_control_block log_control_block_##module = \
{ \
    #module, \
    (LOG_CONTROL_ON), \
    (level), \
    golbal_save_file, \
    log_system_out, \
    log_file_out, \
    &g_log_ctx \
}

#define LOG_D(module, is_save_file, message, ...) \
do { \
    extern struct log_control_block log_control_block_##module; \
    log_control_block_##module.system(&log_control_block_##module, \
                                            __FUNCTION__, \
                                            __LINE__, \
                                            LOG_LEVEL_DEBUG, \
                                            (message), \
                                            ##__VA_ARGS__); \
    enum log_save_file save_file = (enum log_save_file)is_save_file;\
    if (save_file || log_control_block_##module.golbal_save_file) {\
        log_control_block_##module.file(&log_control_block_##module, \
                                                __FUNCTION__, \
                                                __LINE__, \
                                                LOG_LEVEL_DEBUG, \
                                                (message), \
                                                ##__VA_ARGS__); \
    }\
} while (0)

#define LOG_I(module, is_save_file, message, ...) \
do { \
    extern struct log_control_block log_control_block_##module; \
    log_control_block_##module.system(&log_control_block_##module, \
                                            __FUNCTION__, \
                                            __LINE__, \
                                            LOG_LEVEL_INFO, \
                                            (message), \
                                            ##__VA_ARGS__); \
    enum log_save_file save_file = (enum log_save_file)is_save_file;\
    if (save_file || log_control_block_##module.golbal_save_file) {\
        log_control_block_##module.file(&log_control_block_##module, \
                                                __FUNCTION__, \
                                                __LINE__, \
                                                LOG_LEVEL_INFO, \
                                                (message), \
                                                ##__VA_ARGS__); \
    }\
} while (0)

#define LOG_W(module, is_save_file, message, ...) \
do { \
    extern struct log_control_block log_control_block_##module; \
    log_control_block_##module.system(&log_control_block_##module, \
                                            __FUNCTION__, \
                                            __LINE__, \
                                            LOG_LEVEL_WARNING, \
                                            (message), \
                                            ##__VA_ARGS__); \
    enum log_save_file save_file = (enum log_save_file)is_save_file;\
    if (save_file || log_control_block_##module.golbal_save_file) {\
        log_control_block_##module.file(&log_control_block_##module, \
                                                __FUNCTION__, \
                                                __LINE__, \
                                                LOG_LEVEL_WARNING, \
                                                (message), \
                                                ##__VA_ARGS__); \
    }\
} while (0)

#define LOG_E(module, is_save_file, message, ...) \
do { \
    extern struct log_control_block log_control_block_##module; \
    log_control_block_##module.system(&log_control_block_##module, \
                                            __FUNCTION__, \
                                            __LINE__, \
                                            LOG_LEVEL_ERROR, \
                                            (message), \
                                            ##__VA_ARGS__); \
    enum log_save_file save_file = (enum log_save_file)is_save_file;\
    if (save_file || log_control_block_##module.golbal_save_file) {\
        log_control_block_##module.file(&log_control_block_##module, \
                                                __FUNCTION__, \
                                                __LINE__, \
                                                LOG_LEVEL_ERROR, \
                                                (message), \
                                                ##__VA_ARGS__); \
    }\
} while (0)

/*
 * 配置单个模块的log输出等级限制
 * @log_level: enum log_level
 */
#define log_config_level(module, log_level) \
do { \
    extern struct log_control_block log_control_block_##module; \
    log_control_block_##module.level = (log_level); \
} while (0)

/*
 * 配置单个模块的log输出总开关,如果模块配置为LOG_CONTROL_OFF,
 * 则当前模块无任何输出.
 * @log_control: enum log_control
 */
#define log_config_control(module, log_control) \
do { \
    extern struct log_control_block log_control_block_##module; \
    log_control_block_##module.control = (log_control); \
} while (0)

/*
 * 配置单个模块是否保存文件.
 * @log_save_file: enum log_save_file
 */
#define log_config_golbal_save_file(module, log_save_file) \
do { \
    extern struct log_control_block log_control_block_##module; \
    log_control_block_##module.golbal_save_file = (log_save_file); \
} while (0)

int log_file_init(const char *work_dir);
int log_file_deinit(void);

#endif /* __LOG_H__ */