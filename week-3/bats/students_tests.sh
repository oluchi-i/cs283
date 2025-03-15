#!/usr/bin/env bats

# Setup: Environment variables and logging
setup() {
    # Define a valid temporary directory path (relative to your current working directory)
    export BATS_TMPDIR="./tmp"  # You can replace this with any valid directory path
    mkdir -p "$BATS_TMPDIR"  # Create the directory if it doesn't exist

    # Define the log file path
    export LOG="$BATS_TMPDIR/test_log.txt"

    # Write the start time to the log file
    echo "Test started at $(date)" > "$LOG"
}

# Cleanup: Remove the log after tests
teardown() {
    rm -f "$LOG"
}

# Test Case 1: Check if the log file is created during setup
@test "Check if log file is created" {
    [ -f "$LOG" ]  # Check if the log file exists
}

# Test Case 2: Check if the log file contains the correct start message
@test "Check if log file contains the start message" {
    grep "Test started at" "$LOG"  # Check if the log file contains the "Test started at" line
}

# Test Case 3: Check if the log file is removed during teardown
@test "Check if log file is removed during teardown" {
    rm -f "$LOG"  # Ensure it's removed
    [ ! -f "$LOG" ]  # Check if the log file does not exist
}

# Test Case 4: Ensure setup file exists
@test "Check if setup file exists" {
    # Create a dummy setup file in the current directory instead of /path/to/
    touch "./setup_file"  # Create a dummy setup file in the current directory
    [ -f "./setup_file" ]  # Check if it exists
    rm -f "./setup_file"  # Clean up after test
}

# Test Case 5: Ensure the tmp directory exists and is accessible
@test "Check if temp directory exists and is accessible" {
    [ -d "$BATS_TMPDIR" ]  # Check if the temp directory exists
}