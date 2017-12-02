#!/bin/bash

### THESE HAVE TO BE FILLED IN CORRECTLY ###

compiler="../../build/run"; # Full path to compiler executable
interpret="./ic17int"; # Full path to interpret executable


### DIRS ###

testDir="./tests";
outputCompilerDir="./out-compiler";
outputInterpretDir="./out-interpret";


### FUNCTIONS ###

function header {
    line="----------";
    let chars="43 - ${#1}";

    printf "%s %s " $line $1;
    for (( i=0; i < $chars; i++)); do
        printf '-';
    done;

    echo;
}

function line {
    for (( i=0; i<55; i++)); do
        printf "-";
    done;
    echo;
}

### MAIN ###

[[ -z $compiler || -z $interpret || ! -f $compiler || ! -f $interpret ]] && echo "Compiler or Interpreter not found" && exit;

# Create folders
[[ ! -d "$outputCompilerDir" ]] && mkdir "$outputCompilerDir";
[[ ! -d "$outputInterpretDir" ]] && mkdir "$outputInterpretDir";

for test in $(find $testDir -maxdepth 1 -type f); do
    # Setup variables
    name=$(basename $test);
    testName=$(head -n 1 $test);
    
    inputFile="$testDir/in/$name";
    outputCompilerFile="$outputCompilerDir/$name";
    outputInterpretFile="$outputInterpretDir/$name";
    
    expectedFile="$testDir/out/$name";
    expectedCompilerCode=$(head -2 $test | tail -1);
    expectedInterpretCode=$(head -3 $test | tail -1);
    
    # Header
    header "TEST-$name";
    echo " $testName";
    line;

    # Run compiler
    $compiler < $inputFile > "$outputCompilerFile.stdout" 2> "$outputCompilerFile.stderr"
    compilerCode=$?;

    # Print compiler code
    printf " Compiler code:\t\t";
    if [[ $compilerCode -eq $expectedCompilerCode ]]; then
        echo -e "\e[32m"$compilerCode"\e[39m";
    else
        echo -e "\e[31m$compilerCode\e[39m (expected $expectedCompilerCode)";
    fi;

    # If expecting compiler to fail, print result and skip interpretaion
    if [[ ! $expectedCompilerCode -eq 0 ]]; then
        if [[ $compilerCode -eq $expectedCompilerCode ]]; then
            echo -e " Result:\t\t\e[32mSUCCESS\e[39m";
        else 
            echo -e " Result:\t\t\e[31mFAILED\e[39m";
        fi;
        
        line && echo && continue;
    fi;

    # Run interpreter if not expecting compiler to fail
    $interpret "$outputCompilerFile.stdout" > "$outputInterpretFile.stdout" 2> "$outputInterpretFile.stderr";
    interpretCode=$?;

    # Print interpreter code
    printf " Interpret code:\t";
    if [[ $interpretCode -eq $expectedInterpretCode ]]; then
        echo -e "\e[32m"$interpretCode"\e[39m";
    else 
        echo -e "\e[31m$interpretCode\e[39m (expected $expectedInterpretCode)";
    fi;

    # Check interpreter output
    if [[ -f $expectedFile ]]; then
        diff "$outputInterpretFile.stdout" $expectedFile > /dev/null;
        diffOutput=$?;

        printf " Interpret output:\t";
        if [[ $diffOutput -eq 0 ]]; then
            echo -e "\e[32mMatch\e[39m";
        else
            echo -e "\e[31mMismatch\e[39m";
        fi;
    else
        diffOutput=1;
    fi;

    # Print results
    printf " Result:\t\t";
    if [[ $interpretCode -eq $expectedCode && $diffOutput -eq 0 ]]; then
        echo -e "\e[32mSUCCESS\e[39m";
    else
        echo -e "\e[31mFAILED\e[39m";
    fi;

    line; echo;
done;