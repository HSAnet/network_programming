#include <openssl/opensslv.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdlib.h>

int
main()
{
	printf("%s\n", OPENSSL_VERSION_TEXT);
	printf("%s\n", OpenSSL_version(OPENSSL_CFLAGS));
	printf("%s\n", OpenSSL_version(OPENSSL_PLATFORM));

	BIO *file_bio = BIO_new(BIO_s_file());

	if (!file_bio) {
		printf("Unable to create file BIO\n");
		return EXIT_FAILURE;
	}

	if(!BIO_write_filename(file_bio, "file_bio.txt")) {
		ERR_print_errors(file_bio);
		return EXIT_FAILURE;
	}

	BIO_printf(file_bio, "Hello OpenSSL\n");

	BIO_free(file_bio);

	return EXIT_SUCCESS;
}
