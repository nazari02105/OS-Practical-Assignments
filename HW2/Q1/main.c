// main sources
// https://linuxhint.com/posix-open-function-c-programming/
// https://man7.org/linux/man-pages/man2/syscalls.2.html

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

// status to pass to waitpid function
int main_status = 0;

int main(){
    // let's make a child process
    pid_t pid = fork();
    // we know that if value of fork is less than 0, it means there is an error
    if (pid < 0){
        // for writing we can use "write" function and pass 1 then our message and at last
        // size of our message
        write(1, "there is an error in fork function", sizeof("there is an error in fork function"));
        // then we use _exit and we should pass it a number. but the number should not be 0
        _exit(1);
    }
    // we know that if pid is 0 it means we are in child process
    // and if pid is not 0, it means we are in parent process
    // we should start with child process
    int is_child;
    if (pid == 0) is_child = 1;
    else is_child = 0;
    if (is_child){
        // then we should specify number of bytes but I take the max for example 10000 is good
        char main_string[10000];
        // this variable is for reading character by character from file
        ssize_t character_by_character;
        // for opening file I used this link
        // https://linuxhint.com/posix-open-function-c-programming/
        int file_to_read = open("./file_to_read.txt", O_RDONLY, 0666);
        // if value of file_to_read is -1 it means we have an error
        if (file_to_read == -1){
            write(1, "file opening failed", sizeof("file opening failed"));
            _exit(1);
        }
        int file_to_write = open("./file_to_write.txt", O_WRONLY | O_CREAT, 0666);
        // now we should use a while to read all characters
        // when read function returns a negative number of zero
        // it means file has ended
        character_by_character = read(file_to_read, main_string, 9999);
        int end = character_by_character - 1;
        while (character_by_character > 0) {
            // and reverse string should be the same size
            char reverse_string[10000];
            // first we need to reverse the main sting
            for (int begin = 0; begin < 10000 ;begin++) {
                reverse_string[begin] = main_string[end];
                end--;
            }
            if (write(file_to_write, "\n", 1) == -1 || write(file_to_write, reverse_string, character_by_character) == -1) {
                write(1, "error in child", sizeof("error in child"));
                _exit(1);
            }
            character_by_character = read(file_to_read, main_string, 9999);
            end = character_by_character - 1;
        }
        close(file_to_read);
        close(file_to_write);
    }
    else{
        // we should wait until child process finishes and then we should start parent process
        waitpid(pid, &main_status, 0);
        // then we should specify number of bytes but I take the max for example 10000 is good
        char main_string[10000];
        // this variable is for reading character by character from file
        ssize_t character_by_character;
        // for opening file I used this link
        // https://linuxhint.com/posix-open-function-c-programming/
        int file_to_read = open("./file_to_read.txt", O_RDONLY, 0666);
        // if value of file_to_read is -1 it means we have an error
        if (file_to_read == -1){
            write(1, "file opening failed", sizeof("file opening failed"));
            _exit(1);
        }
        int file_to_write = open("./file_to_write.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
        // now we should use a while to read all characters
        // when read function returns a negative number of zero
        // it means file has ended
        character_by_character = read(file_to_read, main_string, 10000);
        while (character_by_character > 0) {
            if (write(file_to_write, "\n", 1) == -1 || write(file_to_write, main_string, character_by_character) == -1) {
                write(1, "error in parent", sizeof("error in parent"));
                _exit(1);        
            }
            character_by_character = read(file_to_read, main_string, 10000);
        }
        close(file_to_read);
        close(file_to_write);
    }
    return 0;
}