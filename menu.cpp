/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include "menu.h"
#include "storeItems.h"
#include "showItems.h"
#include "billing.h"
#include "updateItems.h"
#include "deleteItems.h"
#include "viewCustomers.h"
#include "resumeOrder.h"
#include "credits.h"
#include "ui.h"

void menu() {

    printf("\n========= MENU =========\n");
    printf("1. STORE NEW ITEMS\n");
    printf("2. BILLING (MULTIPLE ITEMS)\n");
    printf("3. SHOW ALL ITEMS\n");
    printf("4. UPDATE ITEM / PRICE\n");
    printf("5. DELETE ITEMS\n");
    printf("6. VIEW CUSTOMER DATA\n");
    printf("7. MAKE CHANGES TO ORDER\n");
    printf("8. CREDITS\n");
    printf("9. LOGOUT\n");
    printf("========================\n");
}

void handleMenuChoice(char choice) {

    switch (choice) {

        case '1':
            printBoxTitle("STORE NEW ITEMS");
            storeItems();
            break;

        case '2':
            printBoxTitle("BILLING SECTION");
            billing();
            break;

        case '3':
            printBoxTitle("AVAILABLE ITEMS");
            showItemsForBilling();
            break;

        case '4':
            printBoxTitle("UPDATE ITEM / PRICE");
            updateItems();
            break;

        case '5':
            printBoxTitle("DELETE ITEMS");
            deleteItems();
            break;

        case '6':
            printBoxTitle("CUSTOMER BILL HISTORY");
            viewCustomers();
            break;

        case '7':
            printBoxTitle("RESUME / EDIT ORDER");
            resumeOrder();
            break;

        case '8':
            printBoxTitle("CREDITS");
            showCreatorInfo();
            break;

        case '9':
            setColor(GREEN);
            printf("\nLOGGING OUT...\n");
            resetColor();
            return;

        default:
            setColor(RED);
            printf("\nINVALID CHOICE\n");
            resetColor();
    }

    printf("\nPRESS ENTER TO CONTINUE...");
    getchar();
}
