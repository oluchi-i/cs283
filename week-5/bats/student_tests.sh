#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
    
}

@test "single command execution" {
    run ./dsh <<< "ls"
    [ "$status" -eq 0 ]
}

@test "piped command execution" {
    run ./dsh <<< "echo hello | grep hello"
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" == "hello" ]]
}

@test "multiple pipes" {
    run ./dsh <<< "echo hello | grep hello | wc -l"
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" == "1" ]]
}

#  MY OWN TESTS INCLUDED
@test "Output redirection with overwrite" {
    run ./dsh <<< 'echo "Hello, Overwrite!" > test_output.txt'
    [ "$status" -eq 0 ]
    run cat test_output.txt
    [ "$output" = "Hello, Overwrite!" ]
}

@test "Output redirection with append" {
    run ./dsh <<< 'echo "Hello, Append!" >> test_output.txt'
    [ "$status" -eq 0 ]
    run cat test_output.txt
    [[ "$output" == *"Hello, Append!"* ]]
}

@test "ls with grep '.c' extension" {
    run ./dsh <<< "ls | grep '.c'"
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" == "dsh_cli.c" ]]  # Assuming the file dsh_cli.c exists
    [[ "${lines[1]}" == "dshlib.c" ]]  # Assuming the file dshlib.c exists
}

@test "Check exit command" {
    run ./dsh <<< "exit"
    [ "$status" -eq 0 ]
    # Checking that output contains "exiting..." to confirm shell exits
    [[ "$output" == *"exiting..."* ]]
}



