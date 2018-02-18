import bdf
import header_writer

INPUT_FILE_NAME = '../bdf/Helvetica-10.bdf'
OUTPUT_FILE_NAME = '../fonts/helvetica_10.h'
VARIABLE_NAME = 'HELVETICA_10'

font = bdf.read(INPUT_FILE_NAME)

header = header_writer.write(OUTPUT_FILE_NAME, VARIABLE_NAME, font)


#if __name__ == "__main__":
#    main()
