#include <cstdio>
#include "deleteItems.h"
#include "items.h"
#include "showItems.h"
#include "ui.h"

void deleteItems() {

    clearScreen();
    printBoxTitle("DELETE ITEMS");

    showItemsForBilling();

    FILE *fp, *tempfp;
    struct items item;
    int delete_no, found = 0;

    fp = fopen("items.dat", "rb");
    if (fp == NULL) {
        setColor(RED);
        printf("NO ITEMS FOUND.\n");
        resetColor();
        return;
    }

    tempfp = fopen("temp.dat", "wb");
    if (tempfp == NULL) {
        setColor(RED);
        printf("TEMP FILE ERROR.\n");
        resetColor();
        fclose(fp);
        return;
    }

    printf("ENTER THE ITEM ID TO DELETE: ");
    scanf("%d", &delete_no);

    while (fread(&item, sizeof(struct items), 1, fp)) {
        if (item.item_number == delete_no) {
            found = 1;
            continue;
        }
        fwrite(&item, sizeof(struct items), 1, tempfp);
    }

    fclose(fp);
    fclose(tempfp);

    remove("items.dat");
    rename("temp.dat", "items.dat");

    if (found) {
        setColor(GREEN);
        printf("ITEM DELETED SUCCESSFULLY.\n");
        resetColor();
    } else {
        setColor(RED);
        printf("ITEM NOT FOUND.\n");
        resetColor();
    }
}
