//
// Created by roshh on 17-01-2026.
//

#ifndef BILLINGSOFTWARE_UI_H
#define BILLINGSOFTWARE_UI_H

#define RED     31
#define GREEN   32
#define YELLOW  33
#define BLUE    34
#define CYAN    36
#define WHITE   37


void clearScreen();
void setColor(int color);
void resetColor();
void printBoxTitle(const char *title);


#endif //BILLINGSOFTWARE_UI_H