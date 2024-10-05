#!/bin/bash

patch --directory=$2 --unified --strip=1 --posix --force --set-utc --verbose < $1
