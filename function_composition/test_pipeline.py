from pipeline import ID
from pipeline import WithArg
import pytest


def test_everything_starts_with_the_identity_function():
    assert all(ID(x) == x for x in (1, 2, "abcd", None))


def inc(x): return x+1
def double(x): return x*2
def from_hex(s): return int(s, 16)


def test_pipeline_steps_are_applied_in_order():
    pipeline = ID | inc | double
    assert pipeline(0) == (0+1) * 2
    assert pipeline(3) == (3+1) * 2
    assert (ID | from_hex | inc | double)("0x1F") == (31+1)*2


def test_an_almost_natural_pipeline_calling_order():
    assert WithArg(10)(ID | inc | double) == (10+1) * 2
    assert WithArg(10) | (ID | inc | double) == (10+1) * 2
    assert WithArg(10) >> ID * inc * double == (10+1) * 2
