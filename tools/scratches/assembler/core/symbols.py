class SymbolTable:
    def __init__(self):
        self.symbols = {}

    def define(self, name, address):
        self.symbols[name] = address

    def lookup(self, name):
        return self.symbols[name]
