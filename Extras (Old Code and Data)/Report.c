/*
 * << Haru Free PDF Library 2.0.0 >> -- encoding_list.c
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"

jmp_buf env;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler  (HPDF_STATUS   error_no,
                HPDF_STATUS   detail_no,
                void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}

HPDF_Doc  pdf;

static const int PAGE_WIDTH = 420;
static const int PAGE_HEIGHT = 400;
static const int CELL_WIDTH = 50;
static const int CELL_HEIGHT = 19;
static const int CELL_HEADER = 10;

void
draw_graph (HPDF_Page   page);


void
draw_fonts (HPDF_Page   page);


void
draw_graph (HPDF_Page   page)
{
HPDF_Font font1;
font1 = HPDF_GetFont (pdf, "Helvetica", NULL);
    char buf[50];
    int i;
    //HPDF_Font font;
    /* Draw 16 X 15 cells */
    //font = HPDF_GetFont (pdf, "Helvetica", NULL);
    /* Draw vertical lines. */
    HPDF_Page_SetLineWidth (page, 0.5);

    HPDF_Page_MoveTo (page, 40, PAGE_HEIGHT-37);
    HPDF_Page_LineTo(page, 40, 40);
    HPDF_Page_Stroke(page);
    
    HPDF_Page_MoveTo (page, 190, PAGE_HEIGHT-37);
    HPDF_Page_LineTo(page, 190, 40);
    HPDF_Page_Stroke(page);
    
    HPDF_Page_MoveTo (page, 290, PAGE_HEIGHT-37);
    HPDF_Page_LineTo(page, 290, 40);
    HPDF_Page_Stroke(page);
    
    HPDF_Page_MoveTo (page, 390, PAGE_HEIGHT-37);
    HPDF_Page_LineTo(page, 390, 40);
    HPDF_Page_Stroke(page);
    
    HPDF_Page_MoveTo (page, 240, PAGE_HEIGHT-37-CELL_HEIGHT*2);
    HPDF_Page_LineTo(page, 240, 40);
    HPDF_Page_Stroke(page);
    
    HPDF_Page_MoveTo (page, 340, PAGE_HEIGHT-37-CELL_HEIGHT*2);
    HPDF_Page_LineTo(page, 340, 40);
    HPDF_Page_Stroke(page);
    
    int y;
     /* Draw horizontal lines. */
    for (i = 0; i <= 4; i++) {
       y = i * 3*CELL_HEIGHT + 40;

        HPDF_Page_MoveTo (page, 40, y);
        HPDF_Page_LineTo (page, PAGE_WIDTH - 30, y);
        HPDF_Page_Stroke (page);
    }
        HPDF_Page_MoveTo (page, 40, y+CELL_HEIGHT*2);
        HPDF_Page_LineTo (page, PAGE_WIDTH - 30, y+CELL_HEIGHT*2);
        HPDF_Page_Stroke (page);
        
        HPDF_Page_MoveTo (page, 40, 363);
        HPDF_Page_LineTo (page, PAGE_WIDTH - 30, 363);
        HPDF_Page_Stroke (page);
        
        HPDF_Page_MoveTo (page, 190, 325);
        HPDF_Page_LineTo (page, PAGE_WIDTH - 30, 325);
        HPDF_Page_Stroke (page);        
        
        /*Between the Table Lines*/
        
      for (i = 0; i <= 3; i++) {
       int y = i * 3*CELL_HEIGHT + 69;
        HPDF_Page_MoveTo (page, 190, y);
        HPDF_Page_LineTo (page, 240, y);
        HPDF_Page_Stroke (page);
    }
    
    for (i = 0; i <= 3; i++) {
       int y = i * 3*CELL_HEIGHT + 69;
        HPDF_Page_MoveTo (page, 290, y);
        HPDF_Page_LineTo (page, 340, y);
        HPDF_Page_Stroke (page);
    }
    
     for (i = 0; i <= 10; i++) {
       int y = i*CELL_HEIGHT + 59;
        HPDF_Page_MoveTo (page, 240, y);
        HPDF_Page_LineTo (page, 290, y);
        HPDF_Page_Stroke (page);
    }
    
    for (i = 0; i <= 10; i++) {
       int y = i*CELL_HEIGHT + 59;
        HPDF_Page_MoveTo (page, 340, y);
        HPDF_Page_LineTo (page, 390, y);
        HPDF_Page_Stroke (page);
    }
    
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page, 222, 335);
        HPDF_Page_ShowText (page, "OD");
        HPDF_Page_EndText (page);
        
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page, 322, 335);
        HPDF_Page_ShowText (page,"OS");
        HPDF_Page_EndText (page);
        
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page, 82, 280);
        HPDF_Page_ShowText (page,"Baseline");
        HPDF_Page_EndText (page);
        
        for (i=0; i<4;i++)
        {
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page, 65, 60+57*i);
        HPDF_Page_ShowText (page,"Stimulus - ");
        HPDF_Page_EndText (page);
        
        HPDF_Page_BeginText (page);
        snprintf(buf, 5, "%X", 4-i);
        HPDF_Page_MoveTextPos (page, 160, 60+57*i);
        HPDF_Page_ShowText (page,buf);
        HPDF_Page_EndText (page);
        }
        HPDF_Page_SetFontAndSize (page, font1, 13);
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page,197, 310);
        HPDF_Page_ShowText (page,"Radius");
        HPDF_Page_EndText (page);
        
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page,247, 310);
        HPDF_Page_ShowText (page,"Time");
        HPDF_Page_EndText (page);
        
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page,297, 310);
        HPDF_Page_ShowText (page,"Radius");
        HPDF_Page_EndText (page);
        
        HPDF_Page_BeginText (page);
        HPDF_Page_MoveTextPos (page,347, 310);
        HPDF_Page_ShowText (page,"Time");
        HPDF_Page_EndText (page);
               
}


int main (int argc, char **argv)
{
    pdf = HPDF_NewEx (error_handler, NULL, NULL, 0, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }
    char fname[256];
    HPDF_Font font;
   // const char *font_name;
    //int i = 0;
    //HPDF_Outline root;

    strcpy (fname, argv[0]);
    strcat (fname, ".pdf");
    
    /* Set page mode to use outlines. */
 //   HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_OUTLINE);

    /* get default font */
    font = HPDF_GetFont (pdf, "Helvetica", NULL);

    /* load font object */
   // #ifdef __WIN32__
   // font_name = HPDF_LoadType1FontFromFile (pdf, "type1\\a010013l.afm",
   //         "type1\\a010013l.pfb");
  //  #else
  //  font_name = HPDF_LoadType1FontFromFile (pdf, "type1/a010013l.afm",
     //       "type1/a010013l.pfb");
  //  #endif

    /* create outline root. */
  //  root = HPDF_CreateOutline (pdf, NULL, "Encoding list", NULL);
  //  HPDF_Outline_SetOpened (root, HPDF_TRUE);

  //  while (encodings[i]) {
        HPDF_Page page = HPDF_AddPage (pdf);
     //   HPDF_Outline outline;
     //   HPDF_Destination dst;
       //HPDF_Font font2;

        HPDF_Page_SetWidth (page, PAGE_WIDTH);
        HPDF_Page_SetHeight (page, PAGE_HEIGHT);

       // outline = HPDF_CreateOutline (pdf, root, encodings[i], NULL);
       // dst = HPDF_Page_CreateDestination (page);
       // HPDF_Destination_SetXYZ(dst, 0, HPDF_Page_GetHeight(page), 1);
        /* HPDF_Destination_SetFitB(dst); */
      //  HPDF_Outline_SetDestination(outline, dst);
        HPDF_Page_SetFontAndSize (page, font, 20);
        draw_graph (page);
           // if (strcmp (encodings[i], "Symbol-Set") == 0)
     //       font2 = HPDF_GetFont (pdf, "Symbol", NULL);
      //  else if (strcmp (encodings[i], "ZapfDingbats-Set") == 0)
     //       font2 = HPDF_GetFont (pdf, "ZapfDingbats", NULL);
    //    else
    //        font2 = HPDF_GetFont (pdf, font_name, encodings[i]);

        //HPDF_Page_SetFontAndSize (page, font2, 14);
        
       // i++;
   // }

    /* save the document to a file */
    HPDF_SaveToFile (pdf, fname);

    /* clean up */
    HPDF_Free (pdf);

    return 0;
}

