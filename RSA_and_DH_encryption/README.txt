A.M. 201303014 Terzis Alexandros

gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0


========GENERAL=========
-I use the template of exercise exercise 4 from last year, for the core of the tools.
-I used MAKEFILE of exercise 4 from last year and modified it to compile this year's tools.
========================

=========RSA=========

-To save and read the keys I use the functions mpz_inp_raw and mpz_out_raw 

-The numbers needed for the key generation are chosen automaticaly from the program and not from the user, as mentioned in the discussion forum.

-The way that the prime numbers are being generated is as following:
I choose a random number through void mpz_urandomb function, and with the mpz_nextprime function I can assure I have generated a random prime number.

=============================================================

================= DH ================

In the implementation of DH, everything is done as mentioned in the exercise, what is worth being mentioned, is that I used the funcions of the GMP library even though it wasn't necessary.

=============================================================

=======Submitted Files====================

-The submitted folder includes the following:
    public.key
    private.key
    plain_text_input.txt
    plain_text_output.txt
    cipher_text_output.txt
    dh.txt

I constructed the plain_text_input.txt myself and the text written is "Hello there." 
==============================================================================

The rest has occured as following:

-I run ./rsa_assign_1 -g to generate the keys.

- I run ./rsa_assign_1 -e -i plain_text_input.txt -o cipher_text_output.txt -k public.key
to encrypt the encrypt the text plain_text_input.txt

-I run ./rsa_assign_1 -d -i cipher_text_output.txt -o plain_text_output.txt -k private.key
to decrypt the file cipher_text_output.txt that was generated from the lalast command

-I confirmed that the processe is working by comparing plain_text_input.txt and plain_text_output.txt and seeing that they are the same.

-I run ./dh_assign_1 -o dh.txt -p 29 -g 6 -a 2 -b 1 to create the file dh.txt Whith these numbers,my result is 7,6,7 (Public A, Public B, secret)

==============================================================================





