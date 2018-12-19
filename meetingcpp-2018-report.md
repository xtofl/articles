---
title: writing correct programs
author: Kristoffel Pirard
---

# Writing correct programs
In C++ of course

---

# TL; DR;

Common theme: 'correct' programs

* keynote: Lisa Lippincot: ['truth' of a procedure](https://meetingcpp.com/mcpp/slides/2018/The%20Truth%20of%20a%20Procedure.pdf)
* talk: Mateusz Pusz: [Concepts/Ranges](https://meetingcpp.com/mcpp/slides/2018/C++%20Concepts%20and%20Ranges%20-%20How%20to%20use%20them.pdf)
* talk: Phil Nash: Option(al) is not a Failure ([video cppcon](https://www.youtube.com/watch?v=OsRty0KNDZ0))
* secret lightning talk: Dan Saks: Reframing our Craft as a Discipline

---

# Truth of a Procedure

(Lisa Lippincot)

* goal: 'automated' verification
    * add a layer: `interface` vs. `implementation`
    * `interface` contains pre/postconditions
    * `interface` _calls_ `implementation`

--

## Example

```
template<class F> auto fib(int a, int b, F yield) {
    if(yield(a)) return;
    if(yield(b)) return;
    while(true) {
        auto next = a + b;
        if(yield(next)) return;
        std::tie(a, b) = std::tie(b, next);
    }
}
```
--

## Example: extended


```
namespace checked {
    template<class F> auto fib(int a, int b, F yield) {
        /// prologue(
        ///     claim a > 0,
        ///     claim b > 0,
        ///     claim(x > maxint/2) => !yield(x)
        /// );
        ::fib(a, b, yield);
        /// epilogue();
    }
}
```

--

## Why I think it matters

* Common voc => better understanding
* Learn from logicians => reuse proven material!
* From task-locally towards thinking function-locally
* We forget the edge cases (the devil in the details)
* Does this function always end?

note: "theorem" <-> program: cf. Dan Saks

--

## How does it help _now_?

Emphasize the importance of

* the border between "us" and "them"
    * clear responsibility
    * scope often becomes smaller
* the design process
    * forgiving on your input
    * strict on your output

--

## What I take away

Logic as a science is cumbersome to me.  Lots of funny words and symbols, lots of 'famous' scientists with proven theory are lost to us (Incompleteness, CAP, Curry-Howard, ...)

---

# Concepts

(Mateusz Pusz)

Way too much encyclopedic knowledge on one talk.  Too good to throw away, though.
Cppcon 2018: Stroustrup talk

Here: just touching "Concepts"

--

## What are Concepts

"Named sets of requirements on a type"

Primary goal: Semantic vs. syntactical info

--

## How do they help

Library dev: can express expectations explicitly

```
template<Monoid T>
auto sum(vector<T>  ms) {...}
```

--

## How do they help

Application dev: gets a 'real' error message

    s = sum(ds);

Decent error message:

    $ g++-8 -std=gnu++2a -fconcepts concepts.cxx

    constraints not satisfied
    auto sum(std::vector<T> ms) {
         ^~~
    within ‘template<class T> concept const bool
    Monoid<T> [with T= Distance]’
    template<typename T> concept bool Monoid =
                                      ^~~~~~
    ...
    the required expression ‘T::identity()’ would be ill-formed


--

## 'Truth of a Procedure'

* compiler does some 'concept' check
* ~= automated 'prologue' verification

---

# Option(al) is not a failure

(Phil Nash)

* finally an evaluation method
* 50% of devs: no exceptions!
* Sum types to the rescue!

--

## Optional, briefly

"Sum" type: contains the union of all possible values
of other types.

* card (int + bool) = 32484 + 2
* card (char + int + bool) = 256 + 32484 + 2
* card (my_type + {nothing}) = card my_type + 1

```
    optional<int> i = parse("abcd");
    optional<int> ii = i ? optional<int>(*i * *i) : nullopt;
```

--

## Evaluation of Error Handling Method

* With a "Score Card"
  * overhead happy path
  * overhead error path
  * safety
  * noise
  * separate paths
  * reasonability
  * composability
  * message
* Still a bit subjective

--
<style>
.reveal .table {
    font-size: .5em;
    color: red;
}
</style>

|           | err |  catch |  sumT |  expct |
| --------- | --  | --     |  --   |  --    |
| happy     | 9   | 10     |  8    |  8     |
| error     | 9   | 1      |  8    |  8     |
| safety    | 3   | 6      |  6    |  9     |
| noise     | 3   | 8      |  1    |  5     |
| separate  | 1   | 10     |  1    |  8     |
| reason    | 8   | 5      |  10   |  10    |
| compose   | 3   | 9      |  5    |  10    |
| message   | 1   | 10     |  10   |  10    |

---

# Reframing our Craft as a Discipline

(Dan Saks)

How we can barely call ourselves "engineers" let alone "scientists"

Cf. the tabs/spaces, vim/emacs, C++/D/Rust/C... discussions

TODO:

- arguing/losing
- we want it to be a discipline ... because?
- we can use Linguistic Framing to achieve this

--

## Linguistic Framing

--

## Theory, Hypothesis, Gut feeling

Gut feeling is valuable when experimenting!

We abuse the word "theory" for "gut feeling"

"In theory, this could happen" means: it's not going
to matter.

--

## "If you're arguing, you're losing"

That's science!

We can save time & effort by taking conscious decisions


---

# References

* https://meetingcpp.com/mcpp/slides/
* https://blog.conan.io/2018/11/21/MeetingCpp-trip-report.html
