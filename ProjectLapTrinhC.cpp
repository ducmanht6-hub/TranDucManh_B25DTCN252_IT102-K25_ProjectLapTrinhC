#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_TEXT 100
#define MAX_LEN 100
#define PAGE_SIZE 100
#define MAX_BORROWS 100

typedef struct {
	int day;
	int month;
	int year;
}Date;

typedef struct {
    int bookId;
    char title[50];
    char author[50];
    int quantity;
    int publishYear;
}Book;

typedef struct {
	int borrowId;
	int bookId;
	Date borrowDate;
	Date borrowReturn;
	char borrowerName[50];
	int status;
}Borrow;

Book bookList[MAX_BOOKS];
int bookNumber = 0;

Borrow borrowList[MAX_BORROWS];
int borrowCount = 0;

Book books[MAX_TEXT];
int totalBooks = 0;

void clearBuffer() {
    while (getchar() != '\n');
}

int my_tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

int my_isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

int my_isdigit(char c) {
    return (c >= '0' && c <= '9');
}

void toLowerStr(char *dest, const char *src) {
    for (int i = 0; src[i]; i++) {
        dest[i] = my_tolower(src[i]);
    }
    dest[strlen(src)] = '\0';
}

int isEmpty(const char *s) {
    while (*s) {
        if (!my_isspace(*s)) return 0;
        s++;
    }
    return 1;
}

int isNumber(char s[]) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (!my_isdigit(s[i])) return 0;
    }
    return 1;
}

int generateBookId() {
    int max = 0;
    for (int i = 0; i < bookNumber; i++)
        if (bookList[i].bookId > max)
            max = bookList[i].bookId;
    return max + 1;
}

Book* findBookById(int id) {
    for (int i = 0; i < bookNumber; i++)
        if (bookList[i].bookId == id)
            return &bookList[i];
    return NULL;
}

int isDuplicateTitle(const char *title) {
    for (int i = 0; i < bookNumber; i++)
        if (strcmp(bookList[i].title, title) == 0)
            return 1;
    return 0;
}

int generateBorrowId() {
    static int id = 1000;
    return id++;
}

int isPositiveInteger(const char *str) {
    char tmp[100];
    strcpy(tmp, str);

    char *start = tmp;
    while (*start == ' ') start++;

    char *end = start + strlen(start) - 1;
    while (end >= start && *end == ' ') {
        *end = '\0';
        end--;
    }

    if (strlen(start) == 0) return 0;

    for (int i = 0; start[i] != '\0'; i++) {
        if (start[i] < '0' || start[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return 1;
    }
    return 0;
}

int isValidDate(int day, int month, int year) {

    if (month < 1 || month > 12) {
        return 0;
    }

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1]) {
        return 0; 
    }

    return 1;
}

void trim(char *str) {
    int start = 0;
    int end = strlen(str) - 1;

    while (str[start] == ' ' && str[start] != '\0')
        start++;

    while (end > start && str[end] == ' ')
        end--;

    int j = 0;
    for (int i = start; i <= end; i++)
        str[j++] = str[i];

    str[j] = '\0';
}

int isAllSpace(const char *str) {
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ' && str[i] != '\t')
            return 0;
    return 1;
}

int isOnlySpaces(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ') 
            return 0; 
    }
    return 1;
}

void sortBooksAZ() {
    for (int i = 0; i < bookNumber - 1; i++) {
        for (int j = i + 1; j < bookNumber; j++) {
            if (strcasecmp(bookList[i].title, bookList[j].title) > 0) {
                Book temp = bookList[i];
                bookList[i] = bookList[j];
                bookList[j] = temp;
            }
        }
    }
}

int isValidName(const char *str) {
    for (int i = 0; str[i]; i++) {
        char c = str[i];


        if (c == ' ')
            continue;

        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            return 0; 
        }
    }
    return 1;
}

void addBook() {
    Book b;

    do {
        b.bookId = generateBookId();
    } while (findBookById(b.bookId) != NULL);

    if (b.bookId <= 0) {
        printf("Loi: ID khong hop le!\n");
        return;
    }

    printf("**********NHAP THEM SACH************\n");
    printf("\nID tu dong: %d\n", b.bookId);

    while (1) {
        printf("Nhap ten sach: ");
        fgets(b.title, sizeof(b.title), stdin);
        b.title[strcspn(b.title, "\n")] = '\0';

        if (isEmpty(b.title)) {
            printf("Loi: Ten sach khong duoc de trong!\n");
        }
        else if (isDuplicateTitle(b.title)) {
            printf("Loi: Ten sach da ton tai!\n");
        }
        else break;
    }

    while (1) {
        printf("Nhap ten tac gia: ");
        fgets(b.author, sizeof(b.author), stdin);
        b.author[strcspn(b.author, "\n")] = '\0';

        if (isEmpty(b.author)) {
            printf("Loi: Tac gia khong duoc de trong!\n");
        } else {
            break;
        }
    }

    while (1) {
    char input[50];

    printf("Nhap so luong (so nguyen duong > 0): ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Loi: Khong duoc de trong!\n");
        continue;
    }

    input[strcspn(input, "\n")] = '\0';

    char *start = input;
    while (*start == ' ') start++;

    char *end = input + strlen(input) - 1;
    while (end > start && *end == ' ') {
        *end = '\0';
        end--;
    }

    if (strlen(start) == 0) {
        printf("Loi: Ban chua nhap gi!\n");
        continue;
    }

    int isValid = 1;
    for (int i = 0; start[i] != '\0'; i++) {
        if (start[i] < '0' || start[i] > '9') {
            isValid = 0;
            break;
        }
    }

    if (!isValid) {
        printf("Loi: So luong phai la so nguyen duong (khong duoc chua ky tu khac)!\n");
        continue;
    }

    b.quantity = atoi(start);

    if (b.quantity <= 0) {
        printf("Loi: So luong phai > 0!\n");
        continue;
    }
    break;
	}

    while (1) {
    char buffer[50];

    printf("Nhap nam xuat ban (>1900): ");

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Loi! Vui long nhap lai.\n");
        continue;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    char *start = buffer;
    while (*start == ' ') start++;

    char *end = buffer + strlen(buffer) - 1;
    while (end > start && *end == ' ') {
        *end = '\0';
        end--;
    }

    if (strlen(start) == 0) {
        printf("Khong duoc de trong!\n");
        continue;
    }

    int isValid = 1;
    for (int i = 0; start[i] != '\0'; i++) {
        if (start[i] < '0' || start[i] > '9') {
            isValid = 0;
            break;
        }
    }

    if (!isValid) {
        printf("Chi duoc nhap so!\n");
        continue;
    }

    b.publishYear = atoi(start);

    if (b.publishYear <= 1900) {
        printf("Nam phai > 1900!\n");
        continue;
    }

    break;
	}

    clearBuffer(); 
    bookList[bookNumber++] = b;
    printf(">> Them sach thanh cong!\n");
}

void updateBook() {
    char input[100];
    int id;

    while (1) {
        printf("\nNhap ID sach can cap nhat: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf(">> Khong duoc de trong!\n");
            continue;
        }

        if (!isPositiveInteger(input)) {
            printf(">> ID phai la so nguyen duong!\n");
            continue;
        }

        id = atoi(input);
        if (id <= 0) {
            printf(">> ID phai > 0!\n");
            continue;
        }

        break;
    }

    Book *b = findBookById(id);
    if (!b) {
        printf(">> Khong tim thay sach!\n");
        return;
    }

    while (1) {
        printf("Ten moi (Enter de bo qua): ");
        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "\n") == 0) break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf(">> Tieu de khong duoc de trong!\n");
            continue;
        }

        if (isDuplicateTitle(input) && strcmp(b->title, input) != 0) {
            printf(">> Tieu de da ton tai!\n");
            continue;
        }

        strcpy(b->title, input);
        break;
    }

    while (1) {
        printf("Tac gia moi (Enter de bo qua): ");
        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "\n") == 0) break;

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf(">> Tac gia khong duoc de trong!\n");
            continue;
        }

        strcpy(b->author, input);
        break;
    }

    while (1) {
        printf("So luong moi (Enter = bo qua): ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) break;

        if (!isPositiveInteger(input)) {
            printf(">> Vui long nhap so nguyen duong hop le!\n");
            continue;
        }

        int qty = atoi(input);
        if (qty <= 0) {
            printf(">> So luong phai > 0!\n");
            continue;
        }

        b->quantity = qty;
        break;
    }

    while (1) {
        printf("Nam XB moi (Enter = bo qua): ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) break;

        if (!isPositiveInteger(input)) {
            printf(">> Vui long nhap so nguyen duong hop le!\n");
            continue;
        }

        int year = atoi(input);

        if (year < 1900) {
            printf(">> Nam xuat ban phai >= 1900!\n");
            continue;
        }

        b->publishYear = year;
        break;
    }
    printf(">> Cap nhat thanh cong!\n");
}

void displayBook() {
    if (bookNumber == 0) {
        printf("\n>>> Chua co sach nao!\n");
        return;
    }
	
	sortBooksAZ();
	
    int perPage = 10;
    int totalPage = (bookNumber + perPage - 1) / perPage;
    int page = 1;
    char input[20];
    int choice;

	    while (1) {
	        printf("\n=========== TRANG %d / %d ===========\n", page, totalPage);
	
	        int start = (page - 1) * perPage;
	        int end = start + perPage;
	        if (end > bookNumber) end = bookNumber;
	
	        printf("+------+----------------------+----------------------+--------+----------+\n");
	        printf("| ID   | Tieu de              | Tac gia              |  Nam   | So luong |\n");
	        printf("+------+----------------------+----------------------+--------+----------+\n");
	
	        for (int i = start; i < end; i++) {
	            printf("| %-4d | %-20s | %-20s | %-6d | %-8d |\n",
	                bookList[i].bookId,
	                bookList[i].title,
	                bookList[i].author,
	                bookList[i].publishYear,
	                bookList[i].quantity
	            );
	        }
	
	        printf("+------+----------------------+----------------------+--------+----------+\n");

        while (1) {
	    printf("\n1. Trang truoc | 2. Thoat | 3. Trang sau\n");
	    printf("Nhap lua chon: ");
	
	    fgets(input, sizeof(input), stdin);
	
	    input[strcspn(input, "\n")] = '\0';
	
	    trim(input);
	
	    if (strlen(input) == 0) {
	        printf("Loi: Khong duoc de trong hoac chi chua dau cach!\n");
	        continue;
	    }
	
	    int valid = 1;
	    for (int i = 0; i < strlen(input); i++) {
	        if (input[i] < '0' || input[i] > '9') {
	            valid = 0;
	            break;
	        }
	    }
	
	    if (!valid) {
	        printf("Loi: Chi duoc nhap so (1, 2, 3)!\n");
	        continue;
	    }
	
	    choice = atoi(input);
	
	    if (choice < 1 || choice > 3) {
	        printf("Loi: Chi duoc nhap 1, 2 hoac 3!\n");
	        continue;
	    }
	
	    break;
		}
        if (choice == 1 && page > 1) page--;
        else if (choice == 2) break;
        else if (choice == 3 && page < totalPage) page++;
    	}
}

void deleteBook() {
    int id;
    printf("Nhap ID sach muon xoa: ");
    scanf("%d", &id);
    clearBuffer();

    int pos = -1;
    for (int i = 0; i < bookNumber; i++)
        if (bookList[i].bookId == id)
            pos = i;

    if (pos == -1) {
        printf(">> Khong tim thay sach!\n");
        return;
    }

    for (int i = pos; i < bookNumber - 1; i++)
        bookList[i] = bookList[i + 1];

    bookNumber--;
    printf(">> Xoa thanh cong!\n");
}

void searchBook() {
    char key[MAX_LEN];

    printf("Nhap tu khoa tim kiem: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    if (isAllSpace(key)) {
        printf("Khong duoc de trong!\n");
        return;
    }

    trim(key);

    char keyLower[MAX_LEN];
    toLowerStr(keyLower, key);

    int found = 0;
    printf("\n=== KET QUA ===\n");
    printf("+------+------------------------------+--------------------------+-------+\n");
    printf("| ID   | Title                        | Author                   |  SL   |\n");
    printf("+------+------------------------------+--------------------------+-------+\n");

    for (int i = 0; i < bookNumber; i++) {
        char titleLower[MAX_LEN];
        char authorLower[MAX_LEN];

        toLowerStr(titleLower, bookList[i].title);
        toLowerStr(authorLower, bookList[i].author);

        if (strstr(titleLower, keyLower) || strstr(authorLower, keyLower)) {
            found = 1;
            printf("| %-4d | %-28s | %-24s | %-5d |\n",
                   bookList[i].bookId,
                   bookList[i].title,
                   bookList[i].author,
                   bookList[i].quantity);
        }
    }
    printf("+------+------------------------------+--------------------------+-------+\n");
    if (!found)
        printf("Khong tim thay sach!\n");
}

void addBorrow() {
    if (borrowCount >= MAX_BORROWS) {
        printf("Danh sach phieu muon da day!\n");
        return;
    }

    Borrow b = {0};
    char input[50];

    b.borrowId = generateBorrowId();
    printf("\n=== THEM PHIEU MUON ===\n");
    printf("borrowId duoc tao tu dong: %d\n", b.borrowId);

    while (1) {
        printf("Nhap bookId (so nguyen duong, khong de trong): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0 || isOnlySpaces(input)) {
            printf("Loi: bookId khong duoc de trong hoac chi co dau cach!\n");
            continue;
        }

        if (!isPositiveInteger(input)) {
            printf("Loi: bookId phai la so nguyen duong!\n");
            continue;
        }

        b.bookId = atoi(input);
        if (b.bookId <= 0) {
            printf("Loi: bookId phai > 0!\n");
            continue;
        }

        break;
    }

    while (1) {
    printf("Nhap ngay muon (dd mm yyyy): ");
    fgets(input, sizeof(input), stdin);

    if (strlen(input) == 1 || isOnlySpaces(input)) {
        printf("Loi: Khong duoc de trong hoac chi nhap dau cach!\n");
        continue;
    }

    int d, m, y;
    if (sscanf(input, "%d %d %d", &d, &m, &y) != 3) {
        printf("Loi: Moi nhap dung 3 so (dd mm yyyy)!\n");
        continue;
    }

    if (y <= 0) {
        printf("Loi: Nam phai la so nguyen duong!\n");
        continue;
    }

    if (!isValidDate(d, m, y)) {
        printf("Loi: Ngay khong hop le! (Ngay <= 31, Thang <= 12, Nam > 0, Ngay thang 2 phai <= 29 trong nam nhuan)\n");
        continue;
    }

    b.borrowDate.day = d;
    b.borrowDate.month = m;
    b.borrowDate.year = y;
    break;
	}

    while (1) {
    printf("Nhap ten nguoi muon (khong de trong, chi nhan chu): ");
    fgets(b.borrowerName, sizeof(b.borrowerName), stdin);
    b.borrowerName[strcspn(b.borrowerName, "\n")] = '\0';

    if (strlen(b.borrowerName) == 0 || isOnlySpaces(b.borrowerName)) {
        printf("Loi: Ten nguoi muon khong duoc de trong hoac chi co dau cach!\n");
        continue;
    }

    if (!isValidName(b.borrowerName)) {
        printf("Loi: Ten chi duoc phep chua chu cai va dau cach! (Khong duoc co so hoac ky tu dac biet)\n");
        continue;
    }

    break;
	}

    b.status = 1;
    borrowList[borrowCount++] = b;

    printf(">> Them phieu muon thanh cong! (Trang thai: Dang muon)\n");
}

void returnBook() {
    char input[50];
    int id;

    while (1) {
        printf("Nhap borrowId: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (isAllSpace(input)) {
            printf("Loi: borrowId khong duoc de trong!\n");
            continue;
        }

        trim(input);

        if (strlen(input) == 0) {
            printf("Loi: borrowId khong duoc de trong!\n");
            continue;
        }

        int valid = 1;
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Loi: borrowId phai la so nguyen duong!\n");
            continue;
        }

        id = atoi(input);
        if (id <= 0) {
            printf("Loi: borrowId phai > 0!\n");
            continue;
        }

        break;
    }

    for (int i = 0; i < borrowCount; i++) {
        if (borrowList[i].borrowId == id) {

            if (borrowList[i].status == 0) {
                printf("Phieu muon nay da tra truoc do!\n");
                return;
            }

            char cleanName[50];
            strcpy(cleanName, borrowList[i].borrowerName);
            trim(cleanName);

            printf("Nguoi muon: %s\n", cleanName);
            printf("Nguoi tra mac dinh la nguoi muon.\n");

            int d, m, y;

            while (1) {
                printf("Nhap ngay tra (dd mm yyyy): ");
                fgets(input, sizeof(input), stdin);

                if (isAllSpace(input)) {
                    printf("Loi: Khong duoc de trong!\n");
                    continue;
                }

                trim(input);

                if (sscanf(input, "%d %d %d", &d, &m, &y) != 3) {
                    printf("Loi: Moi nhap dung 3 so (dd mm yyyy)!\n");
                    continue;
                }

                if (y <= 0) {
                    printf("Loi: Nam phai la so nguyen duong!\n");
                    continue;
                }

                if (!isValidDate(d, m, y)) {
                    printf("Loi: Ngay khong hop le!\n");
                    continue;
                }

                Date borrow = borrowList[i].borrowDate;

                if (y < borrow.year ||
                   (y == borrow.year && m < borrow.month) ||
                   (y == borrow.year && m == borrow.month && d < borrow.day)) 
                {
                    printf("Loi: Ngay tra khong the som hon ngay muon!\n");
                    continue;
                }

                break;
            }

            borrowList[i].borrowReturn.day = d;
            borrowList[i].borrowReturn.month = m;
            borrowList[i].borrowReturn.year = y;

            borrowList[i].status = 0;

            printf(">> Tra sach thanh cong! Nguoi tra: %s\n", cleanName);
            return;
        }
    }

    printf("Khong tim thay phieu muon!\n");
}

void displayBorrows() {
    if (borrowCount == 0) {
        printf("Chua co phieu muon!\n");
        return;
    }

    printf("\n=== DANH SACH PHIEU MUON ===\n");

    for (int i = 0; i < borrowCount; i++) {

        printf("\nBorrowId: %d\n", borrowList[i].borrowId);
        printf("BookId: %d\n", borrowList[i].bookId);

        printf("Ngay muon: %02d/%02d/%04d\n",
               borrowList[i].borrowDate.day,
               borrowList[i].borrowDate.month,
               borrowList[i].borrowDate.year);

        if (borrowList[i].borrowReturn.day == 0 &&
            borrowList[i].borrowReturn.month == 0 &&
            borrowList[i].borrowReturn.year == 0) 
        {
            printf("Ngay tra: Chua tra\n");
        } 
        else 
        {
            printf("Ngay tra: %02d/%02d/%04d\n",
                   borrowList[i].borrowReturn.day,
                   borrowList[i].borrowReturn.month,
                   borrowList[i].borrowReturn.year);
        }

        char cleanName[50];
        strcpy(cleanName, borrowList[i].borrowerName);
        trim(cleanName);

        printf("Nguoi muon: %s\n", cleanName);

        printf("Trang thai: %s\n",
               borrowList[i].status == 1 ? "Dang muon" : "Da tra");
        printf("-------------------------\n");
    }
}

void libraryMenu() {
    int choice;
	char input[50];
	
	do {
	    printf("*****************MENU********************\n");
	    printf("||1. Them moi sach                     ||\n");
	    printf("||2. Cap nhat thong tin sach           ||\n");
	    printf("||3. Hien thi danh sach (co phan trang)||\n");
	    printf("||4. Xoa thong tin sach                ||\n");
	    printf("||5. Tim kiem sach (tim kiem tuong doi)||\n");
	    printf("||6. Quay lai menu chinh               ||\n");
	    printf("*****************************************\n");
	    printf("Lua chon cua ban: ");
	
	    if (fgets(input, sizeof(input), stdin) == NULL) {
	        printf("Loi: Vui long nhap mot so nguyen!\n");
	        continue;
	    }
	
	    input[strcspn(input, "\n")] = '\0';
	
	    char *start = input;
	    while (*start == ' ') start++;
	
	    char *end = input + strlen(input) - 1;
	    while (end > start && *end == ' ') {
	        *end = '\0';
	        end--;
	    }
	
	    if (strlen(start) == 0) {
	        printf("Loi: Khong duoc bo trong! Vui long nhap so nguyen hop le!\n");
	        continue;
	    }
	
	    int isValid = 1;
	    for (int i = 0; start[i] != '\0'; i++) {
	        if (start[i] < '0' || start[i] > '9') {
	            isValid = 0;
	            break;
	        }
	    }
	
	    if (!isValid) {
	        printf("Loi: Vui long chi nhap so nguyen hop le!\n");
	        continue;
	    }
	
	    choice = atoi(start);
	
	    if (choice >= 1 && choice <= 6) {
	        switch (choice) {
	            case 1:
	                addBook();
	                break;
	            case 2:
	                updateBook();
	                break;
	            case 3:
	                displayBook();
	                break;
	            case 4:
	                deleteBook();
	                break;
	            case 5:
	                searchBook();
	                break;
	            case 6:
	                return;
	        }
	    } else {
	        printf("So ban vua nhap khong co trong menu, vui long nhap lai!\n");
	    }	
	} while (1);
}

void borrowMenu() {
    int choice;
    char input[50];

    do {
        printf("*****************MENU***************\n");
        printf("||1. Them moi phieu muon          ||\n");
        printf("||2. Tra sach                     ||\n");
        printf("||3. Hien thi danh sach phieu muon||\n");
        printf("||4. Quay ve menu chinh           ||\n");
        printf("************************************\n");

        printf("Lua chon cua ban: ");

        while (1) {
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Loi: Khong duoc de trong! Nhap lai: ");
                continue;
            }

            input[strcspn(input, "\n")] = '\0';

            char *start = input;
            while (*start == ' ') start++;

            char *end = input + strlen(input) - 1;
            while (end > start && *end == ' ') {
                *end = '\0';
                end--;
            }

            if (strlen(start) == 0) {
                printf("Loi: Khong duoc de trong! Nhap lai: ");
                continue;
            }

            int isValid = 1;
            for (int i = 0; start[i] != '\0'; i++) {
                if (start[i] < '0' || start[i] > '9') {
                    isValid = 0;
                    break;
                }
            }

            if (!isValid) {
                printf("Loi: Vui long nhap so nguyen hop le (1-4): ");
                continue;
            }

            choice = atoi(start);
            break;
        }

        switch (choice) {
            case 1:
                addBorrow();
                break;
            case 2:
                returnBook();
                break;
            case 3:
                displayBorrows();
                break;
            case 4:
                return;
            default:
                printf("So ban vua nhap khong co trong menu, vui long nhap lai!\n");
        }
    } while (choice != 4);
}

int main() {
    int choice;
    char input[50];

    do {
        printf("**********MENU***********\n");
        printf("||1. Quan ly thu vien  ||\n");
        printf("||2. Quan ly phieu muon||\n");
        printf("||3. Thoat chuong trinh||\n");
        printf("*************************\n");
        printf("Lua chon cua ban: ");

        while (1) {
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Loi: Vui long nhap mot so nguyen!\n");
                continue;
            }

            input[strcspn(input, "\n")] = 0;
            char *start = input;
            while (*start == ' ') start++;

            char *end = input + strlen(input) - 1;
            while (end > start && *end == ' ') {
                *end = '\0';
                end--;
            }

            if (strlen(start) == 0) {
                printf("Loi: Khong duoc de trong! Vui long nhap mot so nguyen: ");
                continue;
            }

            int isValid = 1;
            for (int i = 0; i < strlen(start); i++) {
                if (start[i] < '0' || start[i] > '9') {
                    isValid = 0;
                    break;
                }
            }

            if (!isValid) {
                printf("Loi: Vui long nhap so nguyen hop le: ");
                continue;
            }

            choice = atoi(start);
            break;
        }

        switch (choice) {
            case 1:
                printf("Ban chon menu 1\n");
                libraryMenu();
                break;
            case 2:
                printf("Ban chon menu 2\n");
                borrowMenu();
                break;
            case 3:
                printf("Hen gap lai lan sau!\n");
                exit(0);
            default:
                printf("So ban vua nhap khong co trong menu, vui long nhap lai!\n");
        }

    } while (choice != 3);

    return 0;
}
