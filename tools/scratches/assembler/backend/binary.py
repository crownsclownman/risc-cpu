import struct


def write_binary(words, path):
    with open(path, "wb") as f:
        for word in words:
            f.write(struct.pack("<I", word))
