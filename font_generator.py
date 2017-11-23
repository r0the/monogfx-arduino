"""
MonoGFX Font Converter

Converts a bitmap font represented as a PNG image to a MonoGFX C++ font file for use with
the MonoGFX Arduino library.

"""

FONT_NAME = 'FONT_VICTORIA_8x8'
OUTPUT_FILE_NAME = 'fonts/victoria_8x8.h'
INPUT_FILE_NAME = 'Victoria.png'
GLYPH_WIDTH = 8
GLYPH_HORIZONTAL_SPACING = 1
GLYPH_HEIGHT = 8
FIRST_CHARACTER = '!'
LAST_CHARACTER = '~'
FOREGROUND_COLOR = 15

import png
import sys

def init_dest_file():
    return


def parse_glyph(char, rows, x, y, glyph_width, glyph_height):
    global output_file
    xi = 0
    output_file.write('    /* U+00')
    output_file.write(hex(char)[2:])
    output_file.write(' (')
    output_file.write(chr(char))
    output_file.write(') */    ')
    while xi < glyph_width:
        yi = 0
        byte = 0
        while yi < glyph_height:
            pixel = rows[y+yi][x+xi]
            if pixel == FOREGROUND_COLOR:
                byte += 1 << yi

            yi += 1

        output_file.write(hex(byte))
        output_file.write(', ')
        xi += 1

    output_file.write('\n')
    return


def parse_file(file_name, glyph_width, glyph_height, glyph_horizontal_spacing):
    global png_data
    global rows

    reader = png.Reader(file_name)
    png_data = reader.read()
    width = png_data[0]
    height = png_data[1]
    rows = []
    for row in png_data[2]:
        rows.append(row)

    y = 0
    char = ord(FIRST_CHARACTER)
    while y + glyph_height < height and char <= ord(LAST_CHARACTER):
        x = 0
        while x + glyph_width < width and char <= ord(LAST_CHARACTER):
            parse_glyph(char, rows, x, y, glyph_width, glyph_height)
            char += 1
            x += glyph_width + glyph_horizontal_spacing

        y += glyph_height

    return


def main():
    global output_file
    output_file = open(OUTPUT_FILE_NAME, 'w')
    output_file.write('#include "../monogfx_font.h"\n')
    output_file.write('\n')
    output_file.write('PROGMEM const uint8_t ')
    output_file.write(FONT_NAME)
    output_file.write('_PGM[] PROGMEM = {\n')

    output_file.write('    ');
    output_file.write(str(GLYPH_WIDTH))
    output_file.write(', // glyph width\n')

    output_file.write('    ');
    output_file.write(str(GLYPH_HEIGHT))
    output_file.write(', // glyph height\n')

    output_file.write('    \'');
    output_file.write(FIRST_CHARACTER);
    output_file.write('\', // first character\n')
    output_file.write('    \'');
    output_file.write(LAST_CHARACTER);
    output_file.write('\', // last character\n')
    parse_file(INPUT_FILE_NAME, GLYPH_WIDTH, GLYPH_HEIGHT, GLYPH_HORIZONTAL_SPACING)
    output_file.write('};\n')
    output_file.write('\n')
    output_file.write('FixedWidthFont ')
    output_file.write(FONT_NAME)
    output_file.write(' = FixedWidthFont(')
    output_file.write(FONT_NAME)
    output_file.write('_PGM);\n');
    output_file.write('\n')

    output_file.close()
    return


if __name__ == "__main__":
    main()
