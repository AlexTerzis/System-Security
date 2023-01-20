#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include<arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define FAIL    -1
int OpenConnection(const char *hostname, int port)
{
	int server = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in remote= {0};
    remote.sin_addr.s_addr = inet_addr(hostname);
    remote.sin_family = AF_INET;
    remote.sin_port = htons(port);

	if(connect(server,(struct sockaddr *)&remote,sizeof(struct sockaddr_in)) != 0)
	{
		close(server);
		exit(FAIL);
	}
    return server;
}
SSL_CTX* InitCTX(void)
{
	/* Load cryptos, et.al. */
	OpenSSL_add_all_algorithms();
	/* Bring in and register error messages */
	SSL_load_error_strings();
	/* Create new client-method instance */
	SSL_METHOD* method = TLSv1_2_client_method();
	/* Create new context */

	OSSL_LIB_CTX* lctx = OSSL_LIB_CTX_new();
	OSSL_LIB_CTX_load_config(lctx, "openssl_fix.cnf");
    SSL_CTX* ctx = SSL_CTX_new_ex(lctx, NULL, method);
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
void ShowCerts(SSL* ssl)
{
	// den zitite stin ekfwnisi h ektelesi aftis tis sinartishs opote tin vazw se sxoleia
	/* get the server's certificate */
    //if ( cert != NULL )
    //{
    //    printf("Server certificates:\n");
    //    /* */
    //    printf("Subject: %s\n", line);
    //   	/* */
    //    printf("Issuer: %s\n", line);
    //    free(line);
    //}
    //else
    //    printf("Info: No client certificates configured.\n");
}
int main(int count, char *strings[])
{
    char buf[1024] = {0};
    int bytes;
    if ( count != 3 )
    {
        printf("usage: %s <hostname> <portnum>\n", strings[0]);
        exit(0);
    }
    /* */
    /* create new SSL connection state */
    SSL_library_init();
	SSL* ssl;
	SSL_CTX* ctx = InitCTX();
	ssl = SSL_new(ctx);
		/* attach the socket descriptor */
	int server = OpenConnection(strings[1], atoi(strings[2]));
	SSL_set_fd(ssl, server);
		/* perform the connection */
    if ( SSL_connect(ssl) == FAIL )   /* connection fail */
        ERR_print_errors_fp(stderr);
    else
    {
        char acUsername[16] = {0};
        char acPassword[16] = {0};
        const char *cpRequestMessage = "<Body>\
                               <UserName>%s<UserName>\
                 <Password>%s<Password>\
                 <\Body>";
        printf("Enter the User Name : ");
        scanf("%s",acUsername);
        printf("\n\nEnter the Password : ");
        scanf("%s",acPassword);
				/* construct reply */
        printf("\n\nConnected with %s encryption\n", SSL_get_cipher(ssl));
   			/* get any certs */
        /* encrypt & send message */
		char message[1024] = {0};
        sprintf(message, cpRequestMessage, acUsername,acPassword);   /* construct reply */
		SSL_write(ssl,message, strlen(message));
        /* get reply & decrypt */
        bytes = SSL_read(ssl, buf, sizeof(buf)); /* get reply & decrypt */
		buf[bytes] = '\0';
		printf("%s\n", buf);
	      /* release connection state */
		SSL_free(ssl);
    }
		/* close socket */
	close(server);
		/* release context */
	SSL_CTX_free(ctx);
    return 0;
}
