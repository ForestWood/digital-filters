/**
 * ENEL429 FIR and IIR Filter Implementation
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "fixedptc.h"

#define FIR_FITLER
#ifdef FIR_FITLER
#define INPUT_COEFFICIENT_LENGTH 24
#define OUTPUT_COEFFICIENT_LENGTH  1

const fixedpt input_coeffs[INPUT_COEFFICIENT_LENGTH] = {
    fixedpt_rconst(-0.001923),
    fixedpt_rconst(-0.012422),
    fixedpt_rconst(-0.024923),
    fixedpt_rconst(-0.021554),
    fixedpt_rconst(0.0076206),
    fixedpt_rconst(0.036441),
    fixedpt_rconst(0.0199),
    fixedpt_rconst(-0.042904),
    fixedpt_rconst(-0.075707),
    fixedpt_rconst(0.0096843),
    fixedpt_rconst(0.20137),
    fixedpt_rconst(0.36548),
    fixedpt_rconst(0.36548),
    fixedpt_rconst(0.20137),
    fixedpt_rconst(0.0096843),
    fixedpt_rconst(-0.075707),
    fixedpt_rconst(-0.042904),
    fixedpt_rconst(0.0199),
    fixedpt_rconst(0.036441),
    fixedpt_rconst(0.0076206),
    fixedpt_rconst(-0.021554),
    fixedpt_rconst(-0.024923),
    fixedpt_rconst(-0.012422),
    fixedpt_rconst(-0.001923)
};
const fixedpt output_coeffs[] = {fixedpt_rconst(1.0)}; //no output coeffs effectively.




#else//IIR
#define INPUT_COEFFICIENT_LENGTH 15
#define OUTPUT_COEFFICIENT_LENGTH  15
//IIR
#define INPUT_COEFFICIENT_LENGTH 15
#define OUTPUT_COEFFICIENT_LENGTH  15
const fixedpt output_coeffs[OUTPUT_COEFFICIENT_LENGTH] = {
    fixedpt_rconst(5.2474e-06),
    fixedpt_rconst(7.3464e-05),
    fixedpt_rconst(0.00047752),
    fixedpt_rconst(0.0019101),
    fixedpt_rconst(0.0052527),
    fixedpt_rconst(0.010505),
    fixedpt_rconst(0.015758),
    fixedpt_rconst(0.018009),
    fixedpt_rconst(0.015758),
    fixedpt_rconst(0.010505),
    fixedpt_rconst(0.0052527),
    fixedpt_rconst(0.0019101),
    fixedpt_rconst(0.00047752),
    fixedpt_rconst(7.3464e-05),
    fixedpt_rconst(5.2474e-06)
};

const fixedpt input_coeffs[INPUT_COEFFICIENT_LENGTH]= {
    fixedpt_rconst(1),
    fixedpt_rconst(-4.2218),
    fixedpt_rconst(9.8299),
    fixedpt_rconst(-15.429),
    fixedpt_rconst(17.907),
    fixedpt_rconst(-15.997),
    fixedpt_rconst(11.246),
    fixedpt_rconst(-6.2758),
    fixedpt_rconst(2.7795),
    fixedpt_rconst(-0.96819),
    fixedpt_rconst(0.26027),
    fixedpt_rconst(-0.052216),
    fixedpt_rconst(0.0073736),
    fixedpt_rconst(-0.00065475),
    fixedpt_rconst(2.7536e-05)
}; // Don't need first coefficient as it is 1.
#endif


fixedpt xn[INPUT_COEFFICIENT_LENGTH] = {fixedpt_fromint(0)};
fixedpt yn[OUTPUT_COEFFICIENT_LENGTH] = {fixedpt_fromint(0)};


/**
* Insert an element into the start of an array, and move all other
* elements right once, discarding the last element. Acts like a queue.
*/
 void insertIntoArray(fixedpt value, fixedpt array[], int length){
   int i;
    for(i = length - 1; i > 0; i--){
        array[i] = array[i-1];
    }
    array[0] = value;
}


/**
*	Generalised IIR filter.
*/
fixedpt IIR(fixedpt x) {
    fixedpt y = fixedpt_fromint(0);
    int i = 0;

    insertIntoArray(x, xn, INPUT_COEFFICIENT_LENGTH); // Insert x into the start of the input delay line.

    for(i = 0; i < INPUT_COEFFICIENT_LENGTH; i++){
        y = fixedpt_add(y, fixedpt_mul(input_coeffs[i], xn[i]));
    }

    for(i = 0; i < OUTPUT_COEFFICIENT_LENGTH; i++){
        y = fixedpt_sub(y, fixedpt_mul(output_coeffs[i], yn[i]));
    }

    insertIntoArray(y, yn, OUTPUT_COEFFICIENT_LENGTH); // Insert y into the start of the ouput delay line.

   return y;
}

/**
*	Generalised FIR filter.
*/
fixedpt FIR(fixedpt x)
{
	// Save the incoming value in the x array
	insertIntoArray(x, xn, INPUT_COEFFICIENT_LENGTH); // Insert x into the start of the input delay line.

	fixedpt y = fixedpt_fromint(0);
	int i = 0;
    
	for (i = 0; i < INPUT_COEFFICIENT_LENGTH; i++) {
        y = fixedpt_add(y, fixedpt_mul(xn[i], input_coeffs[i]));
	}

	return y;
}


/**
 * Open a file (robustly with error checking).
 * @param filename the file to open
 * @param mode read, write, or some combination
 * @return an open FILE* pointer
 */
FILE* fileopen(char* filename, char* mode) {
    FILE* file;
    file = fopen(filename, mode);
    if (file == NULL) {
        printf("Couldn't open file '%s'", filename);
        exit(1);
    }
    return file;
}

/**
 * The main program executes the filter on each input value in turn.
 */
int main(int argc, char* argv[]) {
    FILE* infile = stdin;
    FILE* outfile = stdout;
    char line[BUFSIZ];
    fixedpt x;
    fixedpt y;

    if (argc > 3) {
        printf("Usage: iir [input file] [output file]\n");
        exit(1);
    }
    if (argc > 2) {
        outfile = fileopen(argv[2], "w");
        infile = fileopen(argv[1], "r");
    }
    if (argc > 1) {
        infile = fileopen(argv[1], "r");
    }

    while(fgets(line, BUFSIZ, infile)) {
         if (sscanf(line, "%d", &x) == 1) {

            #ifdef FIR
                y = FIR(fixedpt_rconst(x));
            #else
                y = IIR(fixedpt_rconst(x));
            #endif

            char strToPrint[100];
            fixedpt_str(y, strToPrint, -2);
            fprintf(outfile, "%s\n", strToPrint);
        } else {
            printf("Error reading input.\n");
            break;
        }
    }

    fclose(outfile);
    fclose(infile);
    exit(0);
}
