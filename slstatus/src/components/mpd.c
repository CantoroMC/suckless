#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpd/client.h>

#include "../util.h"

void
format_title(char *title, const char *mpd_title, size_t n)
{
	if (strlen(mpd_title) >= n) {
		strncpy(title, mpd_title, n - 4);
		memset(title + n - 4, '.', 3);
		title[n] = '\0';
		return;
	}
	strcpy(title, mpd_title);
}

void
format_artist(char *artist, const char *mpd_arist, size_t n)
{
	if (strlen(mpd_arist) >= n) {
		strncpy(artist, mpd_arist, n - 4);
		memset(artist + n - 4, '.', 3);
		artist[n] = '\0';
		return;
	}
	strcpy(artist, mpd_arist);
}

const char *
mpd_stat()
{
	struct mpd_connection *conn;
	struct mpd_status *status;
	struct mpd_song *song;
	char *mpc_title,  title[25];
	char *mpc_artist, artist[20];
	unsigned state;
	int elapsed, total;

	if (!((conn = mpd_connection_new(NULL, 0, 0)) || mpd_connection_get_error(conn)))
		return NULL;

	mpd_command_list_begin(conn, true);
	mpd_send_status(conn);
	mpd_send_current_song(conn);
	mpd_command_list_end(conn);
	status = mpd_recv_status(conn);
	if (status) {
		state = mpd_status_get_state(status);
		if (state == MPD_STATE_PLAY || state == MPD_STATE_PAUSE) {
			mpd_response_next(conn);
			song = mpd_recv_song(conn);

			elapsed    = mpd_status_get_elapsed_time(status);
			total      = mpd_status_get_total_time(status);
			mpc_artist = (char *)mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
			mpc_title  = (char *)mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
			format_artist(artist, mpc_artist, sizeof(artist));
			format_title(title, mpc_title, sizeof(title));

			if (state == MPD_STATE_PLAY) {
				snprintf(buf, sizeof(buf), "\uf04b %s - %s (%.2d:%.2d/%.2d:%.2d)",
							artist, title, elapsed / 60, elapsed %60, total / 60, total %60);
			} else if (state == MPD_STATE_PAUSE) {
				snprintf(buf, sizeof(buf), "\uf04c %s - %s (%.2d:%.2d/%.2d:%.2d)",
							artist, title, elapsed / 60, elapsed %60, total / 60, total %60);
			}
			free(mpc_title);
			free(mpc_artist);
		} else if (state == MPD_STATE_STOP) {
			mpd_response_next(conn);
			if ((song = mpd_recv_song(conn))) {
				mpc_artist = (char *)mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
				mpc_title  = (char *)mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
				format_artist(artist, mpc_artist, sizeof(artist));
				format_title(title, mpc_title, sizeof(title));
				snprintf(buf, sizeof(buf), "\uf04d %s - %s ", artist, title);
				free(mpc_title);
				free(mpc_artist);
			} else {
				snprintf(buf, sizeof(buf), "\uf886 MPD ");
			}
		} else {
			/* state == MPD_STATE_UNKNOWN */
			snprintf(buf, sizeof(buf), "\uf886 MPD ");
		}
	} else {
		return NULL;
	}
	mpd_response_finish(conn);
	mpd_connection_free(conn);

	return buf;
}
