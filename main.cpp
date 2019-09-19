#include <iostream>
#include <unistd.h>

const int MAXSIZE = 200;
const int MINLINE = 20;

char *Read_File_To_Buffer(const char *name, int *state_func, bool UNIX);

int Converter_for_Verse(char *buf, char **pointer_to_text);

int main() {
    const char temp_name[] = "Test_File.txt";
    int state = 0;
    char *buf1 = Read_File_To_Buffer(temp_name, &state, true);


    return 0;
}

char *Read_File_To_Buffer(const char *name, int *state_func, bool UNIX) {
    // Сам очистит буффер при ошибке
    // --------------------------------------------------------------------
    // В системе UNIX можно сделать проверку на существование
    // и на доступность чтения данного файла и при помощи fseek
    // и ftell узнать размер файла (Переменной UNIX передать true).
    // Но для кроссплатформенности нужно использовать fstat (UNIX == false)
    // --------------------------------------------------------------------

    FILE *file = nullptr;
    unsigned long file_size = 0;
    if (UNIX) { // Узнаём размер буду в ОС на UNIX
        if (!access(name, 3) || !access(name, 4)) {  // Проверим доступность файла
            if ((file = fopen(name, "r")) != nullptr) {
                fseek(file, 0, SEEK_END);
                file_size = ftell(file);
                fseek(file, 0, SEEK_SET);
                printf("file size: %lu, ", file_size);
            } else {
                printf("\n""ERROR Read_File_To_Buffer: %s nullptr \n", name);
            }
        } else {    // Узнаём размер буду в ОС на Windows
            printf("\n""ERROR Read_File_To_Buffer: "
                   "the file %s is either not present or access is denied\n", name);
        }
    } else {
        
    }
    printf("%c\n", getc(file));
    if (file_size == 0) {
        *state_func = 1;
        return nullptr;
    }

    *state_func = 0;
    return nullptr;
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