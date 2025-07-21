#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 定义一个结构体来存储账户中的每条信息
typedef struct Account {
    int id; // 编号
    char Account_location[50]; // 账号位置
    char description[50]; // 账号描述
    char username[50]; // 用户名
    char password[50]; // 密码
    struct Account* next;
} Account;

// 使用链表来存储所有的账户信息
//设置头节点
Account* head = NULL;

// 密码验证功能模块
void passwordCheck() {
    char password[20];
    int attempts = 0;
    const char* correctPassword = "123456";
    printf("欢迎使用用户隐私信息管理系统！\n");
    while (attempts < 3) {
        printf("请输入管理员密码: ");
        scanf("%s", password);
        if (strcmp(password, correctPassword) == 0) {
            printf("密码正确！\n");
            return;
        }
        else {
            attempts++;
            printf("密码错误！\n");
        }
    }
    printf("您是非法用户！\n");
    exit(0);
}

// 用户信息录入功能模块
void addAccount() {
    Account* newAccount = (Account*)malloc(sizeof(Account));
    int c;
    if (newAccount == NULL) {
        printf("内存分配失败！\n");
        return;
    }

    printf("请输入编号: ");
    if (scanf("%d", &newAccount->id) != 1) {
        printf("输入错误，请输入有效的编号（数字）。\n");

        //清理缓存区
        while ((c = getchar()) != '\n' && c != EOF);
        free(newAccount);
        return;
    }

    //清理缓存区
    while ((c = getchar()) != '\n' && c != EOF);


    printf("请输入账号位置（网址）: ");
    if (fgets(newAccount->Account_location, sizeof(newAccount->Account_location), stdin) == NULL) {
        printf("输入错误，请输入有效的账号位置(49个字符以内)。\n");
        free(newAccount);
        return;
    }
    // 安全移除换行符
    newAccount->Account_location[strcspn(newAccount->Account_location, "\n")] = '\0';


    printf("请输入账号描述: ");
    if (fgets(newAccount->description, sizeof(newAccount->description), stdin) == NULL) {
        printf("输入错误，请输入有效的账号描述(49个字符以内)。\n");
        free(newAccount);
        return;
    }
    // 安全移除换行符
    newAccount->description[strcspn(newAccount->description, "\n")] = '\0';


    printf("请输入用户名: ");
    if (fgets(newAccount->username, sizeof(newAccount->username), stdin) == NULL) {
        printf("输入错误，请输入有效的用户名(49个字符以内)。\n");
        free(newAccount);
        return;
    }
    // 安全移除换行符
    newAccount->username[strcspn(newAccount->username, "\n")] = '\0';


    printf("请输入密码: ");
    if (fgets(newAccount->password, sizeof(newAccount->password), stdin) == NULL) {
        printf("输入错误，请输入有效的密码(49个字符以内)。\n");
        free(newAccount);
        return;
    }
    // 安全移除换行符
    newAccount->password[strcspn(newAccount->password, "\n")] = '\0';


    // 用户账号密码加密
    for (int i = 0; newAccount->password[i] != '\0'; i++) {
        newAccount->password[i] += 1;
    }

    newAccount->next = NULL;

    if (head == NULL) {
        head = newAccount;
    }
    else {
        Account* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newAccount;
    }
    printf("账户信息添加成功！\n");
}

// 用户信息浏览功能模块
void browseAccounts() {
    if (head == NULL) {
        printf("没有账户信息。\n");
        return;
    }

    int showPlaintext;
    printf("是否显示密码明文？(1: 是, 0: 否): ");
    if (scanf("%d", &showPlaintext) != 1) {
        printf("输入错误，请输入有效的选择。\n");
        return;
    }

    Account* temp = head;
    while (temp != NULL) {

        if (showPlaintext == 1) {

            // 解密密码
            char decryptedPassword[50];
            strcpy(decryptedPassword, temp->password);

            for (int i = 0; decryptedPassword[i] != '\0'; i++) {
                decryptedPassword[i] -= 1;
            }
            printf("编号: %d  账号位置: %s  账号描述: %s  用户名: %s  密码: %s\n ",
                temp->id, temp->Account_location, temp->description, temp->username, decryptedPassword);
        }
        else if (showPlaintext == 0) {
            printf("编号: %d  账号位置: %s  账号描述: %s  用户名: %s  密码: %s\n",
                temp->id, temp->Account_location, temp->description, temp->username, temp->password);
        }
        else {
            printf("输入错误，请输入有效的选择。\n");
            return;
        }
        temp = temp->next;
    }
}

// 用户信息查询功能模块
void searchAccount() {
    char temp1[50];
    int found = 0;  // 标记是否找到匹配项
    int c;

    printf("****请输入要查询的用户名/账号位置（网址）/账号描述----（选填一个）**** \n");

    //清理缓存区
    while ((c = getchar()) != '\n' && c != EOF);

    if (fgets(temp1, sizeof(temp1), stdin) == NULL) {
        printf("输入错误，请输入有效的查询内容（49个字符以内）。\n");
        return;
    }
    // 安全移除换行符
    temp1[strcspn(temp1, "\n")] = '\0';

    Account* temp = head;
    while (temp != NULL) {
        // 检查三个字段中任意一个匹配
        if (strcmp(temp->username, temp1) == 0 ||
            strcmp(temp->Account_location, temp1) == 0 ||
            strcmp(temp->description, temp1) == 0 ||
            strcmp(temp->password, temp1) == 0) {
            printf("找到账户: 编号: %d  账号位置: %s  账号描述: %s  用户名: %s  密码: %s\n",
                temp->id, temp->Account_location, temp->description, temp->username, temp->password);
            found = 1;  // 标记已找到
        }
        temp = temp->next;
    }

    // 未找到时输出提示
    if (!found) {
        printf("未找到用户名/账号位置（网址）/账号描述/密码为 %s 的账户。\n", temp1);
    }
}

// 用户信息删除功能模块

void deleteAccount() {
    char username[50];
    char password[50];
    char Account_location[50];
    int found = 0;
    int c;

    if (head == NULL) {
        printf("暂无用户信息，无法进行删除\n");
        return;
    }

    //清理缓存区
    while ((c = getchar()) != '\n' && c != EOF);

    // 输入账户位置（网址）-----防止不同账号因用户名和密码名相同而误删
    printf("请输入要删除的账户位置（网址）: ");
    if (fgets(Account_location, sizeof(Account_location), stdin) == NULL) {
        printf("输入错误，请输入有效的账户位置（网址）。\n");
        return;
    }
    Account_location[strcspn(Account_location, "\n")] = '\0';

    // 输入用户名
    printf("请输入要删除的用户名: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        printf("输入错误，请输入有效的用户名。\n");
        return;
    }
    username[strcspn(username, "\n")] = '\0';

    // 输入密码
    printf("请输入对应的密码: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("输入错误，请输入有效的密码。\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    // 对输入的密码进行加密比对
    for (int i = 0; password[i] != '\0'; i++) {
        password[i] += 1;
    }

    Account* current = head;
    Account* prev = NULL;

    // 遍历链表，删除所有匹配的节点
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0 && strcmp(current->Account_location, Account_location) == 0) {
            found = 1;
            if (prev == NULL) {
                // 删除头节点
                head = current->next;
                free(current);
                current = head;  // 继续检查新的头节点
            }
            else {
                // 删除中间节点
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            printf("账户删除成功！\n");
        }
        else {
            // 继续遍历
            prev = current;
            current = current->next;
        }
    }

    if (!found) {
        printf("未找到用户名为 %s 且账号密码和账号位置都匹配的账户。\n", username);
    }
}

// 用户信息修改功能模块
void modifyAccount() {
    char username[50];
    char password[50];
    char Account_location[50];
    int found = 0;
    int c;

    //清理缓存区
    while ((c = getchar()) != '\n' && c != EOF);

    // 输入账户位置（网址）-----防止不同账号因用户名和密码名相同而误改
    printf("请输入要修改的账户位置（网址）: ");
    if (fgets(Account_location, sizeof(Account_location), stdin) == NULL) {
        printf("输入错误，请输入有效的账户位置（网址）。\n");
        return;
    }
    Account_location[strcspn(Account_location, "\n")] = '\0';

    // 输入用户名
    printf("请输入要修改的用户名: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        printf("输入错误，请输入有效的用户名。\n");
        return;
    }
    username[strcspn(username, "\n")] = '\0';

    // 输入密码
    printf("请输入对应的密码: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("输入错误，请输入有效的密码。\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    // 对输入的密码进行加密，便于比对
    for (int i = 0; password[i] != '\0'; i++) {
        password[i] += 1;
    }

    Account* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0 && strcmp(temp->Account_location, Account_location) == 0) {
            found = 1;
            break;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("未找到用户名为 %s 且账号密码和账号位置都匹配的账户。\n", username);
        return;
    }

    printf("请输入新的编号: ");
    if (scanf("%d", &temp->id) != 1) {
        printf("输入错误，请输入有效的编号（数字）。\n");

        //清理缓存区
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    //清理缓存区
    while ((c = getchar()) != '\n' && c != EOF);

    printf("请输入新的账号位置（网址）: ");
    if (fgets(temp->Account_location, sizeof(temp->Account_location), stdin) == NULL) {
        printf("输入错误，请输入有效的账号位置(49个字符以内)。\n");
        return;
    }
    // 安全移除换行符
    temp->Account_location[strcspn(temp->Account_location, "\n")] = '\0';


    printf("请输入新的账号描述: ");
    if (fgets(temp->description, sizeof(temp->description), stdin) == NULL) {
        printf("输入错误，请输入有效的账号描述(49个字符以内)。\n");
        return;
    }
    // 安全移除换行符
    temp->description[strcspn(temp->description, "\n")] = '\0';


    printf("请输入新的用户名: ");
    if (fgets(temp->username, sizeof(temp->username), stdin) == NULL) {
        printf("输入错误，请输入有效的用户名(49个字符以内)。\n");
        return;
    }
    // 安全移除换行符
    temp->username[strcspn(temp->username, "\n")] = '\0';


    printf("请输入新的密码: ");
    if (fgets(temp->password, sizeof(temp->password), stdin) == NULL) {
        printf("输入错误，请输入有效的密码(49个字符以内)。\n");
        return;
    }
    // 安全移除换行符
    temp->password[strcspn(temp->password, "\n")] = '\0';


    // 用户账号密码加密
    for (int i = 0; temp->password[i] != '\0'; i++) {
        temp->password[i] += 1;
    }

    printf("账户信息修改成功！\n");
}

// 保存（信息录入）文件功能模块
void saveToFile() {
    FILE* file = fopen("accounts.txt", "w");
    int found = 0;

    if (file == NULL) {
        printf("无法打开文件进行写入。\n");
        return;
    }

    Account* temp = head;
    while (temp != NULL) {
        found = 1;
        fprintf(file, "%d %s %s %s %s\n",
            temp->id, temp->Account_location, temp->description, temp->username, temp->password);
        temp = temp->next;
    }

    fclose(file);
    if (found)
        printf("账户信息已保存到文件。\n");
    else
        printf("账户信息保存失败。\n");
}

// 文件信息加载（导入链表）功能模块（将之前保存在文件里的信息返回给链表当中，即恢复之前保留的信息）
void loadFromFile() {
    FILE* file = fopen("accounts.txt", "r");

    if (file == NULL) {
        printf("无法打开文件进行读取。\n");
        return;
    }

    Account* newAccount;
    while (!feof(file)) {
        newAccount = (Account*)malloc(sizeof(Account));
        if (fscanf(file, "%d %s %s %s %s",
            &newAccount->id, newAccount->Account_location,
            newAccount->description, newAccount->username,
            newAccount->password) != 5) {
            free(newAccount);
            break;
        }

        newAccount->next = NULL;

        if (head == NULL) {
            head = newAccount;
        }
        else {
            Account* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newAccount;
        }
    }

    fclose(file);
    printf("账户信息已从文件加载。\n");
}

//一键清理文件信息模块
void cleanfile() {
    FILE* file = fopen("accounts.txt", "w");

    if (file == NULL) {
        printf("无法打开文件进行写入。\n");
        return;
    }

    fclose(file);
    printf("文件存储信息已被完全删除。\n");
}

// 释放链表内存
void freeList() {
    Account* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    passwordCheck();

    int choice;
    do {
        printf("-------------------------------------------\n");
        printf("|+++++++++++++++++++++++++++++++++++++++++|\n");
        printf("| *                                     * |\n");
        printf("|* *            1. 添加账户            * *|\n");
        printf("| * *           2. 浏览账户           * * |\n");
        printf("| * *           3. 查询账户           * * |\n");
        printf("| * *           4. 删除账户           * * |\n");
        printf("| * *           5. 修改账户           * * |\n");
        printf("| * *           6. 保存到文件中       * * |\n");
        printf("| * *           7. 文件信息加载       * * |\n");
        printf("| * *           8. 一键清理文件信息   * * |\n");
        printf("|* *            9. 退出                * *|\n");
        printf("| *                                     * |\n");
        printf("|+++++++++++++++++++++++++++++++++++++++++|\n");
        printf("-------------------------------------------\n");
        printf("请选择操作: ");
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入有效的选择。\n");
            while (getchar() != '\n'); // 清除输入缓冲区
            continue;
        }

        switch (choice) {
        case 1:
            addAccount();
            break;
        case 2:
            browseAccounts();
            break;
        case 3:
            searchAccount();
            break;
        case 4:
            deleteAccount();
            break;
        case 5:
            modifyAccount();
            break;
        case 6:
            saveToFile();
            break;
        case 7:
            loadFromFile();
            break;
        case 8:
            cleanfile();
            break;
        case 9:
            printf("退出程序。\n");
            break;
        default:
            printf("无效的选择。\n");
        }
    } while (choice != 9);

    // 释放链表内存
    freeList();

    return 0;
}