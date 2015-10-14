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
	if (argc < 6) {
		cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
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
	if (myfile.is_open()){
		string line;
		string lines;
		while ( getline (myfile,line) ){
		  //cout << line << '\n';
		  lines += line;
		  lines += "\n";
		}

		int length = lines.length()*sizeof(char);
		char *plaintext = new char[length];
		const char *plaintext_tmp = lines.c_str();
		strcpy(plaintext,plaintext_tmp);

		unsigned char ciphertext[1000] = "";
		unsigned char * plaintext1 = (unsigned char*)plaintext;
		int plaintext_len = strlen((char *)plaintext1);

//		cout << "-----------" <<endl;
//		cout << "plaintext1" << plaintext1 << endl;
//		cout << "plaintext_len" << plaintext_len << endl;
//		cout << "ciphertext" << ciphertext << endl;
//		cout << "key" << key << endl;
//		cout << "type" << type << endl;

		encrypt(plaintext1, plaintext_len, ciphertext, key, type);
		cout << "ciphertext:" <<endl;
		cout << ciphertext;

		myfile.close();
		delete[] plaintext1;

	} else {
		cout << "Please specify an existing file.\n";
	}


	return 0;
}


