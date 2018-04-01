#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <mosquitto.h>


struct mosquitto *mosq mqtt_setup(void);
int mqtt_cleanup(struct mosquitto *mosq);

