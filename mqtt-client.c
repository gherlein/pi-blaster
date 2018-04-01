#include "mqtt-client.h"

#define mqtt_host "localhost"
#define mqtt_port 1883

static int run = 1;

FILE *fp;

void handle_signal(int s);
void connect_callback(struct mosquitto *mosq, void *obj, int result);


/* calling modeule needs to declare a mosq variable */
//struct mosquitto *mosq;


mqtt_setup(void)
{
    char clientid[24];
    struct mosquitto *mosq;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    mosquitto_lib_init();

    memset(clientid, 0, 24);
    snprintf(clientid, 23, "pi_blaster_mqtt_log_%d", getpid());
    mosq = mosquitto_new(clientid, true, 0);

    if (mosq)
    {
        mosquitto_connect_callback_set(mosq, connect_callback);
        mosquitto_message_callback_set(mosq, message_callback);

        rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 60);

        mosquitto_subscribe(mosq, NULL, "pi-blaster-mqtt", 0);
    }

    return mosq;
}

int mqtt_cleanup(struct mosquitto *mosq)
{
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void handle_signal(int s)
{
    run = 0;
}

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("connect callback, rc=%d\n", result);
}
