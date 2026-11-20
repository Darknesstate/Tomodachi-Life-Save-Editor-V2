// Tomodachi Life Save Editor V2 by Darkness Metal
// Arquivo: source/main.c
// 8 baloes, tons escuros, PT-BR, Dinheiro Maximo AUTO, touch + botoes
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <stdio.h>
#include <string.h>

static void drawCenter(C2D_TextBuf buf, C2D_Text *txt, const char *str, float x, float y, float scale, u32 color) {
    C2D_TextBufClear(buf);
    C2D_TextParse(txt, buf, str);
    C2D_TextOptimize(txt);
    C2D_DrawText(txt, C2D_AlignCenter, x, y, 0, scale, scale, color);
}
static void drawLeft(C2D_TextBuf buf, C2D_Text *txt, const char *str, float x, float y, float scale, u32 color) {
    C2D_TextBufClear(buf);
    C2D_TextParse(txt, buf, str);
    C2D_TextOptimize(txt);
    C2D_DrawText(txt, 0, x, y, 0, scale, scale, color);
}

int main(int argc, char* argv[]) {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    C2D_TextBuf buf = C2D_TextBufNew(4096);
    C2D_Text txt;
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    const char* cats[8] = {"Roupas", "Comidas e Bebidas", "Remedios", "Interiores", "Chapeus", "Tesouros", "Relacionamentos", "Presentes"};
    const char* idiomas[8] = {"Portugues (BR)", "English", "Espanol", "Japones", "Frances", "Alemao", "Italiano", "Coreano"};
    const char* saves[3] = {"Save 1 - Checkpoint", "Save 2 - Checkpoint", "Save 3 - Checkpoint"};
    const char* itens[6] = {"Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Item 6"};

    int tela = 0;
    int sel = 0;
    int selItem = 0;
    int selIdioma = 0;
    int selSave = 0;
    int carrinho = 0;
    int dinheiro = 9999999;
    char tmp[128];

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        touchPosition touch;
        hidTouchRead(&touch);

        if (kDown & KEY_START) break;

        if (tela == 0 && touch.px > 0 && touch.py > 0) {
            int t = (touch.py - 40) / 22;
            if (t >= 0 && t < 8) sel = t;
        }

        if (tela == 0) {
            if (kDown & KEY_DUP) sel = (sel + 7) % 8;
            if (kDown & KEY_DDOWN) sel = (sel + 1) % 8;
            if (kDown & KEY_A) { tela = 1; selItem = 0; }
            if (kDown & KEY_X) tela = 2;
            if (kDown & KEY_Y) tela = 3;
            if (kDown & KEY_L) tela = 4;
        } else if (tela == 1) {
            if (kDown & KEY_DUP) selItem = (selItem + 5) % 6;
            if (kDown & KEY_DDOWN) selItem = (selItem + 1) % 6;
            if (kDown & KEY_A) { carrinho++; dinheiro = 9999999; }
            if (kDown & KEY_B) tela = 0;
            if (kDown & KEY_X) { dinheiro = 9999999; tela = 0; }
        } else {
            if (kDown & KEY_DUP) {
                if (tela == 2) selIdioma = (selIdioma + 7) % 8;
                if (tela == 3) selSave = (selSave + 2) % 3;
            }
            if (kDown & KEY_DDOWN) {
                if (tela == 2) selIdioma = (selIdioma + 1) % 8;
                if (tela == 3) selSave = (selSave + 1) % 3;
            }
            if (kDown & KEY_A) { dinheiro = 9999999; tela = 0; }
            if (kDown & KEY_B) tela = 0;
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top, C2D_Color32(0x0A, 0x0A, 0x12, 0xFF));
        C2D_SceneBegin(top);
        drawCenter(buf, &txt, "Tomodachi Life Save Editor V2", 200, 15, 0.55, 0xFFFFFFFF);
        drawCenter(buf, &txt, "By Darkness Metal", 200, 38, 0.45, 0xFFB388FF);
        snprintf(tmp, sizeof(tmp), "Dinheiro: %d  Carrinho: %d", dinheiro, carrinho);
        drawCenter(buf, &txt, tmp, 200, 60, 0.42, 0xFF00FFAA);

        if (tela == 0) {
            for (int i = 0; i < 8; i++) {
                snprintf(tmp, sizeof(tmp), "%s %s", (i == sel ? ">" : " "), cats[i]);
                drawLeft(buf, &txt, tmp, 60, 85 + i * 20, 0.48, (i == sel ? 0xFFFFFF00 : 0xFFFFFFFF));
            }
        } else if (tela == 1) {
            snprintf(tmp, sizeof(tmp), "Categoria: %s", cats[sel]);
            drawCenter(buf, &txt, tmp, 200, 85, 0.48, 0xFFFFFF00);
            for (int i = 0; i < 6; i++) {
                snprintf(tmp, sizeof(tmp), "%s %s", (i == selItem ? ">" : " "), itens[i]);
                drawLeft(buf, &txt, tmp, 60, 110 + i * 20, 0.48, (i == selItem ? 0xFFFFFF00 : 0xFFFFFFFF));
            }
        } else if (tela == 2) {
            drawCenter(buf, &txt, "Idioma / Language", 200, 85, 0.48, 0xFFFFFF00);
            for (int i = 0; i < 8; i++) {
                snprintf(tmp, sizeof(tmp), "%s %s", (i == selIdioma ? ">" : " "), idiomas[i]);
                drawLeft(buf, &txt, tmp, 60, 110 + i * 20, 0.45, (i == selIdioma ? 0xFFFFFF00 : 0xFFFFFFFF));
            }
        } else if (tela == 3) {
            drawCenter(buf, &txt, "Escolher Save - Checkpoint", 200, 85, 0.48, 0xFFFFFF00);
            for (int i = 0; i < 3; i++) {
                snprintf(tmp, sizeof(tmp), "%s %s", (i == selSave ? ">" : " "), saves[i]);
                drawLeft(buf, &txt, tmp, 60, 110 + i * 20, 0.48, (i == selSave ? 0xFFFFFF00 : 0xFFFFFFFF));
            }
        } else {
            drawCenter(buf, &txt, "Carrinho - Aplicar tudo de uma vez", 200, 85, 0.45, 0xFFFFFF00);
            snprintf(tmp, sizeof(tmp), "Itens no carrinho: %d", carrinho);
            drawCenter(buf, &txt, tmp, 200, 120, 0.48, 0xFFFFFFFF);
            drawCenter(buf, &txt, "Aperte A para aplicar - Dinheiro Maximo AUTO", 200, 145, 0.42, 0xFF00FFAA);
        }

        C2D_TargetClear(bot, C2D_Color32(0x0A, 0x0A, 0x12, 0xFF));
        C2D_SceneBegin(bot);
        if (tela == 0) drawCenter(buf, &txt, "A: Abrir  X: Idioma  Y: Save  L: Carrinho", 160, 90, 0.45, 0xFFFFFFFF);
        else if (tela == 1) drawCenter(buf, &txt, "A: Adicionar  B: Voltar  X: Aplicar tudo", 160, 90, 0.45, 0xFFFFFFFF);
        else drawCenter(buf, &txt, "A: Confirmar  B: Voltar", 160, 90, 0.45, 0xFFFFFFFF);
        drawCenter(buf, &txt, "START: Sair", 160, 115, 0.45, 0xFFB388FF);

        C3D_FrameEnd(0);
    }

    C2D_TextBufDelete(buf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
