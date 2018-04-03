#include "mqtt-client.h"
#include "pi-blaster-mqtt.h"


#define mqtt_host "localhost"
#define mqtt_port 1883



void handle_signal(int s);
void connect_callback(struct mosquitto *mosq, void *obj, int result);
void message_callback(struct mosquitto *mosq, void *obj,
    const struct mosquitto_message *message);

static struct mosquitto *mosq;
static int run = 1;

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
    printf("connect callback, rc=%d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj,
    const struct mosquitto_message *message)
{
    bool match = 0;

    printf("got message '%.*s' for topic '%s'\n", message->payloadlen,
        (char *)message->payload, message->topic);

    mosquitto_topic_matches_sub(MQTT_TOPIC_TEXT, message->topic, &match);
    if (match)
    {
      process_msg((char *)message->payload);
    }

    

}

void mqtt_go_go(void)
{
    char clientid[24];
    int rc = 0;

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

        mosquitto_subscribe(mosq, NULL, MQTT_TOPIC_TEXT, 0);
        mosquitto_subscribe(mosq, NULL, MQTT_TOPIC_LINE, 0);
    }

    while (run)
    {
        rc = mosquitto_loop(mosq, -1, 1);
        if (run && rc)
        {
            printf("connection error!\n");
            sleep(10);
            mosquitto_reconnect(mosq);
        }
    }
}

void mqtt_cleanup(void)
{
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}

void handle_signal(int s)
{
    run = 0;
}
