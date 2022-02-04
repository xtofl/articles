---
title: `tuple` Reveals Your Intent
published: true
description: Reframing the tuple as the go-to container for passing data between different sections of the code.
tags: tuple, python, correctness, codereview
cover_image: https://tse4.mm.bing.net/th?id=OIP.fSFioDohpz47KfnWoVvexgHaDt&pid=Api
---

## Brief

`tuple` is framed as the less potent brother of `list`.  In long-living code, however, limiting functionality is an asset.  This article shows where `list`-ridden code would be better of using a tuple.

## Context

Very often, I bump onto code that happens to work, by pure merit of the code author, but that doesn't give me an easy time to realize why.

Very often, this has to do with over-achieving data types with many bells and whistles, that allow all sorts of things to go wrong: `dict` objects passed around to convey data that has a fixed structure, `list` objects that happen to not be appended to, inserted to, sliced from, sorted, `int` objects that can be multiplied, added to, ... etc....

In any professional project, code has to be reviewed, maintained and debugged.  This code is _read_ an order of magnitude more often than it is written.

So in order to save the reader some time, we can tell them about the objects we create by choosing the proper type for them.  In this post, I want to make the case for using `tuple` over other data structures.

## The Right Frame of Mind

When [looking for this], I bump on mostly discussions that say what you _cannot_ do with a `tuple`, and most of the time this is seen as a liability ([not always](https://towardsdatascience.com/python-tuples-when-to-use-them-over-lists-75e443f9dcd7)).  This is a frame rooted in the *comfort of the code author* to choose a data type that can do everything they would ever need.

I claim that *Not Allowing Unintended Stuff is an asset* of the code: it makes it easy for the code reader to limit the possibilities where the program can go wrong.  I want you to look at `tuple` from the frame of code reviewing and maintenance, and starts from the *comfort of the reader*.

## When to Shun `list`

Try to reason about this code:

```python
def main():
  inputs = [1, 2, -3, 4, -5]
  results = process(inputs)
  tabulate(inputs, results)
```

From the example, you may conclude we're creating a sort of table that maps a list of input values onto a list of results.

But what can you really tell about the correctness?

How can you tell that `tabulate` receives `[1, 2, 3, 4, 5]` as its inputs argument?

Yes: you'll need to check what the `process` function does.  It may be implemented as

```python
def process(inputs):
  # all my inputs are mine!
  originals = tuple(inputs)
  inputs.sort(reverse=True, key=abs)
  total = sum(
      multiprocess_pool.map(
          sum,
          more_itertools.chunked(inputs, 3)))
  return tuple(i/total for i in originals)
```

Not too far fetched, though this is a trivial example.  In production code, the functions would be larger, more complex, and farther apart.

Now imagine this code would have used tuples:

```python
def main():
  inputs = (1, 2, 3, 4, 5)
  results = process(inputs)
  tabulate(inputs, results)
```

This time, there is no way that the `tabulate` function could receive anything but `(1, 2, 3, 4, 5)`.  It becomes trivial to see that the `inputs` object cannot be mutated.  How neat is that!?

## When to Shun `dict`

The idea of limiting the possible uses of a construct is not limited to array-like structures.  On the contrary: it is applicable to whatever code you're designing.

Another trivial design decision, now we're at Python 3.9+, is to no longer use the overly versatile dictionaries that have hidden the true meaning of your program.

```python
def generate_data():
  return tuple({
    "temperature °C": float(tc),
    "time (ms)": float(tms),
    "location": loc
  } for tc, tms, loc in csv_rows("datapoints.csv"))

# In a totally different module, we find the consumer:

def print_location_temperature(data):
  get_loc = lambda s: s["location"]
  for loc, samples in group_by(data, key=get_loc):
    sum_t = sum(s["temperature"] for s in samples)
    avg_t = sum_t / len(data)
    print(f"average temperature in {loc}: {avg_t}")
```

Of course there must be a bug.

Can you spot it?  You know where I'm heading, right?  I'm nagging about dictionaries holding structured data.  The `s["temperature"]` will surely raise `KeyError`!

So far for the review?  Nope.  But admit you were (or at least, I would have been) distracted by checking for key lookups.

What if we could ask the static checker to do this for us?  Not with `dicts`, it won't, but by giving it `dataclass`es!  This totally makes sense, because we know the structure of the data upfront, and we want to have two pieces of code using the same structure - exactly what `dataclass` is for!

```python
@dataclass(frozen=True)  # reap benefits of immutable data
class Sample:
  temperature_C: float
  time_ms: float
  location_id: str


def generate_data() -> Iterable[Sample]:
  return Sample(
          temperature_C=float(tc),
          time_ms=float(tms),
          location_id=loc)
      for tc, tms, loc in csv_rows("datapoints.csv"))

# In a totally different module, we find the consumer:

def print_location_temperature(data: Iterable[Sample]):
  get_loc = lambda s: s.location_id
  for loc, samples in group_by(data, key=get_loc):
    sum_t = sum(s.temperature_C for s in samples)
    avg_t = sum_t / len(data)
    print(f"average temperature in {loc}: {avg_t}")
```

Now you we can focus on the logic of the function, since tools like `mypy` can check for attribute errors.  (I bet you're smarter than me and long spotted that the `len(data)` should have been `len(samples)`).

## Generalizing the Idea

The functionality we care about should be easy to verify.

Using immutable data, and statically check-able constructs, can greatly help with that, because it gives the reader language-guaranteed certainties.

I'm glad I did get this off my chest.  And I'm happy you agree ;).

## Want More?

This post is mostly about the syntactic level; the ideas here can be lifted to the whole world of software design.

* Learn about [strong] [types], meaningful objects as opposed to [primitive-obsession]
* Learn about the [Repository pattern] in Domain Driven Design, I learned about the Repository pattern producing guaranteed-valid Domain Objects.

And I might want to write some more about this, too.

[looking for this]: https://www.ecosia.org/search?q=python+why+list+over+tuple
[strong]: https://arne-mertz.de/2016/11/stronger-types/
[types]: https://www.fluentcpp.com/2016/12/05/named-constructors/
[primitive-obsession]: https://blog.ploeh.dk/2015/01/19/from-primitive-obsession-to-domain-modelling/
[Repository pattern]: https://lostechies.com/jimmybogard/2009/09/03/ddd-repository-implementation-patterns/
