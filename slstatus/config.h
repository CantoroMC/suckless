const unsigned int interval = 1000;
static const char unknown_str[] = "n/a";

#define MAXLEN 2048

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 * battery_state       battery charging state          battery name (BAT0)
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/")
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * ram_perc            memory usage in percent         NULL
 * run_command         custom shell command            command (echo foo)
 * separator           string to echo                  NULL
 * swap_perc           swap usage in percent           NULL
 * temp                temperature in degree celsius   sensor file
 * uptime              system uptime                   NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)

 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?) [see keyboard_indicators.c]
 * keymap              layout of current keymap        NULL
 * load_avg            load average                    NULL
 * num_files           number of files in a directory  path
 * ram_used            used memory in GB               NULL
 * ram_total           total memory size in GB         NULL
 * ram_free            free memory in GB               NULL
 * swap_free           free swap in GB                 NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * uid                 UID of current user             NULL
 * username            username of current user        NULL
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */

static const struct arg args[] = {
	/* function          bg         fg         format           argument*/
	{ mpd_stat,          "#8ec07c", "#f9f5d7", " %s ",          NULL},
	{ ram_perc,          "#2c3836", "#fbf1c7", " \uf2db %s%% ", NULL},
	{ swap_perc,         "#2c3836", "#fbf1c7", "(%s%%)",        NULL},
	{ cpu_perc,          "#2c3836", "#fbf1c7", " \uf085 %s%% ", NULL},
	{ cpu_freq,          "#2c3836", "#fbf1c7", "(%sHz) ",       NULL},
	{ temp,              "#2c3836", "#fbf1c7", " \uf8c7 %s ",   "/sys/bus/platform/devices/coretemp.0/hwmon/hwmon4/temp1_input"},
	{ temp,              "#2c3836", "#fbf1c7", "%s ",           "/sys/bus/platform/devices/coretemp.0/hwmon/hwmon4/temp2_input"},
	{ temp,              "#2c3836", "#fbf1c7", "%s ",           "/sys/bus/platform/devices/coretemp.0/hwmon/hwmon4/temp3_input"},
	{ temp,              "#2c3836", "#fbf1c7", "%s ",           "/sys/bus/platform/devices/coretemp.0/hwmon/hwmon4/temp4_input"},
	{ temp,              "#2c3836", "#fbf1c7", "%s°C ",         "/sys/bus/platform/devices/coretemp.0/hwmon/hwmon4/temp5_input"},
	{ ipv4,              NULL,      NULL,      " \uf502 %s ",   "wlan0"},
	{ wifi_perc,         NULL,      NULL,      "(%s%%) ",       "wlan0"},
	{ run_command,       "#fabd2f", "#fbf1c7", " %s ",          "sls-volume"},
	{ battery_perc,      "#fabd2f", "#fbf1c7", "\uf60b %s%% ",  "BAT0"},
	{ battery_state,     "#fabd2f", "#fbf1c7", "(%s",           "BAT0"},
	{ battery_remaining, "#fabd2f", "#fbf1c7", "%s) ",          "BAT0"},
	{ run_command,       "#fabd2f", "#fbf1c7", "%s ",           "sls-backlight"},
	{ uptime,            NULL,      NULL,      " \ufa1a %s ",   NULL},
	{ keymap,            NULL,      NULL,      " \uf80b %s ",   NULL},
	{ datetime,          "#2c3836", "#fbf1c7", " \uf455 %s ",   "%T %e %a %b"},
	/* function          bg         fg         format           argument*/
};

// vim:ft=c:nospell
