
MARKDOWN_SLIDES=$(wildcard *.md)

reveal.js:
	git clone https://github.com/hakimel/reveal.js.git
	cd reveal.js && npm install

reveal_run: reveal.js
	cd reveal.js && npm start

reveal_slides: $(subst .md,.html,$(MARKDOWN_SLIDES))

%.reveal: reveal.js %.html
	cp $*.html reveal.js/index.html

%.html: %.md to_reveal.py
	python3 to_reveal.py $< >$@
	cp $@ reveal.js/index.html
	cp $< reveal.js/$<

PHONY: reveal_slides
