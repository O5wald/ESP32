#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_mac.h"
#include "esp_eth.h"
#include "esp_netif.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_event.h"
#include "esp_system.h"

#include "lwip/inet.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/ip_addr.h"
#include<sys/socket.h>

#include "nvs_flash.h"
#include "ping/ping_sock.h"
#include "driver/gpio.h"
#define SSID "AK"
#define PASS "12345678"



// -------------------- WIFI connection setup and shit ------------------------------------------
static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi connecting WIFI_EVENT_STA_START ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi connected WIFI_EVENT_STA_CONNECTED ... \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection WIFI_EVENT_STA_DISCONNECTED ... \n");
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi got IP ... \n\n");
        break;
    default:
        break;
    }
}

void wifi_connection()
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = SSID,
            .password = PASS}};
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    esp_wifi_connect();
    printf("-------Wifi connected Successfully!------------\n\n");

}

// -------------------------------------- END WIFI setup shit -------------------------------------------------


void ping(){
        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in addr;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(3333);
        addr.sin_family = AF_INET;


       if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr))<0){
               printf("-----Something wrong in binding!--------\n");
       }else{
               if(listen(sockfd,2)<0){
                       printf("--------error in listen------\n");
               }else{
                       int acceptfd = accept(sockfd,0,0);
                       if(acceptfd<0){
                               printf("------Error on Accept------\n");
                       }else{
                               printf("--------Someone Connected!-------\n");
                       }
                        char msg[1024] = "Hello server";
                        send(acceptfd, msg, strlen(msg),0); 
                        char buffer[1024];
                        int len;
                        do{
                                len = recv(acceptfd, buffer, sizeof(buffer),0);
                                if(len < 0){
                                        printf("---Something Went wrong!----\n");
                                        break;
                                }else if(len==0){
                                        printf("Client closed connection!\n");
                                        break;
                                }else{
                                        printf("Client: %s\n",buffer);
                                        char got[1024] = "Server got the data";
                                        send(acceptfd, got, strlen(got),0);
                                }

                        }while(len>0);

               }
       } 
        

}

void app_main(void)
{

        wifi_connection();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        printf("Executing Ping fucntion\n");
        ping(); 
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        esp_wifi_disconnect();
        printf("\n\n----------------Wifi Disconnected!---------------\n\n");

}
