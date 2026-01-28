/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#include <cstdio>
#include <cstring>
#include "storeItems.h"
#include "items.h"
#include "dataPath.h"

int itemIdExists(const char *id){
    FILE *fp=fopen(ITEMS_FILE,"rb");
    if(!fp) return 0;
    items t;
    while(fread(&t,sizeof(t),1,fp))
        if(strcmp(t.item_number,id)==0){ fclose(fp); return 1; }
    fclose(fp);
    return 0;
}

void storeItems(){
    FILE *fp=fopen(ITEMS_FILE,"ab");
    items it;
    int n;

    printf("HOW MANY ITEMS : ");
    scanf("%d",&n); getchar();

    for(int i=0;i<n;i++){
        printf("\nITEM %d\n",i+1);

        printf("ITEM NAME : ");
        fgets(it.item_name,50,stdin);
        it.item_name[strcspn(it.item_name,"\n")]=0;

        do{
            printf("ITEM ID : ");
            fgets(it.item_number,30,stdin);
            it.item_number[strcspn(it.item_number,"\n")]=0;
        }while(itemIdExists(it.item_number));

        printf("PRICE : ");
        scanf("%f",&it.item_price); getchar();

        fwrite(&it,sizeof(it),1,fp);
    }
    fclose(fp);
}
