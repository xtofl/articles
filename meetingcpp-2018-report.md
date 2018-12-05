<!-- $theme: gaia -->
title: writing correct programs
author: Kristoffel Pirard

----

# Writing correct programs
In C++ of course

----

# TL; DR;

Common theme: 'correct' programs

* keynote: Lisa Lippincot: ['truth' of a procedure](https://meetingcpp.com/mcpp/slides/2018/The%20Truth%20of%20a%20Procedure.pdf)
* talk: Phil Nash: Option(al) is not a Failure
* talk: Mateusz Pusz: [Concepts/Ranges](https://meetingcpp.com/mcpp/slides/2018/C++%20Concepts%20and%20Ranges%20-%20How%20to%20use%20them.pdf)
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
    * scrutinous on your output

--

## What I take away

Logic as a science is cumbersome to me.  Lots of funny words and symbols, lots of 'famous' scientists with proven theory are lost to us (Incompleteness, CAP, Curry-Howard, ...)

---

# Concepts and Ranges

(Mateusz Pusz)

Way too much encyclopedic knowledge on one talk.  Too good to throw away, though.
Cppcon 2018: Stroustrup talk

--

## What are Concepts

"Named sets of requirements on a type"

--

## How do they help

* Library dev: can express expectations explicitly

```
template<Sortable S> bool less(S lhs, S rhs);
```

* Application dev: gets a 'real' error message

```
if less(MyType{"a", 1}, MyType{1.5}) ...

// error message:
//  error: cannot call less with MyType
//  note:  concept Sortable<MyType> was not satisfied
```

note: error messages are the first complaint I hear when
introducing people to the C++ stl.

---

# Option(al) is not a failure

(Phil Nash)

* finally an evaluation method
* 50% of devs: no exceptions!
* Sum types to the rescue!

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
