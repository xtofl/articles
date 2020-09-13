
# I want my Bash Pipe

## TL; DR;

Pipelines as we know them from shell scripts are a powerfull mechanism to compose functionality.

This same convenience can be achieved with most programming languages.  In this post, I'll show
how you can introduce pipes into your favourite programming enviroment.


## What?

```bash
function url_domain {
	sed -e "s-http://--" -e "s/\\.com$//g" 
}
wget http://malicient.urls | url_domain | grep ".com$" 
```

Or, simulink
Or, gstreamer?

Now look at this

```C++
std::vector<std::string> report;
for(const auto &url: wget_urls("http://malicient.urls")){
	const auto [protocol, domain, path] = explode(url);
	if (domain.ends_with(".com")){
		report.push_back(domain); 
	}
}
```

<!-- convert to voltage -->

Clearly, the simple pipeline thing is way easier to both write and read.

## What if...

We could use the simplicity of the pipe syntax...

In functional programming, and in math, this concept is known as point-free
function composition. You effectively define a pipeline of functions.

```haskell
split_url $ (\_,domain,_ -> domain) $ (filter ".com$")
```

```C++
wget_urls(...) | explode_url | std::get<1> | filter(".com$")
```

```python
source(wget_urls(...)) | explode_url | get(1) | filter(ends_with(".com"))
```

## But... how?

I'll be using Python for this demo, just for its succinctness. I may add C++
examples later - C++ allows just a little more syntactic sugar.

In most programming languages, you have a way to build [fluent
interfaces](https://en.wikipedia.org/wiki/Fluent_interface), allowing you to
chain operations to an object together:

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

Now if your language allows you to override _operators_ as well, you're golden1
You can create a class `Pipeline` and a pipe operator that extends a pipeline
instance with an extra function.

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

And that seems to be all. It can be tested easily with e.g. `pytest` - a
favourite of mine. For the sake of this article, let's assume the `inc` and
`double` functions to respectively increase and double a value.

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

The `Pipeline` class is our container of functions to be composed. It does so
by storing them in a tuple (`self.functions`).

Now there is this funky member ` __or__(self, f)`. Its sole purpose is to
provide the 'pipe' syntax we know from shell scripting: `p | inc | double`; and
in Python, this is achieved through [operator
overloading](https://docs.python.org/3/reference/datamodel.html#object.__or__).

We could have created a custom name ('and_then') to achieve the same functionality:

```python
    def and_then(self, f):
      return Pipeline(self.functions + (f,))

...
ID.and_then(double).and_then(inc)(10)
```

But choosing  `__or__` as a member name tells Python we want this to be used when a `Pipeline` object is or-ed/piped to a function.

### Calling the Pipeline

Again, another special member: the `__call__` function. You probably guessed
it, but this is what makes objects behave like a function.

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

# Conclusion

Languages allow you to build your own set of constructs these days.  We can make use of this to mimic notations from a known domain.

No language is perfect, though, but we can get close enough to be useful.