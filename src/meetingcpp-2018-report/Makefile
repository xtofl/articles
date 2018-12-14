
show: concepts.diff
	cat concepts.diff

%.cxx17: %.cxx
	-g++ -std=c++17 $^ 2> $@

%.cxx20: %.cxx
	-g++ -std=c++2a -fconcepts $^ 2> $@

concepts.diff: concepts.cxx17 concepts.cxx20
	diff -u concepts.cxx17 concepts.cxx20 | tee concepts.diff
	cat concepts.diff

clean:
	-rm *.cxx17 *.cxx20 *.diff

.PHONY: clean