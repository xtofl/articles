import functools


class Pipeline:
    def __init__(self, functions=tuple()):
        self.functions = functions

    def __mul__(self, f):
        return self | f

    def __or__(self, f):
        return Pipeline(self.functions + (f,))

    def __call__(self, arg):
        return functools.reduce(
            lambda r, f: f(r),  # function
            self.functions,  # iterable
            arg)  # initializer


ID = Pipeline()


class WithArg:
    def __init__(self, value):
        self.value = value

    def __call__(self, p: Pipeline):
        return p(self.value)

    def __or__(self, p: Pipeline):
        return p(self.value)

    def __rshift__(self, p: Pipeline):
        return p(self.value)
