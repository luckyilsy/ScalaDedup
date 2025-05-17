#include "import.h"
#include "PBC.h"
#include "io.h"
#include "struct.h"
#include "crypto.h"
#include "scalaDedup.h"
#include <iostream>

using namespace std;


int main(){
    cout<<"filename: ";
    string filename;
    cin>>filename;
    
    //init
    auto pCloud = new CloudServer();
    auto pClient = new Client();
    auto hF = pClient->hF = getFileMD5(filename);
    auto sh = pClient->sh = getShortHash(hF);

    //dedup check
    Result_Package *result_P = new Result_Package();
    duplicateCheck(sh,result_P);

    if(result_P->dr == 0){
        firstUploadClient(filename,hF,sh,pCloud,pClient);
    }else{
        SecondUploadClient(filename,result_P->IF,hF,sh,pClient,pCloud);
    }

}
