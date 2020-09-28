import functools


class Pipeline:
    def __init__(self, functions = (lambda x: x,)):
        self.functions = functions

    def __or__(self, f):
        return Pipeline(self.functions + (f,))
    
    def __call__(self, arg):
        return functools.reduce(lambda r, f: f(r), self.functions, arg)

ID = Pipeline()

class WithArg:
    def __init__(self, value):
        self.value = value
    def __call__(self, p: Pipeline):
        return p(self.value)