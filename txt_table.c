/*
 * Description:  解析文本格式表格数据
 * Author     :  maohw (), maohongwei@gmail.com
 * Company    :  gsf
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include "txt_table.h"


static ssize_t _readline(int fd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < maxlen; n++)
    {
again:
        if ((rc = read(fd, &c, 1)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
                break; /* newline is stored, like fgets() */
        }
        else if (rc == 0)
        {
            *ptr = 0;
            return(n - 1); /* EOF, n-1 bytes were read */
        }
        else
        {
            if (errno == EINTR)
                goto again;
            return(-1);
        }
    }
    *ptr = 0; /* null terminate like fgets() */
    return(n);
}

static char *_strsep(char **stringp, const char *delim)
{
    char *p;

    while (1)
    {
        p = strsep(stringp, delim);
        if (p == NULL)
        {
            return NULL;
        }
        else
        {
            if (p[0] == '\0')
                continue;
            else
                break;
        }
    }
    return p;
}


int txt_table(char *name, int(*cb)(int row, int col, char fmt, void *v, void *u), void *u)
{
    int  r = 0, c = 0;
    char hdr[256] = {0};
    int fd = open(name, O_RDONLY);
    
    while(1)
    {
        char line[256] = {0};
        if(_readline(fd, line, sizeof(line)) <= 0)
            break;
            
        if(line[0] == '#' || line[0] == '\r')
            continue;
            
        r++;
            
        int len = strlen(line);
        
        if(line[len-2] == '\r') line[len-2] = '\0';
        if(line[len-1] == '\n') line[len-1] = '\0';

        char *left  = line;
        char *token = NULL;
        int n = 0;
        while((token = _strsep(&left, " ")) != NULL)
        {
            int i;
            char *body = NULL;
            for(i = 0; i < strlen(token); i++)
            {
                if(token[i] != '\t' && token[i] != ' ')
                {
                    body = &token[i];
                    n++;
                    break;
                }
            }
            
            if(r == 1)
            {
                if(body)
                {
                    hdr[n] = body[0];
                    c = n;
                }
            }
            else if(body != NULL)
            {
                switch(hdr[n])
                {
                    case 'i':
                        {
                            int v = atoi(body);
                            cb(r, n, hdr[n], &v, u);
                        }
                        break;
                    case 'f':
                        {
                            double v = atof(body);
                            cb(r, n, hdr[n], &v, u);
                        }
                        break;
                    case 's':
                        {
                            char *v = body;
                            cb(r, n, hdr[n], v, u);
                        }
                        break;
                   case 'h':
                        {
                            int v = strtol(body, NULL, 16);
                            cb(r, n, hdr[n], &v, u);
                        }
                        break;
                }
            }
        }
    }
    
    return 0;
}

// 以下为测试程序
//////////////////////////////////////////////////////////////////

#ifdef __TEST_MAIN__

static int row_cb(int r, int c, char f, void *v, void *u)
{
    static int _r = 0;

    if(_r != r)
    {
        printf("\n%08d---", r);
    }
    _r = r;
    
    switch(f)
    {
        case 'i':
            printf("[%d](%d) ", c, *((int*)v));
            break;
        case 'f':
            printf("[%d](%f) ", c, *((double*)v));
            break;
        case 's':
            printf("[%d](%s) ", c, ((char*)v));
            break;
        case 'h':
            printf("[%d](%X) ", c, *((int*)v));
            break;
    }
 
    return 0;
}


int main(int argc, char *argv[])
{
    txt_table(argv[1], row_cb, NULL);
    printf("\n");
    return 0;
}

#endif //__TEST_MAIN__

//////////////////////////////////////////////////////////////////