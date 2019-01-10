<!-- .slide: style="text-align: left;"> -->  

title: Using Monoids in C++
author: Kristoffel Pirard
url: http://github.com/xtofl/articles/monoid.md

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

* Adapting Semigroup to Monoid
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

* Questions (5')

---

## Intro



Imagine this:

* You have to take care of the groceries from now on
* You're a man <!-- .element: class="fragment shrink" -->
  * finding your way around a store??? <!-- .element: class="fragment" -->
  * always too much in your basket <!-- .element: class="fragment" -->
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
* https://soundcloud.com/lambda-cast/12-monoids