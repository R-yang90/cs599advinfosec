////============================================================================
//// Name        : a1t10.cpp
//// Author      :
//// Version     :
//// Copyright   :
//// Description : Hello World in C++, Ansi-style
////============================================================================
//
//#include <openssl/conf.h>
//#include <openssl/evp.h>
//#include <openssl/err.h>
//#include <string.h>
//#include <fstream>
//#include <iostream>
//using namespace std;
//
//void handleErrors(void)
//{
//  ERR_print_errors_fp(stderr);
//  abort();
//}
//
//int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
//  unsigned char *iv, unsigned char *ciphertext)
//{
//  EVP_CIPHER_CTX *ctx;
//
//  int len;
//
//  int ciphertext_len;
//
//  /* Create and initialise the context */
//  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//  /* Initialise the encryption operation. IMPORTANT - ensure you use a key
//   * and IV size appropriate for your cipher
//   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
//   * IV size for *most* modes is the same as the block size. For AES this
//   * is 128 bits */
//  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
//    handleErrors();
//
//  /* Provide the message to be encrypted, and obtain the encrypted output.
//   * EVP_EncryptUpdate can be called multiple times if necessary
//   */
//  if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
//    handleErrors();
//  ciphertext_len = len;
//
//  /* Finalise the encryption. Further ciphertext bytes may be written at
//   * this stage.
//   */
//  if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
//  ciphertext_len += len;
//
//  /* Clean up */
//  EVP_CIPHER_CTX_free(ctx);
//
//  return ciphertext_len;
//}
//
//int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
//  unsigned char *iv, unsigned char *plaintext)
//{
//  EVP_CIPHER_CTX *ctx;
//
//  int len;
//
//  int plaintext_len;
//
//  /* Create and initialise the context */
//  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();
//
//  /* Initialise the decryption operation. IMPORTANT - ensure you use a key
//   * and IV size appropriate for your cipher
//   * In this example we are using 256 bit AES (i.e. a 256 bit key). The
//   * IV size for *most* modes is the same as the block size. For AES this
//   * is 128 bits */
//  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
//    handleErrors();
//
//  /* Provide the message to be decrypted, and obtain the plaintext output.
//   * EVP_DecryptUpdate can be called multiple times if necessary
//   */
//  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))handleErrors();
//  plaintext_len = len;
//
//  /* Finalise the decryption. Further plaintext bytes may be written at
//   * this stage.
//   */
//  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
//  plaintext_len += len;
//
//  /* Clean up */
//  EVP_CIPHER_CTX_free(ctx);
//
//  return plaintext_len;
//}
//
//
//int main (void)
//{
//
//  /* A 256 bit key */
//  unsigned char *key = (unsigned char *)"00112233445566778889aabbccddeeff";
//
//  /* A 128 bit IV */
//  unsigned char *iv = NULL;
//
//  /* Message to be encrypted */
//
//
//  		ifstream myfile;
//  		myfile.open ("file.txt");
//
//  			string line;
//  			string lines;
//  			while ( getline (myfile,line) ){
//  			  lines += line;
//  			  lines += "\n";
//  			}
//
//  			int length = lines.length()*sizeof(char);
//  			char *plaintextFile = new char[length];
//  			const char *plaintext_tmp = lines.c_str();
//  			strcpy(plaintextFile,plaintext_tmp);
//
//  			unsigned char * plaintext1 = (unsigned char*)plaintextFile;
//
//  			cout << plaintext1;
//
//  			int plaintext_len = strlen((char *)plaintext1);
//
//
//
////  unsigned char *plaintext =
////                (unsigned char *)"Hello world!\nHello world!";
//
//
//  unsigned char ciphertext[128];
//  unsigned char decryptedtext[128];
//
//  int decryptedtext_len, ciphertext_len;
//
//  ERR_load_crypto_strings();
//  OpenSSL_add_all_algorithms();
//  OPENSSL_config(NULL);
//
//
//  ciphertext_len = encrypt (plaintext1, strlen ((char *)plaintext1), key, iv,
//                            ciphertext);
//
//  printf("Ciphertext is:\n");
//  //BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);
//  printf("%s\n", ciphertext);
//
//  /* Decrypt the ciphertext */
//  decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv, decryptedtext);
//
//  /* Add a NULL terminator. We are expecting printable text */
//  decryptedtext[decryptedtext_len] = '\0';
//
//  /* Show the decrypted text */
//  printf("Decrypted text is:\n");
//  printf("%s\n", decryptedtext);
////
////  /* Clean up */
////  EVP_cleanup();
////  ERR_free_strings();
//
//  return 0;
//}
//
//============================================================================
// Name        : assignment1task10.cpp
//============================================================================

//
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

	unsigned char * intext1;
	int intext_len;

	if (myfile.is_open()){
		string line;
		string lines;

		while ( getline (myfile,line) ){
		  lines += line;
		  lines += '\n';
		}

		int length = lines.length()*sizeof(char);
		char *intext = new char[length];
		const char *intext_tmp = lines.c_str();
		strcpy(intext,intext_tmp);

		intext1 = (unsigned char*)intext;

		intext_len = strlen((char *)intext1);
		myfile.close();



		ERR_load_crypto_strings();
		OpenSSL_add_all_algorithms();
		OPENSSL_config(NULL);

		//if(strcmp(argv[6], "-e") == 0){

			unsigned char ciphertext[1024] = "";
			int c_len = encrypt (intext1, intext_len, key, ciphertext, type);

			//cout << ciphertext;

		//}
		//else{
			unsigned char plaintext[1024] = "";
			int p_len;
			//cout << intext1;

			//p_len = decrypt(intext1, intext_len, key, plaintext, type);
			p_len = decrypt(ciphertext, c_len, key, plaintext, type);
			plaintext[p_len] = '\0';

			//cout << plaintext;

			cout << "------------Original data from [ " << argv[2] << " ]"<< endl;
			cout << intext1 << endl;

			cout << "------------Encrypt data with " << argv[5] << endl;
			cout << ciphertext << endl;

			cout << "------------Decrypt data with " << argv[5] << endl;
			cout << plaintext << endl;

		//}

	}
	else {
		cout << "Please specify an existing file.\n";
	}




  EVP_cleanup();
  ERR_free_strings();

  return 0;
}




//int main (int argc, char **argv)
//{
//	//argv[1] -in
//	//argv[2] input filename
//	//argv[3] -k option
//	//argv[4] key
//	//argv[5] -ecb or -cbc
//	//argv[6] -e or -d
//	//argb[7] output filename
//	//128bit k: 11234567891123456
//
//	if (argc < 8) {
//		cout << "Usage: cryptotool -in input -k key -ecb[-cbc] -e[-d] output\n";
//		return 0;
//	} else if (strcmp(argv[1], "-in") != 0) {
//		cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
//		return 0;
//	} else if (strcmp (argv[3], "-k") != 0){
//		cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
//		return 0;
//	} else if (strcmp (argv[5], "-cbc") != 0 || strcmp (argv[5], "-ecb") != 0) {
//		if (strcmp (argv[5], "-cbc") != 0 && strcmp (argv[5], "-ecb") != 0) {
//			cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
//			return 0;
//		}
//	} else if (strcmp (argv[6], "-e") != 0 || strcmp (argv[6], "-d") != 0) {
//		if (strcmp (argv[6], "-e") != 0 && strcmp (argv[6], "-d") != 0) {
//			cout << "Usage: cryptotool -in file -k key -ecb[-cbc]\n";
//			return 0;
//		}
//	}
//
//
//	int type;
//	if(strcmp (argv[5], "-cbc") == 0){
//		type = cbc;
//	}
//	else{
//		type = ecb;
//	}
//
//	unsigned char *key = (unsigned char *) argv[4];
//
//	ERR_load_crypto_strings();
//	OpenSSL_add_all_algorithms();
//	OPENSSL_config(NULL);
//
//	if(strcmp(argv[6], "-e") == 0){
//		//encryption:
//		ifstream myfile;
//		myfile.open (argv[2]);
//
//		if (myfile.is_open()){
//			string line;
//			string lines;
//			while ( getline (myfile,line) ){
//			  lines += line;
//			  lines += "\n";
//			}
//
//			int length = lines.length()*sizeof(char);
//			char *plaintext = new char[length];
//			const char *plaintext_tmp = lines.c_str();
//			strcpy(plaintext,plaintext_tmp);
//
//			unsigned char * plaintext1 = (unsigned char*)plaintext;
//
//			cout << plaintext1;
//
//			int plaintext_len = strlen((char *)plaintext1);
//
//			unsigned char ciphertext[1000] = "";
//
//			encrypt(plaintext1, plaintext_len, ciphertext, key, type);
//			cout << ciphertext;
//
//			ofstream fout;
//			fout.open(argv[7]);
//			fout << ciphertext;
//			fout.close();
//
//			//myfile.close();
//			delete[] plaintext1;
//
//		}
//			else {
//			cout << "Please specify an existing file.\n";
//		}
//
//	}//end of encryption
//
//	else{
//		//decryption
//
//
//	}
//
//	return 0;
//}
