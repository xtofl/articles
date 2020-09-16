
# I want my Bash Pipe

## TL; DR;

Pipelines as we know them from shell scripts are a powerful
mechanism to compose functionality.

This same convenience can be achieved with most programming
languages.  In this post, I'll show how you can introduce
pipes into your favourite programming enviroment.


## What?

Lots of platforms come with a way to build programs out of small blocks.  In many contexts, there is a convenient way to do this functional composition.  We can call these 'pipelines'.

This post came to being after a discussion on pipes in bash (slightly controversial analysis on why [your bash scriptsare rubbish](https://dev.to/taikedz/your-bash-scripts-are-rubbish-use-another-language-5dh7)), but the domain is much larger than that.



So this is what we're talking about: a bash script composed of small commands connected through pipes.

```bash
cat urls.txt | sed -e "s-http://--" -e "s/\\.com$//g"  | grep ".com$"
```

The pipe concept exists in different domains, too - sound processing is a nice example (image from [gstreamer website](https://gstreamer.freedesktop.org))

<img alt="gstreamer sound processing pipeline" src="https://gstreamer.freedesktop.org/documentation/application-development/introduction/images/simple-player.png" height="150px">

Composition is the main driving goal of most programming languages.  We chase some value through a function, and use its return value as input for another function.


```C++
std::vector<std::string> report;
for(const auto &url: read_lines("urls.txt")){
	const auto [protocol, domain, path] = explode(url);
	if (domain.ends_with(".com")){
		report.push_back(domain);
	}
}
```

Clearly, the bash pipeline syntax is way easier to both write and read.

## What if...

We could use the simplicity of the pipe syntax...

In functional programming, and in math, this concept is known as point-free function composition.  You effectively define a pipeline of functions.


```python
read_lines("urls.txt") \
    | explode_url \
    | tuple_get(1) \
    | filter(ends_with(".com"))
```

## But... how?

I'll be using Python for this demo, just for its succinctness.  I may add C++ examples later - C++ allows us to add just a little more syntactic sugar with function overload resolution.

### Fluent Interfaces

In most programming languages, you have a way to build [fluent interfaces](https://en.wikipedia.org/wiki/Fluent_interface), allowing you to chain operations to an object together:

```python
ServerBuilder()\
  .on_address([localhost])\
  .on_port(1050)\
  .with_database(that_db)\
  .that_handles("rq1", handle_rq1)\
  .that_quits_on("quit")\
  .build()
```

This works by creating methods that return builders again (spot the recursion).


### Operators

Now if your language allows you to override _operators_ as well, you're golden1 You can create a class `Pipeline` and a pipe operator that extends a pipeline instance with an extra function.

```python
class Pipeline:
    def __init__(self, functions = (lambda x: x,)):
        self.functions = functions

    def __or__(self, f):
        return Pipeline(self.functions + (f,))

    def __call__(self, arg):
        return functools.reduce(lambda r, f: f(r), self.functions, arg)

ID = Pipeline()
```

### Testing it

And that seems to be all.  It can be demoed easily with e.g. `pytest` .  For the sake of this article, let's assume the `inc` and `double` functions to respectively increase and double a value.

```python
def test_everything_starts_with_the_identity_function():
    assert all(ID(x) == x for x in (1, 2, "abcd", None))

def test_pipeline_steps_are_applied_in_order():
    pipeline = ID | inc | double
    assert pipeline(0) == (0+1) * 2
    assert pipeline(3) == (3+1) * 2
```

## But still... how???

Now let's explain this step by step.

### Building the Pipeline

The `Pipeline` class is our container of functions to be composed.  It does so by storing them in a tuple (`self.functions`).  (As an aside, I prefer `tuple` rather than `list` for its immutability)

The module also adds a very first object we can use as a starting point for our construction - a pipeline hat does nothing but returning the identical element it receives.  It is called `ID`, just like in the functional programming world.

Now our class has this special member ` __or__(self, f)`. Its sole purpose is to provide the 'pipe' syntax we know from shell scripting: `p | inc | double`; and in Python, this is achieved through [operator overloading](https://docs.python.org/3/reference/datamodel.html#object.__or__).

We could have created a custom name ('and_then') to achieve the same functionality:

```python
    def and_then(self, f):
      return Pipeline(self.functions + (f,))

...
ID.and_then(double).and_then(inc)
```

But choosing `__or__` as a member name tells Python we want this to be used when a `Pipeline` object is or-ed/piped to a function.

### Calling the Pipeline

Again, another special member: the `__call__` function.  You probably guessed it, but this is what makes objects behave like a function.

I have implemented it using `functools.reduce`, but you could just as well hand-code a loop feeding the first argument to tne first function, the return value to the next function, and so on.

Here, too, we could have called it something else, like `invoke_with`.  A non-special-member pipeline would have looked like this:

```python
ID.and_then(inc).and_then(double).invoke_with(10)
```

But choosing `__call__` tells Python to choose this method when the braces are used:

```python
(ID | inc | double)(10)
```

### But What's With That Funny ID Stuff?

What I would _really_ want to write is something this:

```python

twentytwo = 10 >> inc | double
fourty_eight = "0x18" >> from_hex | double
```

But in Python, I don't know how.  I can _almost_ work around it by adding another trick-helper class that reverses things:

```python
class WithArg:
    def __init__(self, value):
        self.value = value
    def __call__(self, p: Pipeline):
        return p(self.value)

def test_an_almost_natural_pipeline_calling_order():
    assert WithArg(10)(ID | inc | double) == (10+1) * 2
```

# Limitations

The pipes you can build with the code in this article are nice, but lack still one important aspect of the ones you use on you shell.  It has to do with breaking structure.

Take a look at [this pipeline](https://www.howtogeek.com/438882/how-to-use-pipes-on-linux/):

```bash
function top5 {
   grep "page" | awk '{print $5 " " $3 " " $9}' | sort -n | tail -5
}
```

What we see here is a line-based `grep` and `awk`.  But then comes `sort`.  How is this different?  Wel, it will swallow all of its input before generating output.

The python pipeline will allow us to compose functions that accept and return data in lock step. `ID | grep("page") | print_elements(5, 3, 9)` is going to process a single argument to produce a single value.  How are we going to break free from that?  Parts of the pipe need to be able to 'buffer' their input to produce one output (or a stream?) when the input stops.

Indeed, text based processing has two kinds of events: new-line, and end-of-file.  As a matter of fact, all of these command line stream-processing tools are composed of a buffering/chunking part and a chunk-processing part.  We just _may_ use this knowledge to make our pipeline smarter. But not in this post.



# Conclusion

Languages allow you to build your own set of constructs these days.  We can make use of this to mimic notations from a known domain.

No language is perfect, though, but we can get close enough to be useful.