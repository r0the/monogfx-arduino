class Writer:
    def __init__(self, file_name, variable_name):
        self._file_name = file_name
        self._variable_name = variable_name
        return


    def write(self, font):
        file = open(self._file_name, 'w')
        file.write('#include "../monogfx_font.h"\n')
        file.write('\n')
        file.write('PROGMEM const uint8_t ')
        file.write(self._variable_name)
        file.write('_PGM[] PROGMEM = {\n')
        file.write('    \'');
        file.write(font.first_glyph());
        file.write('\', // first glyph\n')
        file.write('    \'');
        ile.write(font.last_glyph());
        file.write('\', // last glyph\n')
        for glyph in font.glyphs:
            self.write_glyph(file, glyph)

        file.write('};\n')
        file.write('\n')
        file.write('VariableWidthFont ')
        file.write(self._variable_name)
        file.write(' = VariableWidthFont(')
        file.write(self._variable_name)
        file.write('_PGM);\n');
        file.write('\n')
        file.close()
        return


    def write_glyph(self, file, glyph):
        file.write('    /* ')
        file.write(glyph.unicode_str())
        file.write(' (')
        file.write(glyph.char())
        file.write(') */    ')
        file.write(glyph.width())
        file.write(', ')
        for byte in glyph.bytes():
            file.write(hex(byte))
            file.write(', ')

        file.write('\n')
        return


def write(file_name, variable_name, font):
    Writer(file_name, variable_name).write(font)
    return
