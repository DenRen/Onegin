#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

const int MINLINE = 20;

//#pragma pack(1)
struct String_option {
    char *pString;      // Указатель на начало строки
    unsigned length;    // Длина строки с учётом \n
};
//#pragma pack(8)

char *Read_File_To_Buffer(const char *name, int *state_func, bool UNIX = false);

FILE *open_file(const char *name, unsigned long *file_size, bool UNIX);

int Converter_for_Verse(char *buf, struct String_option **pointer_to_text);

int comparison_str_rev(String_option *structString1, String_option *structString2);

int comparison_str(String_option *structString1, String_option *structString2);

int comp1(const void *String1, const void *String2) {
    return comparison_str((String_option *) String1, (String_option *) String2);
}

int comp2(const void *String1, const void *String2) {
    return comparison_str_rev((String_option *) String1, (String_option *) String2);
}

int main() {
    // Ввожу имена файлов ввода и вывода спеллчекер
    const char name[] = "Test_File.txt", name_out[] = "OUTPUT.txt";
    FILE *f = fopen(name_out, "w");

    // Копирую файл name в беффер и проверяю ошибки
    int state_func_ReadFile = 0;
    char *buf = Read_File_To_Buffer(name, &state_func_ReadFile);
    if (buf == nullptr) {
        printf("\n" "main: ERROR in Read_File_To_Buffer: %d\n", state_func_ReadFile);
        return 0;
    }

    // Делаем пригодным для сортировки текст в buf и возвращем
    // sentences как массив указателей на начала предложений в buf

    struct String_option *sentences = nullptr;
    int caunt_sentences = Converter_for_Verse(buf, &sentences);

    if (caunt_sentences == -1 || sentences[0].pString == nullptr) {
        printf("\n" "main: ERROR in Converter_for_Verse\n");
        free(buf);
        return 0;
    }


    //qsort(sentences, caunt_sentences, sizeof(String_option), comp1);
    printf("%s", sentences[0].pString);

    printf("%s %d" ,sentences->pString, sentences[0].length);
    for (int i = 0; i < caunt_sentences; i++) {
        fwrite(sentences[i].pString, sizeof(char), sentences[i].length, stdout);
    }

    fclose(f);
    return 0;
}

bool is_numb_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool is_addi_space(char c) {
    return c == '\t' || c == '\v' || c == '\r';
}

bool is_numb(char c) {
    return c >= '0' && c <= '9';
}

char lower(char c) {
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
}

FILE *open_file(const char *name, unsigned long *file_size, bool UNIX = false) {
    // Размер файла выдаётся в байтах
    // --------------------------------------------------------------------
    // В системе UNIX можно сделать проверку на существование и на
    // доступность чтения данного файла перед тем, как его открыть, и при
    // помощи fseek и ftell узнать размер файла (Переменной UNIX передать true).
    // Но для кроссплатформенности нужно использовать fstat (UNIX == false)
    // То есть: !(UNIX) => UNIX == false
    // --------------------------------------------------------------------

    FILE *file = nullptr;
    if (UNIX) { // Узнаём размер будучи в ОС на UNIX
        if (!access(name, 3) || !access(name, 4)) {  // Проверим доступность файла
            if ((file = fopen(name, "r")) != nullptr) {
                fseek(file, 0, SEEK_END);
                *file_size = ftell(file);
                fseek(file, 0, SEEK_SET);
            } else {
                printf("\n""ERROR open_file UNIX: %s nullptr \n", name);
            }
        } else {
            printf("\n""ERROR open_file UNIX: "
                   "the file %s is ", name);
            if (access(name, 0))
                printf("not present\n");
            else
                printf("access is denied\n");
        }
    } else {    // Узнаём размер будучи в ОС на Windows
        if ((file = fopen(name, "r")) != nullptr) {
            struct stat file_description{};
            if (!fstat(fileno(file), &file_description)) {
                *file_size = file_description.st_size;
            } else {
                printf("\n""ERROR open_file: %s nullptr \n", name);
            }
        } else {
            printf("\n""ERROR open_file: "
                   "the file %s is either not present or access is denied\n", name);
        }
    }
    return file;
}

char *Read_File_To_Buffer(const char *name, int *state_func, bool UNIX) {
    // Сам очистит буффер при ошибке
    // В конце ставит \n \0 для удобства конвертирования
    // state_func == 0 ошибки отсутствуют
    // state_func == 1 файл пустой
    // state_func == 2 ошибка чтения или записи в файл

    unsigned long file_size = 0;
    bool error_read = false;
    FILE *file = open_file(name, &file_size, UNIX);

    if (file_size == 0) {
        *state_func = 1;
        return nullptr;
    }

    char *buf = (char *) calloc(file_size + 1, sizeof(char));
    if (fread(buf, sizeof(char), file_size, file) != file_size) {
        if (feof(file)) {
            printf("Read_File_To_Buffer: Error fread file %s\n"
                   "feof(%s) == 1\n", name, name);
            error_read = true;
        } else if (ferror((file))) {
            printf("Read_File_To_Buffer: Error fread file %s\n"
                   "ferror(%s) == 1\n", name, name);
            error_read = true;
        }
    }

    fclose(file);

    if (error_read) {
        *state_func = 2;
        return nullptr;
    }
    buf[file_size - 1] = '\n';
    buf[file_size] = '\0';
    *state_func = 0;
    return buf;
}

int Converter_for_Verse(char *buf, struct String_option **pointer_to_text) {
    char *pRead = buf, *pWrite = buf, *save_write = buf, temp = 0;
    size_t size_string = 0, size_pointerString = 0;
    bool back_n = true;
    for (; *pRead != '\0'; pRead++) {
        temp = *pRead;
        if (!is_addi_space(temp)) {
            if (temp == '\n') {
                if (!back_n) {
                    back_n = true;
                    *pWrite++ = temp;
                }
            } else if (temp == ' ') {
                if (!back_n)
                    *pWrite++ = temp;
            } else {
                if (back_n) {
                    if (size_string < MINLINE) {
                        pWrite = save_write;
                    } else {
                        save_write = pWrite;
                        size_pointerString++;
                    }
                    size_string = 0;
                    back_n = false;
                }
                size_string++;
                *pWrite = temp;
                pWrite++;
            }
        }
    }
    *pWrite = '\0';
    size_pointerString++;
    buf = (char *) realloc(buf, (pWrite - buf + 1) * sizeof(char));
    if (buf == nullptr) {
        printf("\n" "ERROR Converter_for_Verse realloc: buf == nullptr\n");
        return -1;
    }

    // Создаём массив указателей на структуры String_option

    *pointer_to_text = (struct String_option *) calloc(size_pointerString, sizeof(struct String_option));
    unsigned number_pRead = 0, length_sentence = 1;
    for (pRead = buf; *pRead != '\0'; pRead++, length_sentence++) {
        if (*pRead == '\n') {
            char *stemp =  pRead - length_sentence + 1;
            pointer_to_text[number_pRead]->pString = pRead - length_sentence + 1;
            pointer_to_text[number_pRead]->length = length_sentence+1;
            number_pRead++;
            length_sentence = 0;
        }
    }
    printf("LOL %s", pointer_to_text[number_pRead]->pString);

    if (size_pointerString == 0) {
        free(buf);
        free(pointer_to_text);
        printf("\n" "ERROR Converter_for_Verse: size_pointerString == 0\n");
        return -1;
    }
    return size_pointerString;
}
/*
// Ввожу имена файлов ввода и вывода спеллчекер
const char name[] = "INPUT.txt", name_out[] = "OUTPUT.txt";

// Копирую файл name в беффер и проверяю ошибки
int state_func_ReadFile = 0;
char *buf = Read_File_To_Buffer(name, &state_func_ReadFile);
if (buf == nullptr) {
    printf("\n" "main: ERROR in Read_File_To_Buffer: %d\n", state_func_ReadFile);
    return 0;
}

// Делаем пригодным для сортировки текст в buf и возвращем
// text как массив указателей на начала предложений в buf
char *text = nullptr;
int state_converter = Converter_for_Verse(buf, &text);
if (state_converter == -1 && text == nullptr) {
    printf("\n" "main: ERROR in Converter_for_Verse\n");
    free(buf);
    return 0;
}
*/

//! This function comparison of string to the left
//! @param [in] structPS_1
//! @param [in] structPS_2
//!
//! @return Left and right row difference

int comparison_str(String_option *structString1, String_option *structString2) {
    // Считается, что "aaa" < "aaaa"
    char *ptrStr1 = structString1->pString, *ptrStr2 = structString2->pString;
    while (true) {
        while (!is_numb_letter(*ptrStr1) && *ptrStr1 != '\n')
            ptrStr1++;
        while (!is_numb_letter(*ptrStr2) && *ptrStr2 != '\n')
            ptrStr2++;

        if (*ptrStr1 == '\n') {
            return -1;
        } else if (*ptrStr2 == '\n') {
            return 1;
        } else if (lower(*ptrStr1) - lower(*ptrStr2))
            return (lower(*ptrStr1) - lower(*ptrStr2));

        ptrStr1++, ptrStr2++;
    }
}

//! This function comparison of string to the right
//! @param [in] structPS_1
//! @param [in] structPS_2
//!
//! @return Right and left row difference

int comparison_str_rev(String_option *structString1, String_option *structString2) {
    // Считается, что "aaa" < "aaaa"
    char *ptrStr1 = structString1->pString + structString1->length - 2;
    char *ptrStr2 = structString2->pString + structString2->length - 2;

    while (true) {
        while (!is_numb_letter(*ptrStr1) && (*ptrStr1 != '\0' && *ptrStr1 != '\n'))
            ptrStr1--;
        while (!is_numb_letter(*ptrStr2) && (*ptrStr2 != '\0' && *ptrStr2 != '\n'))
            ptrStr2--;

        if (*ptrStr1 == '\0' || *ptrStr1 == '\n')
            return -1;
        else if (*ptrStr2 == '\0' || *ptrStr2 == '\n')
            return 1;
        else if (lower(*ptrStr1) - lower(*ptrStr2))
            return (lower(*ptrStr1) - lower(*ptrStr2));

        ptrStr1--, ptrStr2--;
    }
}

// Сортируем и fwrite - ом записываем в name_out*/

/*int Converter_for_Verse(char **buf, String_option **pointer_to_text) {
    // При ошибке конвертирования автоматически освобождается
    // память выделанная на pointer_text и возвращается 0
    //
    // После долгих размышлений, пришёл к выводу, что не буду заменять
    // \n на \0 во всём тексте. Просто когда мне понадобится вывести,
    // к примеру, строку я вызову fwrite с указанием необходимой длины,
    // которую я получу из struct String

    // Не учитываются {...}, (...), number ..., double \n, и is_punct_mark
    // Не учитываются строки меньше MINLINE
    // Не учитываются строки, которые начинаются с цифры
    // Сначала конвертируем buf
    bool back_n = true, brace = false, bracket = false;
    int size_pointerString = 0;
    char *pRead = *buf, *pWrite = *buf, temp;

    // Для строк строго меньше MINLINE
    char *save_write = *buf;
    unsigned size_string = 0;

    for (; *pRead != '\0'; pRead++) {
        temp = *pRead;
        // Проверки на цифры
        if (is_numb(temp)) {
            while (*pRead++ != '\n')
                if (*pRead == '\0')
                    break;
            size_string = 0;
            pWrite = save_write;
            pRead--;
            continue;
        }

        // Проверки на {} и ()
        if (brace) {
            if (temp != '}') {
                pWrite = save_write;
                continue;
            } else {
                brace = false;
                continue;

            }
        } else if (bracket) {
            if (temp != ')') {
                pWrite = save_write;
                continue;
            } else {
                bracket = false;
                continue;

            }
        }

        if (temp == '{') {
            brace = true;
            continue;
        } else if (temp == '(') {
            bracket = true;
            continue;
        }

        if (!is_addi_space(temp)) {
            if (temp == '\n') {
                if (!back_n) {
                    back_n = true;
                    *pWrite++ = temp;
                }
            } else if (temp == ' ') {
                if (!back_n)
                    *pWrite++ = temp;
            } else {
                if (back_n) {
                    if (size_string < MINLINE) {
                        pWrite = save_write;
                    } else {
                        save_write = pWrite;
                        size_pointerString++;
                    }
                    size_string = 0;
                    back_n = false;
                }
                size_string++;
                *pWrite++ = temp;
            }
        }
    }

    *pWrite = '\0';
    size_t i = ((pWrite - *buf));
    *buf = (char *) realloc(*buf, i * sizeof(char));
    if (*buf == nullptr) {
        printf("\n" "ERROR Converter_for_Verse realloc: buf == nullptr\n");
        return 0;
    }

    // Создаём массив указателей на структуры String_option
    *pointer_to_text = (String_option *) calloc(size_pointerString, sizeof(String_option *));
    unsigned number_pRead = 0, length_sentence = 1;
    for (pRead = *buf; *pRead != '\0'; pRead++, length_sentence++) {
        if (*pRead == '\n') {
            (*pointer_to_text)[number_pRead].pString = pRead - length_sentence + 1;
            (*pointer_to_text)[number_pRead].length = length_sentence;
            number_pRead++;
            length_sentence = 0;
        }
    }
    return size_pointerString;
}*/
