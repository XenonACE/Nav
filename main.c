#include <gtk/gtk.h>
#include <libgen.h> // dirname()
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

GtkWidget *wifi_image;
GtkWidget *bluetooth_image;

GtkBuilder* builderfromfile(char* path) {
	char ui_file_path[256];
	if (readlink("/proc/self/exe", ui_file_path, sizeof(ui_file_path)) == -1) {
		printf("Couldn't get location of binary\n");
		exit(1);
	}

	dirname(ui_file_path);
	strcat(ui_file_path, "/");
	strcat(ui_file_path, path);
	return gtk_builder_new_from_file(ui_file_path);
}

char timebuf[80];
const char* datetime(const char *fmt) {
	time_t t;
	t = time(NULL);
	if (!strftime(timebuf, sizeof(timebuf), fmt, localtime(&t))) {
		g_print("strftime: Result string exceeds buffer size\n");
		return NULL;
	}

	return timebuf;
}

static void wifi_toggle(__attribute__((unused)) GtkToggleButton *toggle) {
	if (gtk_toggle_button_get_active(toggle)) {
		gtk_image_set_from_file(wifi_image, "wifi.png");
		// system("sudo ifconfig wlan0 down");
	} else {
		gtk_image_set_from_file(wifi_image, "wifi-disabled.png");
		// system("sudo ifconfig wlan0 up");
	}
}

static void bluetooth_toggle(__attribute__((unused)) GtkToggleButton *toggle) {
	if (gtk_toggle_button_get_active(toggle)) {
		gtk_image_set_from_file(bluetooth_image, "bluetooth.png");
		// system("sudo systemctl stop bluetooth.service");
	} else {
		gtk_image_set_from_file(bluetooth_image, "bluetooth-disabled.png");
		// system("sudo systemctl start bluetooth.service");
	}
}

int main(int argc, char **argv) {
	gtk_init(&argc, &argv);

	GtkWidget *window;

	GtkWidget *wifiButton;
	GtkWidget *bluetoothButton;

	GtkWidget *datetime_label;

	GtkBuilder *builder = builderfromfile("layout");

	// Get objects
	window = GTK_WIDGET(gtk_builder_get_object(builder, "mainWindow"));

	wifiButton = GTK_WIDGET(gtk_builder_get_object(builder, "wifi-btn"));
	bluetoothButton = GTK_WIDGET(gtk_builder_get_object(builder, "bluetooth-btn"));
	wifi_image = GTK_WIDGET(gtk_builder_get_object(builder, "wifi-image"));
	bluetooth_image = GTK_WIDGET(gtk_builder_get_object(builder, "bluetooth-image"));
	datetime_label = GTK_WIDGET(gtk_builder_get_object(builder, "datetime"));

	// Signals
	
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	g_signal_connect(wifiButton, "toggled", G_CALLBACK(wifi_toggle), NULL);
	g_signal_connect(bluetoothButton, "toggled", G_CALLBACK(bluetooth_toggle), NULL);

	// General
	
	gtk_label_set_text(datetime_label, datetime("%I:%M %p"));


	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
