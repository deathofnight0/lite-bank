#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
struct cust_st
{
    int acc_no;
    char cust_nm[30];
    float bal;
};
struct tran_st
{
    int acc_no;
    char trantype;
    float amt;
};
bool accountExists(int acc_no)
{
    FILE *fp;
    struct cust_st custdata;
    int size = sizeof(struct cust_st);
    if ((fp = fopen("customer.txt", "r")) == NULL)
    {
        printf("\nError opening customer file!");
        getch();
        return false;
    }
    while ((fread(&custdata, size, 1, fp)) == 1)
    {
        if (custdata.acc_no == acc_no)
        {
            fclose(fp);
            return true;
        }
    }
    fclose(fp);
    return false;
}

void addcust()
{
    FILE *fp;
    char flag = 'y';
    struct cust_st custdata;
    if ((fp = fopen("customer.txt", "a+")) == NULL)
    {
        printf("\nError opening customer file!");
        getch();
        return;
    }
    while (flag == 'y')
    {
        printf("\nEnter Account Number:");
        scanf("%d", &custdata.acc_no);
        if (accountExists(custdata.acc_no))
        {
            printf("\nAccount number already exists. Please enter a different account number.\n");
            continue;
        }
        printf("\nEnter Customer Name:");
        fflush(stdin);
        gets(custdata.cust_nm);
        printf("\nEnter Account Balance:");
        scanf("%f", &custdata.bal);
        fwrite(&custdata, sizeof(struct cust_st), 1, fp);
        printf("\n\nAdd Another Account? (y/n):");
        fflush(stdin);
        scanf("%c", &flag);
    }
    fclose(fp);
}
void rectran()
{
    FILE *fp1, *fp2;
    char flag = 'y', found, val_flag;
    struct cust_st custdata;
    struct tran_st trandata;
    int size = sizeof(struct cust_st);
    if ((fp1 = fopen("customer.txt", "r+w")) == NULL)
    {
        printf("\nError opening customer file!");
        getch();
        return;
    }
    if ((fp2 = fopen("trans.txt", "a+")) == NULL)
    {
        printf("\nError opening transaction file!");
        getch();
        return;
    }
    while (flag == 'y')
    {
        printf("Enter account number:");
        scanf("%d", &trandata.acc_no);
        found = 'n';
        val_flag = 'n';
        rewind(fp1);
        while ((fread(&custdata, size, 1, fp1)) == 1 && found == 'n')
        {
            if (custdata.acc_no == trandata.acc_no)
            {
                found = 'y';
                break;
            }
        }
        if (found == 'y')
        {
            while (val_flag == 'n')
            {
                printf("\nEnter Transaction Type (D/W):");
                fflush(stdin);
                scanf("%c", &trandata.trantype);
                if (trandata.trantype != 'D' && trandata.trantype != 'd' && trandata.trantype != 'W' && trandata.trantype != 'w')
                    printf("Invalid transaction type! Please reenter");
                else
                    val_flag = 'y';
            }
            val_flag = 'n';
            while (val_flag == 'n')
            {
                printf("\nEnter amount:");
                scanf("%f", &trandata.amt);
                if (trandata.trantype == 87 || trandata.trantype == 119)
                {
                    if (trandata.amt > custdata.bal)
                        printf("\nAccount balance is %f. Please reenter withdrawal amount", custdata.bal);
                    else
                    {
                        custdata.bal = custdata.bal - trandata.amt;
                        val_flag = 'y';
                    }
                }
                else
                {
                    custdata.bal += trandata.amt;
                    val_flag = 'y';
                }
            }
            fwrite(&trandata, sizeof(struct tran_st), 1, fp2);
            fseek(fp1, (long)(-size), 1);
            fwrite(&custdata, size, 1, fp1);
        }
        else
            printf("\nThis account number doesn't exist!");
        printf("\nRecord another transaction? (y/n):");
        fflush(stdin);
        scanf("%c", &flag);
    }
    fclose(fp1);
    fclose(fp2);
}
void prnacc()
{
    FILE *fp;
    struct cust_st custdata;
    char flag = 'n';
    if ((fp = fopen("customer.txt", "r")) == NULL)
    {
        printf("\n\nError opening customer file!");
        getch();
        return;
    }
    printf("\nReport on all accounts:");
    printf("\n\nAcc no.\t\tName\t\tBalance\n");
    while ((fread(&custdata, sizeof(struct cust_st), 1, fp)) == 1)
    {
        flag = 'y';
        printf("\n%d\t\t%s\t\t%f", custdata.acc_no, custdata.cust_nm, custdata.bal);
    }
    if (flag == 'n')
        printf("\nNo account found!!");
    getch();
    fclose(fp);
}
void main()
{
    int choice = 1;
    while (choice != 4)
    {
        printf("\nSelect choice from menu:-\n\n");
        printf("1. Accept customer details\n");
        printf("2. Record Withdrawal/Deposit trasaction\n");
        printf("3. Print all accounts\n");
        printf("4. Exit\n\n");
        printf("Enter choice:");
        scanf("%d", &choice);
        if (choice == 1)
            addcust();
        if (choice == 2)
            rectran();
        else if (choice == 3)
            prnacc();
    }
}
