#!/bin/sh
##
# Takes a file containing a list of `{tone}:{duration}` as parameters and plays it on Bleep.
#
# Usage: ./partition.sh "<server url>" "<filename>"
#

SERVER_ADDRESS="$1"
PLAY_PAUSE=0

# If PLAY_PAUSE is `true` pause before pushing notes
if [ ${PLAY_PAUSE} -eq 1 ]; then
    curl "${SERVER_ADDRESS}/playing/0"
fi

echo -n "Loading notes onto server"
while read note;
do
    # Push each note to the Bleep server
    curl "${SERVER_ADDRESS}/beep/$note"
    echo -n "."
done < "$2"
echo " Done"

# If PLAY_PAUSE is `true` start playing
if [ ${PLAY_PAUSE} -eq 1 ]; then
    curl "${SERVER_ADDRESS}/playing/1"
fi
