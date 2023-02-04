/* See LICENSE file for copyright and license details. */
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>

#include "arg.h"
#include "util.h"

// Macros {{{
#define MAXLEN 2048
// }}}

//  Enums, structs and unions {{{
struct arg {
	const char *(*func)();
	const char *background_color;
	const char *foreground_color;
	const char *fmt;
	const char *args;
};
// }}}

// Functions {{{
/* battery */
const char *battery_perc(const char *);
const char *battery_state(const char *);
const char *battery_remaining(const char *);
/* cpu */
const char *cpu_freq(void);
const char *cpu_perc(void);
/* datetime */
const char *datetime(const char *fmt);
/* disk */
const char *disk_free(const char *path);
const char *disk_perc(const char *path);
const char *disk_total(const char *path);
const char *disk_used(const char *path);
/* entropy */
const char *entropy(void);
/* hostname */
const char *hostname(void);
/* ip */
const char *ipv4(const char *interface);
const char *ipv6(const char *interface);
/* kernel_release */
const char *kernel_release(void);
/* keyboard_indicators */
const char *keyboard_indicators(void);
/* keymap */
const char *keymap(void);
/* load_avg */
const char *load_avg(void);
/* media */
const char *mpd_stat(void);
/* netspeeds */
const char *netspeed_rx(const char *interface);
const char *netspeed_tx(const char *interface);
/* num_files */
const char *num_files(const char *path);
/* ram */
const char *ram_free(void);
const char *ram_perc(void);
const char *ram_total(void);
const char *ram_used(void);
/* run_command */
const char *run_command(const char *cmd);
/* separator */
const char *separator(const char *separator);
/* swap */
const char *swap_free(void);
const char *swap_perc(void);
const char *swap_total(void);
const char *swap_used(void);
/* temperature */
const char *temp(const char *);
/* uptime */
const char *uptime(void);
/* user */
const char *gid(void);
const char *username(void);
const char *uid(void);
/* volume */
const char *vol_perc(const char *card);
/* wifi */
const char *wifi_perc(const char *interface);
const char *wifi_essid(const char *interface);
// }}}

// Variables {{{
const unsigned int interval = 1000;
static const char unknown_str[] = "n/a";
char buf[1024];
static volatile sig_atomic_t done;
static Display *dpy;
// }}}

// Configuration {{{
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
	{ mpd_stat,          "#268bd2", "#002b36", " %s ",          NULL},
	{ ram_perc,          "#073642", "#fdf6e2", " \uf2db %s%% ", NULL},
	{ swap_perc,         "#073642", "#fdf6e2", "(%s%%)",        NULL},
	{ cpu_perc,          "#073642", "#fdf6e2", " \uf085 %s%% ", NULL},
	{ cpu_freq,          "#073642", "#fdf6e2", "(%sHz)",        NULL},
	{ temp,              "#073642", "#fdf6e2", " \uf8c7 %s°C ", "/sys/bus/platform/devices/thinkpad_hwmon/hwmon/hwmon5/temp1_input"},
	{ ipv4,              NULL,      NULL,      " \uf502 %s ",   "wlan0"},
	{ wifi_perc,         NULL,      NULL,      "(%s%%) ",       "wlan0"},
	{ run_command,       "#dc322f", "#eee8d5", " %s ",          "sls-volume"},
	{ battery_perc,      "#dc322f", "#eee8d5", "\uf60b %s%% ",  "BAT0"},
	{ battery_state,     "#dc322f", "#eee8d5", "(%s",           "BAT0"},
	{ battery_remaining, "#dc322f", "#eee8d5", "%s) ",          "BAT0"},
	{ run_command,       "#dc322f", "#eee8d5", "%s ",           "sls-backlight"},
	{ uptime,            NULL,      NULL,      " \ufa1a %s ",   NULL},
	{ keymap,            NULL,      NULL,      " \uf80b %s ",   NULL},
	{ datetime,          "#eee8d5", "#002b36", " \uf455 %s ",   "%T %e %a %b"},
	/* function          bg         fg         format           argument*/
};
// }}}

// vim:fdm=marker
