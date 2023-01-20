#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>


void
usage(void)
{
	printf(
			"\n" 
			"usage: ./df_assign_1 [OPTION]...\n"
			"Options:\n"
			"-o     path        Path to output file\n"
			"-p     number      Prime number\n"
			"-g     number      Primitive Root for previous prime number\n"
			"-a     number      Private key A\n"
			"-b     number      Private key B\n"
			"-h                 This help message\n"
			);

	exit(1);
}

void
check_args(char *output_file, mpz_t p, mpz_t g, mpz_t a, mpz_t b)
{
    mpz_t zero;
    mpz_init(zero);

	if ((!output_file)) {
		printf("Error: No output file!\n");
		usage();
	}
    
    if (!(mpz_sgn(p) > 0)) {
        printf("Error: No Prime number!\n");
        usage();
    }
    if (mpz_probab_prime_p(p, 50) != 2)
    {
        printf("Error: p is not prime!\n");
        usage();
    }

    if (!(mpz_sgn(g) > 0)) {
        printf("Error: No Primitive Root!\n");
        usage();
    }

    if (!(mpz_sgn(a) > 0)) {
        printf("Error: No Private key A!\n");
        usage();
    }

    if (mpz_cmp(a, p) >= 0) {
        printf("Error: give a > p!\n");
        usage();
    }

    if (!(mpz_sgn(b) > 0)) {
        printf("Error: No Private key B!\n");
        usage();
    }

    if (mpz_cmp(b, p) >= 0) {
        printf("Error: give b > p!\n");
        usage();
    }

}

int generate_public_key(mpz_t pub, mpz_t p, mpz_t g, mpz_t priv)
{
    return 0;
}

int main(int argc, char **argv)
{
	int opt;
    int error;
	char *output_file;		/* path to the output file         */
	char *key_file;			/* path to the key file            */
	mpz_t p, g, a, b;
	mpz_inits(p, g, a, b, NULL);

	while ((opt = getopt(argc, argv, "o:p:g:a:b:h")) != -1) {
		switch (opt) {
			case 'o':
				output_file = strdup(optarg);
				break;
			case 'p':
				error = mpz_set_str(p, strdup(optarg), 10);
                if (error == -1)
                {
                    usage();
                }
				break;
			case 'g':
				error = mpz_set_str(g, strdup(optarg), 10);
                if (error == -1)
                {
                    usage();
                }
				break;
			case 'a':
				error = mpz_set_str(a, strdup(optarg), 10);
                if (error == -1)
                {
                    usage();
                }
				break;
			case 'b':
				error = mpz_set_str(b, strdup(optarg), 10);
                if (error == -1)
                {
                    usage();
                }
				break;
			case 'h':
			default:
				usage();
		}
	}

	/* check arguments */
    check_args(output_file, p, g, a, b);

    mpz_t pubA, pubB, secret;
    mpz_inits(pubA, pubB, secret, NULL);
    mpz_powm (pubA, g, a, p);
    mpz_powm (pubB, g, b, p);
    mpz_powm (secret, pubA, b, p);

    FILE * outfile;
    outfile = fopen(output_file, "w");
    //char * numChar;
    //numChar = mpz_get_str(NULL, 10, pubA);

    //fprintf(outfile, "%s, ", numChar);
    //free(numChar);

    //numChar = mpz_get_str(NULL, 10, pubB);
    //fprintf(outfile, "%s, ", numChar);
    //free(numChar);

    //numChar = mpz_get_str(NULL, 10, secret);
    //fprintf(outfile, "%s\n", numChar);
    //free(numChar);

    mpz_out_str(outfile, 10, pubA);
    fprintf(outfile,", ");
    mpz_out_str(outfile, 10, pubB);
    fprintf(outfile,", ");
    mpz_out_str(outfile, 10, secret);
    fprintf(outfile,"\n");

    fclose(outfile);

	/* Clean up */
	free(output_file);
	mpz_clears(p, g, a, b, NULL);

	/* END */
	return 0;
}
