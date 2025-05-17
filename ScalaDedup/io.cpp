#include "io.h"

void write128BitUnsignedChar(string filepath, unsigned char* data){
    ofstream outfile;
    outfile.open(filepath);
    if(outfile.is_open()){
        outfile << data;
    }else{
        cout << "open error" << endl;
    }
}

unsigned char* read128BitUnsignedChar(string filepath){
    unsigned char* data = new unsigned char[17];
    ifstream infile;
    infile.open(filepath);
    if(infile.is_open()){
        infile >> data;
    }else{
        cout << "open error" << endl;
    }
    return data;
}

string charEncode(char* data){
    string str;
    for(int i = 0; i < strlen(data); i++){
        int code = data[i] + 228;
        str += to_string(code);
    }
    return str;
}

char* charDecode(string str){
    int len = str.size() / 3;
    char* data = new char[len];
    for(int i = 0; i < len; i++){
        int code = 0;
        code += (str[3*i] - '0') * 100;
        code += (str[3*i + 1] - '0') * 10;
        code += str[3*i + 2] - '0';
        data[i] = code - 228;
    }
    data[len] = '\0';
    return data;
}

string unsignedCharEncode(unsigned char* data, int len){
    string str;
    for(int i = 0; i < len; i++){
        int code = data[i] + 100;
        str += to_string(code);
    }
    return str;
}

unsigned char* unsignedCharDecode(string str){
    int len = str.size() / 3;
    unsigned char* data = new unsigned char[len];
    for(int i = 0; i < len; i++){
        int code = 0;
        code += (str[3*i] - '0') * 100;
        code += (str[3*i + 1] - '0') * 10;
        code += str[3*i + 2] - '0';
        data[i] = code - 100;
    }
    data[len] = '\0';
    return data;
}

unsigned char* unsignedCharDecode(string str, unsigned char* data){
    int len = str.size() / 3;
    data = new unsigned char[len];
    for(int i = 0; i < len; i++){
        int code = 0;
        code += (str[3*i] - '0') * 100;
        code += (str[3*i + 1] - '0') * 10;
        code += str[3*i + 2] - '0';
        data[i] = code - 100;
    }
    data[len] = '\0';
    return data;
}

bool writeStringAppend(string filepath, string str){
    ofstream outfile;
    outfile.open(filepath, ios::app);
    if(outfile.is_open()){
        outfile << str;
        outfile << '\n';
        return true;
    }else{
        return false;
    }
}

void unsignedCharCopy(unsigned char* str1, unsigned char* str2) {
	for (int i = 0; i < 32; i++) {
		str1[i] = str2[i];
	}
	str1[32] = '\0';
}

string readString(string filepath){
    ifstream infile;
    infile.open(filepath);
    string str = "";
    if(infile.is_open()){
        infile >> str;
    }
    return str;
}

bool writeString(string filepath, string str){
    ofstream outfile;
    outfile.open(filepath);
    if(outfile.is_open()){
        outfile << str;
        return true;
    }else{
        return false;
    }
}

string getElementString(element_t& elem){
    int elem_size = element_length_in_bytes_compressed(elem);
    unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
    element_to_bytes_compressed(elem_uc, elem);
    return unsignedCharEncode(elem_uc, elem_size);
}

string getElementString(element_s* elem){
    if (!elem) return "";
    int elem_size = element_length_in_bytes(elem);
    unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
    element_to_bytes(elem_uc, elem);
    return unsignedCharEncode(elem_uc, elem_size);
}

element_s* getStringElement(string elem_s,pairing_s *pairing){
    if (elem_s == "") return nullptr;
    unsigned char* elem_uc = unsignedCharDecode(elem_s);
    element_s* elem = new element_s();
    element_init_Zr(elem, pairing);
    element_from_bytes(elem,elem_uc);
    return elem;
}
unsigned char* getElementBytes(element_s* elem) {
    int elem_size = element_length_in_bytes_compressed(elem);
    unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
    element_to_bytes_compressed(elem_uc, elem);
    return elem_uc;
}

string getElementStringCompressed(element_t& elem){
    int elem_size = element_length_in_bytes_compressed(elem);
    unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
    element_to_bytes_compressed(elem_uc, elem);
    return unsignedCharEncode(elem_uc, elem_size);
}

string getElementString(element_t& elem, bool isCompressed){
    if(isCompressed == true){
        return getElementStringCompressed(elem);
    }else{
        int elem_size = element_length_in_bytes(elem);
        unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
        element_to_bytes(elem_uc, elem);
        return unsignedCharEncode(elem_uc, elem_size);
    }
}


string getZpString(element_s* elem){
    int elem_size = element_length_in_bytes(elem);
    unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
    element_to_bytes(elem_uc, elem);
    return unsignedCharEncode(elem_uc, elem_size);
}

string getGString(element_s* elem){
    int elem_size = element_length_in_bytes_compressed(elem);
    unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
    element_to_bytes_compressed(elem_uc, elem);
    return unsignedCharEncode(elem_uc, elem_size);
}

unsigned char* getElementByte(element_s* elem){
    int elem_size = element_length_in_bytes_compressed(elem);
    unsigned char* elem_uc = (unsigned char*)pbc_malloc(elem_size);
    element_to_bytes_compressed(elem_uc, elem);
    return elem_uc;
}

void printElement(string name, element_s* elem){
    element_printf("%B\n", elem);
}

void printElement(element_s* elem){
    element_printf("%B\n", elem);
}

int genRandomElementInZr(string filepath){
    char param[1024];
    FILE* file = fopen("param/f.param", "r");
    size_t count = fread(param, 1, 1024, file);
    fclose(file);
    if (!count) {
        pbc_die("input error");
        return -1;
    }
    pairing_t e;
    pairing_init_set_buf(e, param, count);

    // generate sk0 and sk1
    element_t sk0, sk1;
    element_init_Zr(sk0, e);
    element_init_Zr(sk1, e);

    element_random(sk0);
    element_random(sk1);

    string sk0_str = getElementString(sk0, false);
    string sk1_str = getElementString(sk1, false);

    cout << sk0_str << endl;
    cout << sk1_str << endl;

    writeString(filepath + "gen_sk0.txt", sk0_str);
    writeString(filepath + "gen_sk1.txt", sk1_str);

    element_clear(sk0);
    element_clear(sk1);

    return 0;
}

int genRandomElementInG(string filepath){
    char param[1024];
    FILE* file = fopen("param/f.param", "r");
    size_t count = fread(param, 1, 1024, file);
    fclose(file);
    if (!count) {
        pbc_die("input error");
        return -1;
    }
    pairing_t e;
    pairing_init_set_buf(e, param, count);

    // generate sk0 and sk1
    element_t g1, g2;
    element_init_G1(g1, e);
    element_init_G2(g2, e);

    element_random(g1);
    element_random(g2);

    string sk0_str = getElementString(g1, true);
    string sk1_str = getElementString(g2, true);

    cout << sk0_str << endl;
    cout << sk1_str << endl;

    writeString(filepath + "gen_g1.txt", sk0_str);
    writeString(filepath + "gen_g2.txt", sk1_str);

    element_clear(g1);
    element_clear(g2);

    return 0;
}

int get_elem_length(element_s* elem){
    if (!elem) return 0;
    return element_length_in_bytes(elem); 
}

bool sendFile(const string& filename, const string& server_ip, int port) {
    // 创建socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket创建失败");
        return false;
    }

    // 设置服务器地址
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        perror("地址转换失败");
        close(sock);
        return false;
    }

    // 连接服务器
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("连接服务器失败");
        close(sock);
        return false;
    }

    // 打开文件
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        close(sock);
        return false;
    }

    // 获取文件大小
    streamsize file_size = file.tellg();
    file.seekg(0, ios::beg);

    // 发送文件名(不含路径)
    size_t last_slash = filename.find_last_of("/\\");
    string pure_filename = (last_slash == string::npos) ? 
                          filename : filename.substr(last_slash + 1);
    
    if (write(sock, pure_filename.c_str(), pure_filename.size() + 1) <= 0) {
        perror("发送文件名失败");
        file.close();
        close(sock);
        return false;
    }

    // 发送文件大小
    if (write(sock, &file_size, sizeof(file_size)) <= 0) {
        perror("发送文件大小失败");
        file.close();
        close(sock);
        return false;
    }

    // 发送文件内容
    char buffer[1024 * 1024]; 
    while (file_size > 0) {
        size_t to_read = min(file_size, (streamsize)sizeof(buffer));
        file.read(buffer, to_read);
        
        ssize_t sent = write(sock, buffer, to_read);
        if (sent <= 0) {
            perror("发送文件内容失败");
            file.close();
            close(sock);
            return false;
        }
        file_size -= sent;
    }

    file.close();
    close(sock);
    return true;
}

unsigned char* genKey128Bit(element_s *kf_e){
    int elem_l = element_length_in_bytes(kf_e);
    unsigned char* kf_c = new unsigned char[elem_l];
    element_to_bytes(kf_c,kf_e);
    unsigned char* kf = new unsigned char[MD5_LENGTH];
    memcpy(kf,kf_c,MD5_LENGTH);
    return kf;
}