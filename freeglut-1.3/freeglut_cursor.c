/*
 * freeglut_cursor.c
 *
 * The mouse cursor related stuff.
 *
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * Creation date: Thu Dec 16 1999
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define  G_LOG_DOMAIN  "freeglut-cursor"

#include "../include/GL/freeglut.h"
#include "../include/GL/freeglut_internal.h"

#if TARGET_HOST_UNIX_X11
    #include <X11/cursorfont.h>
#endif

/*
 * TODO BEFORE THE STABLE RELEASE:
 *
 *  fgDisplayCursor()   -- this waits for better times
 *  glutSetCursor()     -- both X and Win32 mappings are incomplete
 *  glutWarpPointer()   -- check the Win32 version
 *
 * It would be good to use custom mouse cursor shapes, and introduce
 * an option to display them using glBitmap() and/or texture mapping,
 * apart from the windowing system version.
 */

/* -- INTERNAL FUNCTIONS --------------------------------------------------- */

/*
 * Display the mouse cursor using OpenGL calls
 */
void fgDisplayCursor( void )
{
    /*
     * Do nothing for the moment
     */
}


/* -- INTERFACE FUNCTIONS -------------------------------------------------- */

/*
 * Set the cursor image to be used for the current window
 */
void FGAPIENTRY glutSetCursor( int cursorID )
{
    /*
     * Make sure freeglut is ready and there is a current window set
     */
    freeglut_assert_ready; freeglut_assert_window;

#if TARGET_HOST_UNIX_X11
	{
		Cursor cursor;

	    /*
		 * For now we'll limit ourselves to the X cursor fonts...
		 */
#		define MAP_CURSOR(a,b) case a: cursor = XCreateFontCursor( fgDisplay.Display, b ); break;

		switch( cursorID )
		{
			MAP_CURSOR( GLUT_CURSOR_RIGHT_ARROW, XC_left_ptr        );
			MAP_CURSOR( GLUT_CURSOR_LEFT_ARROW,  XC_right_ptr       );
			MAP_CURSOR( GLUT_CURSOR_INFO,        XC_question_arrow  );
			MAP_CURSOR( GLUT_CURSOR_DESTROY,     XC_target          );
			MAP_CURSOR( GLUT_CURSOR_HELP,        XC_question_arrow  );
			MAP_CURSOR( GLUT_CURSOR_CYCLE,       XC_circle          );
			MAP_CURSOR( GLUT_CURSOR_SPRAY,       XC_spraycan        );
			MAP_CURSOR( GLUT_CURSOR_WAIT,        XC_watch           );
			MAP_CURSOR( GLUT_CURSOR_TEXT,        XC_draft_large     );
			MAP_CURSOR( GLUT_CURSOR_CROSSHAIR,   XC_crosshair       );
			MAP_CURSOR( GLUT_CURSOR_NONE,        XC_trek            );

			default:
			MAP_CURSOR( GLUT_CURSOR_UP_DOWN,     XC_arrow           );
		}

	    /*
	     * Define a window's cursor now
	     */
	    XDefineCursor( fgDisplay.Display, fgStructure.Window->Window.Handle, cursor );
	}

#elif TARGET_HOST_WIN32
	/*
	 * This is a temporary solution only...
	 */
#	define MAP_CURSOR(a,b) case a: SetCursor( LoadCursor( NULL, b ) ); break;

	switch( cursorID )
	{
		MAP_CURSOR( GLUT_CURSOR_RIGHT_ARROW, IDC_ARROW     );
		MAP_CURSOR( GLUT_CURSOR_LEFT_ARROW,  IDC_ARROW     );
		MAP_CURSOR( GLUT_CURSOR_INFO,        IDC_HELP      );
		MAP_CURSOR( GLUT_CURSOR_DESTROY,     IDC_CROSS     );
		MAP_CURSOR( GLUT_CURSOR_HELP,        IDC_HELP	   );
		MAP_CURSOR( GLUT_CURSOR_CYCLE,       IDC_SIZEALL   );
		MAP_CURSOR( GLUT_CURSOR_SPRAY,       IDC_CROSS     );
		MAP_CURSOR( GLUT_CURSOR_WAIT,		 IDC_WAIT      );
		MAP_CURSOR( GLUT_CURSOR_TEXT,        IDC_UPARROW   );
		MAP_CURSOR( GLUT_CURSOR_CROSSHAIR,   IDC_CROSS     );
		MAP_CURSOR( GLUT_CURSOR_NONE,        IDC_NO		   );

		default:
		MAP_CURSOR( GLUT_CURSOR_UP_DOWN,	 IDC_UPARROW   );
	}

#endif

    /*
     * Remember the currently selected cursor
     */
    fgStructure.Window->State.Cursor = cursorID;
}

/*
 * Moves the mouse pointer to given window coordinates
 */
void FGAPIENTRY glutWarpPointer( int x, int y )
{
    freeglut_assert_ready; freeglut_assert_window;

#if TARGET_HOST_UNIX_X11
    /*
     * Move the mouse pointer to given window coordinates
     */
    XWarpPointer(
        fgDisplay.Display,
        None,
        fgStructure.Window->Window.Handle,
        0, 0, 0, 0,
        x, y
    );

    XFlush( fgDisplay.Display );

#elif TARGET_HOST_WIN32
    {
        POINT coords = { x, y };

        /*
         * First of all, we need to find the new screen-relative coordinates of the mouse cursor
         */
        ClientToScreen( fgStructure.Window->Window.Handle, &coords );

        /*
         * Now set the new mouse cursor position...
         */
        SetCursorPos( coords.x, coords.y );
    }

#endif
}

/*** END OF FILE ***/