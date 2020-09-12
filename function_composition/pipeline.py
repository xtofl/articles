import functools

class Pipeline:
    def __init__(self, functions = tuple()):
        self.functions = functions

    def __gt__(self, f):
        return Pipeline(self.functions + f)
    
    def __call__(self, arg):
        return functools.reduce(self.functions, lambda r, f: f(r), arg)

