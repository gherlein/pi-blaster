AUTOMAKE_OPTIONS = foreign

CC = @CC@
LIBS = @LIBS@


#CFLAGS = -g -DDEBUG -Wall -pedantic
CFLAGS = -Wall -pedantic -std=c99
EXTRA_DIST = pi-blaster-mqtt.service

sbin_PROGRAMS = pi-blaster-mqtt
pi_blaster_mqtt_SOURCES = pi-blaster-mqtt.c mailbox.c mqtt-client.c
pi_blaster_mqtt_LDADD = -lmosquitto

if !DEBIAN_PKG
if HAVE_SYSTEMD
systemdsystemunit_DATA = pi-blaster-mqtt.service

# Unless we are cross-compiling, start now.
if !CROSS_COMPILING
install-data-hook:
	systemctl enable pi-blaster-mqtt
	systemctl start pi-blaster-mqtt
	sudo systemctl daemon-reload
	sudo systemctl enable pi-blaster-mqtt.service

uninstall-hook:
	systemctl stop pi-blaster-mqtt
	systemctl disable pi-blaster-mqtt
	sudo systemctl daemon-reload
	sudo systemctl disable pi-blaster-mqtt.service
endif # !CROSS_COMPILING

else
init_ddir = $(sysconfdir)/init.d
init_d_SCRIPTS = pi-blaster-mqtt.boot

# Unless we are cross-compiling, start now.
if !CROSS_COMPILING
install-data-hook:
	update-rc.d pi-blaster-mqtt.boot defaults
	$(init_ddir)/pi-blaster-mqtt.boot start

uninstall-hook:
	update-rc.d pi-blaster-mqtt.boot remove
	killall pi-blaster-mqtt
endif # !CROSS_COMPILING

endif # !HAVE_SYSTEMD
endif # !DEBIAN_PKG
