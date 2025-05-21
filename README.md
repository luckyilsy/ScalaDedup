# Introduction
​	Encrypted deduplication is attractive because it can provide high storage efficiency while protecting data privacy. Most existing schemes achieve encrypted deduplication against brute-force attacks (BFAs) based on server-aided encryption. Unfortunately, the centralized key server in server-aided encryption can potentially become a single point of failure. To this end, distributed server-aided encryption is presented, which splits a system-level master key into multiple shares and distributes them across several key servers.  However, it is hard to improve security and scalability with this method simultaneously. We present a secure and scalable encrypted deduplication scheme **ScalaDep**. **ScalaDep** achieves a new design paradigm centered on location-hiding secret sharing of data keys. As the number of deployed key servers increases, the attack cost of adversaries increases while the number of requests handled by each key server decreases, enhancing both scalability and security. Furthermore, we propose a two-phase duplicate detection method for our paradigm, which utilizes short hashes and key identifiers to achieve secure duplicate detection against BFAs.  Additionally, based on the allreduce algorithm, **ScalaDep** enables all key servers to collaboratively record the number of client requests and resist online BFAs by enforcing rate limiting. Security analysis and performance evaluation demonstrate the security and efficiency of **ScalaDep**.

# ScalaDep Running Guide

## Configuration

To configure the database information for the Cloud Server (CS) and Key Servers (KS) in `./postgres.h`, you need to define the relevant table schemas, field mappings, and relationships. In the code, the table name in the CS database is "sh-iF", with two fields: "sh" and "iff".The table name in the KS database is "iF-ki", with five fields: "iff", "x", "y", "ki", and "count".

# Build

```c++
make
```

# Usage

```
./main
```

- Initial upload
![image](https://github.com/user-attachments/assets/36a41143-f025-44ec-9147-88fa21ee4d91)
- Subsequent upload
![image](https://github.com/user-attachments/assets/0df36d9e-b7a0-4823-94cf-d227beb11622)

