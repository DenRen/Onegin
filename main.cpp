#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

const int MAXSIZE = 200;
const int MINLINE = 20;

char *Read_File_To_Buffer(const char *name, int *state_func, bool UNIX = false);

FILE *open_file(const char *name, unsigned long *file_size, bool UNIX);

int Converter_for_Verse(char *buf, char **pointer_to_text);

int main() {
    // Ввожу имена файлов ввода и вывода спеллчекер
    const char name[] = "INPUT.txt", name_out[] = "OUTPUT.txt";

    // Копирую файл name в беффер и проверяю ошибки
    int state_func_ReadFile = 0;
    char *buf = Read_File_To_Buffer(name, &state_func_ReadFile);
    if (buf == nullptr) {
        printf("\n" "main: ERROR in Read_File_To_Buffer: %d\n", state_func_ReadFile);
        return 0;
    }
    printf("%s", buf);
    
    return 0;
    const char temp_name[] = "Test_File.txt";
    int state = 0;
    char *buf1 = Read_File_To_Buffer(temp_name, &state, true);

    return 0;
}

char *Read_File_To_Buffer(const char *name, int *state_func, bool UNIX) {
    // Сам очистит буффер при ошибке
    // state_func == 0 ошибки отсутствуют
    // state_func == 1 файл пустой
    // state_func == 2 ошибка чтения или записи в файл

    unsigned long file_size = 0;
    bool error_read = 0;
    FILE *file = open_file(name, &file_size, UNIX);

    if (file_size == 0) {
        *state_func = 1;
        return nullptr;
    }

    char *buf = (char *) calloc(file_size, sizeof(char));
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

    *state_func = 0;
    return buf;
}

int Converter_for_Verse(char *buf, char **pointer_to_text) {
    // При ошибке конвертирования автоматически освобождается
    // память выделанная на pointer_text
    return 0;
}

char lower(char c) {
    if (c >= 'A' && c <= 'Z')
        return c + 'a' - 'A';
    return c;
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

// Сортируем и fwrite - ом записываем в name_out*/


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