//============================================================================
// Name        : assignment1task10.cpp
//============================================================================

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

enum {ecb = 0, cbc = 1};

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext, unsigned char *key, int type)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int ciphertext_len;
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	if(type == cbc){
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL)) handleErrors();
		if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
		ciphertext_len = len;
	}
	else if (type == ecb){
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) handleErrors();
		if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
		ciphertext_len = len;
	}

	return ciphertext_len;

}


int main (int argc, char **argv)
{
	//argv[1] -in
	//argv[2] input filename
	//argv[3] -k option
	//argv[4] key
	//argv[5] -ebc or -cbc
	//argv[6] -e or -d
	//argb[7] output filename

	if (argc < 8) {
		cout << "Usage: cryptotool -in input -k key -ecb[-cbc] -e[-d] output\n";
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
	} else if (strcmp (argv[6], "-e") != 0 || strcmp (argv[6], "-d") != 0) {
		if (strcmp (argv[6], "-e") != 0 && strcmp (argv[6], "-d") != 0) {
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


	if(strcmp(argv[6], "-e") == 0){
		//encryption:
		ifstream myfile;
		myfile.open (argv[2]);

		if (myfile.is_open()){
			string line;
			string lines;
			while ( getline (myfile,line) ){
			  lines += line;
			  lines += "\n";
			}

			int length = lines.length()*sizeof(char);
			char *plaintext = new char[length];
			const char *plaintext_tmp = lines.c_str();
			strcpy(plaintext,plaintext_tmp);

			unsigned char * plaintext1 = (unsigned char*)plaintext;

//			cout << plaintext1;

			int plaintext_len = strlen((char *)plaintext1);

			unsigned char * ciphertext = new unsigned char[10];

			encrypt(plaintext1, plaintext_len, ciphertext, key, type);
			cout << ciphertext;

			ofstream fout;
			fout.open(argv[7]);
			fout << ciphertext;
			fout.close();

			//myfile.close();
			delete[] plaintext1;

		}
			else {
			cout << "Please specify an existing file.\n";
		}

	}//end of encryption

	else{
		//decryption


	}

	return 0;
}


