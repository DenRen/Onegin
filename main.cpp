#include <iostream>
#include <cassert>

#define MAXSIZE 200
#define MINLINE 20


struct struct_pS {
    char *p;
    unsigned l;
};

unsigned Symbol_in_file(char name[], unsigned *x);

unsigned getline(char *s, FILE *input);

bool is_numb_letter(char c);

void str_cpy(char *s1, char *s2);

int comparison_str(struct_pS structPS_1, struct_pS structPS_2);

int comparison_str_rev(struct_pS structPS_1, struct_pS structPS_2);

char lower(char c);

int comp1(const void *a, const void *b) {
    return comparison_str(*(struct_pS *) a, *(struct_pS *) b);
}

int comp2(const void *a, const void *b) {
    return comparison_str_rev(*(struct_pS *) a, *(struct_pS *) b);
}

int main() {
    // Воожу имена файлов ввода и вывода
    char name[] = "INPUT.txt", name_out[] = "OUTPUT.txt";

    //Считаю кол-во символов в файле (тип getline()) и кол-во строк
    unsigned number_s = 0;
    unsigned size_array = Symbol_in_file(name, &number_s);
    if (size_array == 0) {
        printf("%s", "\nmain ERROR: size_array = 0\n");
        return 0;
    }

    // Заполняю массив S и запоминаю в pS указатели на начало предложений
    char *S = (char *) calloc(size_array, sizeof(char **)), s[MAXSIZE];
    struct_pS pS[number_s];

    unsigned i, j, length = 0; // Длина текущего предложения
    FILE *F = fopen(name, "r");
    if (F == nullptr) {
        printf("Error open file %s\n", name);
        fclose(F);
        return 0;
    }
    for (i = j = 0; (length = getline(s, F)) != 0; i += length, j++) {
        str_cpy(&S[i], s);
        pS[j].p = &S[i++];
        pS[j].l = length;
    }
    fclose(F);

    FILE *O = fopen(name_out, "w");
    if (O == nullptr) {
        printf("\n%s%s\n", "main ERROR: no open file ", name_out);
        fclose(O);
        free(S);
        return 0;
    }

    // Сортирую слева и записываю в O
    qsort(pS, number_s, sizeof(struct_pS), comp1);
    for (i = 0; i < number_s; i++)
        fprintf(O, "%s\n", pS[i].p);
    fprintf(O, "\n%s\n", "--------------------------------------------------");

    // Сортирую справа и записываю в O
    qsort(pS, number_s, sizeof(struct_pS), comp2);
    for (i = 0; i < number_s; i++)
        fprintf(O, "%s\n", pS[i].p);
    fprintf(O, "\n%s\n", "--------------------------------------------------");

    for (j = i = 0; i < number_s;) {
        if (S[j] != '\0')
            fprintf(O, "%c", S[j++]);
        else {
            i++, j++;
            fprintf(O, "%c", '\n');
        }
    }
    char temp = '{';

    printf("Hello. Want to get a super verse? y/n");
    scanf("%c", &temp);
    if (temp == 'y') {
        printf("\nHow many lines?\n");
        unsigned line = 0;
        if (scanf("%d", &line) == 1) {
            if (line > number_s)
                line = number_s;
            srand(time(nullptr));

            int random = rand() % (number_s);

            unsigned left = random - line / 2, right = random + line / 2;
            if (left < 0)
                left = 0;
            if (right > number_s)
                right = number_s;

            for (i = left; i < right + 1; i++)
                printf("%s\n", pS[i].p);

        } else {
            printf("I don’t understand you. Bye see you later\n");
        }
    } else {
        printf("Okey :(\n");
    }

    free(S);
    return 0;
}

char lower(char c) {
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
}

bool is_punct_mark(unsigned c) {
    //Проверка на знаки препинания и не букв-симв сиволы
    return c == ' ' || c == '\"' || c == '.' || c == '!' ||
           c == '\?' || c == '-' || c == '\'';
}

bool is_numb_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool is_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool is_strock_liter(unsigned c) {
    return c == '\t' || c == '\n' || c == '\v';
}

unsigned Symbol_in_file(char name[], unsigned *x) {
    // Возвращает длину предложения с учётом '\0'
    // Специально для формата getline()
    assert(x != nullptr);

    FILE *input = fopen(name, "r");
    if (input == nullptr) {
        printf("Symbol_in_file: Error open file %s\n", name);
        fclose(input);
        return 0;
    }
    unsigned c = ' ', i = MINLINE - 1, n = 0, j = 0;

    // Пропускаем безинформативные строки
    while (i != 0) {
        i = 0;
        while (i < MINLINE && c != EOF) {
            i = 0;
            while ((c = getc(input)) != EOF && (is_strock_liter(c) || c == ' '));

            do {
                if (is_strock_liter(c)) {
                    break;
                } else {
                    i++;
                }
            } while ((c = getc(input)) != EOF && c != '\n');
        }
        n += i;
        if (i != 0)
            j++;
    }
    *x = j - 1;
    fclose(input);
    return n;
}

unsigned getline(char *s, FILE *input = stdin) {
    // Без проверки потока (контракт)
    // Возвращает длину предложения без учёта '\0'
    // И записывает считанную строку в s
    // Чтобы не запоминать строки, которые начинаются с цифры
    // нужно для проверок установить какой-нибудь первый символ
    s[0] = '}';
    unsigned c = ' ', i = 0;
    // Пропускаем безинформативные строки
    while (i < MINLINE && c != EOF) {
        i = 0;
        while ((c = getc(input)) != EOF && (is_strock_liter(c) || c == ' '));
        // Читаем и записываем строку полностью
        /* Можно было написать и оптимизированнее, но
         * хотел как можно подробнее
         */
        do {
            if (is_strock_liter(c)) {
                break;
            } else {
                s[i++] = (char) c;
            }
        } while ((c = getc(input)) != EOF && c != '\n');
        s[i] = '\0';
    }
    if (i < MINLINE)
        return 0;
    return i;
}

void str_cpy(char *s1, char *s2) {
    while ((*(s1)++ = *(s2)++) != '\0');
}

//! This function comparison of string to the left
//! @param [in] structPS_1
//! @param [in] structPS_2
//!
//! @return Left and right row difference

int comparison_str(struct_pS structPS_1, struct_pS structPS_2) {
    // Считается, что "aaa" < "aaaa"
    char *s1 = structPS_1.p, *s2 = structPS_2.p;
    while (true) {
        while (!is_numb_letter(*s1) && *s1 != '\0')
            s1++;
        while (!is_numb_letter(*s2) && *s2 != '\0')
            s2++;

        if (*s1 == '\0' && *s2 != '\0')
            return -1;
        else if (*s1 != '\0' && *s2 == '\0')
            return 1;
        else if (*s1 == '\0' && *s2 == '\0')
            return 0;

        if (lower(*s1) != lower(*s2))
            return (lower(*s1) - lower(*s2));
        s1++, s2++;
    }
}

//! This function comparison of string to the right
//! @param [in] structPS_1
//! @param [in] structPS_2
//!
//! @return Right and left row difference

int comparison_str_rev(struct_pS structPS_1, struct_pS structPS_2) {
    // Считается, что "aaa" < "aaaa"
    char *s1 = structPS_1.p + structPS_1.l - 1;
    char *s2 = structPS_2.p + structPS_2.l - 1;

    while (true) {
        while (!is_numb_letter(*s1) && *s1 != '\0')
            s1--;
        while (!is_numb_letter(*s2) && *s2 != '\0')
            s2--;

        if (*s1 == '\0' && *s2 != '\0')
            return -1;
        else if (*s1 != '\0' && *s2 == '\0')
            return 1;
        else if (*s1 == '\0' && *s2 == '\0')
            return 0;

        if (lower(*s1) != lower(*s2))
            return (lower(*s1) - lower(*s2));
        s1--, s2--;
    }
}