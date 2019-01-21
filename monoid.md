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

* we, developers need
  * proven methodologies (scaling, )
  * reuse, over language boundaries
* mathematicians have been forming theories for ages
* we don't understand mathematese
  * cf. the `iota` discussions lately
* mathematicians don't speak developese (nobody does)

--

## Why am I doing this

* I'm enthousiastic (a bit nerdy)
  * c'mon... "(lenses and bananas)[https://maartenfokkinga.github.io/utwente/mmf91m.pdf]" in one sentence!
* I think we _will_ benefit
* Can you help me?
  * I want to help _you_ taking the first hurdle
  * Let's take the rest together!

--

## What I want to achieve

TODO

* not feeling 'alone' anymore

---

## Intro: epiphany


Imagine this:

* You have to take care of the family groceries from now on
* You're a man <!-- .element: class="fragment shrink" -->
  * finding your way around a store??? <!-- .element: class="fragment" -->
  * always too much in your cart <!-- .element: class="fragment" -->
  * always important stuff missing <!-- .element: class="fragment" -->

Note: < 3 minutes

--

### Intro

It's Saturday, 9am.  Stores are crowded from 10am.  No idea what we need for next week.

Make a week menu!

--

### Intro

* Monday: saucage, potatoes, compote
* Tuesday: spaghetti bolognese
* Wednesday: chicken, rice, curry sauce
* ...

--

### Intro

But... what groceries do I need?

... Recipes?

--

### Intro

5 chipolata's, 2kg potatoes, 5 apples, 2 packs of pasta, 400g of minced meat, grated cheese, chicken breast, basmati rice, curry sauce ...

--

O - we still _got_ potatoes.

--

Week

after week

after week

--

_can't a computer do that_?

--

Saturday, 9am.

Start coding.

--

Saturday, 11am.

... coding

--

Saturday, 12am.

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


## Intro: rationale

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

* I was scared, first
  * Wikipedia... alien lingo <!-- .element: class="fragment" -->
  * Colleagues think I'm a freak <!-- .element: class="fragment" -->
* I guess I'm not alone <!-- .element: class="fragment" -->
* This is a stupid hurdle <!-- .element: class="fragment" -->
  * Fear leads to anger <!-- .element: class="fragment" -->
  * Anger leads to ... <!-- .element: class="fragment" -->
* Common vocabulary => more collaboration <!-- .element: class="fragment" -->
  * with math people
  * with other devs

--

An attempt to take away

* some fear
* some dismay

=> small steps!

=> real-life!

--

## Intro: rationale

So... in order of 'difficulty'

* Monoid <-- you are here
* Functor
* Applicative
* Monad

---

* Examples (10'):
  * A glimpse of my complicated grocery list app
    * how complicated it looked (2')
    * the epiphany (5'): changing hand-rolled into `sum`
  * ? A 2d masking problem (3')
    * rolling density, 1D (2')
    * rolling density, 2D (1')

---

* Generalizing the pattern (10')

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

### So... what good is it?

#### Closure

Operation can be chained.

Indefinitely.

--

#### Associativity

Divide and Conquer

--

####

* An identity element


--

### What good is Closure?

* Closure
* Associativity
* An identity element

Note: 
* What good is it (5')
  * Closure (1')
  * The Identity element (1')
  * Associativity (3')

---

* Adapting Semigroup to Monoid (5')
  * `sum(map(mon, elements))`
  * using Maybe<S>

---

* Applying it in C++ (20')
  * Adapting to boost accumulators (3')
  * Adapting to `std::accumulate` (3')
  * Building a `sum<Ts...>` template (5')
  * Functions under composition (5') [so question](https://math.stackexchange.com/questions/92787/how-does-a-set-of-functions-form-a-monoid)
  * How can Concepts help?

---

* What's next (10')
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

## 

Magic: Monoid a => all possible Accumulation functions just work,
and so does function composition (on a Monoid).  Cf "So What?" in
https://wiki.haskell.org/Monoid

    sortStrings = sortBy (comparing length <> compare)

---

## Why all the Hard Stuff

* Generalizing => "Concepts" => can be reused/Synthesized
* Cf. popularity of point-free style
  * Addition of lambda's (C++11)
  * Ranges
  * Boost.HOF

---

## References

* Monoid. Encyclopedia of Mathematics. URL: http://www.encyclopediaofmath.org/index.php?title=Monoid&oldid=29746
* https://fsharpforfunandprofit.com/posts/monoids-without-tears/
* https://bartoszmilewski.com/2017/02/09/monoids-on-steroids/
* https://wiki.haskell.org/Monoid
* https://youtu.be/J9UwWo2qifg?t=720 Cyrille Martraire on patterns
* https://soundcloud.com/lambda-cast/12-monoids