#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <limits>
#include <conio.h>
#include <ctime>
#include <chrono>
#include <sstream>
#include<unordered_map>

using namespace std;

// ʹ�����װ�˻�ϵͳ
class AccountSystem {
private:
    // �˻���
    class Account {
    public:
        int id;
        string accountLocation;  // �˻�λ�ã���ַ��
        string description;      // �˻�����
        string username;         // �˻���
        string password;         // ����
        string createTime;       // �˻�����ʱ��

        // ���Ĭ�Ϲ��캯��
        Account() = default;

        Account(int i, const string& loc, const string& desc, const string& user, const string& pass, const string& time)
            : id(i), accountLocation(loc), description(desc), username(user), password(pass), createTime(time) {}
    };

    vector<Account> accounts;
    const string dataFile = "accounts.dat";
    const string masterPassword = "123456";
    int nextId = 1;

    // ���ܺ������Ľ���
    string encrypt(const string& input) {
        string result = input;
        for (char& c : result) {
            c = (c + 3) % 128; // �򵥵���ԭ���Ժõļ���
        }
        return result;
    }

    // ���ܺ���
    string decrypt(const string& input) {
        string result = input;
        for (char& c : result) {
            c = (c - 3 + 128) % 128;
        }
        return result;
    }

    // ��ȫ����������
    string getPasswordInput(const string& prompt) {
        string password;
        cout << prompt;

#ifdef _WIN32
        // Windows�µ�������������
        char c;
        while ((c = _getch()) != '\r') {
            if (c == '\b' && !password.empty()) {
                cout << "\b \b";
                password.pop_back();
            }
            else if (c != '\b') {
                cout << '*';
                password += c;
            }
        }
        cout << endl;
#else
        // Linux/macOS�µ�������������
        system("stty -echo");
        cin >> password;
        system("stty echo");
        cout << endl;
#endif

        return password;
    }

    // ������뻺����
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ���ID�Ƿ�Ψһ
    bool isIdUnique(int id) {
        for (const auto& acc : accounts) {
            if (acc.id == id) return false;
        }
        return true;
    }

    // ����������ɾ�������˻���ԭɾ���߼���
    void deleteSingleAccount() {
        int id;
        cout << "������Ҫɾ�����˻�ID: ";
        if (!(cin >> id)) {
            cout << "��Ч�����롣\n";
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        auto it = find_if(accounts.begin(), accounts.end(),
            [id](const Account& acc) { return acc.id == id; });

        if (it != accounts.end()) {
            string confirm;
            do {
                cout << "��ȷ��Ҫɾ���˻� " << id << " ��? (y/n): ";
                cin >> confirm;
                if (confirm == "y" || confirm == "Y") {
                    accounts.erase(it);
                    cout << "�˻�ɾ���ɹ���\n";
                    break;
                }
                else if (confirm == "n" || confirm == "N") {
                    cout << "ɾ��������ȡ����\n";
                    break;
                }
                else {
                    cout << confirm << "����Ч���룬����������!\n";
                }
            } while (true);
        }
        else {
            cout << "δ�ҵ�IDΪ " << id << " ���˻���\n";
        }
    }

    // ����������ɾ�������˻�
    void deleteAllAccounts() {
        string confirm;
        cout << "���棺�⽫ɾ�������˻���Ϣ��\n";
        do {
            cout << "��ȷ��Ҫɾ��ȫ�� " << accounts.size() << " ���˻���? (y/n): ";
            cin >> confirm;
            clearInputBuffer();

            if (confirm == "y" || confirm == "Y") {
                accounts.clear();
                nextId = 1; // ����ID������
                cout << "�����˻��ѳɹ�ɾ����\n";
                break;
            }
            else if (confirm == "n" || confirm == "N") {
                cout << "ɾ��������ȡ����\n";
                break;
            }
            else {
                cout << confirm << "����Ч���룬����������!\n";
            }
        } while (true);
    }

    // ��ȡ��ǰʱ���ַ�������ʽ��EEE,dd MMM yyyy HH:mm:ss��
    string getCurrentTime() {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm timeinfo;

        // ʹ���̰߳�ȫ��localtime_s��Windows����localtime_r��Linux��
        #ifdef _WIN32
            localtime_s(&timeinfo, &now_time);
        #else
            localtime_r(&now_time, &timeinfo);
        #endif

        ostringstream oss;
        oss << put_time(&timeinfo, "%a,%d %b %Y %H:%M:%S");
        return oss.str();
    }

    // ��ʱ���ַ���ת��Ϊtime_t��������ȷ�Ƚ�ʱ�䣩
    time_t parseTimeString(const string& timeStr) {
        tm timeStruct = {};
        istringstream ss(timeStr);
        ss >> get_time(&timeStruct, "%a,%d %b %Y %H:%M:%S");
        if (ss.fail()) {
            return 0; // ����ʧ�ܷ���0
        }
        return mktime(&timeStruct);
    }

    // ��ID�����˻�������
    void sortAccountsAscending() {
        sort(accounts.begin(), accounts.end(),
            [](const Account& a, const Account& b) {
                return a.id < b.id;
            });
    }

    // ��ID�����˻�������
    void sortAccountsDescending() {
        sort(accounts.begin(), accounts.end(),
            [](const Account& a, const Account& b) {
                return a.id > b.id;
            });
    }

    // ������ʱ����������
    void sortAccountsByTimeAscending() {
        sort(accounts.begin(), accounts.end(),
            [this](const Account& a, const Account& b) {
                return parseTimeString(a.createTime) < parseTimeString(b.createTime);
            });
    }

    // ������ʱ�����򣨽���
    void sortAccountsByTimeDescending() {
        sort(accounts.begin(), accounts.end(),
            [this](const Account& a, const Account& b) {
                return parseTimeString(a.createTime) > parseTimeString(b.createTime);
            });
    }

    // �ָ�Ĭ��˳�򣨰�����ʱ������
    void restoreDefaultOrder() {
        sortAccountsByTimeAscending();
    }

    // ɾ�������ļ�
    void cleanEntireFile() {
        if (remove(dataFile.c_str())) {
            cout << "�ļ�ɾ��ʧ�ܡ�\n";
        }
        else {
            cout << "�ļ�����洢��Ϣ�ѱ���ȫɾ����\n";
            cout << "�����������ɡ�\n";
        }
    }

    // ����ļ����ݣ������ļ���
    void truncateFile() {
        // ���ȼ���ļ��Ƿ����
        struct stat buffer;
        bool fileExists = (stat(dataFile.c_str(), &buffer) == 0);

        // ����ļ�������
        if (!fileExists) {
            cout << "�ļ������ڣ��޷�������ݡ�\n";
            return;
        }

        // ���ļ����ضϣ����������
        ofstream file(dataFile, ios::trunc | ios::binary);
        if (!file) {
            cerr << "�޷����ļ�������ա�\n";
            return;
        }
        file.close();

        // ��֤�ļ��Ƿ񱻳ɹ����
        ifstream check(dataFile, ios::ate | ios::binary); // �򿪵��ļ�ĩβ
        if (check.tellg() == 0) { // ����ļ���С�Ƿ�Ϊ0
            cout << "�ļ������ѳɹ���ա�\n";
            cout << "�����������ɡ�\n";
        }
        else {
            cerr << "���棺�ļ���ղ�������δ�ɹ���\n";
        }
        check.close();
    }

    // ���ļ���ɾ��ָ���˻�
    void deleteAccountFromFile() {
        // ���ȴ��ļ������˻�����ʱ�б�
        vector<Account> tempAccounts;
        ifstream file(dataFile, ios::binary);
        if (!file) {
            cerr << "�޷����ļ����ļ������ڡ�\n";
            return;
        }

        int id;
        while (file.read(reinterpret_cast<char*>(&id), sizeof(id))) {
            auto readString = [&]() -> string {
                size_t len;
                if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) return "";
                string str(len, '\0');
                file.read(&str[0], len);
                return str;
                };

            string location = readString();
            string desc = readString();
            string user = readString();
            string pass = readString();
            string createTime = readString();

            tempAccounts.emplace_back(id, location, desc, user, pass, createTime);
        }
        file.close();

        if (tempAccounts.empty()) {
            cout << "�ļ���û���˻���Ϣ��\n";
            return;
        }

        // ��ʾ�ļ��е��˻�
        cout << "\n�ļ��е��˻��б�:\n";
        for (const auto& acc : tempAccounts) {
            cout << "ID: " << acc.id << " | λ��: " << acc.accountLocation
                << " | ����: " << acc.description << " | �û�: " << acc.username << "\n";
        }

        // ��ȡҪɾ�����˻�ID
        int deleteId;
        cout << "������Ҫ���ļ���ɾ�����˻�ID: ";
        if (!(cin >> deleteId)) {
            cout << "��Ч�����롣\n";
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        // ���Ҳ�ɾ��
        auto it = remove_if(tempAccounts.begin(), tempAccounts.end(),
            [deleteId](const Account& acc) { return acc.id == deleteId; });

        if (it != tempAccounts.end()) {
            string confirm;
            do {
                cout << "��ȷ��Ҫ���ļ���ɾ���˻� " << deleteId << " ��? (y/n): ";
                cin >> confirm;
                if (confirm == "y" || confirm == "Y") {
                    tempAccounts.erase(it, tempAccounts.end());
                    cout << "�˻��Ѵ��ļ��б����Ƴ���\n";

                    // �����º���б�д���ļ�
                    ofstream outFile(dataFile, ios::binary | ios::trunc);
                    if (!outFile) {
                        cerr << "�޷����ļ�����д�롣\n";
                        return;
                    }

                    for (const auto& acc : tempAccounts) {
                        outFile.write(reinterpret_cast<const char*>(&acc.id), sizeof(acc.id));

                        auto writeString = [&](const string& str) {
                            size_t len = str.size();
                            outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
                            outFile.write(str.c_str(), len);
                            };

                        writeString(acc.accountLocation);
                        writeString(acc.description);
                        writeString(acc.username);
                        writeString(acc.password);
                        writeString(acc.createTime);
                    }
                    cout << "�ļ��Ѹ��£��˻� " << deleteId << " �Ѵ��ļ���ɾ����\n";
                    break;
                }
                else if (confirm == "n" || confirm == "N") {
                    cout << "ɾ��������ȡ����\n";
                    break;
                }
                else {
                    cout << confirm << "����Ч���룬����������!\n";
                }
            } while (true);
        }
        else {
            cout << "δ���ļ����ҵ�IDΪ " << deleteId << " ���˻���\n";
        }
    }

public:

    // ������֤
    bool passwordCheck() {
        int attempts = 0;
        //cout << "��ӭʹ���û���˽��Ϣ����ϵͳ��\n";

        while (attempts < 3) {
            string password = getPasswordInput("���������Ա����: ");

            if (password == masterPassword) {
                cout << "������ȷ��\n";
                return true;
            }
            else {
                attempts++;
                cout << "�������ʣ�ೢ�Դ���: " << (3 - attempts) << "\n";
            }
        }

        cout << "���ǷǷ��û���\n";
        return false;
    }

    // ����˻�
    void addAccount() {
        int id;
        string location, desc, user, pass;

        // ��ȡID��ȷ��Ψһ��
        while (true) {
            cout << "��������: ";
            if (!(cin >> id)) {
                cout << "���������������Ч�����ֱ�š�\n";
                clearInputBuffer();
                continue;
            }

            if (!isIdUnique(id)) {
                cout << "��ID�Ѵ��ڣ���ʹ������ID��\n";
            }
            else {
                clearInputBuffer();
                break;
            }
        }

        cout << "�������˺�λ�ã���ַ��: ";
        getline(cin, location);

        cout << "�������˺�����: ";
        getline(cin, desc);

        cout << "�������û���: ";
        getline(cin, user);

        pass = getPasswordInput("����������: ");

        // ��ȡ��ǰʱ��
        string createTime = getCurrentTime();

        // �����˻�ʱ���ʱ���
        accounts.emplace_back(id, location, desc, user, encrypt(pass), createTime);
        cout << "�˻���Ϣ��ӳɹ�������ʱ��: " << createTime << "\n";

        // ������һ������ID
        if (id >= nextId) nextId = id + 1;
    }

    // ��ʾ�˻�
    void browseAccounts(bool showPlaintext = false) {
        if (accounts.empty()) {
            cout << "û���˻���Ϣ��\n";
            return;
        }

        // ��ͷ
        cout << "\n" << string(140, '-') << "\n";
        cout << left << setw(8) << "ID" << setw(25) << "�˺�λ��"
            << setw(20) << "����" << setw(20) << "�û���"
            << setw(20) << "����" << setw(25) << "����ʱ��" << "\n";
        cout << string(140, '-') << "\n";

        for (const auto& acc : accounts) {
            cout << left << setw(8) << acc.id
                << setw(25) << (acc.accountLocation.length() > 24 ? acc.accountLocation.substr(0, 21) + "..." : acc.accountLocation)
                << setw(20) << (acc.description.length() > 19 ? acc.description.substr(0, 16) + "..." : acc.description)
                << setw(20) << (acc.username.length() > 19 ? acc.username.substr(0, 16) + "..." : acc.username);

            if (showPlaintext) {
                cout << setw(20) << decrypt(acc.password);
            }
            else {
                cout << setw(20) << "********";
            }

            // ��Ӵ���ʱ����ʾ
            cout << setw(25) << acc.createTime << "\n";
        }
        cout << string(140, '-') << "\n";
    }

    // �����˻�
    void searchAccount() {
        if (accounts.empty()) {
            cout << "û���˻���Ϣ��\n";
            return;
        }

        string query;
        cout << "������Ҫ��ѯ�Ĺؼ��� (ID/λ��/����/�û���): ";
        getline(cin, query);

        vector<Account> results;
        for (const auto& acc : accounts) {
            if (to_string(acc.id) == query ||
                acc.accountLocation.find(query) != string::npos ||
                acc.description.find(query) != string::npos ||
                acc.username.find(query) != string::npos) {
                results.push_back(acc);
            }
        }

        if (results.empty()) {
            cout << "δ�ҵ�ƥ����˻���\n";
            return;
        }

        cout << "\n�ҵ� " << results.size() << " ��ƥ����˻�:\n";
        for (const auto& acc : results) {
            cout << "ID: " << acc.id << " | λ��: " << acc.accountLocation
                << " | ����: " << acc.description << " | �û�: " << acc.username << "\n";
        }
    }

    // ɾ���˻�
    void deleteAccount() {
        if (accounts.empty()) {
            cout << "û���˻���Ϣ��\n";
            return;
        }

        int deleteChoice;
        do {
            cout << "\n************* ɾ���˻� **************\n";
            cout << "|        1. ɾ��ָ���˻�            |\n";
            cout << "|        2. һ��ɾ�������˻�        |\n";
            cout << "|        3. �������˵�              |\n";
            cout << "*************************************\n";
            cout << "��ѡ��ɾ����������д������ţ�: ";

            if (!(cin >> deleteChoice)) {
                cout << "��Ч�����롣\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (deleteChoice) {
            case 1: // ɾ��ָ���˻���ԭ�߼���
                browseAccounts(false);
                deleteSingleAccount();
                break;
            case 2: // һ��ɾ�������˻�
                browseAccounts(false);
                deleteAllAccounts();
                break;
            case 3: // �������˵�
                cout << "�ѷ������˵���\n";
                break;
            default:
                cout << "��Ч��ѡ��\n";
            }
        } while (deleteChoice != 3);
    }

    // �޸��˻�
    void modifyAccount() {
        if (accounts.empty()) {
            cout << "û���˻���Ϣ��\n";
            return;
        }

        browseAccounts(false);
        int id;
        cout << "������Ҫ�޸ĵ��˻�ID: ";
        if (!(cin >> id)) {
            cout << "��Ч�����롣\n";
            clearInputBuffer();
            return;
        }

        auto it = find_if(accounts.begin(), accounts.end(),
            [id](const Account& acc) { return acc.id == id; });

        if (it != accounts.end()) {
            clearInputBuffer();
            cout << "��ǰ��Ϣ:\n";
            cout << "λ��: " << it->accountLocation << "\n";
            cout << "����: " << it->description << "\n";
            cout << "�û���: " << it->username << "\n";
            cout << "����: " << string(it->password.length(), '*') << "\n";

            cout << "\n��������Ϣ (�����򱣳ֲ���):\n";

            string input;
            cout << "��λ��: ";
            getline(cin, input);
            if (!input.empty()) it->accountLocation = input;

            cout << "������: ";
            getline(cin, input);
            if (!input.empty()) it->description = input;

            cout << "���û���: ";
            getline(cin, input);
            if (!input.empty()) it->username = input;

            string pass = getPasswordInput("������ (�����򱣳ֲ���): ");
            if (!pass.empty()) it->password = encrypt(pass);

            cout << "�˻���Ϣ�޸ĳɹ���\n";
        }
        else {
            cout << "δ�ҵ�IDΪ " << id << " ���˻���\n";
        }
    }

    // ���浽�ļ����������£�
    void saveToFile() {
        // ����1����ȡ�ļ��е������˻�
        vector<Account> fileAccounts;
        ifstream inFile(dataFile, ios::binary);
        if (inFile) {
            while (inFile) {
                int id;
                if (!inFile.read(reinterpret_cast<char*>(&id), sizeof(id))) break;

                auto readString = [&]() -> string {
                    size_t len;
                    if (!inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) return "";
                    string str(len, '\0');
                    inFile.read(&str[0], len);
                    return str;
                    };

                string location = readString();
                string desc = readString();
                string user = readString();
                string pass = readString();
                string createTime = readString();

                fileAccounts.emplace_back(id, location, desc, user, pass, createTime);
            }
            inFile.close();
        }

        // ����2�������ϲ�ӳ���
        unordered_map<int, Account> mergedAccounts;
        for (const auto& acc : fileAccounts) {
            mergedAccounts[acc.id] = acc;
        }

        // ����3�����ڴ��е��˻�����ӳ���
        for (const auto& memAcc : accounts) {
            mergedAccounts[memAcc.id] = memAcc;
        }

        // ����4��д�غϲ��������
        ofstream outFile(dataFile, ios::binary | ios::trunc);
        if (!outFile) {
            cerr << "�޷����ļ�����д�롣\n";
            return;
        }

        for (const auto& pair : mergedAccounts) {
            const auto& acc = pair.second;
            outFile.write(reinterpret_cast<const char*>(&acc.id), sizeof(acc.id));

            auto writeString = [&](const string& str) {
                size_t len = str.size();
                outFile.write(reinterpret_cast<const char*>(&len), sizeof(len));
                outFile.write(str.c_str(), len);
                };

            writeString(acc.accountLocation);
            writeString(acc.description);
            writeString(acc.username);
            writeString(acc.password);
            writeString(acc.createTime);
        }

        cout << "�˻���Ϣ���������浽�ļ��������˻���: "
            << accounts.size() << " / ���˻���: " << mergedAccounts.size() << "\n";
    }

    // ���ļ�����
    void loadFromFile() {
        ifstream file(dataFile, ios::binary);
        if (!file) {
            cerr << "�޷����ļ����ļ������ڡ�\n";
            return;
        }

        int loadedCount = 0; // ʵ�ʼ��ص����˻�����
        int duplicateCount = 0; // �ظ��˻�����

        while (file) {
            int id;
            if (!file.read(reinterpret_cast<char*>(&id), sizeof(id))) break;

            auto readString = [&]() -> string {
                size_t len;
                if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) return "";
                string str(len, '\0');
                file.read(&str[0], len);
                return str;
            };

            string location = readString();
            string desc = readString();
            string user = readString();
            string pass = readString();
            string createTime = readString();

            // ���ID�Ƿ��Ѵ���
            bool duplicate = false;
            for (const auto& acc : accounts) {
                if (acc.id == id) {
                    duplicate = true;
                    duplicateCount++;
                    break;
                }
            }

            if (!duplicate) {
                // ��ӵ��˻��б�
                accounts.emplace_back(id, location, desc, user, pass, createTime);
                loadedCount++;
                // ������һ������ID
                if (id >= nextId) nextId = id + 1;
            }
        }

        cout << "�Ѽ��� " << loadedCount << " �����˻���";
        if (duplicateCount > 0) {
            cout << " ���� " << duplicateCount << " ���ظ��˻���";
        }
        cout << "\n";
    }

    // �����ļ�
    void cleanFile() {
        // ����������֤
        if (!passwordCheck()) {
            cout << "������֤ʧ�ܣ��������˵���\n";
            return;
        }

        int cleanChoice;
        do {
            cout << "\n************** �����ļ� **************\n";
            cout << "|     1. ɾ���ļ��е�ָ���˻���Ϣ    |\n";
            cout << "|     2. �����ļ��е������˻���Ϣ    |\n";
            cout << "|     3. �������˵�                  |\n";
            cout << "**************************************\n";
            cout << "��ѡ�������������д������ţ�: ";

            if (!(cin >> cleanChoice)) {
                cout << "��Ч�����롣\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (cleanChoice) {
            case 1: // ɾ���ļ��е�ָ���˻�
                deleteAccountFromFile();
                break;
            case 2: // һ�������ļ�
            {
                string confirm;
                do {
                    cout << "��ȷ��Ҫ�����ļ��е������˻���Ϣ��? (y/n): ";
                    cin >> confirm;
                    clearInputBuffer();

                    if (confirm == "y" || confirm == "Y") {
                        // ѯ���Ƿ����ļ��ṹ
                        string fileConfirm;
                        do {
                            cout << "�Ƿ������ļ�? (y:�����ļ��ṹ/n:��ȫɾ���ļ�/c:ȡ������): ";
                            cin >> fileConfirm;
                            clearInputBuffer();

                            if (fileConfirm == "y" || fileConfirm == "Y") {
                                truncateFile(); // ����ļ����ݵ������ļ�
                                break;
                            }
                            else if (fileConfirm == "n" || fileConfirm == "N") {
                                cleanEntireFile(); // ��ȫɾ���ļ�
                                break;
                            }
                            else if (fileConfirm == "c" || fileConfirm == "C") {
                                cout << "������ȡ����\n";
                                break;
                            }
                            else {
                                cout << fileConfirm << "����Ч���룬����������!\n";
                            }
                        } while (true);
                        
                        break;
                    }
                    else if (confirm == "n" || confirm == "N") {
                        cout << "���������ȡ����\n";
                        break;
                    }
                    else {
                        cout << confirm << "����Ч���룬����������!\n";
                    }
                } while (true);

                break;
            }
            case 3: // �������˵�
                cout << "�ѷ������˵���\n";
                return;
            default:
                cout << "��Ч��ѡ��\n";
            }
        } while (cleanChoice != 3);
    }


    // �˻�������
    void sortAccounts() {
        if (accounts.empty()) {
            cout << "û���˻���Ϣ������\n";
            return;
        }

        int sortChoice;
        do {
            cout << "\n*************** �˻����� ***************\n";
            cout << "|     1. ��ID��������                  |\n";
            cout << "|     2. ��ID��������                  |\n";
            cout << "|     3. ������ʱ�併�����У������磩|\n";
            cout << "|     4. �ָ�Ĭ�ϣ�������ʱ���������У�|\n";
            cout << "|     5. �������˵�                    |\n";
            cout << "****************************************\n";
            cout << "��ѡ������ʽ����д������ţ�: ";

            if (!(cin >> sortChoice)) {
                cout << "��Ч�����롣\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (sortChoice) {
            case 1:
                sortAccountsAscending();
                cout << "�˻��Ѱ�ID�������С�\n";
                browseAccounts(false);  // ��ʾ�����Ľ��
                break;
            case 2:
                sortAccountsDescending();
                cout << "�˻��Ѱ�ID�������С�\n";
                browseAccounts(false);
                break;
            case 3:
                sortAccountsByTimeDescending();
                cout << "�˻��Ѱ�����ʱ�併�����У������磩��\n";
                browseAccounts(false);
                break;
            case 4:
                restoreDefaultOrder();
                cout << "�ѻָ�Ĭ��˳��\n";
                browseAccounts(false);
                break;
            case 5:
                cout << "�ѷ������˵���\n";
                break;
            default:
                cout << "��Ч��ѡ��\n";
            }
        } while (sortChoice!=5);
    }

    // ���˵�
    void mainMenu() {
        int choice;
        do {
            cout << "\n-------------------------------------------\n";
            cout << "|          �û���˽��Ϣ����ϵͳ           |\n";
            cout << "-------------------------------------------\n";
            cout <<("|+++++++++++++++++++++++++++++++++++++++++|\n");
            cout << "| *            1. ����˻�              * |\n";
            cout << "|* *           2. ����˻�             * *|\n";
            cout << "| * *          3. �����˻�            * * |\n";
            cout << "| * *          4. ɾ���˻�            * * |\n";
            cout << "| * *          5. �޸��˻�            * * |\n";
            cout << "| * *          6. ���浽�ļ�          * * |\n";
            cout << "| * *          7. ���ļ�����          * * |\n";
            cout << "| * *          8. �����ļ�            * * |\n";
            cout << "|* *           9. �����˻�             * *|\n";
            cout << "| *            0. �˳�                   *|\n";
            cout <<("|+++++++++++++++++++++++++++++++++++++++++|\n");
            cout << "-------------------------------------------\n";
            cout << "��ѡ���������д������ţ�: ";

            if (!(cin >> choice)) {
                cout << "���������������Ч��ѡ��\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (choice) {
            case 1: addAccount(); break;
            case 2: {
                if (accounts.empty()) {
                    cout << "û���˻���Ϣ��\n";
                }
                else {
                    int show;
                    cout << "��ʾ��������? (1:��, 0:��): ";
                    cin >> show;
                    clearInputBuffer();
                    browseAccounts(show == 1);
                }
                break;
            }
            case 3: searchAccount(); break;
            case 4: deleteAccount(); break;
            case 5: modifyAccount(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
            case 8: cleanFile(); break;
            case 9: sortAccounts(); break;
            case 0: cout << "�˳�����\n"; break;
            default: cout << "��Ч��ѡ��\n";
            }
        } while (choice != 0);
    }
};

int main() {
    AccountSystem system;
    cout << "��ӭʹ���û���˽��Ϣ����ϵͳ��\n";
    if (system.passwordCheck()) {
        system.mainMenu();
    }
    return 0;
}