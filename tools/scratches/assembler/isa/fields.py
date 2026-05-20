from dataclasses import dataclass

@dataclass
class Field:
    name: str
    shift: int
    width: int
