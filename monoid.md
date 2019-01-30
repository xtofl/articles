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

* wetting some appetite
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

---

## So... what good is it?

--

### So... what good is it?

Generic functions, of course!

* Runtime
* Compile time

--

#### Closure

* Operation can be chained
  * "define pairwise => get range operation for free"
* Only 1 type needed
  * less mental burden

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

---

* Applying it in C++ (20')
  * Adapting to `std::accumulate` (3')
  * Adapting to boost accumulators (3')
  * Building a `sum<Ts...>` template (5')
  * Functions under composition (5') [so question](https://math.stackexchange.com/questions/92787/how-does-a-set-of-functions-form-a-monoid)
  * How can Concepts help?

---

* Adapting Semigroup to Monoid (5')
  * `sum(map(mon, elements))`
  * using Maybe<S>

---

* What's next (5')
  * Category Theory references
  * Functor
  * Monad

---

* Conclusion (2')
  * I'm afraid, too :(.  And excited.
  * boost readability through 'common voc + abstraction'

---

Questions (5')

---

## References

* Monoid. Encyclopedia of Mathematics. URL: http://www.encyclopediaofmath.org/index.php?title=Monoid&oldid=29746
* https://fsharpforfunandprofit.com/posts/monoids-without-tears/
* https://bartoszmilewski.com/2017/02/09/monoids-on-steroids/
* https://wiki.haskell.org/Monoid
* https://youtu.be/J9UwWo2qifg?t=720 Cyrille Martraire on patterns
* https://soundcloud.com/lambda-cast/12-monoids

---

## Credits

* My son [Jona for the artwork](https://www.instagram.com/jonap_art/)
* [Bartosz Milewski](https://bartoszmilewski.com/) who's [pigs](https://bartoszmilewski.files.wordpress.com/2014/12/monoid.jpg) I borrowed ([cc license](https://creativecommons.org/licenses/by/4.0/))

---

## Questions