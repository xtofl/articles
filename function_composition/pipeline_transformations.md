
## Pipeline transformations

Imagine that some of the pipeline steps could fail.  You could rewrite your program:

### Error propagation

* add validation and error notification to each step
* redefine composition to include error-propagation

### Vectorization

Let each step accept a vector/tuple/... of elements

### Recordization

Let each step work on, and construct an new structure

```haskell
Record a => (b->c) -> (a->b) -> (Record a -> Record c)
```


* pipeline notation (visual thinkers)
  * examples
  * + extra properties of pipeline notation (error checking, distributed deploymnet, ...)
* function composition in some languages
  * python
  * c++
  * Haskell
* pipeline decorations in said languages
* references
  * milewski
  * monads
  * barbed wire, bananas
