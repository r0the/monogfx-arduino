class Writer:
    def __init__(self, file_name, variable_name, first_char, last_char):
        self._bitmap_offset = 0
        self._file_name = file_name
        self._first_char = ord(first_char)
        self._last_char = ord(last_char)
        self._variable_name = variable_name
        self._file = open(file_name, 'w')
        return


    def write(self, font):
        self._file.write('#include "../monogfx_font.h"\n')
        self._file.write('\n')
        self.write_font_bitmap(font)
        self._file.write('\n')
        self.write_font_data(font)
        self._file.write('\n')
        self._file.write('VariableWidthFont ')
        self._file.write(self._variable_name)
        self._file.write(' = VariableWidthFont(\'')
        self._file.write(chr(self._first_char))
        self._file.write('\', \'')
        self._file.write(chr(self._last_char))
        self._file.write('\', ')
        self._file.write(str(font.bounding_box().height()))
        self._file.write(', ')
        self._file.write(self._variable_name)
        self._file.write('_PGM);\n');
        self._file.write('\n')
        self._file.close()
        return


    def write_font_bitmap(self, font):
        self._file.write('PROGMEM const uint8_t ')
        self._file.write(self._variable_name)
        self._file.write('_BITMAP_PGM[] PROGMEM = {\n')
        for char in range(self._first_char, self._last_char):
            self.write_glyph_bitmap(font.glyph(char))

        self._file.write('};\n')
        return


    def write_glyph_bitmap(self, glyph):
        print(glyph.unicode_str() + ' (' + glyph.char() + ')')
        print(glyph)
        self.write_glyph_comment(glyph)
        bit = 1
        result = ''
        for y in glyph.range_y():
            line = ''
            for x in glyph.range_x():
                if glyph.bit_set(x, y):
                    line += 'X'
                else:
                    line += '.'
            result = line + '\n' + result

        self._file.write('\n')
        return


    def write_font_data(self, font):
        self._file.write('PROGMEM const uint8_t ')
        self._file.write(self._variable_name)
        self._file.write('_DATA_PGM[] PROGMEM = {\n')
        self._file.write('    /* bitmap pointer offset, width */\n')
        for char in range(self._first_char, self._last_char):
            glyph = font.glyph(char)
            self.write_glyph_data(glyph)
            self._bitmap_offset += glyph.width() * font.bounding_box().byte_height()

        self._file.write('};\n')
        return


    def write_glyph_comment(self, glyph):
        self._file.write('    /* ')
        self._file.write(glyph.unicode_str())
        self._file.write(' (')
        self._file.write(glyph.char())
        self._file.write(') */    ')
        return


    def write_glyph_data(self, glyph):
        self.write_glyph_comment(glyph)
        self._file.write(str(self._bitmap_offset))
        self._file.write(', ')
        self._file.write(str(glyph.width()))
        self._file.write(', ')
        self._file.write(',\n')
        return


def write(file_name, variable_name, font, first_char = '!', last_char = '~'):
    Writer(file_name, variable_name, first_char, last_char).write(font)
    return
