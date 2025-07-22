#include "../../include/minishell.h"
#include <stdarg.h>

// Helper function to write a string to file descriptor
static void ft_putstr_fd_len(char *s, int fd, int *len)
{
    if (!s)
        return;
    int str_len = ft_strlen(s);
    write(fd, s, str_len);
    *len += str_len;
}

// Helper function to write a character to file descriptor
static void ft_putchar_fd_len(char c, int fd, int *len)
{
    write(fd, &c, 1);
    (*len)++;
}

// Helper function to convert integer to string and write it
static void ft_putnbr_fd_len(int n, int fd, int *len)
{
    char *num_str = ft_itoa(n);
    if (num_str)
    {
        ft_putstr_fd_len(num_str, fd, len);
        free(num_str);
    }
}

// Helper function to handle format specifiers
static void handle_format_specifier(va_list args, char specifier, int fd, int *len)
{
    if (specifier == 's')
    {
        char *str = va_arg(args, char *);
        if (!str)
            ft_putstr_fd_len("(null)", fd, len);
        else
            ft_putstr_fd_len(str, fd, len);
    }
    else if (specifier == 'c')
    {
        char c = (char)va_arg(args, int);
        ft_putchar_fd_len(c, fd, len);
    }
    else if (specifier == 'd' || specifier == 'i')
    {
        int num = va_arg(args, int);
        ft_putnbr_fd_len(num, fd, len);
    }
    else if (specifier == '%')
    {
        ft_putchar_fd_len('%', fd, len);
    }
    else
    {
        // Unknown specifier, just print it literally
        ft_putchar_fd_len('%', fd, len);
        ft_putchar_fd_len(specifier, fd, len);
    }
}

// Main ft_fprintf function
int ft_fprintf(int fd, const char *format, ...)
{
    va_list args;
    int len = 0;
    int i = 0;

    if (!format)
        return -1;

    va_start(args, format);

    while (format[i])
    {
        if (format[i] == '%' && format[i + 1])
        {
            i++; // Skip '%'
            handle_format_specifier(args, format[i], fd, &len);
        }
        else if (format[i] == '%')
        {
            // % at end of string
            ft_putchar_fd_len('%', fd, &len);
        }
        else
        {
            ft_putchar_fd_len(format[i], fd, &len);
        }
        i++;
    }

    va_end(args);
    return len;
}

// Convenience wrapper for stderr
int ft_fprintf_stderr(const char *format, ...)
{
    va_list args;
    int len = 0;
    int i = 0;

    if (!format)
        return -1;

    va_start(args, format);

    while (format[i])
    {
        if (format[i] == '%' && format[i + 1])
        {
            i++; // Skip '%'
            handle_format_specifier(args, format[i], STDERR_FILENO, &len);
        }
        else if (format[i] == '%')
        {
            // % at end of string
            ft_putchar_fd_len('%', STDERR_FILENO, &len);
        }
        else
        {
            ft_putchar_fd_len(format[i], STDERR_FILENO, &len);
        }
        i++;
    }

    va_end(args);
    return len;
}
