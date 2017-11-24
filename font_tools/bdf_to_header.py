import bdf
import header_writer

INPUT_FILE_NAME = '../bdf/ArtosSans-8.bdf'
OUTPUT_FILE_NAME = 'fonts/artos_sans_8.h'


def main():
    global font
    font = bdf.read(INPUT_FILE_NAME)
    for glyph in font.glyphs:
        print(glyph)

    return

if __name__ == "__main__":
    main()
