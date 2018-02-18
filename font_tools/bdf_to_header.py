import bdf
import header_writer

import argparse


INPUT_FILE_NAME = '../bdf/Helvetica-18.bdf'
OUTPUT_FILE_NAME = '../fonts/helvetica_18.h'
VARIABLE_NAME = 'HELVETICA_18'


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--name', dest='name', help='font name')
    name = parser.parse_args().name
    input_file = '../bdf/' + name.lower() + '.bdf'
    output_file = '../fonts/' + name.lower() + '.h'
    print('Converting', input_file, 'to', output_file)
    font = bdf.read(input_file)
    header = header_writer.write(output_file, name, font)

if __name__ == "__main__":
    main()
