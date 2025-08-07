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

// 使用类封装账户系统
class AccountSystem {
private:
    // 账户类
    class Account {
    public:
        int id;
        string accountLocation;  // 账户位置（网址）
        string description;      // 账户描述
        string username;         // 账户名
        string password;         // 密码
        string createTime;       // 账户创建时间

        // 添加默认构造函数
        Account() = default;

        Account(int i, const string& loc, const string& desc, const string& user, const string& pass, const string& time)
            : id(i), accountLocation(loc), description(desc), username(user), password(pass), createTime(time) {}
    };

    vector<Account> accounts;
    const string dataFile = "accounts.dat";
    const string masterPassword = "123456";
    int nextId = 1;

    // 加密函数（改进）
    string encrypt(const string& input) {
        string result = input;
        for (char& c : result) {
            c = (c + 3) % 128; // 简单但比原版稍好的加密
        }
        return result;
    }

    // 解密函数
    string decrypt(const string& input) {
        string result = input;
        for (char& c : result) {
            c = (c - 3 + 128) % 128;
        }
        return result;
    }

    // 安全的密码输入
    string getPasswordInput(const string& prompt) {
        string password;
        cout << prompt;

#ifdef _WIN32
        // Windows下的密码隐藏输入
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
        // Linux/macOS下的密码隐藏输入
        system("stty -echo");
        cin >> password;
        system("stty echo");
        cout << endl;
#endif

        return password;
    }

    // 清空输入缓冲区
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // 检查ID是否唯一
    bool isIdUnique(int id) {
        for (const auto& acc : accounts) {
            if (acc.id == id) return false;
        }
        return true;
    }

    // 新增函数：删除单个账户（原删除逻辑）
    void deleteSingleAccount() {
        int id;
        cout << "请输入要删除的账户ID: ";
        if (!(cin >> id)) {
            cout << "无效的输入。\n";
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        auto it = find_if(accounts.begin(), accounts.end(),
            [id](const Account& acc) { return acc.id == id; });

        if (it != accounts.end()) {
            string confirm;
            do {
                cout << "您确定要删除账户 " << id << " 吗? (y/n): ";
                cin >> confirm;
                if (confirm == "y" || confirm == "Y") {
                    accounts.erase(it);
                    cout << "账户删除成功！\n";
                    break;
                }
                else if (confirm == "n" || confirm == "N") {
                    cout << "删除操作已取消。\n";
                    break;
                }
                else {
                    cout << confirm << "是无效输入，请重新输入!\n";
                }
            } while (true);
        }
        else {
            cout << "未找到ID为 " << id << " 的账户。\n";
        }
    }

    // 新增函数：删除所有账户
    void deleteAllAccounts() {
        string confirm;
        cout << "警告：这将删除所有账户信息！\n";
        do {
            cout << "您确定要删除全部 " << accounts.size() << " 个账户吗? (y/n): ";
            cin >> confirm;
            clearInputBuffer();

            if (confirm == "y" || confirm == "Y") {
                accounts.clear();
                nextId = 1; // 重置ID计数器
                cout << "所有账户已成功删除！\n";
                break;
            }
            else if (confirm == "n" || confirm == "N") {
                cout << "删除操作已取消。\n";
                break;
            }
            else {
                cout << confirm << "是无效输入，请重新输入!\n";
            }
        } while (true);
    }

    // 获取当前时间字符串（格式：EEE,dd MMM yyyy HH:mm:ss）
    string getCurrentTime() {
        auto now = chrono::system_clock::now();
        time_t now_time = chrono::system_clock::to_time_t(now);
        tm timeinfo;

        // 使用线程安全的localtime_s（Windows）或localtime_r（Linux）
        #ifdef _WIN32
            localtime_s(&timeinfo, &now_time);
        #else
            localtime_r(&now_time, &timeinfo);
        #endif

        ostringstream oss;
        oss << put_time(&timeinfo, "%a,%d %b %Y %H:%M:%S");
        return oss.str();
    }

    // 将时间字符串转换为time_t（用于正确比较时间）
    time_t parseTimeString(const string& timeStr) {
        tm timeStruct = {};
        istringstream ss(timeStr);
        ss >> get_time(&timeStruct, "%a,%d %b %Y %H:%M:%S");
        if (ss.fail()) {
            return 0; // 解析失败返回0
        }
        return mktime(&timeStruct);
    }

    // 按ID排序账户（升序）
    void sortAccountsAscending() {
        sort(accounts.begin(), accounts.end(),
            [](const Account& a, const Account& b) {
                return a.id < b.id;
            });
    }

    // 按ID排序账户（降序）
    void sortAccountsDescending() {
        sort(accounts.begin(), accounts.end(),
            [](const Account& a, const Account& b) {
                return a.id > b.id;
            });
    }

    // 按创建时间排序（升序）
    void sortAccountsByTimeAscending() {
        sort(accounts.begin(), accounts.end(),
            [this](const Account& a, const Account& b) {
                return parseTimeString(a.createTime) < parseTimeString(b.createTime);
            });
    }

    // 按创建时间排序（降序）
    void sortAccountsByTimeDescending() {
        sort(accounts.begin(), accounts.end(),
            [this](const Account& a, const Account& b) {
                return parseTimeString(a.createTime) > parseTimeString(b.createTime);
            });
    }

    // 恢复默认顺序（按创建时间升序）
    void restoreDefaultOrder() {
        sortAccountsByTimeAscending();
    }

    // 删除整个文件
    void cleanEntireFile() {
        if (remove(dataFile.c_str())) {
            cout << "文件删除失败。\n";
        }
        else {
            cout << "文件及其存储信息已被完全删除。\n";
            cout << "清理操作已完成。\n";
        }
    }

    // 清空文件内容（保留文件）
    void truncateFile() {
        // 首先检查文件是否存在
        struct stat buffer;
        bool fileExists = (stat(dataFile.c_str(), &buffer) == 0);

        // 如果文件不存在
        if (!fileExists) {
            cout << "文件不存在，无法清空内容。\n";
            return;
        }

        // 打开文件并截断，即清空内容
        ofstream file(dataFile, ios::trunc | ios::binary);
        if (!file) {
            cerr << "无法打开文件进行清空。\n";
            return;
        }
        file.close();

        // 验证文件是否被成功清空
        ifstream check(dataFile, ios::ate | ios::binary); // 打开到文件末尾
        if (check.tellg() == 0) { // 检查文件大小是否为0
            cout << "文件内容已成功清空。\n";
            cout << "清理操作已完成。\n";
        }
        else {
            cerr << "警告：文件清空操作可能未成功！\n";
        }
        check.close();
    }

    // 从文件中删除指定账户
    void deleteAccountFromFile() {
        // 首先从文件加载账户到临时列表
        vector<Account> tempAccounts;
        ifstream file(dataFile, ios::binary);
        if (!file) {
            cerr << "无法打开文件或文件不存在。\n";
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
            cout << "文件中没有账户信息。\n";
            return;
        }

        // 显示文件中的账户
        cout << "\n文件中的账户列表:\n";
        for (const auto& acc : tempAccounts) {
            cout << "ID: " << acc.id << " | 位置: " << acc.accountLocation
                << " | 描述: " << acc.description << " | 用户: " << acc.username << "\n";
        }

        // 获取要删除的账户ID
        int deleteId;
        cout << "请输入要从文件中删除的账户ID: ";
        if (!(cin >> deleteId)) {
            cout << "无效的输入。\n";
            clearInputBuffer();
            return;
        }
        clearInputBuffer();

        // 查找并删除
        auto it = remove_if(tempAccounts.begin(), tempAccounts.end(),
            [deleteId](const Account& acc) { return acc.id == deleteId; });

        if (it != tempAccounts.end()) {
            string confirm;
            do {
                cout << "您确定要从文件中删除账户 " << deleteId << " 吗? (y/n): ";
                cin >> confirm;
                if (confirm == "y" || confirm == "Y") {
                    tempAccounts.erase(it, tempAccounts.end());
                    cout << "账户已从文件列表中移除。\n";

                    // 将更新后的列表写回文件
                    ofstream outFile(dataFile, ios::binary | ios::trunc);
                    if (!outFile) {
                        cerr << "无法打开文件进行写入。\n";
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
                    cout << "文件已更新，账户 " << deleteId << " 已从文件中删除。\n";
                    break;
                }
                else if (confirm == "n" || confirm == "N") {
                    cout << "删除操作已取消。\n";
                    break;
                }
                else {
                    cout << confirm << "是无效输入，请重新输入!\n";
                }
            } while (true);
        }
        else {
            cout << "未在文件中找到ID为 " << deleteId << " 的账户。\n";
        }
    }

public:

    // 密码验证
    bool passwordCheck() {
        int attempts = 0;
        //cout << "欢迎使用用户隐私信息管理系统！\n";

        while (attempts < 3) {
            string password = getPasswordInput("请输入管理员密码: ");

            if (password == masterPassword) {
                cout << "密码正确！\n";
                return true;
            }
            else {
                attempts++;
                cout << "密码错误！剩余尝试次数: " << (3 - attempts) << "\n";
            }
        }

        cout << "您是非法用户！\n";
        return false;
    }

    // 添加账户
    void addAccount() {
        int id;
        string location, desc, user, pass;

        // 获取ID（确保唯一）
        while (true) {
            cout << "请输入编号: ";
            if (!(cin >> id)) {
                cout << "输入错误，请输入有效的数字编号。\n";
                clearInputBuffer();
                continue;
            }

            if (!isIdUnique(id)) {
                cout << "该ID已存在，请使用其他ID。\n";
            }
            else {
                clearInputBuffer();
                break;
            }
        }

        cout << "请输入账号位置（网址）: ";
        getline(cin, location);

        cout << "请输入账号描述: ";
        getline(cin, desc);

        cout << "请输入用户名: ";
        getline(cin, user);

        pass = getPasswordInput("请输入密码: ");

        // 获取当前时间
        string createTime = getCurrentTime();

        // 创建账户时添加时间戳
        accounts.emplace_back(id, location, desc, user, encrypt(pass), createTime);
        cout << "账户信息添加成功！创建时间: " << createTime << "\n";

        // 更新下一个可用ID
        if (id >= nextId) nextId = id + 1;
    }

    // 显示账户
    void browseAccounts(bool showPlaintext = false) {
        if (accounts.empty()) {
            cout << "没有账户信息。\n";
            return;
        }

        // 表头
        cout << "\n" << string(140, '-') << "\n";
        cout << left << setw(8) << "ID" << setw(25) << "账号位置"
            << setw(20) << "描述" << setw(20) << "用户名"
            << setw(20) << "密码" << setw(25) << "创建时间" << "\n";
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

            // 添加创建时间显示
            cout << setw(25) << acc.createTime << "\n";
        }
        cout << string(140, '-') << "\n";
    }

    // 搜索账户
    void searchAccount() {
        if (accounts.empty()) {
            cout << "没有账户信息。\n";
            return;
        }

        string query;
        cout << "请输入要查询的关键字 (ID/位置/描述/用户名): ";
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
            cout << "未找到匹配的账户。\n";
            return;
        }

        cout << "\n找到 " << results.size() << " 个匹配的账户:\n";
        for (const auto& acc : results) {
            cout << "ID: " << acc.id << " | 位置: " << acc.accountLocation
                << " | 描述: " << acc.description << " | 用户: " << acc.username << "\n";
        }
    }

    // 删除账户
    void deleteAccount() {
        if (accounts.empty()) {
            cout << "没有账户信息。\n";
            return;
        }

        int deleteChoice;
        do {
            cout << "\n************* 删除账户 **************\n";
            cout << "|        1. 删除指定账户            |\n";
            cout << "|        2. 一键删除所有账户        |\n";
            cout << "|        3. 返回主菜单              |\n";
            cout << "*************************************\n";
            cout << "请选择删除操作（填写操作序号）: ";

            if (!(cin >> deleteChoice)) {
                cout << "无效的输入。\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (deleteChoice) {
            case 1: // 删除指定账户（原逻辑）
                browseAccounts(false);
                deleteSingleAccount();
                break;
            case 2: // 一键删除所有账户
                browseAccounts(false);
                deleteAllAccounts();
                break;
            case 3: // 返回主菜单
                cout << "已返回主菜单。\n";
                break;
            default:
                cout << "无效的选择。\n";
            }
        } while (deleteChoice != 3);
    }

    // 修改账户
    void modifyAccount() {
        if (accounts.empty()) {
            cout << "没有账户信息。\n";
            return;
        }

        browseAccounts(false);
        int id;
        cout << "请输入要修改的账户ID: ";
        if (!(cin >> id)) {
            cout << "无效的输入。\n";
            clearInputBuffer();
            return;
        }

        auto it = find_if(accounts.begin(), accounts.end(),
            [id](const Account& acc) { return acc.id == id; });

        if (it != accounts.end()) {
            clearInputBuffer();
            cout << "当前信息:\n";
            cout << "位置: " << it->accountLocation << "\n";
            cout << "描述: " << it->description << "\n";
            cout << "用户名: " << it->username << "\n";
            cout << "密码: " << string(it->password.length(), '*') << "\n";

            cout << "\n输入新信息 (留空则保持不变):\n";

            string input;
            cout << "新位置: ";
            getline(cin, input);
            if (!input.empty()) it->accountLocation = input;

            cout << "新描述: ";
            getline(cin, input);
            if (!input.empty()) it->description = input;

            cout << "新用户名: ";
            getline(cin, input);
            if (!input.empty()) it->username = input;

            string pass = getPasswordInput("新密码 (留空则保持不变): ");
            if (!pass.empty()) it->password = encrypt(pass);

            cout << "账户信息修改成功！\n";
        }
        else {
            cout << "未找到ID为 " << id << " 的账户。\n";
        }
    }

    // 保存到文件（增量更新）
    void saveToFile() {
        // 步骤1：读取文件中的现有账户
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

        // 步骤2：创建合并映射表
        unordered_map<int, Account> mergedAccounts;
        for (const auto& acc : fileAccounts) {
            mergedAccounts[acc.id] = acc;
        }

        // 步骤3：用内存中的账户更新映射表
        for (const auto& memAcc : accounts) {
            mergedAccounts[memAcc.id] = memAcc;
        }

        // 步骤4：写回合并后的数据
        ofstream outFile(dataFile, ios::binary | ios::trunc);
        if (!outFile) {
            cerr << "无法打开文件进行写入。\n";
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

        cout << "账户信息已增量保存到文件。更新账户数: "
            << accounts.size() << " / 总账户数: " << mergedAccounts.size() << "\n";
    }

    // 从文件加载
    void loadFromFile() {
        ifstream file(dataFile, ios::binary);
        if (!file) {
            cerr << "无法打开文件或文件不存在。\n";
            return;
        }

        int loadedCount = 0; // 实际加载的新账户计数
        int duplicateCount = 0; // 重复账户计数

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

            // 检查ID是否已存在
            bool duplicate = false;
            for (const auto& acc : accounts) {
                if (acc.id == id) {
                    duplicate = true;
                    duplicateCount++;
                    break;
                }
            }

            if (!duplicate) {
                // 添加到账户列表
                accounts.emplace_back(id, location, desc, user, pass, createTime);
                loadedCount++;
                // 更新下一个可用ID
                if (id >= nextId) nextId = id + 1;
            }
        }

        cout << "已加载 " << loadedCount << " 个新账户。";
        if (duplicateCount > 0) {
            cout << " 跳过 " << duplicateCount << " 个重复账户。";
        }
        cout << "\n";
    }

    // 清理文件
    void cleanFile() {
        // 二次密码验证
        if (!passwordCheck()) {
            cout << "密码验证失败，返回主菜单。\n";
            return;
        }

        int cleanChoice;
        do {
            cout << "\n************** 清理文件 **************\n";
            cout << "|     1. 删除文件中的指定账户信息    |\n";
            cout << "|     2. 清理文件中的所有账户信息    |\n";
            cout << "|     3. 返回主菜单                  |\n";
            cout << "**************************************\n";
            cout << "请选择清理操作（填写操作序号）: ";

            if (!(cin >> cleanChoice)) {
                cout << "无效的输入。\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (cleanChoice) {
            case 1: // 删除文件中的指定账户
                deleteAccountFromFile();
                break;
            case 2: // 一键清理文件
            {
                string confirm;
                do {
                    cout << "您确定要清理文件中的所有账户信息吗? (y/n): ";
                    cin >> confirm;
                    clearInputBuffer();

                    if (confirm == "y" || confirm == "Y") {
                        // 询问是否保留文件结构
                        string fileConfirm;
                        do {
                            cout << "是否保留空文件? (y:保留文件结构/n:完全删除文件/c:取消操作): ";
                            cin >> fileConfirm;
                            clearInputBuffer();

                            if (fileConfirm == "y" || fileConfirm == "Y") {
                                truncateFile(); // 清空文件内容但保留文件
                                break;
                            }
                            else if (fileConfirm == "n" || fileConfirm == "N") {
                                cleanEntireFile(); // 完全删除文件
                                break;
                            }
                            else if (fileConfirm == "c" || fileConfirm == "C") {
                                cout << "操作已取消。\n";
                                break;
                            }
                            else {
                                cout << fileConfirm << "是无效输入，请重新输入!\n";
                            }
                        } while (true);
                        
                        break;
                    }
                    else if (confirm == "n" || confirm == "N") {
                        cout << "清理操作已取消。\n";
                        break;
                    }
                    else {
                        cout << confirm << "是无效输入，请重新输入!\n";
                    }
                } while (true);

                break;
            }
            case 3: // 返回主菜单
                cout << "已返回主菜单。\n";
                return;
            default:
                cout << "无效的选择。\n";
            }
        } while (cleanChoice != 3);
    }


    // 账户排序功能
    void sortAccounts() {
        if (accounts.empty()) {
            cout << "没有账户信息可排序。\n";
            return;
        }

        int sortChoice;
        do {
            cout << "\n*************** 账户排序 ***************\n";
            cout << "|     1. 按ID升序排列                  |\n";
            cout << "|     2. 按ID降序排列                  |\n";
            cout << "|     3. 按创建时间降序排列（从晚到早）|\n";
            cout << "|     4. 恢复默认（按创建时间升序排列）|\n";
            cout << "|     5. 返回主菜单                    |\n";
            cout << "****************************************\n";
            cout << "请选择排序方式（填写操作序号）: ";

            if (!(cin >> sortChoice)) {
                cout << "无效的输入。\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (sortChoice) {
            case 1:
                sortAccountsAscending();
                cout << "账户已按ID升序排列。\n";
                browseAccounts(false);  // 显示排序后的结果
                break;
            case 2:
                sortAccountsDescending();
                cout << "账户已按ID降序排列。\n";
                browseAccounts(false);
                break;
            case 3:
                sortAccountsByTimeDescending();
                cout << "账户已按创建时间降序排列（从晚到早）。\n";
                browseAccounts(false);
                break;
            case 4:
                restoreDefaultOrder();
                cout << "已恢复默认顺序。\n";
                browseAccounts(false);
                break;
            case 5:
                cout << "已返回主菜单。\n";
                break;
            default:
                cout << "无效的选择。\n";
            }
        } while (sortChoice!=5);
    }

    // 主菜单
    void mainMenu() {
        int choice;
        do {
            cout << "\n-------------------------------------------\n";
            cout << "|          用户隐私信息管理系统           |\n";
            cout << "-------------------------------------------\n";
            cout <<("|+++++++++++++++++++++++++++++++++++++++++|\n");
            cout << "| *            1. 添加账户              * |\n";
            cout << "|* *           2. 浏览账户             * *|\n";
            cout << "| * *          3. 搜索账户            * * |\n";
            cout << "| * *          4. 删除账户            * * |\n";
            cout << "| * *          5. 修改账户            * * |\n";
            cout << "| * *          6. 保存到文件          * * |\n";
            cout << "| * *          7. 从文件加载          * * |\n";
            cout << "| * *          8. 清理文件            * * |\n";
            cout << "|* *           9. 排序账户             * *|\n";
            cout << "| *            0. 退出                   *|\n";
            cout <<("|+++++++++++++++++++++++++++++++++++++++++|\n");
            cout << "-------------------------------------------\n";
            cout << "请选择操作（填写操作序号）: ";

            if (!(cin >> choice)) {
                cout << "输入错误，请输入有效的选择。\n";
                clearInputBuffer();
                continue;
            }

            clearInputBuffer();

            switch (choice) {
            case 1: addAccount(); break;
            case 2: {
                if (accounts.empty()) {
                    cout << "没有账户信息。\n";
                }
                else {
                    int show;
                    cout << "显示密码明文? (1:是, 0:否): ";
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
            case 0: cout << "退出程序。\n"; break;
            default: cout << "无效的选择。\n";
            }
        } while (choice != 0);
    }
};

int main() {
    AccountSystem system;
    cout << "欢迎使用用户隐私信息管理系统！\n";
    if (system.passwordCheck()) {
        system.mainMenu();
    }
    return 0;
}