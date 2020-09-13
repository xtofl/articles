from pipeline import Pipeline
import pytest

def test_everything_starts_with_an_empty_pipeline():
    """empty pipeline is the identity function"""
    p = Pipeline()
    assert all(p(x) == x for x in (1, 2, "abcd", None))

def test_pipeline_steps_are_applied_in_order():
    inc = lambda x: x+1
    double = lambda x: x*2

    pipeline = Pipeline() | inc | double
    assert pipeline(0) == (0+1) * 2
    assert pipeline(3) == (3+1) * 2
