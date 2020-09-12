from pipeline import Pipeline
import pytest

def test_everything_starts_with_an_empty_pipeline():
    p = Pipeline()
    assert p(1) == 1