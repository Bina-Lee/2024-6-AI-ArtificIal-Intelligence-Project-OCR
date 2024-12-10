class KoreanWord:
    def __init__(self, first, middle, last):
        self.first = first
        self.middle = middle
        self.last = last

def convert_VAR2IDX(var):
    first = int(round(var[0] * 21.0)) - 1
    middle = int(round(var[1] * 23.0)) - 1
    last = int(round(var[2] * 30.0)) - 1

    first = max(0, min(18, first))
    middle = max(0, min(20, middle))
    last = max(0, min(27, last))

    return KoreanWord(first, middle, last)

def convert_IDX2Char(idx):
    unicode_value = 0xAC00 + (idx.first * 21 * 28) + (idx.middle * 28) + idx.last
    return chr(unicode_value)