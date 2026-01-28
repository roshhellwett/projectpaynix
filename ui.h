/*
* PayNix - Billing Software
 * Copyright (c) 2026 Roshan Kumar Singh
 * GitHub: https://github.com/roshhellwett/PayNix
 * Licensed under MIT License
 */
#ifndef BILLINGSOFTWARE_UI_H
#define BILLINGSOFTWARE_UI_H

enum Color { RED=31, GREEN=32, YELLOW=33, BLUE=34, CYAN=36, RESET=0 };

void setColor(int);
void resetColor();
void clearScreen();
void printBoxTitle(const char*);
void inputHidden(char* buffer, int size);
#endif //BILLINGSOFTWARE_UI_H