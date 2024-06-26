#include "ft.hpp"

namespace ft {
    bool number_is_in(int value, int *arr, int size)
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i] == value)
                return true;
        }
        return false;
    }

    std::streamsize get_file_size(std::ifstream & file)
    {
        file.seekg(0, std::ios::end);
        std::streamsize file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        return file_size;
    }

    void *read_binary(std::ifstream & file, std::streamsize size)
    {
        if (file.bad() || size == 0)
            return NULL;

        char *data = new char[size];
        file.read(data, size);
        return data;
    }

    bool has_only_digits(std::string str)
    {
        return str.find_first_not_of("0123456789") == std::string::npos;
    }

    std::string int_to_str(size_t num)
    {
        std::ostringstream oss;
        oss << num;
        return oss.str();
    }

    size_t str_to_int(std::string str)
    {
        std::stringstream ss(str);
        size_t n;
        ss >> n;
        return n;
    }

    char *get_body_position(char *data)
    {
        while (*data)
        {
            if (std::strncmp("\n\n", data, 2) == 0)
                return  data + 2;
            if (std::strncmp("\r\n\r\n", data, 4) == 0)
                return  data + 4;
            if (std::strncmp("\r\n\n", data, 3) == 0)
                return  data + 3;
            if (std::strncmp("\n\r\n", data, 3) == 0)
                return  data + 3;
            data++;
        }
        return data;
    }

    char *get_body_position(char *response, size_t response_size)
    {
        for (size_t i = 0; i < response_size; i++)
        {
            if (response_size - i >= 2 && std::strncmp("\n\n", &response[i], 2) == 0)
                return  &response[i + 2];
            if (response_size - i >= 4 && std::strncmp("\r\n\r\n", &response[i], 4) == 0)
                return  &response[i + 4];
            if (response_size - i >= 3 && std::strncmp("\r\n\n", &response[i], 3) == 0)
                return  &response[i + 3];
            if (response_size - i >= 3 && std::strncmp("\n\r\n", &response[i], 3) == 0)
                return  &response[i + 3];
        }
        return NULL;
    }

    void debug_file(std::string filename, char *buff, size_t size)
    {
        int fd = open(filename.c_str(), O_TRUNC | O_CREAT | O_RDWR, 00666);
        write(fd, buff, size);
        close(fd);
    }

    void debug_file(std::string filename, const char *buff, size_t size)
    {
        int fd = open(filename.c_str(), O_TRUNC | O_CREAT | O_RDWR, 00666);
        if (fd == -1)
            return ;
        write(fd, buff, size);
        close(fd);
    }

    void print_char_array(const char *arr, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            std::cout << int(arr[i]) << " ";
        }
        std::cout << std::endl;
    }

    void print_char_array(const char *arr)
    {
        for (; *arr; arr++)
        {
            std::cout << int(*arr) << " ";
        }
        std::cout << std::endl;
    }

    ssize_t recv_all(int fd, char **buff)
    {
        char *data = NULL;
        ssize_t total_size = 0;
        const ssize_t tmp_size = 20000;
        char tmp[tmp_size];
        while (1)
        {
            ssize_t bytes = recv(fd, tmp, tmp_size, MSG_DONTWAIT);
            if (bytes <= 0)
                break;
            total_size += bytes;
            char *aux = data;
            data = new char[total_size];
            std::memmove(data, aux, total_size - bytes);
            std::memmove(data + total_size - bytes, tmp, bytes);
            if (aux)
                delete [] aux;
        }
        *buff = data;
        return total_size;
    }

    ssize_t write_all(int fd, char *buff, ssize_t size)
    {
        ssize_t const max_write = 10000;
        ssize_t total = 0;
        while (total < size)
        {
            if (size - total <= max_write)
            {
                ssize_t bytes = write(fd, buff, size - total);
                total += bytes;
                break ;
            }
            ssize_t bytes = write(fd, buff + total, max_write);
            total += bytes;
            if (bytes <= 0)
                break ;
        }
        return total;
    }

    void close_pipes(int *pfds1, int *pfds2)
    {
        close(pfds1[0]);
        close(pfds1[1]);
        close(pfds2[0]);
        close(pfds2[1]);
    }

    bool file_exists(std::string filename) {
        std::ifstream file(filename.c_str());
        return file.good();
    }

    std::string get_full_path(std::string relative)
    {
        if (relative.empty())
            return "";
        if (relative.at(0) != '.')
            return relative;
        
        char *cwd = getcwd(NULL, 0);
        if (cwd == NULL)
            return "";
        std::string path(cwd);
        free(cwd);

        return path + relative.substr(1);
    }

    std::string read_file(std::string filename)
    {
        std::ifstream file(filename.c_str());
        if (!file)
            throw std::runtime_error("failed to open file");

        std::string line;
        std::stringstream buffer;
        while (std::getline(file, line))
            buffer << line << "\n";
        file.close();
        return buffer.str();
    }

    std::string read_file(std::ifstream & file)
    {
        if (!file)
            throw std::runtime_error("failed to open file");

        std::string line;
        std::stringstream buffer;
        while (std::getline(file, line))
            buffer << line << "\n";
        return buffer.str();
    }
}
