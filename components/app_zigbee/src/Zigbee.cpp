#include "zigbee/Zigbee.hpp"
#include <cstdint>
#include <cstdio>
#include <expected>
#include <string>
#include "esp_check.h"
#include "esp_err.h"

extern "C" {

#include "zboss_api.h"
#include "esp_zigbee_attribute.h"
#include "esp_zigbee_cluster.h"
#include "esp_zigbee_core.h"
#include "esp_zigbee_type.h"
#include "ha/esp_zigbee_ha_standard.h"
#include "nwk/esp_zigbee_nwk.h"
#include "platform/esp_zigbee_platform.h"
#include "zcl/esp_zigbee_zcl_basic.h"
#include "zcl/esp_zigbee_zcl_common.h"
#include "zcl/esp_zigbee_zcl_core.h"
#include "zcl/esp_zigbee_zcl_electrical_meas.h"
#include "zcl/zb_zcl_common.h"

}

static const char *TAG = "zigbee";

RcService* rc_service;

static void bdb_start_top_level_commissioning_cb(uint8_t mode_mask)
{
    ESP_RETURN_ON_FALSE(esp_zb_bdb_start_top_level_commissioning(mode_mask) == ESP_OK, , TAG, "Failed to start Zigbee bdb commissioning");
}

static void simple_desc_cb(esp_zb_zdp_status_t zdo_status, esp_zb_af_simple_desc_1_1_t *simple_desc, void *user_ctx)
{
    uint16_t short_addr = (uint16_t)(uintptr_t)user_ctx;
    if (zdo_status != ESP_ZB_ZDP_STATUS_SUCCESS || simple_desc == NULL) {
        ESP_LOGW(TAG, "simple desc failed for 0x%04hx: %d", short_addr, zdo_status);
        return;
    }
    ESP_LOGI(TAG, "Simple descriptor for short 0x%04hx ep %d profile 0x%04hx devId 0x%04hx",
             short_addr, simple_desc->endpoint, simple_desc->app_profile_id, simple_desc->app_device_id);
    ESP_LOGI(TAG, "  input cluster count: %d", simple_desc->app_input_cluster_count);
    for (int i = 0; i < simple_desc->app_input_cluster_count + simple_desc->app_output_cluster_count; ++i) {
        ESP_LOGI(TAG, "    cluster[%d]=0x%04hx", i, simple_desc->app_cluster_list[i]);
    }
}

static void bind_cb(esp_zb_zdp_status_t zdo_status, void *user_ctx)
{
    if (zdo_status == ESP_ZB_ZDP_STATUS_SUCCESS) {
        ESP_LOGI(TAG, "Bind succeeded");
    } else {
        ESP_LOGW(TAG, "Bind failed: %d", zdo_status);
    }
    if (user_ctx) {
        free(user_ctx);
    }
}

typedef struct {
    esp_zb_ieee_addr_t ieee_addr;
    uint8_t endpoint;
    uint16_t short_addr;
} device_info_t;

static void send_bind(uint16_t addr, uint8_t endpoint)
{
    device_info_t *dev = (device_info_t *)malloc(sizeof(device_info_t));
    if (!dev) {
        ESP_LOGE(TAG, "OOM for device_info");
        return;
    }
    dev->endpoint = endpoint;
    dev->short_addr = addr;
    esp_zb_ieee_address_by_short(dev->short_addr, dev->ieee_addr);

    esp_zb_zdo_bind_req_param_t bind_req;
    memset(&bind_req, 0, sizeof(bind_req));

    memcpy(bind_req.src_address, dev->ieee_addr, sizeof(esp_zb_ieee_addr_t));
    bind_req.src_endp = endpoint;
    bind_req.cluster_id = ESP_ZB_ZCL_CLUSTER_ID_ON_OFF;

    bind_req.dst_addr_mode = ESP_ZB_ZDO_BIND_DST_ADDR_MODE_64_BIT_EXTENDED;
    esp_zb_get_long_address(bind_req.dst_address_u.addr_long);
    bind_req.dst_endp = ENDPOINT;

    bind_req.req_dst_addr = dev->short_addr;

    ESP_LOGI(TAG, "Sending bind request: short=0x%04hx ep=%d", dev->short_addr, dev->endpoint);
    esp_zb_zdo_device_bind_req(&bind_req, bind_cb, (void *)dev);
}

static void active_ep_cb(esp_zb_zdp_status_t zdo_status, uint8_t ep_count, uint8_t *ep_id_list, void *user_ctx)
{
    uint16_t short_addr = (uint16_t)(uintptr_t)user_ctx;
    if (zdo_status != ESP_ZB_ZDP_STATUS_SUCCESS) {
        ESP_LOGW(TAG, "active ep req failed for 0x%04hx: %d", short_addr, zdo_status);
        return;
    }
    ESP_LOGI(TAG, "Active endpoint response: short=0x%04hx ep_count=%d", short_addr, ep_count);

    for (int i = 0; i < ep_count; ++i) {
        uint8_t ep = ep_id_list[i];
        ESP_LOGI(TAG, "  endpoint: %d -> requesting simple descriptor", ep);
        esp_zb_zdo_simple_desc_req_param_t simple_req = {0};
        simple_req.addr_of_interest = short_addr;
        simple_req.endpoint = ep;
        esp_zb_zdo_simple_desc_req(&simple_req, simple_desc_cb, (void*)(uintptr_t)short_addr);
        send_bind(short_addr, ep);
    }
}

void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal_struct)
{
    uint32_t *p_sg_p       = signal_struct->p_app_signal;
    esp_err_t err_status = signal_struct->esp_err_status;
    esp_zb_app_signal_type_t sig_type = (esp_zb_app_signal_type_t)*p_sg_p;
    esp_zb_zdo_signal_device_annce_params_t *dev_annce_params = NULL;
    esp_zb_zdo_signal_leave_indication_params_t *leave_ind_params = NULL;

    switch (sig_type) {
    case ESP_ZB_ZDO_SIGNAL_SKIP_STARTUP:
        ESP_LOGI(TAG, "Initialize Zigbee stack");
        esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_INITIALIZATION);
         break;
    case ESP_ZB_BDB_SIGNAL_DEVICE_FIRST_START:
    case ESP_ZB_BDB_SIGNAL_DEVICE_REBOOT:
        if (err_status == ESP_OK) {
            ESP_LOGI(TAG, "Device started up in%s factory-reset mode", esp_zb_bdb_is_factory_new() ? "" : " non");
            if (esp_zb_bdb_is_factory_new()) {
                ESP_LOGI(TAG, "Start network formation");
                esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_FORMATION);
            } else {
                esp_zb_bdb_open_network(180);
                ESP_LOGI(TAG, "Device rebooted");
            }
        } else {
            ESP_LOGW(TAG, "%s failed with status: %s, retrying", esp_zb_zdo_signal_to_string(sig_type),
                     esp_err_to_name(err_status));
            esp_zb_scheduler_alarm((esp_zb_callback_t)bdb_start_top_level_commissioning_cb,
                                   ESP_ZB_BDB_MODE_INITIALIZATION, 1000);
        }
        break;
    case ESP_ZB_BDB_SIGNAL_FORMATION:
        if (err_status == ESP_OK) {
            esp_zb_ieee_addr_t extended_pan_id;
            esp_zb_get_extended_pan_id(extended_pan_id);
            ESP_LOGI(TAG, "Formed network successfully (Extended PAN ID: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x, PAN ID: 0x%04hx, Channel:%d, Short Address: 0x%04hx)",
                     extended_pan_id[7], extended_pan_id[6], extended_pan_id[5], extended_pan_id[4],
                     extended_pan_id[3], extended_pan_id[2], extended_pan_id[1], extended_pan_id[0],
                     esp_zb_get_pan_id(), esp_zb_get_current_channel(), esp_zb_get_short_address());
            esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_STEERING);
        } else {
            ESP_LOGI(TAG, "Restart network formation (status: %s)", esp_err_to_name(err_status));
            esp_zb_scheduler_alarm((esp_zb_callback_t)bdb_start_top_level_commissioning_cb, ESP_ZB_BDB_MODE_NETWORK_FORMATION, 1000);
        }
        break;
    case ESP_ZB_BDB_SIGNAL_STEERING:
        if (err_status == ESP_OK) {
            ESP_LOGI(TAG, "Network steering started");
        }
        break;
    case ESP_ZB_ZDO_SIGNAL_DEVICE_ANNCE:
        dev_annce_params = (esp_zb_zdo_signal_device_annce_params_t *)esp_zb_app_signal_get_params(p_sg_p);
        ESP_LOGI(TAG, "New device commissioned or rejoined (short: 0x%04hx)", dev_annce_params->device_short_addr);

        {
            esp_zb_zdo_active_ep_req_param_t active_req = {
                .addr_of_interest = dev_annce_params->device_short_addr,
            };
            esp_zb_zdo_active_ep_req(&active_req, active_ep_cb, (void*)(uintptr_t)dev_annce_params->device_short_addr);
        }
        break;
    case ESP_ZB_ZDO_SIGNAL_LEAVE_INDICATION:
        leave_ind_params = (esp_zb_zdo_signal_leave_indication_params_t *)esp_zb_app_signal_get_params(p_sg_p);
        if (!leave_ind_params->rejoin) {
            esp_zb_ieee_addr_t leave_addr;
            memcpy(leave_addr, leave_ind_params->device_addr, sizeof(esp_zb_ieee_addr_t));
            ESP_LOGI(TAG, "Zigbee Node is leaving network: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x)",
                     leave_addr[7], leave_addr[6], leave_addr[5], leave_addr[4],
                     leave_addr[3], leave_addr[2], leave_addr[1], leave_addr[0]);
        }
        break;
    case ESP_ZB_NWK_SIGNAL_PERMIT_JOIN_STATUS:
        if (err_status == ESP_OK) {
            if (*(uint8_t *)esp_zb_app_signal_get_params(p_sg_p)) {
                ESP_LOGI(TAG, "Network(0x%04hx) is open for %d seconds", esp_zb_get_pan_id(), *(uint8_t *)esp_zb_app_signal_get_params(p_sg_p));
            } else {
                ESP_LOGW(TAG, "Network(0x%04hx) closed, devices joining not allowed.", esp_zb_get_pan_id());
            }
        }
        break;
    default:
        ESP_LOGI(TAG, "ZDO signal: %s (0x%x), status: %s", esp_zb_zdo_signal_to_string(sig_type), sig_type,
                 esp_err_to_name(err_status));
        break;
    }
}


static uint64_t uint64_by_ieee_address(esp_zb_ieee_addr_t ieee_addr)
{
    uint64_t v;
    memcpy(&v, ieee_addr, sizeof(esp_zb_ieee_addr_t));
    return v;
}


static bool my_raw_handler(uint8_t bufid)
{
    // Пример доступа к info (ZBOSS макрос / структура)
    auto cmd_info = ZB_BUF_GET_PARAM(bufid, zb_zcl_parsed_hdr_t);

    if (!cmd_info) {
        return false;
    }

    ESP_LOGI(TAG, "Handle command SRC:%05X AddrType:%02X Cluster:0x%04X Cmd:0x%02X Seq:%u",
            (unsigned)cmd_info->addr_data.common_data.source.u.short_addr,
            (unsigned)cmd_info->addr_data.common_data.source.addr_type,
            (unsigned)cmd_info->cluster_id,
            (unsigned)cmd_info->cmd_id,
            (unsigned)cmd_info->seq_number);

    if (cmd_info->cluster_id == ZB_ZCL_CLUSTER_ID_ON_OFF) {
        esp_zb_ieee_addr_t ieee_addr;
        esp_zb_ieee_address_by_short(cmd_info->addr_data.common_data.source.u.short_addr, ieee_addr);
        if (cmd_info->cmd_id == ZB_ZCL_CMD_ON_OFF_TOGGLE_ID) {
            rc_service->toggleRc(uint64_by_ieee_address(ieee_addr));

            ESP_LOGI(TAG, "Handle command SRC:%05X AddrType:%02X Cluster:0x%04X Cmd:0x%02X Seq:%u",
            (unsigned)cmd_info->addr_data.common_data.source.u.short_addr,
            (unsigned)cmd_info->addr_data.common_data.source.addr_type,
            (unsigned)cmd_info->cluster_id,
            (unsigned)cmd_info->cmd_id,
            (unsigned)cmd_info->seq_number);

            zb_buf_free(bufid);

            return true;
        }
    }

    ESP_LOGI(TAG, "Handle command SRC:%05X AddrType:%02X Cluster:0x%04X Cmd:0x%02X Seq:%u",
            (unsigned)cmd_info->addr_data.common_data.source.u.short_addr,
            (unsigned)cmd_info->addr_data.common_data.source.addr_type,
            (unsigned)cmd_info->cluster_id,
            (unsigned)cmd_info->cmd_id,
            (unsigned)cmd_info->seq_number);

    return false;
}

static void create_custom_endpoint()
{
    auto cluster_list = esp_zb_zcl_cluster_list_create();


    /* basic cluster create with fully customized */

    esp_zb_basic_cluster_cfg_t basic_cfg = {
        .zcl_version = ESP_ZB_ZCL_BASIC_ZCL_VERSION_DEFAULT_VALUE,
        .power_source = ESP_ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE,
    };
    auto basic_cluster = esp_zb_basic_cluster_create(&basic_cfg);
    esp_zb_cluster_list_add_basic_cluster(cluster_list, basic_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);

    /* identify cluster create with fully customized */
    esp_zb_identify_cluster_cfg_t identify_cfg;
    esp_zb_attribute_list_t *identify_cluster = esp_zb_identify_cluster_create(&identify_cfg);
    esp_zb_cluster_list_add_identify_cluster(cluster_list, identify_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    /* on-off cluster create with customized */
    esp_zb_on_off_cluster_cfg_t on_off_cfg;
    esp_zb_attribute_list_t *on_off_cluster = esp_zb_on_off_cluster_create(&on_off_cfg);
    esp_zb_cluster_list_add_on_off_cluster(cluster_list, on_off_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);

    auto endpoint_list = esp_zb_ep_list_create();

    esp_zb_endpoint_config_t endpoint_cfg = {
        .endpoint = ENDPOINT,
        .app_profile_id = ESP_ZB_AF_HA_PROFILE_ID,
        .app_device_id = ESP_ZB_HA_CUSTOM_ATTR_DEVICE_ID,
        .app_device_version = 0,
    };

    esp_zb_ep_list_add_ep(endpoint_list, cluster_list, endpoint_cfg);

    esp_zb_device_register(endpoint_list);
}

Zigbee::Zigbee(RcService* rc_service)
: rc_service_(rc_service)
{}

void Zigbee::zigbeeTask()
{
    esp_zb_cfg_t zb_nwk_config = {
        .esp_zb_role = ESP_ZB_DEVICE_TYPE_COORDINATOR,
        .install_code_policy = false,
        .nwk_cfg = {
            .zczr_cfg = {
                .max_children = 10,
            }
        },
    };

    esp_zb_init(&zb_nwk_config);

    create_custom_endpoint();

    esp_zb_raw_command_handler_register(my_raw_handler);

    esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);
    ESP_ERROR_CHECK(esp_zb_start(false));
    esp_zb_stack_main_loop();
}


std::expected<void, std::string> Zigbee::start()
{
    rc_service = rc_service_;
    esp_zb_platform_config_t config = {
        .radio_config = {
            .radio_mode = ZB_RADIO_MODE_NATIVE,
        },
        .host_config = {
            .host_connection_mode = ZB_HOST_CONNECTION_MODE_NONE,
        },
    };

    esp_zb_platform_config(&config);

    zigbee_thread_ = std::thread(&Zigbee::zigbeeTask, this);

    is_up_ = true;

    return {};
};

std::expected<void, std::string> Zigbee::stop()
{
    is_up_ = false;

    return {};
}

bool Zigbee::isUp() const
{
    return is_up_;
}
