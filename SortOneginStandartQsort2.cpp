#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <fcntl.h>
#include <share.h>
#include <cstring>
#include "locale.h"

struct CLine
    {
    char *felem;
    int length;
    };

void DivideLines (char Onegin[], int *N_lines, CLine Lines[], int read_symb);
void FullAlphOnegin (char Onegin[], int f_alph, int N_lines, CLine Lines[]);
int Comparator (const void *a, const void *b);

int main()
{
    setlocale (LC_ALL, "Russian");

    int f = _sopen ("Onegin.txt", _O_TEXT | _O_SEQUENTIAL | _O_RDWR | _O_CREAT, _SH_DENYNO);
    int f_alph = _sopen ("AlphabetOnegin.txt", _O_TEXT | _O_WRONLY | _O_CREAT, _SH_DENYNO);

    //FILE *f_alph = fopen ("AlphabetOnegin.txt", "wb");

    if (f == -1) printf ("Cannot open file");
    if (f_alph == -1) printf ("Cannot open file");

    //if (!f_alph) printf ("Cannot open file");

    _lseek (f, 0, SEEK_END);
    int size_f = _tell (f);
    _lseek (f, 0, SEEK_SET);

    char *Onegin = new char[size_f + 2] {0};    //need extra one for \n (\0) in last line and must have \0
    if (Onegin == NULL) printf ("hbvgskdfh");

    int read_symb = _read (f, Onegin, size_f);

    if (read_symb <= 0)
        printf ("Problem reading file");

    int N_lines = size_f - read_symb + 1;  //(No \r in last line)

    CLine *Lines = new CLine[N_lines] {};

    //printf ("%d ", Onegin[9238]);
    //printf ("%d", ';');

    DivideLines (Onegin, &N_lines, Lines, read_symb);

    qsort (Lines, N_lines, sizeof (CLine), Comparator);

    //printf ("    %d     ", _write (f_alph, Onegin, read_symb));

    FullAlphOnegin (Onegin, f_alph, N_lines, Lines);

    _close (f);
    _close (f_alph);

    //fclose (f_alph);

    delete []Lines;
    delete []Onegin;

    printf ("  %s  ", strerror (errno));
}

void DivideLines (char Onegin[], int *N_lines, CLine Lines[], int read_symb)
{
    //Lines[0].felem = new char {0};
    Lines[0].felem = &Onegin[0];

    int length = 0;

    for (int i = 0, j = 1; i < read_symb; i++)
        {
        if (i + 1 == read_symb)
            {
            Lines[*N_lines - 1].length = length;
            }

        if (Onegin[i] == '\n' && i + 1 < read_symb)
            {
            //Lines[j] = &Onegin[i + 1];

            //Lines[j].felem = new char {0};

            Lines[j].felem = &Onegin[i + 1];

            j++;

            Onegin[i] = '\0';

            Lines[j - 2].length = length;

            length = 0;
            }

        length++;
        }

    Onegin[read_symb] = '\0';
}

void FullAlphOnegin (char Onegin[], int f_alph, int N_lines, CLine Lines[])
{
    for (int i = 0; i < N_lines; i++)
        {
        _write (f_alph, Lines[i].felem, Lines[i].length);
        _write (f_alph, "\r\n", 2);
        }
}

int Comparator (const void *a, const void *b)
{
    CLine first = *(CLine*)a;
    CLine second = *(CLine*)b;

    return strcmp (first.felem, second.felem);
}
