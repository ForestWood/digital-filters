/**
 * ENEL429 FIR and IIR Filter Implementations
 */

#include <stdio.h>
#include <stdlib.h>

// #define FIR_FITLER
#ifdef FIR_FITLER
#define INPUT_COEFFICIENT_LENGTH 24
#define OUTPUT_COEFFICIENT_LENGTH  1

const float input_coeffs[] = {
    -0.001923,
    -0.012422,
    -0.024923,
    -0.021554,
    0.0076206,
    0.036441,
    0.0199,
    -0.042904,
    -0.075707,
    0.0096843,
    0.20137,
    0.36548,
    0.36548,
    0.20137,
    0.0096843,
    -0.075707,
    -0.042904,
    0.0199,
    0.036441,
    0.0076206,
    -0.021554,
    -0.024923,
    -0.012422,
    -0.001923
};

const float output_coeffs[] = {
            1.0
}; 



#else//IIR
#define INPUT_COEFFICIENT_LENGTH 15
#define OUTPUT_COEFFICIENT_LENGTH  15

const float output_coeffs[OUTPUT_COEFFICIENT_LENGTH] = {
    5.2474e-06,
    7.3464e-05,
    0.00047752,
    0.0019101,
    0.0052527,
    0.010505,
    0.015758,
    0.018009,
    0.015758,
    0.010505,
    0.0052527,
    0.0019101,
    0.00047752,
    7.3464e-05,
    5.2474e-06
};

const float input_coeffs[INPUT_COEFFICIENT_LENGTH]= {
    1,
    -4.2218,
    9.8299,
    -15.429,
    17.907,
    -15.997,
    11.246,
    -6.2758,
    2.7795,
    -0.96819,
    0.26027,
    -0.052216,
    0.0073736,
    -0.00065475,
    2.7536e-05
}; // Don't need first coefficient as it is 1.
#endif

float xn[INPUT_COEFFICIENT_LENGTH] = {0.0};
float yn[OUTPUT_COEFFICIENT_LENGTH] = {0.0};


/**
* Insert an element into the start of an array, and move all other
* elements right once, discarding the last element. Acts like a queue.
*/
 void insertIntoArray(float value, float array[], int length){
   int i;
    for(i = length - 1; i > 0; i--){
        array[i] = array[i-1];
    }
    array[0] = value;
}

/**
*	Generalised FIR Filter Code
*/
float FIR(float x)
{
    // Save the incoming value in the x array
    insertIntoArray(x, xn, INPUT_COEFFICIENT_LENGTH); // Insert x into the start of the input delay line.

    float y = 0.0;
    int i = 0;
    
    for (i = 0; i < INPUT_COEFFICIENT_LENGTH; i++) {
        y += xn[i] * input_coeffs[i];
    }

    return y;
}
/**
*	Generalised IIR Filter Code
*/
float IIR(float x) {
    float y = 0;
    int i = 0;

    insertIntoArray(x, xn, INPUT_COEFFICIENT_LENGTH); // Insert x into the start of the input delay line.

    for(i = 0; i < INPUT_COEFFICIENT_LENGTH; i++){
        y += input_coeffs[i] * xn[i];
    }

    for(i = 0; i < OUTPUT_COEFFICIENT_LENGTH; i++){
        y -= output_coeffs[i] * yn[i];
    }

    insertIntoArray(y, yn, OUTPUT_COEFFICIENT_LENGTH); // Insert y into the start of the ouput delay line.

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
    float x;
    float y;
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
        if (sscanf(line, "%f ", &x) == 1) {
            #ifdef FIR_FITLER
                y = FIR(x);
            #else 
                y = IIR(x);
            #endif
            fprintf(outfile, "%f\n", y);
        } else {
             printf("Error reading input.\n");
             break;
        }
    }

     fclose(outfile);
     fclose(infile);
    exit(0);
}
