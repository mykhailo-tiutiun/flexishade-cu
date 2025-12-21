#include "io/config/Nvs.hpp"

#include "esp_log.h"
#include "nvs.h"
#include <cstdlib>
#include <expected>

static const char* TAG = "nvs";

std::expected<NvsData, NvsError> Nvs::load(const NvsKeys& keys) const
{
    esp_err_t err;
    nvs_handle_t handle;

    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS: %s", esp_err_to_name(err));
        return std::unexpected(NvsError::Other);
    }

    NvsData out;
    out.reserve(keys.size());

    std::vector<char> buf(1000);

    for (const auto& key : keys) {
        size_t required_size = buf.size();
        err = nvs_get_str(handle, key.c_str(), buf.data(), &required_size);
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGW(TAG, "Key not found: %s", key.c_str());
            nvs_close(handle);
            return std::unexpected(NvsError::NotFound);
        }

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to read key %s: %s", key.c_str(), esp_err_to_name(err));
            nvs_close(handle);
            return std::unexpected(NvsError::Other);
        }

        out.push_back({ key, std::string(buf.data(), required_size - 1) });
    }

    nvs_close(handle);

    return out;
}

std::expected<void, NvsError> Nvs::save(const NvsData& data) const
{
    return {};

}
