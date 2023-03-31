# Join-Validator

## How to Use?
* Compile the validator using   
```
g++ main.cpp
```
* Run the validator using 
```
./a.out inputR inputS outputFile
```
* For comparing output files use
```
comm -3 < (sort -u outputFile) < (sort -u yourOutput)
```
2 column output with unique lines in outputFile yourOutput. Check comm's man page for other options

## Command line arguments
* inputR - Path to file containing relation R
* inputS - Path to file containing relation S
* outputFile - Path to output file

