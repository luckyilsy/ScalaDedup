#include "import.h"
#include "PBC.h"
#include "io.h"
#include "struct.h"
#include "crypto.h"
#include <pbc/pbc.h>
#include "postgres.h"

using namespace std;




void firstUploadClient(string filepath,unsigned char* hf,unsigned char* sh,CloudServer *pCloud,Client *pClient){
	element_s* kfe = new element_s();
	element_init_Zr(kfe, pCloud->pairing);
	element_random(kfe);
	auto kf_ = genKey128Bit(kfe);
	auto kf = XOR(hf,kf_,MD5_LENGTH);
	AESEncFile(kf, filepath);
	cout<<"first upload key: "<<unsignedCharEncode(kf,MD5_LENGTH)<<endl;
	auto iF = genRandom128Bit();
	auto iF_ = XOR(hf,iF,MD5_LENGTH);
	sendToCloud(sh,iF_);
	auto kti = genShamirKeyShare(kfe, 2, 5, pCloud->pairing);
	auto seq = pClient->genSequence(5, 10, hf);
	sendToKs(kti,iF);
}


void duplicateCheck(unsigned char* sh,Result_Package* result_P){
	vector<unsigned char*> iF_s = getIF_s(sh);
	if(iF_s.empty()){
		result_P->dr = 0;
	}else{
		result_P->dr = 1;
		result_P->IF = iF_s;
	}

}

void SecondUploadClient(string filename,vector<unsigned char*> iF_s ,unsigned char* hf,unsigned char* sh,Client *pClient,CloudServer *pServer){
	vector<unsigned char*> iFs;
	for(auto x:iF_s){
		iFs.push_back(XOR(x,hf,MD5_LENGTH));
	}
	auto seq = pClient->genSequence(5, 10, hf);
	std::vector<ShamirKey *> *kfi = new std::vector<ShamirKey*>();
	getKfi(kfi,iFs,pServer);
	element_s* ekf=recoverShamirKeyShare(kfi,pServer->pairing);
	unsigned char* kff_ = genKey128Bit(ekf);
	auto kff = XOR(hf,kff_,MD5_LENGTH);
	cout<<"second upload key: "<<unsignedCharEncode(kff,MD5_LENGTH)<<endl;
	AESEncFile(kff, filename);
}

