#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_camera.h"
#include "camera_app.h"

static const char *TAG = "http_server";

esp_err_t stream_handler(httpd_req_t *req) {
    camera_fb_t *fb = NULL;
    esp_err_t res = ESP_OK;

    res = httpd_resp_set_type(req, "multipart/x-mixed-replace;boundary=frame");
    if (res != ESP_OK) return res;

    while (true) {
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Falha ao capturar frame");
            continue;
        }

        res = httpd_resp_send_chunk(req, "--frame\r\n", strlen("--frame\r\n"));
        res |= httpd_resp_send_chunk(req, "Content-Type: image/jpeg\r\n\r\n", strlen("Content-Type: image/jpeg\r\n\r\n"));
        res |= httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
        res |= httpd_resp_send_chunk(req, "\r\n", strlen("\r\n"));

        esp_camera_fb_return(fb);

        if (res != ESP_OK) {
            break;
        }
    }

    return res;
}

httpd_handle_t start_camera_server() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t stream_uri = {
            .uri = "/stream",
            .method = HTTP_GET,
            .handler = stream_handler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &stream_uri);
    }

    ESP_LOGI(TAG, "Servidor HTTP inicializado");
    return server;
}

void app_main() {
    // Inicializar a câmera
    camera_init();

    // Iniciar servidor HTTP
    start_camera_server();
}
