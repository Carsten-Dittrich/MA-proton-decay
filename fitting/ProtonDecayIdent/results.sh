#!/bin/bash

# Create or empty results file
> results.txt

# file for saving output
output_file="results.txt"

# Loop over files with same name but different number
for file in muon_*.txt; do
    # Pass filename to results.txt
    echo "Datei: $file" >> "$output_file"

    # Extracting the important stuff
    proton_decay_events=$(grep "proton decay events =" "$file" | awk '{print $NF}')
    background_events=$(grep "background events =" "$file" | awk '{print $NF}')
    unidentified_events=$(grep "unidentified events =" "$file" | awk '{print $NF}')

    # Save it in the ouput file
    echo "Proton Decay Events = $proton_decay_events" >> "$output_file"
    echo "Background Events = $background_events" >> "$output_file"
    echo "Unidentified Events = $unidentified_events" >> "$output_file"

    echo >> "$output_file"
done
