#!/bin/bash
mount -o rw,remount yaffs2 /system
chmod 755 /system/bin/tinymix
chmod 755 /system/bin/buzzer
tinymix 6 127
tinymix 7 1
tinymix 39 1
tinymix 44 1

