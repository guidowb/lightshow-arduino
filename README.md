# Lightshow for Arduino

## Upcoming Features

- More patterns!
- Per client patterns (on server side)
- Power control
- Server detects inactivity and disconnects
- OTA updates



## Interesting Learnings

When the WiFi disconnects underneath the websocket library, websockets does
not notice. It will continue to happily accept ping() invocations, and will
not send a disconnect notification. Weirdly, the server does not seem to
notice either. They just won't receive any of each other's messages.

I can't get my serial over USB port to behave at anything over 9600 baud,
and that's are the default 26 MHz crystal frequency.
Clocking up to 40 or 80 (by setting -D F_CRYSTAL=80000000) seems to work
as expected, but still doesn't allow me to set the baud rate to 115200.
