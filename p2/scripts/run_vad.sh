#!/bin/bash

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh
#$alfa1 = 13;

# Write here the name and path of your program and database
DIR_P2=$HOME/Desktop/CFIS/4A/PAV/practicas/PAV/p2
DB=$DIR_P2/db.v4


CMD="$DIR_P2/src/p2 --alfa1=3.53 --alfa2=5.01"

for filewav in $DB/*/*wav; do
#    echo
    echo "**************** $filewav ****************"
    if [[ ! -f $filewav ]]; then 
	    echo "Wav file not found: $filewav" >&2
	    exit 1
    fi

    filevad=${filewav/.wav/.vad}

    $CMD -i $filewav -o $filevad || exit 1

# Alternatively, uncomment to create output wave files
#    filewavOut=${filewav/.wav/.vad.wav}
#    $CMD $filewav $filevad $filewavOut || exit 1

done

scripts/vad_evaluation.pl $DB/*/*lab

exit 0
