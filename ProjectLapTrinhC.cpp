#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_BORROWS 100

int my_isspace(char c) {
    return (c==' ' || c=='\t' || c=='\n' || c=='\r' || c=='\v' || c=='\f');
}

int my_isdigit(char c) {
    return (c >= '0' && c <= '9');
}

int my_isalpha(char c) {
    return ( (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') );
}

char my_tolower(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

typedef struct {
    int day, month, year;
} Date;

typedef struct {
    int bookId;
    char title[50];
    char author[50];
    int quantity;
    int publishYear;
} Book;

typedef struct {
    int borrowId;
    int bookId;
    Date borrowDate;
    Date borrowReturn;
    char borrowerName[50];
    int status;
} Borrow;

Book bookList[MAX_BOOKS];
int bookNumber = 0;

Borrow borrowList[MAX_BORROWS];
int borrowCount = 0;

void trim(char *s) {
    int i = 0, j = strlen(s) - 1;

    while (my_isspace(s[i])) i++;
    while (j >= i && my_isspace(s[j])) j--;

    memmove(s, s + i, j - i + 1);
    s[j - i + 1] = '\0';
}

int inputInt(const char *msg, int allowEmpty) {
    char buf[100];

    while (1) {
        printf("%s", msg);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;
        trim(buf);

        if (allowEmpty && strlen(buf)==0) return -1;
        if (strlen(buf)==0) { printf("Khong duoc de trong!\n"); continue; }

        for (int i=0; buf[i]; i++)
            if (!my_isdigit(buf[i])) {
                printf("Chi duoc nhap so!\n");
                goto next;
            }

        return atoi(buf);

        next:;
    }
}

void inputString(char *out, int size, const char *msg) {
    while (1) {
        printf("%s", msg);
        fgets(out, size, stdin);
        out[strcspn(out, "\n")] = 0;
        trim(out);
        if (strlen(out)==0) { printf("Khong duoc de trong!\n"); continue; }
        return;
    }
}

int isLeap(int y){ return (y%4==0 && y%100!=0) || (y%400==0); }

int validDate(int d,int m,int y){
    if (y<1 || m<1 || m>12 || d<1) return 0;
    int md[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(m==2 && isLeap(y)) md[1]=29;
    return d<=md[m-1];
}

Date inputDate(const char *msg){
    Date dt; char buf[100];
    while(1){
        printf("%s", msg);
        fgets(buf,sizeof(buf),stdin); trim(buf);
        int d,m,y;
        if(sscanf(buf,"%d %d %d",&d,&m,&y)!=3){
            printf("Vui long nhap: dd mm yyyy\n"); continue;
        }
        if(!validDate(d,m,y)){
            printf("Ngay khong hop le!\n"); continue;
        }
        dt.day=d; dt.month=m; dt.year=y;
        return dt;
    }
}

int generateBookId(){
    int max=0;
    for(int i=0;i<bookNumber;i++)
        if(bookList[i].bookId > max) max = bookList[i].bookId;
    return max+1;
}

Book* findBook(int id){
    for(int i=0;i<bookNumber;i++)
        if(bookList[i].bookId==id) return &bookList[i];
    return NULL;
}

int duplicateTitle(const char *t){
    for(int i=0;i<bookNumber;i++)
        if(strcmp(bookList[i].title,t)==0) return 1;
    return 0;
}

void toLowerStr(char *s){
    for(int i=0;s[i];i++) s[i] = my_tolower(s[i]);
}

void sortBooks(){
    for(int i=0;i<bookNumber-1;i++)
        for(int j=i+1;j<bookNumber;j++){
            char a[50], b[50];
            strcpy(a,bookList[i].title);
            strcpy(b,bookList[j].title);
            toLowerStr(a); toLowerStr(b);
            if(strcmp(a,b)>0){
                Book tmp=bookList[i];
                bookList[i]=bookList[j];
                bookList[j]=tmp;
            }
        }
}

void addBook(){
    Book b;
    b.bookId = generateBookId();

    printf("\n=== THEM SACH ===\nID = %d\n", b.bookId);

    inputString(b.title,50,"Nhap ten sach: ");
    while(duplicateTitle(b.title)){
        printf("Ten da ton tai!\n");
        inputString(b.title,50,"Nhap lai: ");
    }

    inputString(b.author,50,"Tac gia: ");

    b.quantity = inputInt("So luong: ",0);
    while(b.quantity<=0){ printf("Phai >0!\n"); b.quantity=inputInt("Nhap lai: ",0); }

    b.publishYear = inputInt("Nam XB: ",0);
    while(b.publishYear<1900){ printf("Phai >=1900!\n"); b.publishYear=inputInt("Nhap lai: ",0); }

    bookList[bookNumber++] = b;
    printf(">> Them sach thanh cong!\n");
}

void updateBook(){
    int id = inputInt("Nhap ID can cap nhat: ",0);
    Book* b = findBook(id);
    if(!b){ printf("Khong tim thay!\n"); return; }

    char buf[50];

    printf("Ten moi (Enter=bo qua): ");
    fgets(buf,50,stdin); buf[strcspn(buf,"\n")]=0; trim(buf);
    if(strlen(buf)>0 && (!duplicateTitle(buf) || strcmp(buf,b->title)==0))
        strcpy(b->title,buf);

    printf("Tac gia moi: ");
    fgets(buf,50,stdin); buf[strcspn(buf,"\n")]=0; trim(buf);
    if(strlen(buf)>0) strcpy(b->author,buf);

    int v = inputInt("So luong moi (Enter=bo qua): ",1);
    if(v>0) b->quantity = v;

    v = inputInt("Nam XB moi (Enter=bo qua): ",1);
    if(v>=1900) b->publishYear = v;

    printf(">> Cap nhat thanh cong!\n");
}

void displayBook(){
    if(bookNumber==0){ printf("Chua co sach!\n"); return; }

    sortBooks();
    int perPage=10;
    int totalPage=(bookNumber+perPage-1)/perPage;
    int page=1;

    while(1){
        printf("\n=== TRANG %d / %d ===\n",page,totalPage);
        int start=(page-1)*perPage;
        int end=start+perPage;
        if(end>bookNumber) end=bookNumber;

        printf("+------+----------------------+----------------------+--------+-------+\n");
        printf("| ID   | Tieu de              | Tac gia              |  Nam   |  SL   |\n");
        printf("+------+----------------------+----------------------+--------+-------+\n");

        for(int i=start;i<end;i++){
            printf("| %-4d | %-20s | %-20s | %-6d | %-5d |\n",
                bookList[i].bookId,
                bookList[i].title,
                bookList[i].author,
                bookList[i].publishYear,
                bookList[i].quantity
            );
        }
        printf("+------+----------------------+----------------------+--------+-------+\n");

        int c = inputInt("1.Truoc  2.Thoat  3.Sau: ",0);
        if(c==2) break;
        if(c==1 && page>1) page--;
        if(c==3 && page<totalPage) page++;
    }
}

void deleteBook(){
    int id = inputInt("Nhap ID can xoa: ",0);
    int pos=-1;

    for(int i=0;i<bookNumber;i++)
        if(bookList[i].bookId==id) pos=i;

    if(pos<0){ printf("Khong tim thay!\n"); return; }

    for(int i=pos;i<bookNumber-1;i++)
        bookList[i]=bookList[i+1];

    bookNumber--;
    printf(">> Da xoa!\n");
}

void searchBook(){
    char key[50];
    inputString(key,50,"Nhap tu khoa: ");
    char lowKey[50]; strcpy(lowKey,key);
    toLowerStr(lowKey);

    int found = 0;
    for(int i=0;i<bookNumber;i++){
        char t[50], a[50];
        strcpy(t,bookList[i].title);
        strcpy(a,bookList[i].author);
        toLowerStr(t); toLowerStr(a);

        if(strstr(t,lowKey) || strstr(a,lowKey)){
            found=1;
            printf("[%d] %s - %s (SL %d)\n",
                bookList[i].bookId,
                bookList[i].title,
                bookList[i].author,
                bookList[i].quantity);
        }
    }
    if(!found) printf("Khong tim thay!\n");
}

int genBorrowId(){
    static int id=1000;
    return id++;
}

int validName(const char *s){
    for(int i=0;s[i];i++)
        if(!my_isalpha(s[i]) && s[i] != ' ')
            return 0;
    return 1;
}

void addBorrow(){
    Borrow b={0};

    printf("\n=== THEM PHIEU MUON ===\n");
    b.borrowId = genBorrowId();
    printf("BorrowID = %d\n", b.borrowId);

    b.bookId = inputInt("Nhap bookId: ",0);

    b.borrowDate = inputDate("Ngay muon (dd mm yyyy): ");

    inputString(b.borrowerName,50,"Nguoi muon: ");
    while(!validName(b.borrowerName)){
        printf("Ten chi chua chu cai!\n");
        inputString(b.borrowerName,50,"Nhap lai: ");
    }

    b.status=1;
    borrowList[borrowCount++] = b;

    printf(">> Thanh cong!\n");
}

void returnBook(){
    int id = inputInt("Nhap borrowId: ",0);

    for(int i=0;i<borrowCount;i++){
        Borrow *b = &borrowList[i];
        if(b->borrowId == id){

            if(b->status==0){
                printf("Phieu nay da tra!\n");
                return;
            }

            printf("Nguoi muon: %s\n", b->borrowerName);

            while(1){
                Date rt = inputDate("Nhap ngay tra: ");
                Date bd = b->borrowDate;

                if(rt.year < bd.year ||
                   (rt.year==bd.year && rt.month<bd.month) ||
                   (rt.year==bd.year && rt.month==bd.month && rt.day<bd.day))
                {
                    printf("Ngay tra < ngay muon!\n");
                    continue;
                }

                b->borrowReturn = rt;
                break;
            }

            b->status = 0;
            printf(">> Tra sach thanh cong!\n");
            return;
        }
    }
    printf("Khong tim thay!\n");
}

void displayBorrows(){
    if(borrowCount==0){ printf("Chua co phieu!\n"); return; }

    for(int i=0;i<borrowCount;i++){
        Borrow *b=&borrowList[i];

        printf("\nBorrowID: %d\n", b->borrowId);
        printf("BookId: %d\n", b->bookId);
        printf("Ngay muon: %02d/%02d/%04d\n",
            b->borrowDate.day, b->borrowDate.month, b->borrowDate.year);

        if(b->status==1) printf("Ngay tra: Chua tra\n");
        else printf("Ngay tra: %02d/%02d/%04d\n",
            b->borrowReturn.day, b->borrowReturn.month, b->borrowReturn.year);

        printf("Nguoi muon: %s\n", b->borrowerName);
        printf("Trang thai: %s\n", b->status?"Dang muon":"Da tra");
    }
}

void menuBook(){
    while(1){
        printf("*****************MENU********************\n");
	    printf("||1. Them moi sach                     ||\n");
	    printf("||2. Cap nhat thong tin sach           ||\n");
	    printf("||3. Hien thi danh sach (co phan trang)||\n");
	    printf("||4. Xoa thong tin sach                ||\n");
	    printf("||5. Tim kiem sach (tim kiem tuong doi)||\n");
	    printf("||6. Quay lai menu chinh               ||\n");
	    printf("*****************************************\n");

        int c = inputInt("Lua chon cua ban: ",0);
        switch(c){
            case 1: addBook(); break;
            case 2: updateBook(); break;
            case 3: displayBook(); break;
            case 4: deleteBook(); break;
            case 5: searchBook(); break;
            case 6: return;
            default: printf("Sai lua chon!\n");
        }
    }
}

void menuBorrow(){
    while(1){
        printf("*****************MENU***************\n");
        printf("||1. Them moi phieu muon          ||\n");
        printf("||2. Tra sach                     ||\n");
        printf("||3. Hien thi danh sach phieu muon||\n");
        printf("||4. Quay ve menu chinh           ||\n");
        printf("************************************\n");

        int c = inputInt("Lua chon cua ban: ",0);
        switch(c){
            case 1: addBorrow(); break;
            case 2: returnBook(); break;
            case 3: displayBorrows(); break;
            case 4: return;
            default: printf("Sai lua chon!\n");
        }
    }
}

int main(){
    while(1){
        printf("**********MENU***********\n");
        printf("||1. Quan ly thu vien  ||\n");
        printf("||2. Quan ly phieu muon||\n");
        printf("||3. Thoat chuong trinh||\n");
        printf("*************************\n");

        int c = inputInt("Lua chon cua ban: ",0);
        if(c==1) menuBook();
        else if(c==2) menuBorrow();
        else if(c==3){ printf("Tam biet!\n"); break; }
        else printf("Lua chon sai!\n");
    }
    return 0;
}
