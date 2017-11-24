
class Font:
    def __init__(self):
        self.comments = []
        self.glyphs = []
        return


class Glyph:
    """
    Represents a glyph.
    """
    def __init__(self):
        self.bitmap = []
        self._code = 0
        self.width = 0
        return

    def width(self):
        """
        Returns the actual width of this glyph in pixels.
        """
        return 0


    def char(self):
        """
        Returns the character represented by this glyph as string.
        """
        return chr(self._code)


    def unicode_str(self):
        """
        Returns the unicode code point of this glyph, e.g. Ux0032.
        """
        return 'U+{0:0{1}x}'.format(self._code, 4)


    def __repr__(self):
        return 'Glyph(' + unicode_str() + ')'


    def __str__(self):
        """
        Returns a textual representation of this glyph.
        """
        result = ''
        for row in self.bitmap:
            line = ''
            for byte in row:
                for bit in range(0, 8):
                    if byte & (0x80 >> bit):
                        line += 'X'
                    else:
                        line += ' '

            result += line + '\n'

        return result


class Reader:
    """
    Reads a font from a Glyph Bitmap Distribution Format (BDF) file.
    """
    def __init__(self):
        self._font = Font()
        self._current_glyph = None
        self._bitmap = False

    def parse_comment(self, value):
        self._font.comments.append(value)

    def parse_font_name(self, value):
        self._font.name = value

    def parse_start_glyph(self, value):
        self._current_glyph = Glyph()
        self._font.glyphs.append(self._current_glyph)

    def parse_encoding(self, value):
        self._current_glyph._code = int(value)

    def parse_bbx(self, value):
        parts = value.split(' ')
        self._current_glyph.width = int(parts[0])
        self._current_glyph.height = int(parts[1])
        self._current_glyph.offset_x = int(parts[2])
        self._current_glyph.offset_y = int(parts[3])

    def parse_bitmap_start(self, value):
        self._bitmap = True
    
    def parse_bitmap(self, line):
        line = bytearray.fromhex(line.replace(' ', ''))
        self._current_glyph.bitmap.append(line)

    def read(self, iterable):
        table = {
            'COMMENT': self.parse_comment,
            'FONT_NAME': self.parse_font_name,
            'STARTCHAR': self.parse_start_glyph,
            'ENCODING': self.parse_encoding,
            'BBX': self.parse_bbx,
            'BITMAP': self.parse_bitmap_start
        }

        for line in iterable:
            # remove trailing newline character
            line = line[:-1]
            if self._bitmap:
                if line == 'ENDCHAR':
                    self._bitmap = False
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
