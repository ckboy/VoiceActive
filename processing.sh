#!/bin/bash

gpio export 17 out



tts "$*"

if [ "$*" == "Hallo" ]; then
  echo "Hi"

elif [ "$*" == "Licht an" ]; then
  gpio -g write 17 1

elif [ "$*" == "Licht aus" ]; then
  gpio -g write 17 0

elif [ "$*" == "Musik 1" ]; then
  mpg123 "/root/HellnBack.mp3"

elif [ "$*" == "Musik 2" ]; then
  mpg123 "/root/Poison.mp3"

elif [ "$*" == "Musik 3" ]; then
  mpg123 "/root/FiveHours.mp3"

elif [[ "$*" == *Colin* ]] || [[ "$*" == *colin* ]]
then
  echo "Er ist der Boss";

else
  tts "Befehl nicht erkannt"
fi
