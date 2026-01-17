#include <cstdio>
#include "viewCustomers.h"
#include "items.h"
#include "ui.h"

void viewCustomers() {

    clearScreen();
    printBoxTitle("CUSTOMER BILL HISTORY");

    FILE *fp;
    struct BillHeader bh;

    fp = fopen("bills.dat", "rb");
    if (fp == NULL) {
        setColor(RED);
        printf("NO CUSTOMER DATA FOUND.\n");
        resetColor();
        return;
    }

    struct BillItem bi;

    while (fread(&bh, sizeof(struct BillHeader), 1, fp)) {

        printf("\nCUSTOMER NAME : %s", bh.customer_name);
        printf("\nPHONE NUMBER  : %s", bh.phone);
        printf("\nDATE & TIME   : %s", bh.datetime);
        printf("\nTOTAL BILL    : %.2f\n", bh.grand_total);
        printf("--------------------------------------------\n");

        for (int i = 0; i < bh.item_count; i++) {
            fread(&bi, sizeof(struct BillItem), 1, fp);
        }
    }


    fclose(fp);
}
