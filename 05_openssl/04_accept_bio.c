#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char *argv[])
{

	BIO *accept_bio = BIO_new_accept("80");

	BIO *stdout_bio = BIO_new_fp(stdout, BIO_NOCLOSE);

	if (!accept_bio || !stdout_bio) {
		printf("Failed to open one of the BIOs\n");
		printf("BIO error\n");
		return EXIT_FAILURE;
	}

	char http_response[] = "HTTP/1.1 200 OK\r\n\r\n<html><body>Hello World!</body></html>\r\n\r\n";

	//setup
	if(BIO_do_accept(accept_bio) <= 0) {
		printf("Failed to accept\n");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	//accept incoming connections
	if(BIO_do_accept(accept_bio) <= 0) {
		printf("Failed to accept\n");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}

	BIO *new_connection_bio = BIO_pop(accept_bio);

	char in_buf[1024];

	printf("----Received:\n");

	for(int len = 0;;) {
		len = BIO_read(new_connection_bio, in_buf, 1024);

		if(len <= 0) {
			break;
		}


		BIO_write(stdout_bio, in_buf, len);

		//dirty hack that can indeed fail
		if(len > 4 && strncmp(&in_buf[len - 4], "\r\n\r\n", 4) == 0) {
			break;
		}
	}

	BIO_flush(stdout_bio);

	BIO_write(new_connection_bio, http_response, strlen(http_response));
	BIO_flush(new_connection_bio);

	BIO_free(new_connection_bio);
	BIO_free(accept_bio);
	BIO_free(stdout_bio);

	return EXIT_SUCCESS;
}


