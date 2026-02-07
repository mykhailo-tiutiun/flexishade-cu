#ifndef CONFIG_ERROR_HPP
#define CONFIG_ERROR_HPP

enum class ConfigError {
    NvsOpenFailed,
    NvsConfigNotFound,
    NvsCommitFailed,
    NvsSaveFailed,
    NvsLoadFailed,
    ConfigNotFound,
    InvalidJson,
    Other,
};

#endif
