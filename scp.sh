#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <local_file>"
    exit 1
fi

# Assign arguments to variables
local_file="$1"
remote_user="2024EDA017"
remote_host="140.113.225.204"
remote_directory="/uhome/chome/2024EDA/2024EDA017"

# Perform SCP transfer
scp "$local_file" "$remote_user@$remote_host:$remote_directory"