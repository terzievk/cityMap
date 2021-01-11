# cityMap
SDP project, FMI
### what's my data structure?
As Benjamin Franklin famously wrote in 1789, in a letter to Jean-Baptiste Le Roy, 
who, among other things, was one of the contributors, known as Encyclopédistes,
with 108 articles to  the Encyclopédie, which was co-edited by the famous and
beloved by many students, althought now as much as, among others, Isaac Newton or
Augustin-Louis Cauchy, Jean le Rond d'Alembert, who contributed to the same 
Encyclopédie with 1309 articles, which is an impressive amount, but somewhat
diminished by the 17288 articles, about 1/4,  contributed by the most prolific
Encyclopédistes - Louis de Jaucourt:

```
    Our new Constitution is now established, and has an appearance that promises
    permanency; but in this world nothing can be said to be certain,
    except death and taxes.
```

Anyway.
As I was questioning my approach to solving the given problems in this project,
I thought about other certainties in life, which might help me with my project.
I couldn't find any, but I remembered three, which come as close as it gets:
- If you don't know a word, you can find it in the dictionary.
Here is a good one: https://ibl.bas.bg/rbe/

- If you don't know how to solve an integral, integration by parts.

- If you have a data structures and algorithms problem,
binary search and/or hash tables.

Based on the last close to universal certainty, I picked my adjacency list to be of type
```
  using Node = std::string;
  using Distance = int;
  using AdjacentTo = std::unordered_map<Node, Distance>;

  std::unordered_map<Node, AdjacentTo*> nodes;

```
### why are the nodes in AdjacentTo not references/pointers?
std::unordered_map guarantees  ` References and pointers to either key or data
stored in the container are only invalidated by erasing that element, even when
the corresponding iterator is invalidated. ` So the adjacentTo nodes could be
references to the nodes in the nodes map, but too much early optimization
is not cool.
Plus I don't know how exactly std::string works.
I guess the compiler does a pretty good job at optimising the memory anyway...

![xkcd](https://imgs.xkcd.com/comics/optimization.png)

### how to run
- `make` runs creates the necessary object and dependencies files,
    compiles, then it runs the tests and runs the program itself
- `make clean` ~~before commiting run  to clean unnecessary files like
    object files and the exe~~ gitignore ignores them anyway
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

### doctest doesn't like logic
```
    CHECK((d == std::make_pair("a", "d")
           || d == std::make_pair("g", "h")
           || d == std::make_pair("g", "i")));

```

```
$ make 2> err
    echo -Iincludes -Idoctest -MMD -MP
    -Iincludes -Idoctest -MMD -MP
    g++ -Iincludes -Idoctest -MMD -MP -Wall -Werror -std=c++20 -c sources/graph.cpp -o objects/graph.o
$ wc err
  2453  12544 181304 err

```
# the 7 requirements

### 1. is there a path between two nodes?
- BFS:
`bool isPath(Node from, std::optional<Node> to = std::optional<Node>())`

### 2. k-th shortest paths
- Yen's algorihtm with Dijkstra: to do

### 3. k-th shortest paths with list of closed
- same as 2: BFS: to do

### 4. is there a loop starting given node?
- BFS from 1. :
`bool isPath(Node from, std::optional<Node> to = std::optional<Node>())`

### 5. find Euler's cycle
- Hierholzer's algorithm: to do

### 6. is Mother? (there is a path from it to all the rest)
- same as 1. and 4. BFS:
`bool isPath(Node from, std::optional<Node> to = std::optional<Node>())`

### 7. find all dead ends
- get all the nodes with no adjacent to nodes. check if any node 
is adjacent to one of those:
`std::list<std::pair<std::string, std::string>> getDeadEnds()`
