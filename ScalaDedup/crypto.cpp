#include "crypto.h"

unsigned char* getStringMD5(const char* str) 
{
    unsigned char* result = new unsigned char[MD5_DIGEST_LENGTH] {0};
	MD5((unsigned char*)str, strlen(str), result);
    return result;
}


unsigned char* getFileMD5(string filepath)
{
    MD5_CTX c;
    int len=0;
    unsigned char buffer[1024]={0};
    unsigned char* digest = new unsigned char[16];
    FILE *pFile = fopen((char*)filepath.data(), "rb");
    MD5_Init(&c);
    while((len = fread(buffer, 1, 1024, pFile))>0){
        MD5_Update(&c, buffer, len);
    }
    MD5_Final(digest, &c);
    fclose(pFile);
    return digest;
}

unsigned char* XOR(unsigned char* data1,unsigned char* data2,int length){
    unsigned char* result = new unsigned char[MD5_LENGTH];
    for(int i=0;i<length;i++){
        result[i] = data1[i] ^ data2[i];
    }
    return result;
}

unsigned char* getStringSHA256(const char* str)
{
	unsigned char* result = new unsigned char[SHA256_DIGEST_LENGTH];
	SHA256((unsigned char*)str, strlen(str), result);
	return result;
}

unsigned char* getFileSHA256(string filepath)
{
	SHA256_CTX c;
	int len = 0;
	unsigned char buffer[1024] = { 0 };
	unsigned char* digest = new unsigned char[32];
	FILE* pFile = fopen((char*)filepath.data(), "rb");
	SHA256_Init(&c);
	while ((len = fread(buffer, 1, 1024, pFile)) > 0) {
		SHA256_Update(&c, buffer, len);
	}
	SHA256_Final(digest, &c);
	fclose(pFile);
	return digest;
}

unsigned char* getFileSHA256(string filepath, unsigned char* random)
{
	SHA256_CTX c;
	int len = 0;
	unsigned char buffer[1024] = { 0 };
	unsigned char* digest = new unsigned char[32];
	FILE* pFile = fopen((char*)filepath.data(), "rb");
	SHA256_Init(&c);
	while ((len = fread(buffer, 1, 1024, pFile)) > 0) {
		SHA256_Update(&c, buffer, len);
	}
	SHA256_Update(&c, random, 32);
	SHA256_Final(digest, &c);
	fclose(pFile);
	return digest;
}

unsigned char* getShortHash(string filepath){
    unsigned char* res = new unsigned char[SHORTHASH_LENGTH];
    res[SHORTHASH_LENGTH] = '\0';
    unsigned char* md5 = getFileMD5(filepath);
    for(int i = 0; i < SHORTHASH_LENGTH; i++){
        res[i] = md5[i];
    }
    delete md5;
    return res;
}
unsigned char* getShortHash(unsigned char* md5 ){
    unsigned char* res = new unsigned char[SHORTHASH_LENGTH];
    res[SHORTHASH_LENGTH] = '\0';

    for(int i = 0; i < SHORTHASH_LENGTH; i++){
        res[i] = md5[i];
    }

    return res;
}

unsigned char* genRandom256Bit() {
	unsigned char* key = new unsigned char[33]{ 0 };
	int j = 0;
	for (int i = 0; i < 32; i++)		//生成9个随机数，每个随机数最大都是2^15
	{
		int temp1 = rand() % 256;
		key[i] = temp1;
	}
	key[32] = '\0';
	// printHash(key, 32);
	return key;
}

unsigned char* genRandom128Bit() {
	unsigned char* key = new unsigned char[17]{ 0 };
	int j = 0;
	for (int i = 0; i < 16; i++)		//生成9个随机数，每个随机数最大都是2^15
	{
		int temp1 = rand() % 256;
		key[i] = temp1;
	}
	key[16] = '\0';
	// printHash(key, 16);
	return key;
}

unsigned char* genRandom64Bit() {
	unsigned char* key = new unsigned char[9]{ 0 };
	int j = 0;
	for (int i = 0; i < 8; i++)		//生成9个随机数，每个随机数最大都是2^15
	{
		int temp1 = rand() % 256;
		key[i] = temp1;
	}
	key[8] = '\0';
	// printHash(key, 8);
	return key;
}

int AESEncFile(unsigned char* key, string filepath) 
{
    const char enc_suffix[20]=".encrypted";
    //clock_t start, end, tmp1, tmp2;
    //double tmp = 0;
    //start = clock();
    unsigned char iv[EVP_MAX_KEY_LENGTH] = "EVP_AES_CTR";

    EVP_CIPHER_CTX* ctx;    
    int outl, inl, rv, i;
    ctx = EVP_CIPHER_CTX_new();
    unsigned char out[1024];  
    unsigned char in[1024];   

    FILE *fpIn, *fpOut;
    fpIn = fopen(filepath.c_str(), "rb");
    if(fpIn == NULL)
    {
        return -1;
    }

    char encryptedfname[100];
    strcpy(encryptedfname, filepath.c_str());
    strcat(encryptedfname, enc_suffix);
    fpOut = fopen(encryptedfname, "wb");
    if(fpOut == NULL)
    {
        fclose(fpIn);
        return -1;
    }

    EVP_CIPHER_CTX_init(ctx);
    rv = EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);
    if(rv != 1)
    {
        printf("Err\n");
        return -1;
    }

    for(;;)
    {
        inl = fread(in,1,1024,fpIn);
        if(inl <= 0)
            break;
        rv = EVP_EncryptUpdate(ctx, out, &outl, in, inl);
        if(rv != 1)
        {
            fclose(fpIn);
            fclose(fpOut);
            EVP_CIPHER_CTX_cleanup(ctx);
            return -1;
        }
        //tmp1 = clock();
        fwrite(out, 1, outl, fpOut);
        //tmp2 = clock();
        //tmp += ((double)(tmp2 - tmp1)/CLOCKS_PER_SEC)*1000;
    }

    rv = EVP_EncryptFinal_ex(ctx, out, &outl);
    if(rv != 1)
    {
        fclose(fpIn);
        fclose(fpOut);
        EVP_CIPHER_CTX_cleanup(ctx);
        return -1;
    }
    //end = clock();
    //double time = ((double)(end - start)/CLOCKS_PER_SEC)*1000 - tmp;
    //cout << "AESEncFile : "<< time << "ms" << endl;
    //writeStringAppend("log.txt", "AESEncFile :\t" + to_string(time));

    fwrite(out,1,outl,fpOut); 
    fclose(fpIn);
    fclose(fpOut);
    EVP_CIPHER_CTX_cleanup(ctx); 
    return 1;
}

int AESDecFile(unsigned char* key, string filepath)
{
    const char dec_suffix[20]=".decrypted";
    //clock_t start, end, tmp1, tmp2;
    //double tmp = 0;
    //start = clock();

    unsigned char iv[EVP_MAX_KEY_LENGTH] = "EVP_AES_CTR";  
    EVP_CIPHER_CTX* ctx;    
    ctx=EVP_CIPHER_CTX_new();
    unsigned char out[1024+EVP_MAX_KEY_LENGTH];
    int outl, inl, rv, i;
    unsigned char in[1024];   

    FILE *fpIn, *fpOut;

    fpIn = fopen(filepath.c_str(), "rb");
    if(fpIn == NULL)
    {
        return -1;
    }

    char decryptedfname[100];
    strcpy(decryptedfname, filepath.c_str());
    strcat(decryptedfname, dec_suffix);

    fpOut = fopen(decryptedfname, "wb");
    if(fpOut == NULL)
    {
        fclose(fpIn);
        return -1;
    }

    EVP_CIPHER_CTX_init(ctx);
    rv = EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv);
    if(rv != 1)
    {
        EVP_CIPHER_CTX_cleanup(ctx);
        return -1;
    }

    for(;;)
    {
        inl = fread(in, 1, 1024, fpIn);
        if(inl <= 0)
            break;
        rv = EVP_DecryptUpdate(ctx, out, &outl, in, inl);
        if(rv != 1)
        {
            fclose(fpIn);
            fclose(fpOut);
            EVP_CIPHER_CTX_cleanup(ctx);
            return -1;
        }
        //tmp1 = clock();
        fwrite(out, 1, outl, fpOut);
        //tmp2 = clock();
        //tmp += ((double)(tmp2 - tmp1)/CLOCKS_PER_SEC)*1000;
    }

    rv = EVP_DecryptFinal_ex(ctx, out, &outl);
    if(rv != 1)
    {
        fclose(fpIn);
        fclose(fpOut);
        EVP_CIPHER_CTX_cleanup(ctx);
        return -1;
    }
    //end = clock();
    //double time = ((double)(end - start)/CLOCKS_PER_SEC)*1000 - tmp;
    //cout << "AESDecFile : "<< time << "ms" << endl;
    //writeStringAppend("log.txt", "AESDecFile :\t" + to_string(time));
    

    fwrite(out,1,outl,fpOut);
    fclose(fpIn);
    fclose(fpOut);
    EVP_CIPHER_CTX_cleanup(ctx);
    return 1;
}

element_s* genRandomZpElement(pairing_s* e){
    element_s* pElement = new element_s();
    element_init_Zr(pElement, e);
    element_random(pElement);
    return pElement;
}

element_s* genRandomG1Element(pairing_s* e){
    element_s* pElement = new element_s();
    element_init_G1(pElement, e);
    element_random(pElement);
    return pElement;
}

element_s* genRandomG2Element(pairing_s* e){
    element_s* pElement = new element_s();
    element_init_G2(pElement, e);
    element_random(pElement);
    return pElement;
}

element_s* genRandomGTElement(pairing_s* e){
    element_s* pElement = new element_s();
    element_init_GT(pElement, e);
    element_random(pElement);
    return pElement;
}

element_s* intToZp(int n, pairing_s* e){
    mpz_t bn;
    mpz_init_set_str(bn, (char*)to_string(n).c_str(), 10);

    element_s* pElement = new element_s();
    element_init_Zr(pElement, e);
    element_set_mpz(pElement, bn);
    return pElement;
}

vector<ShamirKey*>* genShamirKeyShare(element_s* s, int min, int n, pairing_s* e){

    //clock_t start, end;
    //double time;
    //start = clock();

    vector<ShamirKey*>* ans = new vector<ShamirKey*>();

    //cout << "a0 : ";
    //element_printf("%B\n", s);

    // generate f : s + a1*x + a2*x^2 + ...
    vector<element_s* > f;
    f.push_back(s);
    for(int i = 1; i < min; i++){
        f.push_back(genRandomZpElement(e));
        //f.push_back(intToZp(i + 12, e));
        //cout << "a" << i << " : ";
        //element_printf("%B\n", f[i]);
    }


    element_s* x_times = genRandomZpElement(e);

    for(int i = 0; i < n; i++){

        ShamirKey* ski = new ShamirKey();
        ski->minKeyCount = min;
        ski->x = intToZp(i + 1, e);
        //cout << "x : ";
        //element_printf("%B\n", ski->x);

        element_s* yi = new element_s();
        element_init_same_as(yi, s);
        element_add(yi, yi, s);
     
        for(int j = 1; j < min; j++){
            element_pow_zn(x_times, ski->x, intToZp(j, e));
            element_mul(x_times, x_times, f[j]);
            element_add(yi, yi, x_times);
        }
        ski->y = yi;
        // cout << "y : ";
        // element_printf("%B\n", ski->y);
        // cout<<"密钥份额大小: "<<sizeof(*ski)<<endl;
        ans->push_back(ski); 
        
    }
    // cout<<"sizeof :"<<sizeof(ans)<<endl;

    //end = clock();
    //time = ((double)(end - start)/CLOCKS_PER_SEC)*1000;
    //cout << "genShamir : "<< time << "ms" << endl;
    //writeStringAppend("log.txt", "genShamir :\t" + to_string(time));

    return ans;
}

element_s* recoverShamirKeyShare(vector<ShamirKey*>* ski, pairing_s* e){
    //clock_t start, end;
    //double time;
    //start = clock();

    if(ski == nullptr) {
        cout << "Error::recoverShamirKeyShare(): ski is nullptr." << endl;
        return nullptr;
    }

    int min = ski->at(0)->minKeyCount;

    if(min > ski->size()){
        cout << "Error::recoverShamirKeyShare(): ski is not enough." << endl;
        return nullptr;
    }

    element_s* p = genRandomZpElement(e);
    for(int i = 0; i < min; i++){
        ski->at(i)->ki = intToZp(1, e);
        for(int j = 0; j < min; j++){
            if(j != i){
                // p = 0 - xj
                element_sub(p, intToZp(0, e), ski->at(j)->x);
                // ki = ki * (0 - xj)
                element_mul(ski->at(i)->ki, ski->at(i)->ki, p);
                // p = xi - xj
                element_sub(p, ski->at(i)->x, ski->at(j)->x);
                // ki = ki / (xi - xj)
                element_div(ski->at(i)->ki, ski->at(i)->ki, p);
            }else{
                // ki = ki * yi
                element_mul(ski->at(i)->ki, ski->at(i)->ki, ski->at(i)->y);
            }
        }
    }

    element_s* s = intToZp(0, e);
    for(int i = 0; i < min; i++){
        // s = s + ki
        element_add(s, s, ski->at(i)->ki);
    }

    //end = clock();
    //time = ((double)(end - start)/CLOCKS_PER_SEC)*1000;
    //cout << "recvrShamir : "<< time << "ms" << endl;
    //writeStringAppend("log.txt", "recvrShamir :\t" + to_string(time));

    //cout << "s : ";
    //element_printf("%B\n", s);

    return s;
}
