#include "nvs/Nvs.hpp"

#include "esp_err.h"
#include "esp_log.h"
#include "misc/Error.hpp"
#include "nvs.h"
#include <cstddef>
#include <cstdlib>
#include <expected>
#include <format>

static const char* TAG = "nvs";

std::expected<void, Error<NvsError>> Nvs::load(const NvsKey& key, NvsData& buf) const
{
    esp_err_t err;
    nvs_handle_t handle;

    err = nvs_open("storage", NVS_READONLY, &handle);
    if (err != ESP_OK) {
        return make_error(NvsError::Other, std::format("Failed to open NVS: {}", esp_err_to_name(err)));
    }

    auto buf_size = buf.size();
    err = nvs_get_blob(handle, key.c_str(), reinterpret_cast<char*>(buf.data()), &buf_size);

    if (err == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI("nvs", "not found");
        nvs_close(handle);
        return make_error(NvsError::NotFound, std::format("Not found: {}", key));
    }

    if (err != ESP_OK) {
        nvs_close(handle);
        return make_error(NvsError::Other, std::format("Failed to load {}: {}", key, esp_err_to_name(err)));
    }

    nvs_close(handle);

    return {};
}

std::expected<std::vector<NvsData>, Error<NvsError>> Nvs::loadMany(const std::vector<NvsKey>& keys) const
{
    return make_error(NvsError::Other, "not implemented");
}

std::expected<void, Error<NvsError>> Nvs::save(const NvsKey& key, NvsData data) const
{
    esp_err_t err;
    nvs_handle_t handle;

    err = nvs_open("storage", NVS_READWRITE, &handle);
    if (err != ESP_OK) {
        return make_error(NvsError::Other, std::format("Failed to open NVS: {}", esp_err_to_name(err)));
    }

    err = nvs_set_blob(handle, key.c_str(), reinterpret_cast<char*>(data.data()), data.size());
    if (err != ESP_OK) {
        nvs_close(handle);
        return make_error(NvsError::Other, std::format("Failed to save {}: {}", key, esp_err_to_name(err)));
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        nvs_close(handle);
        return make_error(NvsError::Other, std::format("Failed to commit: {}", esp_err_to_name(err)));
    }

    nvs_close(handle);

    return {};
}
