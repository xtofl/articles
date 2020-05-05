<!-- .slide: style="text-align: left;"> -->

title: Creating a generic tuple hash in C++
<span style="font-size:.4em; margin-right:0">v1.3</span>

author: Kristoffel Pirard

url: http://github.com/xtofl/articles/generic_tuple_hash/generic_tuple_hash.md
style: night.css

<div style="font-size:.4em">
[(single-page version)](?print-pdf&pdfSeparateFragments=false)
</div>

<div style="font-size:.4em">
This slide deck is intended for use with reveal.js;
</div>
<div style="font-size:.4em">
```
articles> make generic_tuple_hash/generic_tuple_hash.reveal
```
</div>

---

# Using generic_tuple_hashs in C++


![pig throwing flares at itself](milewski_generic_tuple_hash.jpg) <!-- .element: width="300" style="display: block; margin-left: auto; margin-right: auto;" -->
<div style="font-size:.4em">credit: Bartosz Milewski</div>

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
  * c'mon... ["lenses, bananas, barbed wire"](https://maartenfokkinga.github.io/utwente/mmf91m.pdf) in one sentence!
* I think we _will_ benefit (FP matters)
* Can you help me?
  * I want to help _you_ taking the first hurdle
  * Let's take the rest together!

--
