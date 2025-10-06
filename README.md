# Shannon (DE)coder
## Description
Program can Shannon-code or -decode any binary file.

## Requirements
* `C++20`
* `make`
* `will power not to delay tasks till the end`

## Build
``` Bash
make
```
## Some Details
* This project implements classical Shannon coding:
1. Reads raw bytes data 
2. Estimates every symbol probability
3. Sorts in descending probability
4. Then, using pre-known formulas, creates prefix-free code

* While coding, program form long bit string which is being written to new output bin file with 4-byte big-endian header.
This allows us to avoid problems with padding.

* A dictionary at .csv format is also being created which allows us to deocde it easily.

* Generally, this project contains little pieces from previous projects (such as ascii85 and Gauss solver), but of course not deprived of new implementations.

## Author and Contacts
Andikalovskiy Nikita, 24.B82-mm, st131335@student.spbu.ru