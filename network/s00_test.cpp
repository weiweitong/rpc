// #include "s01_open.h"

#include "s00_header.h"


int main() {
    // Work *aw = new Work("Hello World");
    // aw->printHello();


    // delete aw;

    int fd = open("example.txt", O_WRONLY | O_CREAT | O_APPEND);
    if (fd == -1) {
        return -1;
    }
    

    // char buf[BUFSIZ];
    // ssize_t nr;

    // while ((nr = read(fd, buf, BUFSIZ)) != 0) {
        // if (nr == -1) {
        //     if (errno == EINTR) 
        //         continue;
        //     perror ("read");
        //     break;
        // }
    //     printf("%s", buf);
    // }

    // off_t ret;
    // ret = lseek(fd, 100, SEEK_END);
    // if (ret == (off_t) - 1)
    //     perror("lseek");

    const char *buf1 = "Mary, I love you, darling.\n";
    const char *buf2 = "My ship is solid!\n";
    ssize_t nr;

    nr = pwrite(fd, buf1, strlen(buf1), (off_t)0);
    // cout << "not write" << endl;
    nr = pwrite(fd, buf2, strlen(buf2), (off_t)20);
    if (nr == -1) {
        // if (errno == EINTR) 
        //     continue;
        perror ("read");
        // break;
    }

    ftruncate(fd, (off_t)10);

    // cout << "closed" << endl;
    if (close(fd) == -1)
        perror ("close");


    return 0;

}


