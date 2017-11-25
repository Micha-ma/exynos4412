#!/bin/bash
mount -o rw,remount yaffs2 /system
chmod 755 /system/bin/canconfig
chmod 755 /system/bin/cansend
chmod 755 /system/bin/candump
chmod 755 /system/bin/canecho
chmod 755 /system/bin/cansequence
