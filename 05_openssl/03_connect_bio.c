#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdlib.h>
#include <string.h>

void
usage();

int
main(int argc, char *argv[])
{
	if(argc != 2) {
		usage();
	}

	BIO *connect_bio = BIO_new_connect(argv[1]);
	BIO_set_conn_port(connect_bio, "80");

	BIO *stdout_bio = BIO_new_fp(stdout, BIO_NOCLOSE);

	if (!connect_bio || !stdout_bio) {
		printf("Filed to open one of the BIOs\n");
		printf("BIO error\n");
		return EXIT_FAILURE;
	}

	char http_get[] = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";

	if(BIO_do_connect(connect_bio) <= 0) {
		printf("Failed to connect\n");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	BIO_write(connect_bio, http_get, strlen(http_get));
	BIO_flush(connect_bio);

	char in_buf[1024];

	for(int len = 0;;) {
		len = BIO_read(connect_bio, in_buf, 1024);
		if(len <= 0) {
			break;
		}

		BIO_write(stdout_bio, in_buf, len); 
	}

	BIO_flush(stdout_bio);

	BIO_free(connect_bio);
	BIO_free(stdout_bio);

	return EXIT_SUCCESS;
}

void
usage()
{
	printf("Usage:\n\t03 [DNS name]\n\n\
A program to connect to a TCP server on port 80 via a connect BIO\n\n");
	exit(EXIT_FAILURE);

}

