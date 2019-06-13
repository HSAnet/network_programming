#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
	BIO *file_bio = BIO_new_file("base64.txt", "w");

	if (!file_bio) {
		printf("Unable to create file BIO\n");
		return EXIT_FAILURE;
	}

	char message[] = "This text will be transformed\n";

	BIO *base64_bio = BIO_new(BIO_f_base64());

	BIO_push(base64_bio, file_bio);

	BIO_write(base64_bio, message, strlen(message));

	BIO_flush(base64_bio);

	BIO_free_all(base64_bio);

	return EXIT_SUCCESS;
}
