#ifndef _DUPLESS_H_
#define _DUPLESS_H_

#include "import.h"
#include "PBC.h"
#include "io.h"
#include "crypto.h"
#include <thread>
#include<future>
#include <pqxx/pqxx>
bool sql_insert(pqxx::connection& conn, 
               const std::string& table_name,
               const std::string& x_val,
               const std::string& y_val,
               const std::string& ki_val,
               int count_val) 
{
    try {
        pqxx::work txn(conn);
        txn.exec_params(
            "INSERT INTO " + txn.quote_name(table_name) + " ( x,y,ki,count) "
            "VALUES ($1, $2, $3, $4)", x_val, y_val, ki_val, count_val
        );
        txn.commit();
        return true;
    } 
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}


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

void perform_insert(ShamirKey* e1, 
                   const string& host, const string& port,
                   const string& db, const string& user, 
                   const string& password) {
    string x_s = getElementString(e1->x);
    string y_s = getElementString(e1->y);
    string ki_s = getElementString(e1->ki);
    auto conn = sql_connect(host, port, db, user, password);
    if(conn) {
        sql_insert(*conn, "ki", x_s,y_s, ki_s,e1->minKeyCount);
    }
}

ShamirKey*  get_kfi(const string& host, const string& port,
                   const string& db, const string& user, 
                   const string& password,pairing_s *pairing) {
    ShamirKey* kfi = new ShamirKey();
    try {
        auto conn = sql_connect(host, port, db, user, password);
        pqxx::work txn(*conn);
        pqxx::result res = txn.exec_params("SELECT * FROM " + txn.quote_name("ki"));
        for (const auto& row : res) {
            kfi->x = getStringElement(row[0].c_str(),pairing);
            kfi->y = getStringElement(row[1].c_str(),pairing);
            kfi->ki = getStringElement(row[2].c_str(),pairing);
            kfi->minKeyCount = 2;
        }
        txn.commit();
    } catch (const exception &e) {
        cerr <<  e.what() << endl;
    }
    return kfi;
}

void KS(pairing_s *pairing){
    std::vector<std::future<ShamirKey *>> futures;  
    std::vector<ShamirKey *> *results =new std::vector<ShamirKey *>();      
    futures.emplace_back(std::async(std::launch::async, get_kfi, 
                                 "8.130.14.143", "5432", 
                                "appdb1", "admin1", "mysecretpassword",pairing));
    futures.emplace_back(std::async(std::launch::async, get_kfi, 
                                "8.130.14.143", "5433", 
                                "appdb2", "admin2", "mysecretpassword",pairing));
    for (auto& fut : futures) {
        results->push_back(fut.get()); 
    }
}

void sendSki(element_s *sk,pairing_s *e){
    auto kti = genShamirKeyShare(sk, 2, 5, e);
	auto it = kti->begin();
    ShamirKey* e1 = *it;
    std::advance(it, 1);     
    ShamirKey* e2 = *it;
    std::vector<std::thread> threads;
    threads.emplace_back(perform_insert, e1, 
                        "8.130.14.143", "5432", 
                        "appdb1", "admin1", 
                        "mysecretpassword");
    threads.emplace_back(perform_insert , e2, 
                        "8.130.14.143", "5433", 
                        "appdb2", "admin2", 
                        "mysecretpassword");
    for (auto& t : threads) {
        t.join();
    }
}
