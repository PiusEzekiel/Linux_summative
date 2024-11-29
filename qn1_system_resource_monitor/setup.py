from setuptools import setup, Extension

setup(
    name="metrics",
    version="1.0",
    description="C extension for reading system metrics",
    ext_modules=[
        Extension("metrics", sources=["metrics_reader.c"]),
    ],
)
