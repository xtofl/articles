
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
    """builder for function composition"""
    def __init__(self, functions = tuple()):
        self.functions = functions

    def __or__(self, f):
        return Pipeline(self.functions + (f,))
    
    def __call__(self, arg):
        return functools.reduce(lambda r, f: f(r), self.functions, arg)
```

And that seems to be all.  It can be tested easily with e.g. `pytest` - a favourite of mine:

```python

def test_everything_starts_with_an_empty_pipeline():
    """empty pipeline is the identity function"""
    p = Pipeline()
    assert all(p(x) == x for x in (1, 2, "abcd", None))

def test_pipeline_steps_are_applied_in_order():
    inc = lambda x: x+1
    double = lambda x: x*2

    pipeline = Pipeline() | inc | double

    assert pipeline(0) == (0+1) * 2
    assert pipeline(3) == (3+1) * 2
```

## But still... how???

Now let's explain this step by step.