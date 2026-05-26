"""
Точка входа для всех генераторов ISA.

    python -m isa.generate               # запустить все
    python -m isa.generate c_enums       # только один
    python -m isa.generate c_enums codec # несколько
    python -m isa.generate --list        # показать доступные
    python -m isa.generate --out-dir /some/path c_enums

По умолчанию результаты пишутся в isa/generated/.
"""

from __future__ import annotations
import argparse
import os
import sys

import isa.defs          # заполняет реестр инструкций
import isa.codegen       # импортирует __init__ с реестром генераторов


def _default_out_dir() -> str:
    return os.path.join(os.path.dirname(__file__), "generated")


def main() -> None:
    parser = argparse.ArgumentParser(
        prog="python -m isa.generate",
        description="Кодогенератор ISA. Без аргументов запускает все генераторы.",
    )
    parser.add_argument(
        "generators",
        nargs="*",
        metavar="NAME",
        help="Имена генераторов (по умолчанию — все)",
    )
    parser.add_argument(
        "--out-dir",
        default=_default_out_dir(),
        metavar="DIR",
        help=f"Директория для результатов (по умолчанию: isa/generated/)",
    )
    parser.add_argument(
        "--list",
        action="store_true",
        help="Показать доступные генераторы и выйти",
    )
    args = parser.parse_args()

    if args.list:
        names = isa.codegen.list_generators()
        if names:
            print("Доступные генераторы:")
            for n in names:
                print(f"  {n}")
        else:
            print("Генераторов не найдено.")
        sys.exit(0)

    os.makedirs(args.out_dir, exist_ok=True)

    targets = args.generators or isa.codegen.list_generators()

    errors = []
    for name in targets:
        try:
            isa.codegen.run_one(name, args.out_dir)
        except KeyError as e:
            errors.append(str(e))
        except Exception as e:
            errors.append(f"[{name}] {e}")

    if errors:
        for err in errors:
            print(err, file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
