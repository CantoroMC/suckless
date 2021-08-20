st - simple terminal
--------------------
st is a simple terminal emulator for X which sucks less.


Requirements
------------
In order to build st you need the Xlib header files.


Installation
------------
Edit config.mk to match your local setup (st is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install st (if
necessary as root):

    make clean install


Running st
----------
If you did not install st with make clean install, you must compile
the st terminfo entry with the following command:

    tic -sx st.info

See the man page for additional details.

Credits
-------
Based on Aurélien APTEL <aurelien dot aptel at gmail dot com> bt source code.

Patches
-------

- **alpha**: for background transparency
- **anysize**: allows st to resize to any pixel size, makes the inner border size dynamic, and centers the content of the terminal so that the left/right and top/bottom borders are balanced.
- **vertcenter**: vertically center lines in the space available if you have set a larger `chscale` in config.h.
- **boxdraw**: custom rendering of lines/blocks/braille characters for gapless alignment.
- **nnn**: tweak used to better display file icons with O_NERD.
- **ligature**: adds proper drawing of ligatures.
- **scrollback**: scroll back through terminal output using Shift+{PageUp, PageDown}.
- **scrollback-mouse**: scroll back through terminal output using Shift+MouseWheel.
- **scrollback-mouse-altscreen**: allow scrollback using mouse wheel only when not in MODE_ALTSCREEN.
- **desktopentry**: creates a desktop-entry for st.
- **workingdir**: add a switch(-d) to provide initial working directory.
- **copyurl**: select and copy the last displayed URL with CTRL+SHIFT+L.
- **hidecursor**: hide the X cursor when typing.
- **bold-is-not-bright**: bold is not rendered with bright colours.
- **iso14755**: dmenu popup to enter unicode codepoint with CTRL+SHIFT+U.
- **newterm**: allow to spawn a new st instance with the same cwd as the original st instance using CTRL+SHIFT+Return.
- **externalpipe**: allow to execute shell binary with keyboard shortcuts in st.
- **xresources**: add the ability to configure st via Xresources.
- **keyboardselect**: allows you to select and copy text to primary buffer with keyboard shortcuts like the perl extension keyboard-select for urxvt.
