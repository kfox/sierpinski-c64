#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <tgi.h>
#include <cc65.h>


#define SCREEN_X        (tgi_getxres())
#define SCREEN_Y        (tgi_getyres())

/* Use static local variables for speed */
#pragma static-locals (1);


void sierpinski () {
  register unsigned int row, column;

  for (row = 0; row < SCREEN_Y; row++) {
    for (column = 0; column < SCREEN_X; column++) {
      if (!(row & column)) {
        tgi_setpixel (column, row);
      }
    }
  }
}

int main (void)
{
  clock_t t;
  unsigned long sec;
  unsigned sec10;
  unsigned char err;

  clrscr();

  /* Install the graphics driver */
  tgi_install(tgi_static_stddrv);

  err = tgi_geterror();
  if (err != TGI_ERR_OK)
  {
    cprintf("Error #%d initializing graphics.\r\n%s\r\n",
            err, tgi_geterrormsg(err));
    if (doesclrscrafterexit())
    {
      cgetc();
    }
    exit(EXIT_FAILURE);
  };

  /* Initialize graphics */
  tgi_init();
  tgi_clear();

  t = clock();

  /* draw */
  sierpinski();

  t = clock() - t;

  /* Fetch the character from the keyboard buffer and discard it */
  cgetc();

  /* Shut down gfx mode and return to textmode */
  tgi_done();

  /* Calculate stats */
  sec = (t * 10) / CLK_TCK;
  sec10 = sec % 10;
  sec /= 10;

  /* Output stats */
  cprintf("time: %lu.%us\n\r", sec, sec10);

  if (doesclrscrafterexit())
  {
    /* Wait for a key, then end */
    cputs("Press any key when done...\n\r");
    cgetc();
  }

  /* Done */
  return EXIT_SUCCESS;
}
