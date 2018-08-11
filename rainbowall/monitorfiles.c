/*This is the sample program to notify us for the file creation and file deletion takes place in “/tmp” directory*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include <unistd.h>

#include <string.h>
#include <stdbool.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

// it shouldn't matter (due to the correct way of calling execl), but let's whitelist for security
// plluuuuuss we only should have valid colour files in files/
const char* whitelisted_colors[] = {"red", "blue", "green", "yellow", "purple", "orange"};

bool is_valid_color(const char* col) {
    for (size_t i = 0; i < sizeof(whitelisted_colors)/sizeof(whitelisted_colors[0]); ++i) {
        if (strcmp(col, whitelisted_colors[i]) == 0) return true;
    }
    return false;
}

void setcolor(const char* col) {
    // for security...
    if (!is_valid_color(col)) return; 


    // exec in the child process
    pid_t childpid = fork();
    if (childpid == 0) {
        // change the colour & update last modified image
        execl("/bin/bash", "bash", "./setcolor.sh", col, (char *) 0);
    }
}

int main( )
{
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    /*creating the INOTIFY instance*/
    fd = inotify_init();

    /*checking for error*/
    if ( fd < 0 ) {
        perror( "inotify_init" );
    }

    /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
    wd = inotify_add_watch( fd, "/home/npa/public_html/wall/files/", IN_ACCESS);

    /*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/ 

    while (true) {
        int length, i = 0;

        printf("waiting for file change...\n");
        length = read( fd, buffer, EVENT_BUF_LEN ); 

        /*checking for error*/
        if ( length < 0 ) {
            perror( "read" );
        }  

        // go through the events
        while ( i < length ) {     
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     

            if ( event->len ) {
                printf("setting colour to: %s\n", event->name);
                setcolor(event->name);
            }
            i += EVENT_SIZE + event->len;
        }
    }
    // clean up
    inotify_rm_watch( fd, wd );
    close( fd );

}
