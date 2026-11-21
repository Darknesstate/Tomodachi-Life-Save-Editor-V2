#include <3ds.h>
#include <stdio.h>
#include <string.h>

typedef struct { const char *nome; int x; int y; int w; int h; } Balao;

static Balao baloes[8] = {
  {"Roupas", 20, 20, 130, 40},
  {"Comidas e Bebidas", 170, 20, 130, 40},
  {"Remedios", 20, 70, 130, 40},
  {"Interiores", 170, 70, 130, 40},
  {"Chapeus", 20, 120, 130, 40},
  {"Tesouros", 170, 120, 130, 40},
  {"Relacionamentos", 20, 170, 130, 40},
  {"Presentes", 170, 170, 130, 40},
};

static int selecionado = 0;
static int carrinho = 0;
static int dinheiro = 9999999;
static int telaLista = -1;
static int idioma = 0;

static void desenhaTopo(void) {
  consoleClear();
  printf("\x1b[2;2HTomodachi Life Save Editor V2");
  printf("\x1b[4;2HBy Darkness Metal");
  printf("\x1b[6;2HDinheiro: %d (MAX AUTO)", dinheiro);
  printf("\x1b[7;2HCarrinho: %d itens", carrinho);
  printf("\x1b[9;2HTela de cima so mostra.");
  printf("\x1b[10;2HUse toque ou botoes na tela de baixo.");
  if (telaLista >= 0) {
    printf("\x1b[12;2H> %s - lista", baloes[telaLista].nome);
    printf("\x1b[13;2HEx: Item 1 | Item 2 | Item 3");
    printf("\x1b[14;2HA = colocar no carrinho | B = voltar");
  }
}

static void desenhaBaixo(void) {
  consoleClear();
  printf("\x1b[1;2HTOQUE ou D-PAD + A");
  for (int i = 0; i < 8; i++) {
    int linha = 3 + (i / 2) * 3;
    int col = (i % 2 == 0) ? 2 : 20;
    if (i == selecionado) printf("\x1b[%d;%dH> %s", linha, col, baloes[i].nome);
    else printf("\x1b[%d;%dH  %s", linha, col, baloes[i].nome);
  }
  printf("\x1b[16;2HSave | Idioma | Carrinho");
  printf("\x1b[17;2HSTART = Salvar tudo");
}

int main(int argc, char **argv) {
  gfxInitDefault();
  PrintConsole *top = consoleInit(GFX_TOP, NULL);
  PrintConsole *bottom = consoleInit(GFX_BOTTOM, NULL);
  (void)top; (void)bottom;
  dinheiro = 9999999;
  while (aptMainLoop()) {
    hidScanInput();
    u32 kDown = hidKeysDown();
    touchPosition touch;
    hidTouchRead(&touch);
    if (kDown & KEY_START) break;
    if (telaLista >= 0) {
      if (kDown & KEY_B) telaLista = -1;
      else if (kDown & KEY_A) carrinho++;
      else if ((kDown & KEY_TOUCH) && touch.py < 40) telaLista = -1;
      else if ((kDown & KEY_TOUCH) && touch.py >= 40 && touch.py < 80) carrinho++;
    } else {
      if (kDown & KEY_TOUCH) {
        for (int i = 0; i < 8; i++) {
          if (touch.px >= baloes[i].x && touch.px < baloes[i].x + baloes[i].w && touch.py >= baloes[i].y && touch.py < baloes[i].y + baloes[i].h) {
            selecionado = i;
            telaLista = i;
            break;
          }
        }
        if (touch.py >= 200) {
          if (touch.px < 100) { dinheiro = 9999999; }
          else if (touch.px < 200) { idioma = (idioma + 1) % 2; }
          else { carrinho = 0; }
        }
      }
      if (kDown & KEY_UP) selecionado = (selecionado + 6) % 8;
      if (kDown & KEY_DOWN) selecionado = (selecionado + 2) % 8;
      if (kDown & KEY_LEFT) selecionado = (selecionado + 7) % 8;
      if (kDown & KEY_RIGHT) selecionado = (selecionado + 1) % 8;
      if (kDown & KEY_A) telaLista = selecionado;
      if (kDown & KEY_X) dinheiro = 9999999;
    }
    consoleSelect(top);
    desenhaTopo();
    consoleSelect(bottom);
    desenhaBaixo();
    gfxFlushBuffers();
    gfxSwapBuffers();
    gspWaitForVBlank();
  }
  gfxExit();
  return 0;
}
