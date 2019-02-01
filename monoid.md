<!-- .slide: style="text-align: left;"> -->  

title: Using Monoids in C++
author: Kristoffel Pirard
url: http://github.com/xtofl/articles/monoid.md


[pdf](?print-pdf&pdfSeparateFragments=false)

---

<!-- .slide: style="text-align: left; font-size: .5em;"> -->  

### Abstract

Effective coders recognise tedious repetition and transform it into reusable patterns.  Frameworks, libraries and design patterns arise from it.

After the GoF Object Oriented patterns, today we are also learning from the functional programming world.  One of the patterss
_they_ bumped into is the Monoid: it has its application in addition, accumulation, concatenation, you name it.

And it's a simple one.

This talk is about how Monoids seem to be everywhere, and how recognising them has influenced my daily practice.  It will show a glimpse of the land of functional design patterns and look at practical aspects of using Monoids in C++.

I believe it's important.  And I believe there's a lot more to be discovered.


### About the Speaker:

Kristoffel Pirard is a consultant with Sioux Embedded Systems.  Has been coding in C++ since 1998.  He recently started giving talks and trainings.  He loves generalizing ideas, and applying theory to working code.

---

Note:
# Using Monoids in C++

Topics:

* Intro (5')
  * the rationale for this presentation (2')
  * growsery story (3')

* Examples (10'):
  * A glimpse of my complicated grocery list app
    * how complicated it looked (2')
    * the epiphany (5'): changing hand-rolled into `sum`
  * ? A 2d masking problem (3')
    * rolling density, 1D (2')
    * rolling density, 2D (1')

* Generalizing the pattern (10')
  * Briefly back to school (algebra?)
    * Integer under +, *
  * The "Modern" view
    * Booleans under &&, ||
    * Lists under 'concat'
  * Semigroup (2')
  * Monoid (2')

* What good is it (5')

* Adapting Semigroup to Monoid (5')
  * `sum(map(mon, elements))`
  * Maybe

* Applying it in C++ (20')
  * Adapting to boost accumulators (3')
  * Adapting to `std::accumulate` (3')
  * Building a `sum<Ts...>` template (5')
  * Functions under composition (5') [so question](https://math.stackexchange.com/questions/92787/how-does-a-set-of-functions-form-a-monoid)
  * How can Concepts help?
* What's next (10')
  * Category Theory references
  * Functor
  * Monad

* Conclusion (2')
  * I'm afraid, too :(.  And excited.
  * boost readability through 'common voc + abstraction'

* Questions (5')

---

![pig throwing flares at itself](/milewski_monoid.jpg) <!-- .element: width="400" -->

---

## Why am I doing this?

* we, developers, need
  * proven methodologies (scaling the dev process)
  * reuse, over language boundaries
* mathematicians: building theorems for ages
* we don't understand Mathematese and FP-ish
  * cf. the `iota` discussions lately
* mathematicians don't speak Developese (nobody does)

--

## Why am I doing this

* I'm enthousiastic (a bit nerdy)
  * c'mon... "(lenses and bananas)[https://maartenfokkinga.github.io/utwente/mmf91m.pdf]" in one sentence!
* I think we _will_ benefit (FP matters)
* Can you help me?
  * I want to help _you_ taking the first hurdle
  * Let's take the rest together!

--

## What I want to achieve

TODO

* whetting some appetite
* not feeling 'alone' anymore

---

## Intro


Imagine this:

* You have to take care of the family groceries from now on
* You're a man <!-- .element: class="fragment shrink" -->
  * finding your way around a store??? <!-- .element: class="fragment" -->
  * always too much in your cart <!-- .element: class="fragment" -->
  * always important stuff missing <!-- .element: class="fragment" -->
  * stores are _crowded_ from 10am.  <!-- .element: class="fragment" -->

Note: < 3 minutes

--

### Intro

But... what groceries do I need?

* Week menu
* Recipes
* Pantry

--

### Intro

* 5 chipolata's, 2kg potatoes, 5 apples, 2 packs of pasta, 400g of minced meat, grated cheese, chicken breast, basmati rice, curry sauce ...
* O - we still got potatoes.  <!-- .element: class="fragment" -->
* ... and 5 packs of curry sauce  <!-- .element: class="fragment" -->

--

Week

after week

after week

_can't a computer do that_?

--

```bash
> python3 growser.py
o currysaus <1 pak>
o chipolata <1 pak>
o basmati <1 kg>
...
```

--

[[WLOL]]
(Wife Laughing Out Loud)

--

```python
shopping_list_menu = resulting_list(all_dishes, pantry)
shopping_list = join_ingredients(shopping_list_menu, extras)
print_ingredients(shopping_list, shop=the_shop)
```

--

But look at that code... :(

```python
cart = {}
for dish in menu:
    for i in dish.ingredients:
    if not i.name in cart:
        cart[i.name] = i.amount
    else:
        assert
           cart[i.name].amount.unit == i.amount.unit
        cart[i.name].amount.n += i.amount.n
```

(cliffhanger here)

--

## Intro: digression

Reading about Category Theory, bumping into

* Functor
* Monad
* Applicative
* ...

wooow.... scary! <!-- .element: class="fragment" -->

--

## Intro: rationale

* discussions about `str.join` ([needless branching](https://en.cppreference.com/w/cpp/algorithm/accumulate))
```C++
bool sep = false;
for(auto x: elements) {
    if (sep) cout << ", ";
    cout << x;
    sep = true;
}
```

--

## Intro: rationale

Discussions about 'vacuous truth/falsity'

```
* all({true, false, true}) == false
* all({true}) == true
* all({}) == ????
```

--


## Intro: rationale

[![math lingo](Lax_monoidal_functor_associative.svg)](https://en.wikipedia.org/wiki/Monoidal_functor#Definition)

--

### misunderstanding

![prof and dev misunderstanding](/01.misunderstanding.jpg)  <!-- .element: height="400" -->

--


### anger

![prof and dev angry](/02.anger.jpg)  <!-- .element: height="400" -->

--

## Knowledge for the win

* Scientists tend to be clever
* Common vocabulary => more collaboration <!-- .element: class="fragment" -->
  * with scientists
  * amongst developers

--

## This presentation is

An attempt to take away

* some fear
* some dismay

=> small steps!

=> real-life!

--

So... in order of 'difficulty'

* Monoid <-- you are here
* Functor
* Applicative
* Monad

--

![dev at math course](/03.investment_education.jpg)  <!-- .element: height="400" -->

(but I'm not a professor)  <!-- .element: class="fragment" -->

---

* Examples (10'):
  * A glimpse of my complicated grocery list app
    * how complicated it looked (2')
    * the epiphany (5'): changing hand-rolled into `sum`
  * ? A 2d masking problem (3')
    * rolling density, 1D (2')
    * rolling density, 2D (1')

--

### Monoid: back to school

* Back to school: addition
  * 1 + 2 == 3  <!-- .element: class="fragment" -->
  * 234225 + 123415115 = 123649340  <!-- .element: class="fragment" -->
  * 1 + (2 + 3) == (1 + 2) + 3  <!-- .element: class="fragment" -->
  * 0 + x = x  <!-- .element: class="fragment" -->
  * x + 0 = x  <!-- .element: class="fragment" -->

--

### Monoid: back to school

* Back to school: multiplication
  - 3 &middot; 2 == 6
  - 165 &middot; 23 == 3795
  - 4 &middot; ( 2 &middot; 3 ) == ( 4 &middot; 2 ) &middot; 3
  - 1 &middot; x = x
  - x &middot; 1 = x

--

### Generalizing

* + and &middot; are binary operations on ℕ
  * closed
  * associative
  * with an identity element (resp. 0 and 1)

--

### Generalizing

Monoid: a tuple `<S, op, id>` so that

* op(s1, s2) element of S
* op(s1, op(s2, s3)) == op(op(s1, s2), s3)
* op(id, s) == op(s, id) == s
--

### Generalizing

Also: closure + associativity => Semigroup

So Monoid is a Semigroup with an identity element

--

### Generalizing: math notation

Monoid: a tuple &lt;S, &diamond;, id&gt; so that

* &forall; s1, s2 &isin; S
* s1 &diamond; (s2 &diamond; s3) = (s1 &diamond; s2) &diamond; s3
* id &diamond; s = s = s &diamond; id

--

### Examples: &lt;bool, &amp;&amp;, true&gt;

```C++
* a && b is a boolean
* a && (b && c) == (a && b) && c
* a && true == a and true && a == a
```

--

### Examples: &lt;string, +, ""&gt;

```C++
* string operator+(string)
* "ab"s + ("cd"s + "ef"s) == ("ab"s + "cd"s) + "ef"s
* "ab"s + ""s == "ab"s == ""s + "ab"s
```

--

### Examples: floating point

```C++
* double operator+(double)
* .5 + (1. + 2.) == (.5 + 1.) + 2.
* .5 + 0. == 0. + .5 == .5
```

--

### Examples: floating point

```C++
* .1 + (.2 + .3) == (.1 + .2) + .3
```

**NOT** associative!

---

## So... what good is it?

--

### So... what good is it?

Generic functions, of course!

Functions on Ranges can often be generalized
to any monoid.

* Runtime
* Compile time


--

#### Closure

* Operation can be chained
  * "define pairwise => get range operation for free"
* Only 1 type needed
  * less mental burden
  * less template arguments

--

#### Closure

```C++
// explicit knowledge
template<typename InputIt, typename T,
         typename BinaryOperation>
T accumulate(InputIt first, InputIt last, T init,
              BinaryOperation op );
// requires BinaryOperation satisfies
// T t = op(t, i)
```

vs

```C++
// implicit knowledge
template<typename Monoid, typename InputIt>
Monoid accumulate(InputIt first, InputIt last);
```

Note: conservation of complexity: it is moved into the Monoid type 

--

#### Associativity

* Divide and Conquer
```
acc(a, b, c, d) ==
acc(acc(a, b), acc(c, d))
```
  * => parallelization
  * Incrementalism

--

#### An identity element

* operation with 'empty' lists (vacuus truth?)
* allow 'restarting' computation in divide and conquer algo's

--

## Treasure Trove

Theorems for Free!

* m, n are monoid => algebraic types of m, n, too
* foo(Monoid), bar(Monoid) => foo &compfn; bar(Monoid)
* ...

But we need translators

<p class="fragment" style="font-size:.4em;">
Let (S,∘) and (T,∗) be monoids.
Let ϕ:S→T be a mapping such that ∘ has the morphism property under ϕ<br/>

That is, ∀a,b∈S<br/>
    ϕ(a∘b)=ϕ(a)∗ϕ(b)<br/>

Suppose further that ϕ preserves identities, i.e.:
    ϕ(eS)=eT<br/>

Then ϕ:(S,∘)→(T,∗)
is a monoid homomorphism.
</p>

<ul style="font-size:.3em">
<li>http://comonad.com/reader/wp-content/uploads/2009/08/IntroductionToMonoids.pdf</li>
</ul>

---

## Applying it in C++

Different approaches

* overload `operator +` and add a `0` constructor
* template specialization
* use type trait (concepts may help!)

--

## Creating a Monoid in C++

Goal:

Define and use `mconcat(begin, end) -> M`

```
auto result = mconcat(begin(xs), end(xs));
```

--

Generic `mappend` and `mempty`

```
template<typename T> T mempty();
template<typename T> T mappend(T, T);

template<typename M>
auto mconcat(It b, It e) {
    return accumulate(
        b, e,
        mempty<Monoid>(),
        mappend<Monoid>);
}
```

--

Specialization for e.g. `int`

```
template<>
int mempty<int>() { return 0; }

template<>
int mappend<int>(int a, int b) { return a + b; }
```

```
std::vector<int> ints{{1, 2, 3, 4}};
EXPECT_EQ(10, mconcat(begin(ints), end(ints)));
```

--

And for a custom type

```
struct Custom {
    std::string s;
    int n;
};
```
```
template<>
Custom mempty<Custom>() { return {}; }
template<>
Custom mappend<Custom>(Custom c, Custom d) {
    return {
        c.s + d.s,
        c.n + d.n
    };
}
```

--

### Let's break it

For int product:
```
template<>
int mempty<int>() { return 1; }
template<>
int mappend<int>(int a, int b) { return a * b; }
```

Can we have 2 specializations for `int`?

Didn't think so <!-- .element class="fragment" -->

--

So some extra info is needed

```
template<typename T> struct Product {
    T value;
    static constexpr T mempty{};
    static Product mappend(Product a, Product b) {
        return {a.value * b.value};
    }
};
```

```
auto r = mconcat<Product<int>>(b, e).value;
```

--

## But isn't it slow?

[thanks, quick-bench!](http://quick-bench.com/mEJDA6p6zEdR_Wg-Yo_PkXXYwto)


```
accumulate(begin(ints), end(ints), 0);
mconcat<Sum<int>>(begin(ints), end(ints));
```
![benchmark_result.png](benchmark_result.png)  <!-- .element: height="300" -->

<span style="font-size: .5em">
(-O3 and -O2; for -O1, there's a 10% penalty)
Cf. also [Linear Types](https://meetingcpp.com/mcpp/slides/2018/lin.pdf)/[ligthning talk](https://www.youtube.com/watch?v=sN8tI-zleFI)
</span>


--

### C++20: Concepts

```
// remark: not yet compiled
template<typename T>
concept Monoid =
    requires Associative(T::mconcat) &&
    requires(T) { T::mempty() -> T; }

template<typename It, typename T>
    requires Monoid<T>
auto mconcat(It b, It e) {...}
```

---

## Adapting Semigroups

Some times you have no Unit

* Non-empty lists
* Counting from 1

--

## Add a Unit

You can create a 'Sum' type:

* N = {1, 2, 3, 4, ...}
* M<sub>N</sub> = {None} &cup; N

--

## Add a Unit

In C++... use an `optional<T>`

```
auto mempty<optional<T>>() {
    return {};
}
auto mappend(O<T> a, O<T> b) {
    return (a && b)
        ? mappend(*a, *b);
        : mempty<O<T>>();
}
```

---

## What's After Monoid...

* Functor ('mappable': `vector`, `struct`, ...)
* Monad ('programmable semicolon')
* Category Theory for the Working Programmer
    * Milewski (a C++ programmer!)
    * Philippe Wadler (&lambda;)

--

Translators needed!

* slack: Cpplang fp channel
* Nice tutorial

"We're hiring" <!-- .element: class="fragment" -->


---

# Conclusion

* Math and Category theory: treasure island
    * Can be used in C++
    * Establish common vocabulary
    * Reduce complexity through _known abstractions_
* I'm afraid of the math lingo
    * But I'm not on my own
    * Baby steps (note [lambda cast](https://soundcloud.com/lambda-cast))

---

## References

* [lambda cast](https://soundcloud.com/lambda-cast/12-monoids)
* [Cyrille Martraire on patterns](https://youtu.be/J9UwWo2qifg)

* [F# for Fun and Profit](https://fsharpforfunandprofit.com/posts/monoids-without-tears/)
* [Monoid. Encyclopedia of Mathematics](http://www.encyclopediaofmath.org/index.php?title=Monoid&oldid=29746)
* [Monoids on Steroids](https://bartoszmilewski.com/2017/02/09/monoids-on-steroids/)
* [Haskell Wiki: Monoid](https://wiki.haskell.org/Monoid)
* [Some theorems](https://philipnilsson.github.io/Badness10k/algebraic-patterns-monoid/)

---

## Credits

* My son [Jona for the artwork](https://www.instagram.com/jonap_art/)
* [Bartosz Milewski](https://bartoszmilewski.com/) who's [pigs](https://bartoszmilewski.files.wordpress.com/2014/12/monoid.jpg) I borrowed ([cc license](https://creativecommons.org/licenses/by/4.0/))

---

## Questions
