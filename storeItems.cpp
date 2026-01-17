#include <cstdio>
#include <cstring>
#include "storeItems.h"
#include "items.h"

int itemIdExists(int id) {
    FILE *fp;
    struct items temp;

    fp = fopen("items.dat", "rb");
    if (!fp) return 0;

    while (fread(&temp, sizeof(struct items), 1, fp)) {
        if (temp.item_number == id) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void storeItems() {

    struct items item;
    FILE *fp;
    int n;

    printf("\nHOW MANY ITEMS YOU WANT TO ADD : ");
    scanf("%d", &n);

    fp = fopen("items.dat", "ab");
    if (!fp) {
        printf("FILE ERROR!\n");
        return;
    }

    for (int i = 0; i < n; i++) {

        printf("\nITEM %d\n", i + 1);

        getchar();
        printf("ITEM NAME : ");
        fgets(item.item_name, sizeof(item.item_name), stdin);
        item.item_name[strcspn(item.item_name, "\n")] = '\0';

        while (1) {
            printf("ITEM ID : ");
            scanf("%d", &item.item_number);

            if (itemIdExists(item.item_number)) {
                printf("ERROR: ITEM ID ALREADY EXISTS. TRY AGAIN.\n");
            } else {
                break;
            }
        }

        printf("ITEM PRICE : ");
        scanf("%f", &item.item_price);

        fwrite(&item, sizeof(struct items), 1, fp);
    }

    fclose(fp);
    printf("\nITEMS STORED SUCCESSFULLY\n");
}
