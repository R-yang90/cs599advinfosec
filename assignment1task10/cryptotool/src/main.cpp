#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <fstream>
#include <iostream>
using namespace std;
enum {ecb = 0, cbc = 1};


void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int encrypt(unsigned char *plaintext, int p_len, unsigned char *key, unsigned char *ciphertext, int type)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int c_len;


	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	if(type == cbc){
	  if(EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL) != 1){
		  handleErrors();
	  }
	}

	else if(type == ecb){
	  if(EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL) != 1){
		  handleErrors();
	  }
	}


	if(EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, p_len) != 1){
	  handleErrors();
	}

	c_len = len;


	if(EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1){
	  handleErrors();
	}

	c_len += len;


	EVP_CIPHER_CTX_free(ctx);

	return c_len;
}

int decrypt(unsigned char *ciphertext, int c_len, unsigned char *key, unsigned char *plaintext, int type)
{
	EVP_CIPHER_CTX *ctx;

	int len;
	int p_len;

	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	if(type == cbc){
	  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL)){
		  handleErrors();
	  }
	}

	else if(type == ecb){
	  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)){
		  handleErrors();
	  }
	}


	if(EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, c_len) != 1){
	  handleErrors();
	}

	p_len = len;


	if(EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1){
	  handleErrors();
	}
	p_len += len;


	EVP_CIPHER_CTX_free(ctx);

	return p_len;
}


int main (int argc, char **argv)
{
	//argv[1] -in
	//argv[2] input filename
	//argv[3] -k option
	//argv[4] key
	//argv[5] -ecb or -cbc
	// DELETE? argv[6] -e or -d
	// DELETE? argb[7] output filename
	//k: 00112233445566778889aabbccddeeff

	if (argc < 6) {
		cout << "Usage: cryptotool -in input -k key -ecb[-cbc]\n";
		return 0;
	} else if (strcmp(argv[1], "-in") != 0) {
		cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
		return 0;
	} else if (strcmp (argv[3], "-k") != 0){
		cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
		return 0;
	} else if (strcmp (argv[5], "-cbc") != 0 || strcmp (argv[5], "-ecb") != 0) {
		if (strcmp (argv[5], "-cbc") != 0 && strcmp (argv[5], "-ecb") != 0) {
			cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
			return 0;
		}
	}

	int type;
	if(strcmp (argv[5], "-cbc") == 0){
		type = cbc;
	}
	else{
		type = ecb;
	}

	unsigned char *key = (unsigned char *) argv[4];


	ifstream myfile;
	myfile.open (argv[2]);
	int intext_len;

	if (myfile.is_open()){
		myfile.seekg(0, myfile.end);
		intext_len = myfile.tellg();
		myfile.seekg(0, myfile.beg);

		unsigned char *intext = new unsigned char[intext_len];
		myfile.read ((char*) intext,intext_len);
		myfile.close();

		cout << "------------Original data from [ " << argv[2] << " ]"<< endl;
		cout << intext << endl;

		ERR_load_crypto_strings();
		OpenSSL_add_all_algorithms();
		OPENSSL_config(NULL);

		if(strcmp(argv[6], "-e") == 0){

			unsigned char ciphertext[1024] = "";
			cout << "------------Encrypt data with " << argv[5] << endl;
			int c_len = encrypt (intext, intext_len, key, ciphertext, type);
			cout << ciphertext << endl;
			BIO_dump_fp (stdout, (char *)ciphertext, c_len);

			ofstream fout;
			fout.open(argv[7]);
			fout.write ((char *)ciphertext,c_len);
			fout.close();
		} else {
			unsigned char plaintext[1024] = "";
			int p_len;

			cout << "------------Decrypt data with " << argv[5] << endl;
			BIO_dump_fp (stdout, (char *)intext, intext_len);
			p_len = decrypt(intext, intext_len, key, plaintext, type);
			plaintext[p_len] = '\0';
			cout << plaintext << endl;

			ofstream fout;
			fout.open(argv[7]);
			fout.write ((char *)plaintext,p_len);
			fout.close();
		}

	}
	else {
		cout << "Please specify an existing file.\n";
	}




  EVP_cleanup();
  ERR_free_strings();

  return 0;
}
