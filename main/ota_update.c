#include "esp_https_ota.h"
#include "esp_log.h"

static const char *TAG = "ota_update";

void perform_ota_update(const char *ota_url) {
    esp_http_client_config_t http_config = {
        .url = ota_url,
        .cert_pem = NULL,
    };
    esp_https_ota_config_t config = {
        .http_config = &http_config,
    };

    ESP_LOGI(TAG, "Iniciando OTA");
    esp_err_t ret = esp_https_ota(&config);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA bem-sucedido, reiniciando...");
        esp_restart();
    } else {
        ESP_LOGE(TAG, "OTA falhou!");
    }
}
