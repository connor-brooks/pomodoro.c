# pomodoro.c
*A small pomodoro timer written in C*

A very simple pomodoro timer which requires no dependencies. Must be ran as root in order to get sound, as the internal PC speaker is used for the alarm.

## Requirements 
* gcc
* Linux

## How to use 
Compile:

`gcc pomodoro.c -o pomodoro`

If you require sound:

`sudo chown root:root ./pomodoro`

`sudo chmod 4755. /pomodoro`

If the `pcspkr` kernel module isn't already loaded:

`sudo modprobe pcspkr`

Run:

`./pomodoro`

