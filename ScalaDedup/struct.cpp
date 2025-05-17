#include "struct.h"



CloudServer::CloudServer() : CloudServer(make_pair(2, 5), 10){

}

CloudServer::CloudServer(pair<int, int> threshold_, int KS_COUNT_){
    this->threshold = threshold_;
    this->KS_COUNT = KS_COUNT_;
    if(pairing == nullptr){
        char param[1024];
        FILE* file = fopen("param/f.param", "r");
        size_t count = fread(param, 1, 1024, file);
        fclose(file);
        if (!count) {
            pbc_die("input error");
            return;
        }
        pairing = new pairing_s();
        pairing_init_set_buf(pairing, param, count);
    }
    if(sk == nullptr){
        sk = new element_s();
        element_init_Zr(sk, pairing);
        string sk_str = readString("key/sk.txt");
        unsigned char* sk_uc = unsignedCharDecode(sk_str);
        element_from_bytes(sk, sk_uc);
    }
    if(g == nullptr){
        g = new element_s();
        element_init_G1(g, pairing);
        string g_str = readString("key/g1.txt");
        unsigned char* g_uc = unsignedCharDecode(g_str);
        element_from_bytes_compressed(g, g_uc);
    }

    if(pk == nullptr){
        pk = new element_s();
        element_init_G1(pk, pairing);
        string pk_str = readString("key/pk.txt");
        unsigned char* pk_uc = unsignedCharDecode(pk_str);
        element_from_bytes_compressed(pk, pk_uc);
    }
}

void CloudServer::print(){
    cout << "threshold : " << threshold.first << " " << threshold.second << endl;
    cout << "KS count : " << KS_COUNT << endl;
    printElement("g", this->g);
    printElement("sk", this->sk);
    printElement("pk", this->pk);
    
}

bool CloudServer::duplicateCheck(unsigned char* sh, Package* pPackage){
    
    auto i = data.find(sh);
    if(i != data.end()){
        pPackage = i->second;
        return true;
    }else{
        pPackage = nullptr;
        return false;
    }

}


// void CloudServer::duplicateCheck(string sh, Result_Package* result_P){
//     auto i = dataNew.find(sh);
//     if(i != dataNew.end()){
//         result_P->package = i->second;
//         result_P->dr = 1;
//     }else{
//         result_P->package = nullptr;
//         result_P->dr = 0;
//     }

// }

Client::Client() : Client(1){}

Client::Client(int id_){
    id = id_;
}

set<int> Client::genSequence(int t, int KS_COUNT, unsigned char* h){
    unsigned int sd = 0, p = 0;
    while(h[p] != '\0'){
        sd += h[p];
        sd = sd << 8;
        p++;
    }
    srand(sd);
    set<int> ans;
    while(ans.size() < t){
        int d = rand() % KS_COUNT;
        ans.insert(d);
    }
    return ans;
}













