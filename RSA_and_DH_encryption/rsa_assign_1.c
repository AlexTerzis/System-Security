#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void
rsa_keygen(void)
{
	mpz_t p, q, n, e, lambda, d;
    mpz_t p_1, q_1, random, seed, gcd;
    gmp_randstate_t state;
    // me megalitoro apo 32, ta noumera pou vgainoun apo to encryption einai poli megalitera apo 64bit
    mp_bitcnt_t bcnt = 32;

    /* init all gmp vars */

    mpz_inits(p, q, n, e, lambda, d, NULL);
    mpz_inits(p_1, q_1, random, seed, gcd, NULL);
    gmp_randinit_lc_2exp_size(state, 32);
    // gmp_randinit_default(state);
    gmp_randseed_ui(state, 15);

    /* choose two random prime numbers */
    
    mpz_urandomb(random, state, bcnt);
    mpz_abs(random, random);
    mpz_nextprime(p, random);

    do {
        mpz_urandomb(random, state, bcnt);
        mpz_abs(random, random);
        mpz_nextprime(q, random);
    }while (mpz_cmp(p, q) == 0);

    /* calculate n */
    mpz_mul(n, p, q);

    /* calculate lambda */
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);
    mpz_mul(lambda, p_1, q_1);
    
    /* choose a prime e */
    do {
        mpz_urandomb(random, state, bcnt);
        mpz_abs(random, random);
        mpz_nextprime(e, random);
        mpz_gcd(gcd, e, lambda);
    }while (mpz_divisible_p(e, lambda) && mpz_cmp_ui(gcd, 1)==0);
    /* choose d */
    mpz_invert(d, e, lambda);

    //mpz_out_str(stdout, 10, n);
    //printf("\n");
    //mpz_out_str(stdout, 10, d);
    //printf("\n");
    //mpz_out_str(stdout, 10, e);
    //printf("\n");
    FILE* pub_file = fopen("public.key", "w");
    FILE* priv_file = fopen("private.key", "w");
    // mpz_out_str(pub_file, 10, n);
    // mpz_out_str(pub_file, 10, d);

    mpz_out_raw(pub_file, n);
    mpz_out_raw(pub_file, d);
    fprintf(pub_file, "\n");
    mpz_out_raw(priv_file, n);
    mpz_out_raw(priv_file, e);
    fprintf(priv_file, "\n");
    fclose(pub_file);
    fclose(priv_file);
    /* clear all gmp vars */
    gmp_randclear(state);
    mpz_clears(p_1, q_1, random, seed, gcd, NULL);
    mpz_clears(p, q, n, e, lambda, d, NULL);
}

/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_encrypt(char *input_file, char *output_file, char *key_file)
{
    FILE * key = fopen(key_file, "r");
    FILE * inp = fopen(input_file, "r");
    FILE * out = fopen(output_file, "w");

	mpz_t n, e, c_gmp, m_gmp;
    mpz_inits(n, e, c_gmp, m_gmp, NULL);

    mpz_inp_raw(n, key);
    mpz_inp_raw(e, key);

    char m;
    size_t c;

    // printf("Closing key file\n");
    fclose(key);

    while ( fread(&m, sizeof(char), 1, inp))
    {
        // mpz_set_si(m_gmp, (int)m);
        mpz_import(m_gmp, 1, 1, sizeof(m), 0, 0, &m);
        // size_t bits = mpz_sizeinbase(c_gmp, 2);
        // printf("%zu\n", bits);
        mpz_powm(c_gmp, m_gmp, e, n);
        mpz_export(&c, NULL, 1, sizeof(c), 0, 0, c_gmp);
        fwrite(&c, sizeof(c), 1, out);
        // mpz_out_raw(out, c_gmp);
    }

    // fprintf(out, "\n");
    mpz_clears(n, e, c_gmp, m_gmp, NULL);
    // printf("Closing input file\n");
    fclose(inp);
    // printf("Closing output file\n");
    fclose(out);
}

void test()
{
	mpz_t n, e, d;
    mpz_inits(n, e, d, NULL);

	/* TODO */
    FILE* pub_file = fopen("public.key", "r");
    FILE* priv_file = fopen("private.key", "r");

    mpz_inp_raw(n, priv_file);
    mpz_inp_raw(n, pub_file);
    mpz_inp_raw(d, pub_file);
    
    mpz_inp_raw(e, priv_file);
    fclose(pub_file);
    fclose(priv_file);
    mpz_out_str(stdout, 10, n);
    printf("\n");
    mpz_out_str(stdout, 10, d);
    printf("\n");
    mpz_out_str(stdout, 10, e);
    printf("\n");
    mpz_clears(n, e, d, NULL);
}

/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_decrypt(char *input_file, char *output_file, char *key_file)
{

    FILE * key = fopen(key_file, "r");
    FILE * inp = fopen(input_file, "r");
    FILE * out = fopen(output_file, "w");

	mpz_t n, e, c_gmp, m_gmp;
    mpz_inits(n, e, c_gmp, m_gmp, NULL);

    mpz_inp_raw(n, key);
    mpz_inp_raw(e, key);

    fclose(key);
    char m;
    size_t c;

    while ( fread(&c, sizeof(c), 1, inp))
    // while (mpz_inp_raw(c_gmp, inp))
    {
        // mpz_set_si(c_gmp, c);
        mpz_import(c_gmp, 1, 1, sizeof(c), 0, 0, &c);
        
        mpz_powm(m_gmp, c_gmp, e, n);
        //void * mpz_export (void *rop, size_t *countp, int order, size_t size, int endian, size_t nails, const mpz_t op)
        mpz_export(&m, NULL, 1, sizeof(m), 0, 0, m_gmp);
        fwrite(&m, sizeof(m), 1, out);
    }

    mpz_clears(n, e, c_gmp, NULL);
    fclose(inp);
    fclose(out);
	/* TODO */

}

void
usage(void)
{
	printf(
			"\n" 
			"usage: ./rsa_assign1 [OPTION]...\n"
			"Options:\n"
			"-i    path      Path to the input file\n"
			"-o    path      Path to the output file\n"
			"-k    path      Path to the key file\n"
			"-g              Perform RSA key-pair generation\n"
			"-d              Decrypt input and store results to output\n"
			"-e              Encrypt input and store results to output\n"
			"-h              This help message\n"
			);

	exit(1);
}

/*
 * Checks the validity of the arguments
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 * arg3: operation mode
 */
void
check_args(char *input_file, char *output_file, char *key_file, int op_mode)
{
	if ((!input_file) && (op_mode != 2)) {
		printf("Error: No input file!\n");
		usage();
	}

	if ((!output_file) && (op_mode != 2)) {
		printf("Error: No output file!\n");
		usage();
	}

	if ((!key_file) && (op_mode != 2)) {
		printf("Error: No user key!\n");
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}

int main(int argc, char **argv)
{
	int opt;
	int op_mode;			/* operation mode                  */
	char *input_file;		/* path to the input file          */
	char *output_file;		/* path to the output file         */
	char *key_file;			/* path to the key file            */

	/* Init arguments */
	input_file = NULL;
	output_file = NULL;
	key_file = NULL;
	op_mode = -1;


	while ((opt = getopt(argc, argv, "i:o:k:gdteh")) != -1) {
		switch (opt) {
			case 'i':
				input_file = strdup(optarg);
				break;
			case 'o':
				output_file = strdup(optarg);
				break;
			case 'k':
				key_file = strdup(optarg);
				break;
			case 'g':
				op_mode = 2;
				break;
			case 'd':
				op_mode = 0;
				break;
			case 'e':
				op_mode = 1;
				break;
			case 't':
                test();
                exit(0);
                break;
			case 'h':
			default:
				usage();
		}
	}

	/* check arguments */
	check_args(input_file, output_file, key_file, op_mode);

	/* serve each mode... */
	switch (op_mode) {
	case 0:
		rsa_decrypt(input_file, output_file, key_file);
		break;
	case 1:
		rsa_encrypt(input_file, output_file, key_file);
		break;
	case 2:
		rsa_keygen();
		break;
	default:
		break;
	}

		

	/* Clean up */
	free(input_file);
	free(output_file);
	free(key_file);


	/* END */
	return 0;
}
