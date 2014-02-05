#define _BSD_SOURCE
#include <unistd.h> /* sleep() */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>

char *tzmtl = "America/Montreal";

static Display *dpy;

char *
smprintf(char *fmt, ...)
{
	va_list fmtargs;
	char *ret;
	int len;

	va_start(fmtargs, fmt);
	len = vsnprintf(NULL, 0, fmt, fmtargs);
	va_end(fmtargs);

	ret = malloc(++len);
	if (ret == NULL) {
		perror("malloc");
		exit(1);
	}

	va_start(fmtargs, fmt);
	vsnprintf(ret, len, fmt, fmtargs);
	va_end(fmtargs);

	return ret;
}

void
settz(char *tzname)
{
	setenv("TZ", tzname, 1);
}

char *
mktimes(char *fmt, char *tzname)
{
	char buf[129];
	time_t tim;
	struct tm *timtm;

	memset(buf, 0, sizeof(buf));
	settz(tzname);
	tim = time(NULL);
	timtm = localtime(&tim);
	if (timtm == NULL) {
		perror("localtime");
		exit(1);
	}

	if (!strftime(buf, sizeof(buf)-1, fmt, timtm)) {
		fprintf(stderr, "strftime == 0\n");
		exit(1);
	}

	return smprintf("%s", buf);
}

void
setstatus(char *str)
{
	XStoreName(dpy, DefaultRootWindow(dpy), str);
	XSync(dpy, False);
}

char 
*battery(void)
{
    int nmarks,full,now;
    char *bar; 

    FILE *battinfo = fopen("/sys/class/power_supply/BAT0/energy_full","r");
    fscanf(battinfo,"%d",&full);
    fclose(battinfo);
    battinfo = fopen("/sys/class/power_supply/BAT0/energy_now","r");
    fscanf(battinfo,"%d",&now);
    fclose(battinfo);

    bar = (char*)malloc(sizeof(char) * 25);
    nmarks = 10*now/full;

    if((double)now/full <= 0.05) {
        strcpy(bar,"*** BATTERIE < 5% ***");
    } else {
        strcpy(bar,"["); 
        for(int i = 1; i < 11; i++)
        {
            if(i <= nmarks)
                strcat(bar,"#");
            else
                strcat(bar," ");
        }
        strcat(bar,"]");
    }
   return bar;
}

int
main(void)
{
	char *status;
        char *batt;
	char *tmmtl;

	if (!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "dwmstatus: cannot open display.\n");
		return 1;
	}

	for (;;sleep(1)) {
                batt = battery();
		tmmtl = mktimes("%d-%m-%Y %H:%M", tzmtl);

		status = smprintf("%s | %s",
				batt,tmmtl);
		setstatus(status);
                free(batt);
		free(tmmtl);
		free(status);
	}

	XCloseDisplay(dpy);

	return 0;
}
