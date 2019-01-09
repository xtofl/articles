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

* Examples:
  * A glimpse of my complicated grocery list app
    * how complicated it looked
    * how it changed
  * ? A 2d masking problem
* Generalizing the pattern
  * Briefly back to school (algebra?)
  * The "Modern" view
* Applying it in C++
  * Adapting to boost accumulators
  * Adapting to `std::accumulate`
  * Building a `sum<Ts...>` template
  * Functions under composition [so question](https://math.stackexchange.com/questions/92787/how-does-a-set-of-functions-form-a-monoid)
  * How can Concepts help?
* What's next
  * Category Theory references

---

## Intro

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