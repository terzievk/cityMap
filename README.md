# cityMap
SDP project, FMI

### how to run
    - $ `make` runs creates the necessary object and dependencies files,
    compiles, then it runs the tests and runs the program itself
    - $ `make clean` ~~ before commiting run  to clean unnecessary files like
    object files and the exe ~~ gitignore ignores them anyway
    - $ `make test` to build and run only the test

### make
    - https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile/30602701
    - https://makefiletutorial.com/
    - https://www.gnu.org/software/make/manual/make.html

### why add path to includes?
    - because it helps the autocomplete on my config

### doctest
    - https://github.com/onqtam/doctest 
    - `./f` runs both doctests and the programme itself
    - `./f -nr` runs only the programme
    - `./f -e` runs only the doctests
    - see more commands:  https://github.com/onqtam/doctest/blob/master/doc/markdown/commandline.md
    - private methods testing workaround https://github.com/onqtam/doctest/issues/427, but with a friend for better encapsulation
