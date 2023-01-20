#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#define FAIL    -1
// Create the SSL socket and initialize the socket address structure
int OpenListener(int port)
{
    int sd;
    struct sockaddr_in addr;
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
        perror("can't bind port");
        abort();
    }
    if ( listen(sd, 10) != 0 )
    {
        perror("Can't configure listening port");
        abort();
    }
    return sd;
}
int isRoot()
{
    if (getuid() != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
SSL_CTX* InitServerCTX(void)
{
	/* load & register all cryptos, etc. */
	OpenSSL_add_all_algorithms();
	/* load all error messages */
	SSL_load_error_strings();
	/* create new server-method instance */
    SSL_METHOD* method = TLSv1_2_server_method();
	/* create new context from method */

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
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile)
{
    /* set the local certificate from CertFile */
    SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM);
    /* set the private key from KeyFile (may be the same as CertFile) */
	SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM);
    /* verify private key */
    if ( !SSL_CTX_check_private_key(ctx) )
    {
        printf("Private key could be verified\n");
		exit(FAIL);
    }
}

void ShowCerts(SSL* ssl)
{
	// den zitite stin ekfwnisi h ektelesi aftis tis sinartishs opote tin vazw se sxoleia
	/* Get certificates (if available) */
    //if ( cert != NULL )
    //{
    //    printf("Server certificates:\n");
    //    /* */
    //    printf("Subject: %s\n", line);
    //    free(line);
    //    /* */
    //    printf("Issuer: %s\n", line);
    //    free(line);
    //}
    //else
    //    printf("No certificates.\n");
}
void Servlet(SSL* ssl) /* Serve the connection -- threadable */
{
    char buf[1024] = {0};
    int sd, bytes;
    const char* ServerResponse="<\Body>\
                               <Name>sousi.com</Name>\
                 <year>1.5</year>\
                 <BlogType>Embedede and c\c++<\BlogType>\
                 <Author>John Johny<Author>\
                 <\Body>";
    const char *cpValidMessage = "<Body>\
                               <UserName>sousi<UserName>\
                 <Password>123<Password>\
                 <\Body>";
	/* do SSL-protocol accept */
	if ( SSL_accept(ssl) != FAIL )     /* do SSL-protocol accept */
	{
        bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
        buf[bytes] = '\0';
        if ( bytes > 0 )
        {
            if(strcmp(cpValidMessage,buf) == 0)
            {
                SSL_write(ssl, ServerResponse, strlen(ServerResponse)); /* send reply */
            }
            else
            {
                SSL_write(ssl, "Invalid Message", strlen("Invalid Message")); /* send reply */
            }
        }
	}
	else
		/*else print "Invalid Message" */
	{
		printf("Invalid Message\n");
		exit(FAIL);
	}
  
	/* get socket connection */
	sd = SSL_get_fd(ssl);
	/* release SSL state */
	SSL_free(ssl);
  /* close connection */
    close(sd);
}
int main(int count, char *Argc[])
{

//Only root user have the permsion to run the server
    if(!isRoot())
    {
        printf("This program must be run as root/sudo user!!\n");
        exit(0);
    }
    if ( count != 2 )
    {
        printf("Usage: %s <portnum>\n", Argc[0]);
        exit(0);
    }
    // Initialize the SSL library
    SSL_library_init();
    /* initialize SSL */
	SSL_CTX *ctx = InitServerCTX();
    /* load certs */
    LoadCertificates(ctx, "mycert.pem", "mycert.pem");
    /* create server socket */
	int server;
    server = OpenListener(atoi(Argc[1]));
    while (1)
    {
		// logo tou parakato printf katalavaino pos to addr prepei na einai typou sockaddr_in
        struct sockaddr_in addr;
        uint addr_len = sizeof(addr);
			/* accept connection as usual */
		int sock = accept(server,&addr,&addr_len);
        printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
			/* get new SSL state with context */
		SSL* ssl = SSL_new(ctx);
			/* set connection socket to SSL state */
		SSL_set_fd(ssl, sock);
			/* service connection */
		Servlet(ssl);
    }
		/* close server socket */
	close(server);
		/* release context */
	SSL_CTX_free(ctx);
}
