#!/bin/bash

# Persistent rootfs based.
mkimage -c none -A arm -T script -d boot_type1.script boot_type1.scr

# Ram/Fitimage based.
mkimage -c none -A arm -T script -d boot_type2.script boot_type2.scr
