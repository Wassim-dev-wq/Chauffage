#include <stdlib.h>
#include <string.h>
#include "MessageTemperature.h"
#define CHAR_BIT 8

const static byte MESURE = 0;
const static byte CHAUFFER = 1;

int j = 0;
static int getValeur();
static byte getType();
static char *getPiece();
static MessageTemperatureOps MSGOPS = {
    .getValeur = getValeur,
    .getType = getType,
    .getPiece = *getPiece,
};

MessageTemperature *new_MessageTemperature(int valeur, byte type, char *piece)
{
    MessageTemperature *imp;
    MessageTemperature_obj *obj;
    imp = malloc(sizeof(*imp));
    obj = malloc(sizeof(*obj));
    imp->obj = obj;
    imp->ops = &MSGOPS;
    obj->valeur = valeur;
    obj->type = type;
    obj->piece = piece;
    return (MessageTemperature *)imp;
}

void delete_MessageTemperature(MessageTemperature *msg)
{
    free(msg->obj);
    free(msg);
}

MessageTemperature *fromBytes(byte *tab, int length)
{
    int val[4];
    for (int i = 0; i < 4; i++)
    {
        if (tab[i] < 0)
            val[i] = (tab[i] + 256) << (i * 8);
        else
            val[i] = tab[i] << (i * 8);
    }
    int valeur = val[0] | val[1] | val[2] | val[3];
    char *piece = "chambre";
    char *type;
    if (tab[4] == 0)
    {
        type = "mesure";
    }
    else
        type = "chauffer";
    printf("valeur = %d,type = %s, piece = %s", valeur, type, piece);
    j++;
    return new_MessageTemperature(valeur, tab[4], piece);
}

void string2ByteArray(char* input, byte* output)
{
    int loop;
    int i;
    loop = 0;
    i = 0;
    while(input[loop] != '\0')
    {
        output[i++] = input[loop++];
    }
}

byte* toBytes(MessageTemperature *msg){
    MessageTemperature_obj *m = (MessageTemperature_obj *)msg->obj;
    byte* tab = (byte*)malloc((strlen(m->piece)+5)*sizeof(byte));
    //printf("val = %d , piece = %s ",m->valeur,m->piece);
	int val = m->valeur;
	for (int i=0; i < 4; i++)
	    {
		tab[i] = (unsigned int) (val & 0x000000FF);
		val = val >> 8;
        //printf("tab [%d] : %d\n",i,tab[i]);
	    }
	tab[4] = m->type;
	byte* tabPiece = malloc((strlen(m->piece)*sizeof(char)));
    string2ByteArray(m->piece,tabPiece);
	for (int i=0; i < strlen(m->piece); i++){
	    tab[i+5] = tabPiece[i];
    }
    printf("data sent : valeur = %d, type = %d, piece = %s\n",val,tab[4],tabPiece);
	return tab;
}
static int getValeur(MessageTemperature *msg)
{
    MessageTemperature_obj *m = (MessageTemperature_obj *)msg->obj;
    return m->valeur;
}

static byte getType(MessageTemperature *msg)
{
    MessageTemperature_obj *m = (MessageTemperature_obj *)msg->obj;
    return m->type;
}

static char *getPiece(MessageTemperature *msg)
{
    MessageTemperature_obj *m = (MessageTemperature_obj *)msg->obj;
    return m->piece;
}

void display_data(){
        for(int i=0; i<j;i++){
            MessageTemperature *msg;
            MessageTemperature_obj *m = (MessageTemperature_obj *)msg->obj;
            printf("%s",m->piece);
        }

}
// int main(void){
//     byte *data = "[B@1be6f5c3";
//     MessageTemperature *m = fromBytes(data,sizeof(data));
//     printf("%s",getPiece(m));
//     getValeur(m);
// }