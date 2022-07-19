#ifndef BILD_H
#define BILD_H

#include "Nokia5110.h"

const uint8_t termometro_Width    = 84;
const uint8_t termometro_Height   = 48;
const uint8_t termometro[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0x98, 0x3c, 0x7e, 0x7e, 0x7e, 0x3c, 0x98, 0x41, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t zero_Width    = 12;
const uint8_t zero_Height   = 14;
const uint8_t zero[] = {
  0x00, 0x00, 0xf0, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x1f, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x00, 0x00
};


const uint8_t um_Width    = 12;
const uint8_t um_Height   = 14;
const uint8_t um[] = {
  0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x3f, 0x20, 0x20, 0x00, 0x00, 0x00
};


const uint8_t dois_Width    = 12;
const uint8_t dois_Height   = 14;
const uint8_t dois[] = {
  0x00, 0x00, 0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0x88, 0x50, 0x20, 0x00,
  0x00, 0x00, 0x00, 0x30, 0x28, 0x24, 0x22, 0x21, 0x20, 0x20, 0x00, 0x00
};

const uint8_t tres_Width    = 12;
const uint8_t tres_Height   = 14;
const uint8_t tres[] = {
  0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x08, 0x08, 0x08, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x21, 0x21, 0x21, 0x1e, 0x00, 0x00
};

const uint8_t quatro_Width    = 12;
const uint8_t quatro_Height   = 14;
const uint8_t quatro[] = {
  0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x3f, 0x02, 0x00, 0x00, 0x00
};

const uint8_t cinco_Width    = 12;
const uint8_t cinco_Height   = 14;
const uint8_t cinco[] = {
  0x00, 0x00, 0x00, 0xfc, 0x84, 0x84, 0x84, 0x84, 0x84, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x00, 0x00
};

const uint8_t seis_Width    = 12;
const uint8_t seis_Height   = 14;
const uint8_t seis[] = {
  0x00, 0x00, 0x00, 0xf0, 0x08, 0x08, 0x08, 0x08, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0x21, 0x21, 0x21, 0x21, 0x1e, 0x00, 0x00, 0x00
};

const uint8_t sete_Width    = 12;
const uint8_t sete_Height   = 14;
const uint8_t sete[] = {
  0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x88, 0x48, 0x38, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t oito_Width    = 12;
const uint8_t oito_Height   = 14;
const uint8_t oito[] = {
  0x00, 0x00, 0xf0, 0x08, 0x08, 0x08, 0x08, 0x08, 0xf0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1e, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1e, 0x00, 0x00, 0x00
};

const uint8_t nove_Width    = 12;
const uint8_t nove_Height   = 14;
const uint8_t nove[] = {
  0x00, 0x00, 0x00, 0xf0, 0x08, 0x08, 0x08, 0x08, 0x08, 0xf0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x10, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1f, 0x00, 0x00
};

const uint8_t ponto_Width    = 2;
const uint8_t ponto_Height   = 7;
const uint8_t ponto[] = {
  0x60, 0x60
};

const uint8_t dois_pontos_Width    = 2;
const uint8_t dois_pontos_Height   = 7;
const uint8_t dois_pontos[] = {
  0x63, 0x63
};


const uint8_t menos_Width    = 4;
const uint8_t menos_Height   = 3;
const uint8_t menos_Bitmap[] = {
  0x02, 0x02, 0x02, 0x02
};

const uint8_t *numeros_t[10]={zero,um,dois,tres,quatro,cinco,seis,sete,oito,nove};

#endif
