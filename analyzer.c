
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Read one line dynamically
char *read_line(FILE *file)
{
    int ch, i = 0, size = 16;
    char *line = malloc(size);

    if (line == NULL)
        return NULL;

    while ((ch = fgetc(file)) != EOF && ch != '\n')
    {
        if (i == size - 1)
        {
            size = size * 2;
            char *temp = realloc(line, size);

            if (temp == NULL)
            {
                free(line);
                return NULL;
            }

            line = temp;
        }

        line[i++] = ch;
    }

    if (ch == EOF && i == 0)
    {
        free(line);
        return NULL;
    }

    line[i] = '\0';
    return line;
}


// Stage 1: Header inclusion
void include_headers(const char *input_file, const char *temp_file)
{
    FILE *src, *dest, *header;
    char *line, *name, *path;
    char *start, *end, *slash;
    int ch, len;

    src = fopen(input_file, "r");
    dest = fopen(temp_file, "w");

    if (src == NULL || dest == NULL)
    {
        printf("File opening failed\n");

        if (src != NULL) fclose(src);
        if (dest != NULL) fclose(dest);
        return;
    }

    while ((line = read_line(src)) != NULL)
    {
        start = strstr(line, "#include");

        if (start != NULL)
        {
            start = strchr(start, '"');

            if (start != NULL)
            {
                end = strchr(start + 1, '"');

                if (end != NULL)
                {
                    len = end - start - 1;
                    name = malloc(len + 1);

                    if (name == NULL)
                    {
                        free(line);
                        continue;
                    }

                    strncpy(name, start + 1, len);
                    name[len] = '\0';

                    path = malloc(strlen(input_file) + strlen(name) + 1);

                    if (path == NULL)
                    {
                        free(name);
                        free(line);
                        continue;
                    }

                    slash = strrchr(input_file, '/');

                    if (slash != NULL)
                    {
                        len = slash - input_file + 1;

                        strncpy(path, input_file, len);
                        path[len] = '\0';
                        strcat(path, name);
                    }
                    else
                    {
                        strcpy(path, name);
                    }

                    header = fopen(path, "r");

                    free(name);
                    free(path);

                    if (header != NULL)
                    {
                        while ((ch = fgetc(header)) != EOF)
                            fputc(ch, dest);

                        fclose(header);
                        fputc('\n', dest);

                        free(line);
                        continue;
                    }
                }
            }
        }

        fputs(line, dest);
        fputc('\n', dest);
        free(line);
    }

    fclose(src);
    fclose(dest);
}


// Stage 2: Comment removal
void remove_comments(FILE *src, FILE *dest)
{
    int ch, next;

    while ((ch = fgetc(src)) != EOF)
    {
        if (ch == '/')
        {
            next = fgetc(src);

            if (next == '/')
            {
                while ((ch = fgetc(src)) != EOF && ch != '\n');

                if (ch == '\n')
                    fputc('\n', dest);
            }

            else if (next == '*')
            {
                while ((ch = fgetc(src)) != EOF)
                {
                    if (ch == '*')
                    {
                        next = fgetc(src);

                        if (next == '/')
                            break;
                    }
                }
            }

            else
            {
                fputc(ch, dest);

                if (next != EOF)
                    fputc(next, dest);
            }
        }

        else
        {
            fputc(ch, dest);
        }
    }
}


// Stage 3: Conditional compilation
void conditional_compile(const char *input_file, const char *output_file)
{
    FILE *src, *dest;
    char *line, *p;
    char macro[50] = "";
    char name[50];

    int defined = 0;
    int skip = 0;

    src = fopen(input_file, "r");
    dest = fopen(output_file, "w");

    if (src == NULL || dest == NULL)
    {
        printf("File opening failed\n");

        if (src != NULL) fclose(src);
        if (dest != NULL) fclose(dest);

        return;
    }

    while ((line = read_line(src)) != NULL)
    {
        p = line;

        while (*p == ' ' || *p == '\t')
            p++;

        if (strncmp(p, "#define", 7) == 0)
        {
            sscanf(p + 7, "%s", macro);
            defined = 1;

            if (skip == 0)
                fprintf(dest, "%s\n", line);
        }

        else if (strncmp(p, "#ifdef", 6) == 0)
        {
            sscanf(p + 6, "%s", name);

            if (defined && strcmp(name, macro) == 0)
                skip = 0;
            else
                skip = 1;
        }

        else if (strncmp(p, "#ifndef", 7) == 0)
        {
            sscanf(p + 7, "%s", name);

            if (defined && strcmp(name, macro) == 0)
                skip = 1;
            else
                skip = 0;
        }

        else if (strncmp(p, "#else", 5) == 0)
        {
            skip = !skip;
        }

        else if (strncmp(p, "#endif", 6) == 0)
        {
            skip = 0;
        }

        else if (skip == 0)
        {
            fprintf(dest, "%s\n", line);
        }

        free(line);
    }

    fclose(src);
    fclose(dest);
}


// Stage 4: Macro replacement
void replace_macros(const char *input_file, const char *output_file)
{
    FILE *src, *dest;
    char *line, *p, *pos;
    char macro[50] = "";
    char value[50] = "";

    src = fopen(input_file, "r");
    dest = fopen(output_file, "w");

    if (src == NULL || dest == NULL)
    {
        printf("File opening failed\n");

        if (src != NULL) fclose(src);
        if (dest != NULL) fclose(dest);

        return;
    }

    while ((line = read_line(src)) != NULL)
    {
        if (strncmp(line, "#define", 7) == 0)
        {
            p = line + 7;

            while (*p == ' ')
                p++;

            sscanf(p, "%s", macro);

            p += strlen(macro);

            while (*p == ' ')
                p++;

            sscanf(p, "%s", value);

            free(line);
            continue;
        }

        pos = strstr(line, macro);

        if (macro[0] != '\0' && pos != NULL)
        {
            *pos = '\0';

            fputs(line, dest);
            fputs(value, dest);
            fputs(pos + strlen(macro), dest);
        }

        else
        {
            fputs(line, dest);
        }

        fputc('\n', dest);
        free(line);
    }

    fclose(src);
    fclose(dest);
}


// Main processing function
void process_file(const char *input_file, const char *output_file)
{
    FILE *src, *dest;

    // Stage 1: Header inclusion
    include_headers(input_file, "output/temp.c");

    // Stage 2: Comment removal
    src = fopen("output/temp.c", "r");
    dest = fopen("output/cleaned.c", "w");

    if (src == NULL || dest == NULL)
    {
        printf("File opening failed\n");

        if (src != NULL) fclose(src);
        if (dest != NULL) fclose(dest);

        return;
    }

    remove_comments(src, dest);

    fclose(src);
    fclose(dest);

    // Stage 3: Conditional compilation
    conditional_compile("output/cleaned.c",
                        "output/conditional.c");

    // Stage 4: Macro replacement
    replace_macros("output/conditional.c", output_file);

    printf("All four stages completed!\n");
}
