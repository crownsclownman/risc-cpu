import os

import isa.defs
from isa.codegen.c_enums import gen_header

def main():
    out_dir = os.path.join(os.path.dirname(__file__), "generated")
    os.makedirs(out_dir, exist_ok=True)

    out_path = os.path.join(out_dir, "c_enums.h")

    content = gen_header()

    with open(out_path, "w") as f:
        f.write(content)

    print("[isa.generate] written:", out_path)


if __name__ == "__main__":
    main()
