#ifndef MESSAGETEMPERATURE_H
#define MESSAGETEMPERATURE_H

#include<stdlib.h>
#include<stdio.h>

typedef unsigned char byte;

typedef struct MessageTemperature MessageTemperature;

typedef struct MessageTemperatureOps
{
    int (*getValeur)();
    byte (*getType)();
    char *(*getPiece)();
}MessageTemperatureOps;

struct MessageTemperature
{
    void * obj;
    MessageTemperatureOps * ops;
};

typedef struct MessageTemperature_obj
{
    int valeur;
    byte type;
    char *piece;
}MessageTemperature_obj;

MessageTemperature * new_MessageTemperature(int valeur,byte type,char *piece);
MessageTemperature* fromBytes(byte *data,int length);
byte* toBytes();
void delete_MessageTemperature(MessageTemperature *msg);


#endif     