class Field:
    def __init__(
        self,
        source,
        offset,
        width
    ):
        self.source = source
        self.offset = offset
        self.width = width

def field(source, offset, width):
    return Field(
        source,
        offset,
        width
    )
