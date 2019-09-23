#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <ctime>
#include <cstring>

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

bool is_numb(char c);

int program_Onegin(const char *name, const char *name_out, bool generator);

int main() {
    const char name[] = "INPUT.txt", name_out[] = "OUTPUT.txt";
    const bool BREDO_generate = true;
    printf("\n" "State program Onegin: %d\n", program_Onegin(name, name_out, BREDO_generate));
    return 0;
}

//! This function read a file named name and in file named name_out write
//! converted text in three types: sorted from the beginning, sorted from the ending
//! and source text.
//! If generator activation that started BreDoGenerator, which the outputs rap.
//! \param [in] name source file name
//! \param [in] name_out the file to which the output will be written
//! \param [in] generator whether to turn on a BreDoGenerator
//! \return State program: 0 - all rigth, all the rest - error state
int program_Onegin(const char *name, const char *name_out, const bool generator = false) {

    const char *separator = "\n" "--------------------------------\n";

    printf("Program Onegin started.\n\nHello my friend! \n"
           "To a file named %s three texts will be written:\n"
           "sorted left, sorted right and source text\n"
           "from a file named %s" "\n\n", name_out, name);

    // Копирую файл name в беффер и проверяю ошибки
    int state_func_ReadFile = 0;
    char *buf = Read_File_To_Buffer(name, &state_func_ReadFile);
    if (buf == nullptr) {
        printf("\n" "program_Onegin: ERROR in Read_File_To_Buffer: %d\n", state_func_ReadFile);
        return 1;
    }

    // Делаем пригодным для сортировки текст в buf и возвращем
    // sentences как массив указателей на начала предложений в buf

    struct String_option *sentences = nullptr;
    int caunt_sentences = Converter_for_Verse(buf, &sentences);

    if (caunt_sentences == -1 || sentences[0].pString == nullptr) {
        printf("\n" "program_Onegin: ERROR in Converter_for_Verse\n");
        return 2;
    }

    FILE *f_out = fopen(name_out, "w");
    if (f_out == nullptr) {
        printf("program_Onegin: ERROR in open file %s", name_out);
        fclose(f_out);
        free(buf);
        free(sentences);
        return 3;
    }

    // Сортирую с начала строки и записываю в файл
    qsort(sentences, caunt_sentences, sizeof(String_option), comp1);
    for (int i = 0; i < caunt_sentences; i++)
        fwrite(sentences[i].pString, sizeof(char), sentences[i].length, f_out);

    fwrite(separator, sizeof(char), 34, f_out);

    // Сортирую с конца строки и записываю в фаwhether to includeйл
    qsort(sentences, caunt_sentences, sizeof(String_option), comp2);
    for (int i = 0; i < caunt_sentences; i++)
        fwrite(sentences[i].pString, sizeof(char), sentences[i].length, f_out);

    fwrite(separator, sizeof(char), 34, f_out);

    size_t number_chars_in_buf = 0;
    for (int i = 0; i < caunt_sentences; i++)
        number_chars_in_buf += sentences[i].length;

    fwrite(buf, sizeof(char), number_chars_in_buf, f_out);

    if (generator) {
        char temp_char = '{';

        printf("Want to get a super verse? y/n ");
        scanf("%c", &temp_char);

        if (temp_char == 'y') {
            printf("\nHow many lines?\n");
            unsigned line = 0;
            if (scanf("%d", &line) == 1) {
                if (line > caunt_sentences)
                    line %= caunt_sentences;
                srand(time(nullptr));

                int random = rand() % (caunt_sentences);

                unsigned left_border = random - line / 2, right_border = random + line / 2;
                if (left_border < 0)
                    left_border = 0;
                if (right_border > caunt_sentences)
                    right_border = caunt_sentences;

                const char first_line[] = "MC BreDoGenerator\n";
                fwrite(first_line, strlen(first_line), sizeof(char), stdout);
                for (unsigned i = left_border; i < right_border; i++) {

                    // Не вывожу строки, которые начинаются с цифры
                    if (is_numb(sentences[i].pString[0])) {
                        right_border++;
                        if (caunt_sentences < right_border)
                            right_border = caunt_sentences;
                        continue;
                    }

                    if ((i - left_border) % 10 == 0)
                        fwrite("Yeah! ", 6, sizeof(char), stdout);
                    else if ((i - left_border) % 5 == 0)
                        fwrite("Yo! ", 4, sizeof(char), stdout);
                    fwrite(sentences[i].pString, sentences[i].length, sizeof(char), stdout);

                }

            } else {
                printf("I don’t understand you. Bye see you later\n");
            }
        } else {
            printf("Okey :(\n");
        }
    }

    free(sentences);
    free(buf);
    fclose(f_out);
    return 0;
}

bool is_numb(char c) {
    return (c >= '0' && c <= '9');
}

bool is_numb_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool is_addi_space(char c) {
    return c == '\t' || c == '\v' || c == '\r';
}

char lower(char c) {
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
}

//! This function tries to open the file. She can do this in two ways.
//! On a UNIX system, you can check for existence and accessibility
//! of reading this file before opening it, and when help fseek and
//! ftell find out the file size (pass UNIX to true).
//! But for cross-platform use fstat (UNIX == false)
//! That is:! (UNIX) => UNIX == false
//! \param [in] name source file name
//! \param [in] file_size writes the full file size in bytes
//! \param [in] UNIX find out the size in style UNIX
//! \return pointer to the open file
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
        if ((file = fopen(name, "rb")) != nullptr) {
            struct stat file_specification{};
            if (!fstat(fileno(file), &file_specification)) {
                *file_size = file_specification.st_size;
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

//! Fully copies from a file named "name" to  RAM
//! Itself clears the buffer on error. At the end it puts '\n' '\0' for easy conversion
//! \param [in] name source file name
//! \param [in] state_func 0 no errors, 1 file is empty, 2 error reading or writing to the file
//! \param [in] UNIX find out the size in style UNIX
//!
//! \return pointer to buffer with text
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
        fclose(file);
        return nullptr;
    }

    // В buf будет храниться весь файл name + знак '\0'
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
        free(buf);
        return nullptr;
    }

    buf[file_size - 1] = '\n';
    buf[file_size] = '\0';
    *state_func = 0;
    return buf;
}

/*!
 * This converter removes unnecessary indentation and duplicate '\n'
 * If conversion fails, memory is automatically freed
 * highlighted on pointer_text and returns 0
 * After much deliberation, I came to the conclusion that I will not replace
 * '\n' to '\0' throughout the text. Just when I need to withdraw,
 * for example, I’ll call fwrite with the required length,
 * which I will get from struct String
 * Double '\n' and is_addi_space not counted
 */
//! \param buf pointer to buffer with text
//! \param pointer_to_text writes pointer to struct String_option array
//! \return quantity created struct String_option or -1 if error
int Converter_for_Verse(char *buf, struct String_option **pointer_to_text) {
    // Этот конвертер убирает ненужные отступы и дублирующиеся '\n'
    // При ошибке конвертирования автоматически освобождается память
    // выделанная на pointer_text и возвращается -1
    //
    // После долгих размышлений, пришёл к выводу, что не буду заменять
    // \n на \0 во всём тексте. Просто когда мне понадобится вывести,
    // к примеру, строку я вызову fwrite с указанием необходимой длины,
    // которую я получу из struct String

    // Не учитываются double \n и is_addi_space

    char *pRead = buf, *pWrite = buf, *save_write = buf, temp = 0;
    size_t size_string = 0, size_pointerString = 0;
    bool back_n = true;

    // Обработка и перезапись buf
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
        free(buf);
        return -1;
    }

    // Создаём массив указателей на структуры String_option

    *pointer_to_text = (struct String_option *) calloc(size_pointerString, sizeof(struct String_option));
    unsigned number_pRead = 0, length_sentence = 1;
    for (pRead = buf; *pRead != '\0'; pRead++, length_sentence++) {
        if (*pRead == '\n') {
            char *stemp = pRead - length_sentence + 1;
            (*pointer_to_text)[number_pRead].pString = pRead - length_sentence + 1;
            (*pointer_to_text)[number_pRead].length = length_sentence;
            number_pRead++;
            length_sentence = 0;
        }
    }

    if (size_pointerString == 0) {
        free(buf);
        free(pointer_to_text);
        printf("\n" "ERROR Converter_for_Verse: size_pointerString == 0\n");
        return -1;
    }
    return size_pointerString;
}

//! This function comparison of string to the left
//! @param [in] structPS_1 first offer to compare
//! @param [in] structPS_2 second offer to compare
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
//! @param [in] structPS_1 first offer to compare
//! @param [in] structPS_2 second offer to compare
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

/* Шаблон для следующего конвертера
 * int Converter_for_Verse(char **buf, String_option **pointer_to_text) {
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
