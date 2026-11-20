#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <stdio.h>
#include <string.h>

// CORRIGIDO: nao limpa o buf aqui, limpa 1 vez por frame la embaixo
void draw_text(C2D_TextBuf buf, C2D_Text *txt, const char *str, float x, float y, float scale, u32 color) {
  C2D_TextParse(txt, buf, str);
  C2D_TextOptimize(txt);
  C2D_DrawText(txt, C2D_AlignLeft, x, y, 0, scale, scale, color);
}

int main(int argc, char* argv[]) {
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  C2D_TextBuf buf = C2D_TextBufNew(8192);
  C2D_Text txt;
  C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

  const char* categorias[8] = {"Roupas","Comidas e Bebidas","Remedios","Interiores","Chapeus","Tesouros","Relacionamentos","Presentes"};
  const char* idiomas[8] = {"Portugues (BR)","English","Espanol","Japones","Frances","Alemao","Italiano","Coreano"};
  const char* saves[3] = {"Save 1 - Checkpoint","Save 2 - Checkpoint","Save 3 - Checkpoint"};

  int tela = 0;
  int sel = 0;
  int selItem = 0;
  int selSave = 0;
  int selIdioma = 0;
  int carrinho = 0;
  int dinheiro = 9999999;
  char tmp[128];

  while (aptMainLoop()) {
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START) break;

    if (tela == 0) {
      if (kDown & KEY_DUP) { if (sel >= 2) sel -= 2; }
      if (kDown & KEY_DDOWN) { if (sel <= 5) sel += 2; }
      if (kDown & KEY_DLEFT) { if (sel % 2 == 1) sel -= 1; }
      if (kDown & KEY_DRIGHT) { if (sel % 2 == 0) sel += 1; }
      if (kDown & KEY_A) tela = 1;
      if (kDown & KEY_X) tela = 2;
      if (kDown & KEY_Y) tela = 4;
      if (kDown & KEY_SELECT) tela = 3;
      if (kDown & KEY_TOUCH) {
        touchPosition t; hidTouchRead(&t);
        for (int i = 0; i < 8; i++) {
          int col = i % 2; int row = i / 2;
          int bx = 10 + col * 155; int by = 10 + row * 55;
          if (t.px >= bx && t.px <= bx + 145 && t.py >= by && t.py <= by + 45) { sel = i; tela = 1; }
        }
      }
    } else if (tela == 1) {
      if (kDown & KEY_DUP) { if (selItem > 0) selItem--; }
      if (kDown & KEY_DDOWN) { if (selItem < 4) selItem++; }
      if (kDown & KEY_A) { carrinho++; }
      if (kDown & KEY_B) tela = 0;
      if (kDown & KEY_TOUCH) {
        touchPosition t; hidTouchRead(&t);
        for (int i = 0; i < 5; i++) {
          int iy = 30 + i * 38;
          if (t.px >= 10 && t.px <= 300 && t.py >= iy && t.py <= iy + 32) { selItem = i; carrinho++; }
        }
      }
    } else if (tela == 2) {
      if (kDown & KEY_B) tela = 0;
      if (kDown & KEY_A) { carrinho = 0; }
    } else if (tela == 3) {
      if (kDown & KEY_DUP) { if (selSave > 0) selSave--; }
      if (kDown & KEY_DDOWN) { if (selSave < 2) selSave++; }
      if (kDown & KEY_A) tela = 0;
      if (kDown & KEY_B) tela = 0;
    } else if (tela == 4) {
      if (kDown & KEY_DUP) { if (selIdioma > 0) selIdioma--; }
      if (kDown & KEY_DDOWN) { if (selIdioma < 7) selIdioma++; }
      if (kDown & KEY_A) tela = 0;
      if (kDown & KEY_B) tela = 0;
    }

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TextBufClear(buf);

    C2D_TargetClear(top, C2D_Color32(10,10,18,255));
    C2D_SceneBegin(top);
    draw_text(buf, &txt, "Tomodachi Life Save Editor V2", 20, 20, 0.6, 0xFFFFFFFF);
    draw_text(buf, &txt, "By Darkness Metal", 20, 50, 0.5, 0xFFB388FF);
    snprintf(tmp, sizeof(tmp), "Dinheiro: %d MAX automatico", dinheiro);
    draw_text(buf, &txt, tmp, 20, 80, 0.5, 0xFF00FFAA);
    snprintf(tmp, sizeof(tmp), "Carrinho: %d itens", carrinho);
    draw_text(buf, &txt, tmp, 20, 110, 0.5, 0xFFFFFFFF);
    draw_text(buf, &txt, "Tela touch + botoes + D-Pad", 20, 140, 0.45, 0xFFAAAAAA);
    draw_text(buf, &txt, "START = sair  B = voltar", 20, 170, 0.45, 0xFFAAAAAA);

    C2D_TargetClear(bot, C2D_Color32(10,10,18,255));
    C2D_SceneBegin(bot);

    if (tela == 0) {
      for (int i = 0; i < 8; i++) {
        int col = i % 2; int row = i / 2;
        int bx = 10 + col * 155; int by = 10 + row * 55;
        u32 cor = (i == sel) ? C2D_Color32(180,80,255,255) : C2D_Color32(40,40,60,255);
        C2D_DrawRectSolid(bx, by, 0, 145, 45, cor);
        draw_text(buf, &txt, categorias[i], bx + 8, by + 12, 0.45, 0xFFFFFFFF);
      }
      draw_text(buf, &txt, "A=abrir  X=carrinho  Y=idioma  SELECT=save", 10, 230, 0.35, 0xFFAAAAAA);
    } else if (tela == 1) {
      snprintf(tmp, sizeof(tmp), "Lista: %s", categorias[sel]);
      draw_text(buf, &txt, tmp, 10, 5, 0.5, 0xFFFFFFFF);
      for (int i = 0; i < 5; i++) {
        int iy = 30 + i * 38;
        u32 cor = (i == selItem) ? C2D_Color32(180,80,255,255) : C2D_Color32(40,40,60,255);
        C2D_DrawRectSolid(10, iy, 0, 300, 32, cor);
        snprintf(tmp, sizeof(tmp), "Item %d - %s", i + 1, categorias[sel]);
        draw_text(buf, &txt, tmp, 16, iy + 8, 0.4, 0xFFFFFFFF);
      }
      draw_text(buf, &txt, "A = colocar no carrinho  B = voltar", 10, 225, 0.35, 0xFFAAAAAA);
    } else if (tela == 2) {
      draw_text(buf, &txt, "Carrinho", 10, 10, 0.6, 0xFFFFFFFF);
      snprintf(tmp, sizeof(tmp), "Itens: %d - Aplicar tudo de uma vez", carrinho);
      draw_text(buf, &txt, tmp, 10, 50, 0.45, 0xFFFFFFFF);
      draw_text(buf, &txt, "A = aplicar e limpar  B = voltar", 10, 90, 0.4, 0xFFAAAAAA);
    } else if (tela == 3) {
      draw_text(buf, &txt, "Escolher Save - Checkpoint", 10, 10, 0.5, 0xFFFFFFFF);
      for (int i = 0; i < 3; i++) {
        int iy = 40 + i * 40;
        u32 cor = (i == selSave) ? C2D_Color32(180,80,255,255) : C2D_Color32(40,40,60,255);
        C2D_DrawRectSolid(10, iy, 0, 300, 32, cor);
        draw_text(buf, &txt, saves[i], 16, iy + 8, 0.45, 0xFFFFFFFF);
      }
      draw_text(buf, &txt, "A = escolher  B = voltar", 10, 180, 0.4, 0xFFAAAAAA);
    } else if (tela == 4) {
      draw_text(buf, &txt, "Idioma / Language", 10, 10, 0.5, 0xFFFFFFFF);
      for (int i = 0; i < 8; i++) {
        int iy = 35 + i * 22;
        u32 cor = (i == selIdioma) ? C2D_Color32(180,80,255,255) : C2D_Color32(40,40,60,255);
        C2D_DrawRectSolid(10, iy, 0, 300, 18, cor);
        draw_text(buf, &txt, idiomas[i], 16, iy + 2, 0.38, 0xFFFFFFFF);
      }
      draw_text(buf, &txt, "A = escolher  B = voltar", 10, 220, 0.35, 0xFFAAAAAA);
    }

    C3D_FrameEnd(0);
  }

  C2D_TextBufDelete(buf);
  C2D_Fini();
  C3D_Fini();
  gfxExit();
  return 0;
}
