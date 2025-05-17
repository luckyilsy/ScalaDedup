#include <iostream>
#include <string>
#include <pqxx/pqxx> 
#include <optional>
#include <thread>
#include <future>
using namespace std;

size_t len = MD5_LENGTH;

std::unique_ptr<pqxx::connection> sql_connect(
    const std::string& host,
    const std::string& port,
    const std::string& dbname,
    const std::string& user,
    const std::string& password) 
{
    std::ostringstream oss;
    oss << "host=" << host << " "
        << "port=" << port << " "
        << "dbname=" << dbname << " "
        << "user=" << user << " "
        << "password=" << password;
    std::string conn_str = oss.str();
    
    try {
        auto conn = std::make_unique<pqxx::connection>(conn_str);
        if (!conn->is_open()) {
            throw std::runtime_error("Failed to connect to database");
        }
        return conn; 
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw;
    }
}
bool sql_insert(pqxx::connection& conn, 
               const std::string& table_name,
               const std::string& iF,
               const std::string& x_val,
               const std::string& y_val,
               const std::string& ki_val,
               int count_val) 
{
    try {
        pqxx::work txn(conn);
        txn.exec_params(
            "INSERT INTO " + txn.quote_name(table_name) + " (iff, x,y,ki,count) "
            "VALUES ($1, $2, $3, $4, $5)",
            iF, x_val, y_val, ki_val, count_val
        );
        txn.commit();
        return true;
    } 
    catch (const std::exception& e) {
        std::cerr <<  e.what() << std::endl;
        return false;
    }
}

bool sql_insert(pqxx::connection& conn, 
               const std::string& table_name,
               const std::string& sh,
               const std::string& iF_) {
    try {
        pqxx::work txn(conn);
        txn.exec_params(
            "INSERT INTO " + txn.quote_name(table_name) + " (sh,iff) "
            "VALUES ($1, $2)",
            sh,iF_
        );
        txn.commit();
        return true;
    } 
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

}

void perform_insert(const string& iF_s, ShamirKey* e1, 
                   const string& host, const string& port,
                   const string& db, const string& user, 
                   const string& password) {
    string x_s = getElementString(e1->x);
    string y_s = getElementString(e1->y);
    string ki_s = getElementString(e1->ki);
    auto conn = sql_connect(host, port, db, user, password);
    if(conn) {
        sql_insert(*conn, "iF-ki", iF_s, x_s,y_s, ki_s,e1->minKeyCount);
    }
}
vector<string> get_iF_with_sh(pqxx::connection& conn, string x) {
    vector<string> results;
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec_params("SELECT * FROM " + txn.quote_name("sh-iF") + "WHERE sh = $1",x);
        for (const auto& row : res) {
            results.push_back(row[1].c_str());
        }
        txn.commit();
        
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
    return results;
}

ShamirKey*  get_kfi_with_iF(string iFs,const string& host, const string& port,
                   const string& db, const string& user, 
                   const string& password,CloudServer *pServer) {
    ShamirKey* kfi = new ShamirKey();
    try {
        auto conn = sql_connect(host, port, db, user, password);
        pqxx::work txn(*conn);
        pqxx::result res = txn.exec_params("SELECT * FROM " + txn.quote_name("iF-ki") + "WHERE iff = $1",iFs);
        for (const auto& row : res) {
            kfi->x = getStringElement(row[1].c_str(),pServer->pairing);
            kfi->y = getStringElement(row[2].c_str(),pServer->pairing);
            kfi->ki = getStringElement(row[3].c_str(),pServer->pairing);
            kfi->minKeyCount = row[4].as<int>();
        }
        txn.commit();
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
    return kfi;
}
void sendToKs(std::vector<ShamirKey *>* kti,unsigned char *iF){
    auto it = kti->begin();
    ShamirKey* e1 = *it;
    std::advance(it, 1);     
    ShamirKey* e2 = *it;
    string iF_s = unsignedCharEncode(iF, len);
    std::vector<std::thread> threads;
    threads.emplace_back(perform_insert, iF_s, e1, 
                        "ip", "5432", 
                        "appdb1", "admin1", 
                        "password");
    threads.emplace_back(perform_insert, iF_s, e2, 
                        "ip", "5433", 
                        "appdb2", "admin2", 
                        "password");
    for (auto& t : threads) {
        t.join();
    }
}

void sendToCloud(unsigned char * sh,unsigned char * iF_){
    auto conn = sql_connect("ip", "5434", "appdb3", "admin3", "password");
    string iF__s = unsignedCharEncode(iF_, MD5_LENGTH);
    string sh_s = unsignedCharEncode(sh, SHORTHASH_LENGTH);
    sql_insert(*conn,"sh-iF",sh_s,iF__s);

}

vector<unsigned char*> getIF_s(unsigned char* sh){
    vector<unsigned char*>  iF_s;
    string sh_s = unsignedCharEncode(sh, SHORTHASH_LENGTH);
    auto conn = sql_connect("ip", "5434", "appdb3", "admin3", "password");
    vector<string> iF_ss = get_iF_with_sh(*conn, sh_s);
    for(auto xx:iF_ss){
        auto tmp = unsignedCharDecode(xx);
        iF_s.push_back(tmp);
    }
    return iF_s;
}


void getKfi(std::vector<ShamirKey *> *kfi,vector<unsigned char*> iFs,CloudServer *pServer){
    vector<string> iFs_s;
    for(auto x:iFs) iFs_s.push_back(unsignedCharEncode(x,MD5_LENGTH));
    std::vector<std::future<ShamirKey *>> futures;  
    std::vector<ShamirKey *> results;      
    for(auto x:iFs_s){
        futures.emplace_back(std::async(std::launch::async, get_kfi_with_iF, 
                                   iFs_s[0], "ip", "5432", 
                                   "appdb1", "admin1", "password",pServer));

        futures.emplace_back(std::async(std::launch::async, get_kfi_with_iF, 
                                    iFs_s[0], "ip", "5433", 
                                    "appdb2", "admin2", "password",pServer));
    }    
    for (auto& fut : futures) {
        kfi->push_back(fut.get());  
    }
}