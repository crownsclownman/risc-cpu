TOKEN_IDENT  = "IDENT"
TOKEN_NUMBER = "NUMBER"
TOKEN_COMMA  = "COMMA"
TOKEN_COLON  = "COLON"
TOKEN_EOL    = "EOL"

class Token:
    def __init__(self, typ, value):
        self.typ = typ
        self.value = value

def tokenize(line):
    tokens = []

    current = ""

    i = 0

    while i < len(line):
        c = line[i]

        if c in " \t":
            if current:
                tokens.append(make_token(current))
                current = ""

            i += 1
            continue

        if c == ",":
            if current:
                tokens.append(make_token(current))
                current = ""

            tokens.append(Token(TOKEN_COMMA, ","))

            i += 1
            continue

        if c == ":":
            if current:
                tokens.append(make_token(current))
                current = ""

            tokens.append(Token(TOKEN_COLON, ":"))

            i += 1
            continue

        current += c

        i += 1

    if current:
        tokens.append(make_token(current))

    tokens.append(Token(TOKEN_EOL, None))

    return tokens

def make_token(text):
    if text.isdigit():
        return Token(TOKEN_NUMBER, int(text))

    return Token(TOKEN_IDENT, text)
