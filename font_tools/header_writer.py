class Writer:
    def __init__(self, file_name, variable_name, first_char, last_char):
        self._file_name = file_name
        self._variable_name = variable_name
        self._first_char = ord(first_char)
        self._last_char = ord(last_char)
        return


    def write(self, font):
        file = open(self._file_name, 'w')
        file.write('#include "../monogfx_font.h"\n')
        file.write('\n')
        self.write_font_bitmap(font, file)
        file.write('\n')
        self.write_font_data(font, file)
        file.write('\n')
        file.write('VariableWidthFont ')
        file.write(self._variable_name)
        file.write(' = VariableWidthFont(\'')
        file.write(chr(self._first_char))
        file.write('\', \'')
        file.write(chr(self._last_char))
        file.write('\', ')
        file.write(str(font.height()))
        file.write(', ')
        file.write(self._variable_name)
        file.write('_PGM);\n');
        file.write('\n')
        file.close()
        return


    def write_font_bitmap(self, font, file):
        file.write('PROGMEM const uint8_t ')
        file.write(self._variable_name)
        file.write('_BITMAP_PGM[] PROGMEM = {\n')
        for char in range(self._first_char, self._last_char):
            self.write_glyph_bitmap(font.glyphs[char], file)

        file.write('};\n')
        return


    def write_glyph_bitmap(self, glyph, file):
        print(glyph.unicode_str() + ' (' + glyph.char() + ')')
        print(glyph)
        file.write('    /* ')
        file.write(glyph.unicode_str())
        file.write(' (')
        file.write(glyph.char())
        file.write(') */    ')
        for byte in glyph.data():
            file.write(hex(byte))
            file.write(', ')

        file.write('\n')
        return


    def write_font_data(self, font, file):
        file.write('PROGMEM const uint8_t ')
        file.write(self._variable_name)
        file.write('_DATA_PGM[] PROGMEM = {\n')
        file.write('    /* bitmap pointer offset, offset x, offset y, width, height */\n')
        for char in range(self._first_char, self._last_char):
            self.write_glyph_data(font.glyphs[char], file)

        file.write('};\n')
        return


    def write_glyph_data(self, glyph, file):
        file.write('    /* ')
        file.write(glyph.unicode_str())
        file.write(' (')
        file.write(glyph.char())
        file.write(') */    ')
        file.write(str(glyph.bounding_box.left()))
        file.write(', ')
        file.write(str(glyph.bounding_box.bottom()))
        file.write(', ')
        file.write(str(glyph.bounding_box.width()))
        file.write(', ')
        file.write(str(glyph.bounding_box.height()))
        file.write(',\n')
        return


def write(file_name, variable_name, font, first_char = '!', last_char = '~'):
    Writer(file_name, variable_name, first_char, last_char).write(font)
    return
