#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <assert.h>

// inotify
#include <sys/types.h>
#include <sys/inotify.h>

// so we can watch more than one file at once
#include <pthread.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


// the files which are being watched for access
const char* files[] = {"yes", "no"};
// the list of functions that will be called for each respective fn
typedef void (*access_fn)();
void yes_fn();
void no_fn();
access_fn file_fns[] = {yes_fn, no_fn};

pthread_mutex_t stdout_mutex = PTHREAD_MUTEX_INITIALIZER;

struct thread_data {
    /* absolute path for filename, this is allocated and will be freed in stop_workers */
    char* fname_str;
    size_t thread_num;
    access_fn fn;
    // TODO: additional data can go here
};

pthread_t* threads;
struct thread_data* tdata;

/* Set up data-structures */
void init() {
    // TODO: set stack size for threads to be absolute minimal (ram consumption...!)
    // i think that requires us passing shit to the ctor of pthread_init
}

void* worker_thread(void* arg) {

    struct thread_data* td = (struct thread_data*) arg;

    pthread_mutex_lock(&stdout_mutex);
    printf("watching file: %s\n", td->fname_str);
    pthread_mutex_unlock(&stdout_mutex);

    int fd, wd;
    int length;
    char buffer[EVENT_BUF_LEN];

    /*creating the INOTIFY instance*/
    fd = inotify_init();

    wd = inotify_add_watch( fd, td->fname_str, IN_ACCESS );

    // wait until we have a file read
    length = read( fd, buffer, EVENT_BUF_LEN ); 
        
    int  i = 0;
    while ( i < length ) {     
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];     
        // cargo cult stuff
        i += EVENT_SIZE + event->len;

        pthread_mutex_lock(&stdout_mutex);
        printf("file %s read!\n", td->fname_str);
        pthread_mutex_unlock(&stdout_mutex);

        // and call the associated function call
        td->fn();
    }
    
    inotify_rm_watch( fd, wd );

    // close the file descriptor
    close(fd);

    return NULL;
}

char* get_filename(const char* fname) {
    // return the absolute value of the string we're talkin about
    // PWD + fname
    return realpath(fname, NULL);
}

/* Set up each thread to observe each of the strings */
// return number of workers created
size_t start_workers() {
    size_t num_files = sizeof(files) / sizeof(files[0]);
    // sanity check
    assert(num_files == sizeof(file_fns) / sizeof(file_fns[0]) && 
                "mismatched number of files and functions");

    threads = (pthread_t*) calloc(num_files, sizeof(pthread_t));
    tdata = (struct thread_data*) calloc(num_files, sizeof(struct thread_data));

    // go through and spin the threads up
    for (size_t i = 0; i < num_files; ++i) {
        // initialise the passed argument fields to some house keeping software
        tdata[i].fname_str = get_filename(files[i]);
        tdata[i].thread_num = i;
        tdata[i].fn = file_fns[i];

        int err = pthread_create(threads + i, NULL, worker_thread, &tdata[i]);

        if (err) {
            perror("thread creation failed.");
        }
    }

    return num_files;
}

/* For each thread, join them and do some cleanup */
void stop_workers(size_t num_workers) {
    // join them, and deallloc
    for (size_t i = 0; i < num_workers; ++i) {
        void* ignore;
        int err = pthread_join(threads[i], &ignore);

        if (err) {
            perror("thread joining failed");
        }   
        
        free(tdata[i].fname_str);
    }

    free(tdata);
    free(threads);
}

void yes_fn() {
    puts("user accessed the yes file!");
}

void no_fn() {
    puts("user accessed the no file! >>>:(");
}


int main() {

    printf("starting stuff\n");

    init();

    size_t num_workers  = start_workers();
    stop_workers(num_workers);

    printf("done!\n");

}
