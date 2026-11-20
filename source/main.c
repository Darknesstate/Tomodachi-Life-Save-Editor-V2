#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();
  C2D_TextBuf buf = C2D_TextBufNew(4096);
  C2D_Text txt;
  C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
  while (aptMainLoop()) {
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START) break;
    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(top, C2D_Color32(10,10,18,255));
    C2D_SceneBegin(top);
    C2D_TextBufClear(buf);
    C2D_TextParse(&txt, buf, "Tomodachi Life Save Editor V2");
    C2D_TextOptimize(&txt);
    C2D_DrawText(&txt, C2D_AlignCenter, 200, 80, 0, 0.6, 0.6, 0xFFFFFFFF);
    C2D_TargetClear(bot, C2D_Color32(10,10,18,255));
    C2D_SceneBegin(bot);
    C2D_TextBufClear(buf);
    C2D_TextParse(&txt, buf, "By Darkness Metal - Teste OK");
    C2D_TextOptimize(&txt);
    C2D_DrawText(&txt, C2D_AlignCenter, 160, 100, 0, 0.5, 0.5, 0xFF00FFAA);
    C3D_FrameEnd(0);
  }
  C2D_TextBufDelete(buf);
  C2D_Fini();
  C3D_Fini();
  gfxExit();
  return 0;
}
