#!/bin/bash
# 04-b-5000
# Намерете колко физическа памет заемат всички процеси на потребителската група students.

ps -e -g students -o drs | awk '{m+=$1}END{print m}'
