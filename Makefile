
MARKDOWN_SLIDES=$(wildcard *.md)

reveal.js:
	git clone https://github.com/hakimel/reveal.js.git
	cd reveal.js && npm install

DECKTAPE = $(shell npm bin)/decktape
$(message decktape: $(DECKTAPE))

$(DECKTAPE):
	npm install -u decktape

reveal_start: reveal.js
	cd reveal.js && npm start &
	touch reveal_running
	sleep 5

reveal_running: reveal_start

reveal_stop:
	-pkill grunt
	-rm reveal_running

reveal_slides: $(subst .md,.html,$(MARKDOWN_SLIDES))

pdf_slides: $(subst .md,.pdf,$(MARKDOWN_SLIDES))

%.reveal: reveal.js %.html
	cp $*.html reveal.js/index.html

%.pdf: $(DECKTAPE) %.reveal reveal_running
	$(DECKTAPE) http://localhost:8000 $@

%.html: %.md to_reveal.py
	python3 to_reveal.py $< >$@
	cp $@ reveal.js/index.html
	cp $< reveal.js/$<
