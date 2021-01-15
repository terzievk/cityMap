# cityMap
SDP project, FMI
### intro tl;dr: skip the intro
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

### whats my data structure
#explain why I picked my adjacency list to be of type
```
  using Node = std::string;
  using Distance = int;
  using AdjacentTo = std::unordered_map<Node, Distance>;

  std::unordered_map<Node, AdjacentTo*> nodes;

```
### Why unordered_map(hash table)? Why not map(red-black tree) or vector(dynamic array)?
# don't forget to write this
- https://www.bigocheatsheet.com/

### Why not use an adjacency matrix?
Because in a city, most of the crossroads aren't adjacent, and the graph would be
too sparse to justify the used memory.

### Is moving the adjacency list from unordered map to vector justified?
Hash tables have better average search/insertion/deletion than dynamic arrays,
and they are better for reading the graph. 

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

### doctest doesn't like logic
These three lines caused a big fuss:
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

### a few test might give false negative?
a few tests compare the returned list with prechecked lists of results,
which sort of relies on the deterministic properties of the unordered_map iterator,
but maybe to others too

### Dijkstra?
- https://cp-algorithms.com/graph/dijkstra_sparse.html

### best bug yet
Spent a few hours searching for a std::bad_alloc error because I misinterpreted
a pseudocode for loop, adding an extra iteration in some cases...
I had written this:

```
  for i from 0 to size(A[k − 1]) − 2:
```
like this:
```
    for (int i{}; i < size - 1; ++i) {
```
instead of this:
```
    for (int i{}; i < size - 2; ++i) {
```


# the 7 requirements

### 1. is there a path between two nodes?
- BFS:
```
bool isPath(Node from, std::optional<Node> to = std::optional<Node>())

```
### 2. k-th shortest paths
- Yen's algorihtm with Dijkstra:
  - note: Yen's algorithm doesn't count loops
```
std::vector<Path> kTHShortestPath(Node from, Node to, int K, std::set<Node>
                                    nodesToIgnore = std::set<Node>());
```


### 3. k-th shortest paths with list of closed
- same as 2: Yen's:
  - note: Yen's algorithm doesn't count loops
```
std::vector<Path> kTHShortestPath(Node from, Node to, int K, std::set<Node>
                                    nodesToIgnore = std::set<Node>());
```

### 4. is there a loop starting given node?
- BFS from 1. :
```
bool isPath(Node from, std::optional<Node> to = std::optional<Node>())

```
### 5. find Euler's cycle
- Hierholzer's algorithm: 
```
std::optional<std::list<std::string>> findEulerianPath();
```
    - if we need a cycle, the first and last elements of the list, should be the same

### 6. is Mother? (there is a path from it to all the rest)
- same as 1. and 4. BFS:
```
bool isPath(Node from, std::optional<Node> to = std::optional<Node>())
```

### 7. find all dead ends
- get all the nodes with no adjacent to nodes. check if any node 
is adjacent to one of those:
```
std::list<std::pair<std::string, std::string>> getDeadEnds()
```
