#include "esp_http_server.h"

esp_err_t stream_handler(httpd_req_t *req);
httpd_handle_t start_camera_server();
void app_main();

