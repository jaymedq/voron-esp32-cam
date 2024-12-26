idf_component_register(
    SRCS "app_main.c" "camera_app.c" "ota_update.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_driver_cam esp_http_server esp_https_ota rt esp_bootloader_format esp32-camera-master esp_partition
)
