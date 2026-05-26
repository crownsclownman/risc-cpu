"""
Реестр генераторов. Каждый файл в codegen/ регистрирует себя через:

    from isa.codegen import generator

    @generator("c_enums")
    def run(out_dir: str) -> None:
        ...

generate.py просто делает import isa.codegen и вызывает run_all() / run_one().
"""

from __future__ import annotations
import importlib
import pkgutil
import sys
from pathlib import Path
from typing import Callable

_REGISTRY: dict[str, Callable[[str], None]] = {}

def generator(name: str):
    def decorator(fn: Callable[[str], None]) -> Callable[[str], None]:
        if name in _REGISTRY:
            raise ValueError(f"[codegen] генератор '{name}' уже зарегистрирован")
        _REGISTRY[name] = fn
        return fn
    return decorator

def _load_all() -> None:
    pkg_path = str(Path(__file__).parent)
    pkg_name = __name__   # "isa.codegen"
    for finder, module_name, _ in pkgutil.iter_modules([pkg_path]):
        full_name = f"{pkg_name}.{module_name}"
        if full_name not in sys.modules:
            importlib.import_module(full_name)

def list_generators() -> list[str]:
    _load_all()
    return sorted(_REGISTRY.keys())

def run_one(name: str, out_dir: str) -> None:
    _load_all()
    if name not in _REGISTRY:
        available = ", ".join(sorted(_REGISTRY.keys()))
        raise KeyError(f"[codegen] неизвестный генератор '{name}'. Доступны: {available}")
    _REGISTRY[name](out_dir)

def run_all(out_dir: str) -> None:
    _load_all()
    for name in sorted(_REGISTRY.keys()):
        _REGISTRY[name](out_dir)
