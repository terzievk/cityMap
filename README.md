# cityMap
SDP project, FMI

### how to run
- `make` runs creates the necessary object and dependencies files,
    compiles, then it runs the tests and runs the program itself
- `make clean` ~~ before commiting run  to clean unnecessary files like
    object files and the exe ~~ gitignore ignores them anyway
- `make test` to build and run only the test

### make
- https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile/30602701
- https://makefiletutorial.com/
- https://www.gnu.org/software/make/manual/make.html

### doctest
- https://github.com/onqtam/doctest 
- commands
    - `./f` runs both doctests and the programme itself
    - `./f -nr` runs only the programme
    - `./f -e` runs only the doctests
    - `./f -ts=graph` runs only tests in suite graph `-ts   --test-suite=<filters>`
    - more commands: https://github.com/onqtam/doctest/blob/master/doc/markdown/commandline.md

### why add path to includes?
- because it helps the autocomplete on my config
   
### how to test private methods with doctest?
- private methods testing workaround https://github.com/onqtam/doctest/issues/427,
but with a friend for better encapsulation

### `make` vs `make all`?
- I thought `make` was an alias for `make all`. It is not. Apparantly make just builds
the first target in the Makefile

### how to reduce code redundancy since 1., 2., and 6. are pretty much the same?
- when I was planning how I was going to make my project(which, unlike the
coding part, I didn't leave to the very end - in fact just the opposite) I
thought it would be cool to check what's new in the different c++ versions.
That's how I found `std::optional`. here are a few cool links:
  - https://isocpp.org/files/papers/p1319r0.html
  - https://isocpp.org/files/papers/p0636r0.html
  - https://www.learncpp.com/cpp-tutorial/b-1-introduction-to-c11/
  - https://www.learncpp.com/cpp-tutorial/b-2-introduction-to-c14/
  - https://www.learncpp.com/cpp-tutorial/b-3-introduction-to-c17/

# the 7 requirements

### 1. is there a path between two nodes?
- BFS: `isPath(from, to)`

### 2. k-th shortest paths
- Yen's algorihtm with Dijkstra: to do

### 3. k-th shortest paths with list of closed
- same as 2: BFS: to do

### 4. is there a loop starting given node?
- BFS from 1. : `isPath(from, from)`

### 5. find Euler's cycle?
- Hierholzer's algorithm: to do

### 6. is Mother? (there is a path from it to all the rest)
- same as 1. and 2. BFS: `isPath(from);`

### 7. find all dead ends?
- get all the nodes with no adjacent to nodes. check if any node 
is adjacent to one of those: to do
