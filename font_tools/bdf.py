class BoundingBox:
    """
    Represents a bounding box of a glyph.
    """
    def __init__(self):
        self._left = 0
        self._bottom = 0
        self._width = 0
        self._height = 0
        return


    def bottom(self) -> int:
        """
        Returns the y coordinate of the bottom border of the bounding box.
        """
        return self._bottom


    def byte_height(self) -> int:
        return 1 + (self._height - 1) // 8


    def byte_width(self) -> int:
        return 1 + (self._width - 1) // 8


    def contains(self, x: int, y: int) -> bool:
        """
        Checks if the coordinates (x, y) are inside the bounding box.
        """
        return self._left <= x < self.right() and self._bottom <= y < self.top()


    def height(self) -> int:
        """
        Returns the height of the bounding box.
        """
        return self._height


    def left(self) -> int:
        """
        Returns the x coordinate of the left border of the bounding box.
        """
        return self._left


    def parse(self, value: str) -> None:
        parts = value.split(' ')
        self._width = int(parts[0])
        self._height = int(parts[1])
        self._left = int(parts[2])
        self._bottom = int(parts[3])
        return


    def right(self) -> int:
        """
        Returns the first x coordinate beyond the right border of the bounding box.
        """
        return self._left + self._width


    def top(self) -> int:
        """
        Returns the first y coordinate beyond the top border of the bounding box.
        """
        return self._bottom + self._height


    def width(self) -> int:
        """
        Returns the width of the bounding box.
        """
        return self._width


    def __str__(self):
        """
        Returns a textual representation of this glyph.
        """
        return ' '.join([str(self._width), str(self._height), str(self._left), str(self._bottom)])


class Font:
    """
    Represents a bitmap font.
    """
    def __init__(self):
        self.comments = []
        self.glyphs = {}
        self.bounding_box = BoundingBox()
        return


    def bytes_per_column() -> int:
        return (self.bounding_box.height() + 1) // 8


    def height(self) -> int:
        return self.bounding_box.height()


class Glyph:
    """
    Represents a bitmap glyph.
    """
    def __init__(self, font):
        self._code = 0
        self._data = bytearray()
        self._font = font
        self.bounding_box = BoundingBox()
        return


    def bit_set(self, x: int, y: int):
        bbx = self.bounding_box
        byte_width = bbx.byte_width()
        if bbx.contains(x, y):
            x -= bbx.left()
            y -= bbx.bottom()
            y = bbx.height() - y - 1
            bit = x % 8
            pos = y * byte_width + x // 8
            return self._data[pos] & (0x80 >> bit) > 0
        else:
            return False


    def char(self):
        """
        Returns the character represented by this glyph as string.
        """
        return chr(self._code)


    def data(self):
        return self._data


    def width(self):
        """
        Returns the actual width of this glyph in pixels.
        """
        return self._width


    def unicode_str(self):
        """
        Returns the unicode code point of this glyph, e.g. Ux0032.
        """
        return 'U+{0:0{1}x}'.format(self._code, 4)


    def __repr__(self):
        return 'Glyph(' + self.unicode_str() + ')'


    def __str__(self):
        """
        Returns a textual representation of this glyph.
        """
        result = ''
        bbx = self._font.bounding_box
        for y in range(bbx.bottom(), bbx.top()):
            line = ''
            for x in range(bbx.left(), bbx.right()):
                if self.bit_set(x, y):
                    line += 'X'
                else:
                    line += '.'
            result = line + '\n' + result
        return result


class Reader:
    """
    Reads a font from a Glyph Bitmap Distribution Format (BDF) file.
    """
    def __init__(self):
        self._current_glyph = None
        self._font = Font()
        self._parsing_bitmap = False
        return


    def parse_comment(self, value: str):
        self._font.comments.append(value)
        return


    def parse_font_name(self, value: str):
        self._font.name = value
        return


    def parse_start_glyph(self, value: str):
        self._current_glyph = Glyph(self._font)
        return


    def parse_encoding(self, value: str):
        parts = value.split(' ')
        code = int(parts[0])
        if code == -1:
            # not a standard Adobe encoding
            code = int(parts[1])

        self._current_glyph._code = code
        self._font.glyphs[code] = self._current_glyph
        return


    def parse_font_bounding_box(self, value):
        self._font.bounding_box.parse(value)
        return


    def parse_bbx(self, value: str):
        self._current_glyph.bounding_box.parse(value)
        return


    def parse_bitmap_start(self, value: str):
        self._parsing_bitmap = True
        return


    def parse_bitmap(self, line: str):
        for byte in bytearray.fromhex(line.replace(' ', '')):
            self._current_glyph._data.append(byte)
        return
    
    def parse_bitmap_end(self):
        self._parsing_bitmap = False
        return


    def read(self, iterable):
        table = {
            'COMMENT': self.parse_comment,
            'FONT_NAME': self.parse_font_name,
            'STARTCHAR': self.parse_start_glyph,
            'ENCODING': self.parse_encoding,
            'FONTBOUNDINGBOX': self.parse_font_bounding_box,
            'BBX': self.parse_bbx,
            'BITMAP': self.parse_bitmap_start
        }

        for line in iterable:
            # remove trailing newline character
            line = line[:-1]
            if self._parsing_bitmap:
                if line == 'ENDCHAR':
                    self.parse_bitmap_end()
                else:
                    self.parse_bitmap(line)
            else:
                parts = line.strip().split(' ')
                key = parts[0]
                value = ' '.join(parts[1:])
                if key in table:
                    table[key](value)
                else:
                    pass
                    #print('Ignoring unknown line starting with ' + key)

        return self._font


def read(iterable):
    if isinstance(iterable, str):
        iterable = open(iterable, 'r')

    return Reader().read(iterable)
