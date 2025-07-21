#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ����һ���ṹ�����洢�˻��е�ÿ����Ϣ
typedef struct Account {
    int id; // ���
    char Account_location[50]; // �˺�λ��
    char description[50]; // �˺�����
    char username[50]; // �û���
    char password[50]; // ����
    struct Account* next;
} Account;

// ʹ���������洢���е��˻���Ϣ
//����ͷ�ڵ�
Account* head = NULL;

// ������֤����ģ��
void passwordCheck() {
    char password[20];
    int attempts = 0;
    const char* correctPassword = "123456";
    printf("��ӭʹ���û���˽��Ϣ����ϵͳ��\n");
    while (attempts < 3) {
        printf("���������Ա����: ");
        scanf("%s", password);
        if (strcmp(password, correctPassword) == 0) {
            printf("������ȷ��\n");
            return;
        }
        else {
            attempts++;
            printf("�������\n");
        }
    }
    printf("���ǷǷ��û���\n");
    exit(0);
}

// �û���Ϣ¼�빦��ģ��
void addAccount() {
    Account* newAccount = (Account*)malloc(sizeof(Account));
    int c;
    if (newAccount == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
        return;
    }

    printf("��������: ");
    if (scanf("%d", &newAccount->id) != 1) {
        printf("���������������Ч�ı�ţ����֣���\n");

        //��������
        while ((c = getchar()) != '\n' && c != EOF);
        free(newAccount);
        return;
    }

    //��������
    while ((c = getchar()) != '\n' && c != EOF);


    printf("�������˺�λ�ã���ַ��: ");
    if (fgets(newAccount->Account_location, sizeof(newAccount->Account_location), stdin) == NULL) {
        printf("���������������Ч���˺�λ��(49���ַ�����)��\n");
        free(newAccount);
        return;
    }
    // ��ȫ�Ƴ����з�
    newAccount->Account_location[strcspn(newAccount->Account_location, "\n")] = '\0';


    printf("�������˺�����: ");
    if (fgets(newAccount->description, sizeof(newAccount->description), stdin) == NULL) {
        printf("���������������Ч���˺�����(49���ַ�����)��\n");
        free(newAccount);
        return;
    }
    // ��ȫ�Ƴ����з�
    newAccount->description[strcspn(newAccount->description, "\n")] = '\0';


    printf("�������û���: ");
    if (fgets(newAccount->username, sizeof(newAccount->username), stdin) == NULL) {
        printf("���������������Ч���û���(49���ַ�����)��\n");
        free(newAccount);
        return;
    }
    // ��ȫ�Ƴ����з�
    newAccount->username[strcspn(newAccount->username, "\n")] = '\0';


    printf("����������: ");
    if (fgets(newAccount->password, sizeof(newAccount->password), stdin) == NULL) {
        printf("���������������Ч������(49���ַ�����)��\n");
        free(newAccount);
        return;
    }
    // ��ȫ�Ƴ����з�
    newAccount->password[strcspn(newAccount->password, "\n")] = '\0';


    // �û��˺��������
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
    printf("�˻���Ϣ��ӳɹ���\n");
}

// �û���Ϣ�������ģ��
void browseAccounts() {
    if (head == NULL) {
        printf("û���˻���Ϣ��\n");
        return;
    }

    int showPlaintext;
    printf("�Ƿ���ʾ�������ģ�(1: ��, 0: ��): ");
    if (scanf("%d", &showPlaintext) != 1) {
        printf("���������������Ч��ѡ��\n");
        return;
    }

    Account* temp = head;
    while (temp != NULL) {

        if (showPlaintext == 1) {

            // ��������
            char decryptedPassword[50];
            strcpy(decryptedPassword, temp->password);

            for (int i = 0; decryptedPassword[i] != '\0'; i++) {
                decryptedPassword[i] -= 1;
            }
            printf("���: %d  �˺�λ��: %s  �˺�����: %s  �û���: %s  ����: %s\n ",
                temp->id, temp->Account_location, temp->description, temp->username, decryptedPassword);
        }
        else if (showPlaintext == 0) {
            printf("���: %d  �˺�λ��: %s  �˺�����: %s  �û���: %s  ����: %s\n",
                temp->id, temp->Account_location, temp->description, temp->username, temp->password);
        }
        else {
            printf("���������������Ч��ѡ��\n");
            return;
        }
        temp = temp->next;
    }
}

// �û���Ϣ��ѯ����ģ��
void searchAccount() {
    char temp1[50];
    int found = 0;  // ����Ƿ��ҵ�ƥ����
    int c;

    printf("****������Ҫ��ѯ���û���/�˺�λ�ã���ַ��/�˺�����----��ѡ��һ����**** \n");

    //��������
    while ((c = getchar()) != '\n' && c != EOF);

    if (fgets(temp1, sizeof(temp1), stdin) == NULL) {
        printf("���������������Ч�Ĳ�ѯ���ݣ�49���ַ����ڣ���\n");
        return;
    }
    // ��ȫ�Ƴ����з�
    temp1[strcspn(temp1, "\n")] = '\0';

    Account* temp = head;
    while (temp != NULL) {
        // ��������ֶ�������һ��ƥ��
        if (strcmp(temp->username, temp1) == 0 ||
            strcmp(temp->Account_location, temp1) == 0 ||
            strcmp(temp->description, temp1) == 0 ||
            strcmp(temp->password, temp1) == 0) {
            printf("�ҵ��˻�: ���: %d  �˺�λ��: %s  �˺�����: %s  �û���: %s  ����: %s\n",
                temp->id, temp->Account_location, temp->description, temp->username, temp->password);
            found = 1;  // ������ҵ�
        }
        temp = temp->next;
    }

    // δ�ҵ�ʱ�����ʾ
    if (!found) {
        printf("δ�ҵ��û���/�˺�λ�ã���ַ��/�˺�����/����Ϊ %s ���˻���\n", temp1);
    }
}

// �û���Ϣɾ������ģ��

void deleteAccount() {
    char username[50];
    char password[50];
    char Account_location[50];
    int found = 0;
    int c;

    if (head == NULL) {
        printf("�����û���Ϣ���޷�����ɾ��\n");
        return;
    }

    //��������
    while ((c = getchar()) != '\n' && c != EOF);

    // �����˻�λ�ã���ַ��-----��ֹ��ͬ�˺����û�������������ͬ����ɾ
    printf("������Ҫɾ�����˻�λ�ã���ַ��: ");
    if (fgets(Account_location, sizeof(Account_location), stdin) == NULL) {
        printf("���������������Ч���˻�λ�ã���ַ����\n");
        return;
    }
    Account_location[strcspn(Account_location, "\n")] = '\0';

    // �����û���
    printf("������Ҫɾ�����û���: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        printf("���������������Ч���û�����\n");
        return;
    }
    username[strcspn(username, "\n")] = '\0';

    // ��������
    printf("�������Ӧ������: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("���������������Ч�����롣\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    // �������������м��ܱȶ�
    for (int i = 0; password[i] != '\0'; i++) {
        password[i] += 1;
    }

    Account* current = head;
    Account* prev = NULL;

    // ��������ɾ������ƥ��Ľڵ�
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0 && strcmp(current->Account_location, Account_location) == 0) {
            found = 1;
            if (prev == NULL) {
                // ɾ��ͷ�ڵ�
                head = current->next;
                free(current);
                current = head;  // ��������µ�ͷ�ڵ�
            }
            else {
                // ɾ���м�ڵ�
                prev->next = current->next;
                free(current);
                current = prev->next;
            }
            printf("�˻�ɾ���ɹ���\n");
        }
        else {
            // ��������
            prev = current;
            current = current->next;
        }
    }

    if (!found) {
        printf("δ�ҵ��û���Ϊ %s ���˺�������˺�λ�ö�ƥ����˻���\n", username);
    }
}

// �û���Ϣ�޸Ĺ���ģ��
void modifyAccount() {
    char username[50];
    char password[50];
    char Account_location[50];
    int found = 0;
    int c;

    //��������
    while ((c = getchar()) != '\n' && c != EOF);

    // �����˻�λ�ã���ַ��-----��ֹ��ͬ�˺����û�������������ͬ�����
    printf("������Ҫ�޸ĵ��˻�λ�ã���ַ��: ");
    if (fgets(Account_location, sizeof(Account_location), stdin) == NULL) {
        printf("���������������Ч���˻�λ�ã���ַ����\n");
        return;
    }
    Account_location[strcspn(Account_location, "\n")] = '\0';

    // �����û���
    printf("������Ҫ�޸ĵ��û���: ");
    if (fgets(username, sizeof(username), stdin) == NULL) {
        printf("���������������Ч���û�����\n");
        return;
    }
    username[strcspn(username, "\n")] = '\0';

    // ��������
    printf("�������Ӧ������: ");
    if (fgets(password, sizeof(password), stdin) == NULL) {
        printf("���������������Ч�����롣\n");
        return;
    }
    password[strcspn(password, "\n")] = '\0';

    // �������������м��ܣ����ڱȶ�
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
        printf("δ�ҵ��û���Ϊ %s ���˺�������˺�λ�ö�ƥ����˻���\n", username);
        return;
    }

    printf("�������µı��: ");
    if (scanf("%d", &temp->id) != 1) {
        printf("���������������Ч�ı�ţ����֣���\n");

        //��������
        while ((c = getchar()) != '\n' && c != EOF);
        return;
    }

    //��������
    while ((c = getchar()) != '\n' && c != EOF);

    printf("�������µ��˺�λ�ã���ַ��: ");
    if (fgets(temp->Account_location, sizeof(temp->Account_location), stdin) == NULL) {
        printf("���������������Ч���˺�λ��(49���ַ�����)��\n");
        return;
    }
    // ��ȫ�Ƴ����з�
    temp->Account_location[strcspn(temp->Account_location, "\n")] = '\0';


    printf("�������µ��˺�����: ");
    if (fgets(temp->description, sizeof(temp->description), stdin) == NULL) {
        printf("���������������Ч���˺�����(49���ַ�����)��\n");
        return;
    }
    // ��ȫ�Ƴ����з�
    temp->description[strcspn(temp->description, "\n")] = '\0';


    printf("�������µ��û���: ");
    if (fgets(temp->username, sizeof(temp->username), stdin) == NULL) {
        printf("���������������Ч���û���(49���ַ�����)��\n");
        return;
    }
    // ��ȫ�Ƴ����з�
    temp->username[strcspn(temp->username, "\n")] = '\0';


    printf("�������µ�����: ");
    if (fgets(temp->password, sizeof(temp->password), stdin) == NULL) {
        printf("���������������Ч������(49���ַ�����)��\n");
        return;
    }
    // ��ȫ�Ƴ����з�
    temp->password[strcspn(temp->password, "\n")] = '\0';


    // �û��˺��������
    for (int i = 0; temp->password[i] != '\0'; i++) {
        temp->password[i] += 1;
    }

    printf("�˻���Ϣ�޸ĳɹ���\n");
}

// ���棨��Ϣ¼�룩�ļ�����ģ��
void saveToFile() {
    FILE* file = fopen("accounts.txt", "w");
    int found = 0;

    if (file == NULL) {
        printf("�޷����ļ�����д�롣\n");
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
        printf("�˻���Ϣ�ѱ��浽�ļ���\n");
    else
        printf("�˻���Ϣ����ʧ�ܡ�\n");
}

// �ļ���Ϣ���أ�������������ģ�飨��֮ǰ�������ļ������Ϣ���ظ������У����ָ�֮ǰ��������Ϣ��
void loadFromFile() {
    FILE* file = fopen("accounts.txt", "r");

    if (file == NULL) {
        printf("�޷����ļ����ж�ȡ��\n");
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
    printf("�˻���Ϣ�Ѵ��ļ����ء�\n");
}

//һ�������ļ���Ϣģ��
void cleanfile() {
    FILE* file = fopen("accounts.txt", "w");

    if (file == NULL) {
        printf("�޷����ļ�����д�롣\n");
        return;
    }

    fclose(file);
    printf("�ļ��洢��Ϣ�ѱ���ȫɾ����\n");
}

// �ͷ������ڴ�
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
        printf("|* *            1. ����˻�            * *|\n");
        printf("| * *           2. ����˻�           * * |\n");
        printf("| * *           3. ��ѯ�˻�           * * |\n");
        printf("| * *           4. ɾ���˻�           * * |\n");
        printf("| * *           5. �޸��˻�           * * |\n");
        printf("| * *           6. ���浽�ļ���       * * |\n");
        printf("| * *           7. �ļ���Ϣ����       * * |\n");
        printf("| * *           8. һ�������ļ���Ϣ   * * |\n");
        printf("|* *            9. �˳�                * *|\n");
        printf("| *                                     * |\n");
        printf("|+++++++++++++++++++++++++++++++++++++++++|\n");
        printf("-------------------------------------------\n");
        printf("��ѡ�����: ");
        if (scanf("%d", &choice) != 1) {
            printf("���������������Ч��ѡ��\n");
            while (getchar() != '\n'); // ������뻺����
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
            printf("�˳�����\n");
            break;
        default:
            printf("��Ч��ѡ��\n");
        }
    } while (choice != 9);

    // �ͷ������ڴ�
    freeList();

    return 0;
}