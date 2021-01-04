#include <pspkernel.h>
#include <oslib/oslib.h>
#include "danzeff.h"

PSP_MODULE_INFO("Hello World", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12*1024);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init OSLib:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int initOSLib(){
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
    oslInitAudio();
    oslSetQuitOnLoadFailure(1);
    oslSetKeyAutorepeatInit(40);
    oslSetKeyAutorepeatInterval(10);
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
    int skip = 0;
    char textInserted[200] = "";
    int textIndex = 0;

    initOSLib();
    oslIntraFontInit(INTRAFONT_CACHE_MED);

    //Loads image:
    OSL_IMAGE *bkg = oslLoadImageFilePNG("bkg.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);

    //Load font:
    OSL_FONT *pgfFont = oslLoadFontFile("flash0:/font/ltn0.pgf");
    oslIntraFontSetStyle(pgfFont, 1.0, RGBA(255,255,255,255), RGBA(0,0,0,0), INTRAFONT_ALIGN_CENTER);
    oslSetFont(pgfFont);

    while(!osl_quit){
        if (!skip){
            oslStartDrawing();

            oslDrawImageXY(bkg, 0, 0);

            if (danzeff_isinitialized()){
                oslDrawString(240, 200, "Text inserted:");
                oslDrawString(240, 230, textInserted);
            }else{
                oslDrawString(240, 150, "Press Circle to use the Danzeff OSK");
                oslDrawString(240, 250, "Press X to quit");
            }
            if (danzeff_isinitialized())
                danzeff_render();
            oslEndDrawing();
        }
        oslEndFrame();
        skip = oslSyncFrame();

        if (danzeff_isinitialized()){
            SceCtrlData pad;
            sceCtrlReadBufferPositive(&pad, 1);
            char pressed = danzeff_readInput(pad);
            switch (pressed)
            {
                case 0:
                case '\n':
                case DANZEFF_LEFT:
                case DANZEFF_RIGHT:
                    break;
                case DANZEFF_START:
                    danzeff_free();
                    break;
                case DANZEFF_SELECT:
                    memset(textInserted, 0, sizeof(textInserted));
                    danzeff_free();
                    break;
                case 8:
                    if (textIndex)
                        textInserted[--textIndex] = 0;
                    break;
                default:
                    textInserted[textIndex++] = pressed;
                    break;
            }
        }else{
            oslReadKeys();
            if (osl_keys->released.cross)
                oslQuit();
            else if (osl_keys->released.circle){
                textIndex = 0;
                memset(textInserted, 0, sizeof(textInserted));
                danzeff_load();
                danzeff_moveTo(165, 20);
            }
        }
    }

    //Quit OSL:
    oslEndGfx();

    sceKernelExitGame();
    return 0;

}
