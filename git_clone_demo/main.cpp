/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: vorachet
 *
 * Created on March 7, 2018, 3:57 PM
 */

#include <iostream>

#include <git2.h>

using namespace std;

typedef struct progress_data {
    git_transfer_progress fetch_progress;
    size_t completed_steps;
    size_t total_steps;
    const char *path;
} progress_data;

static void print_progress(const progress_data *pd) {
    int network_percent = pd->fetch_progress.total_objects > 0 ?
            (100 * pd->fetch_progress.received_objects) / pd->fetch_progress.total_objects :
            0;
    int index_percent = pd->fetch_progress.total_objects > 0 ?
            (100 * pd->fetch_progress.indexed_objects) / pd->fetch_progress.total_objects :
            0;

    int checkout_percent = pd->total_steps > 0
            ? (100 * pd->completed_steps) / pd->total_steps
            : 0;
    int kbytes = pd->fetch_progress.received_bytes / 1024;

    if (pd->fetch_progress.total_objects &&
            pd->fetch_progress.received_objects == pd->fetch_progress.total_objects) {
        printf("Resolving deltas %u/%u\r",
                pd->fetch_progress.indexed_deltas,
                pd->fetch_progress.total_deltas);
    } else {
        printf("net %3d%% (%4d kb, %5d/%5d)  /  idx %3d%% (%5d/%5d)  /  chk %3d%% (%4d /%4d) %s\n",
                network_percent, kbytes,
                pd->fetch_progress.received_objects, pd->fetch_progress.total_objects,
                index_percent, pd->fetch_progress.indexed_objects, pd->fetch_progress.total_objects,
                checkout_percent,
                pd->completed_steps, pd->total_steps,
                pd->path);
    }
}

static int sideband_progress(const char *str, int len, void *payload) {
    (void) payload; // unused

    printf("remote: %.*s", len, str);
    fflush(stdout);
    return 0;
}

static int fetch_progress(const git_transfer_progress *stats, void *payload) {
    progress_data *pd = (progress_data*) payload;
    pd->fetch_progress = *stats;
    print_progress(pd);
    return 0;
}

static void checkout_progress(const char *path, size_t cur, size_t tot, void *payload) {
    progress_data *pd = (progress_data*) payload;
    pd->completed_steps = cur;
    pd->total_steps = tot;
    pd->path = path;
    print_progress(pd);
}

int do_clone(git_repository *repo, int argc, char **argv) {
    progress_data pd = {
        {0}};
    git_repository *cloned_repo = NULL;
    git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
    git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;
    const char *url = argv[1];
    const char *path = argv[2];
    int error;

    (void) repo; // unused

    // Validate args
    if (argc < 3) {
        printf("USAGE: %s <url> <path>\n", argv[0]);
        return -1;
    }

    // Set up options
    checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
    checkout_opts.progress_cb = checkout_progress;
    checkout_opts.progress_payload = &pd;
    clone_opts.checkout_opts = checkout_opts;
    clone_opts.fetch_opts.callbacks.sideband_progress = sideband_progress;
    clone_opts.fetch_opts.callbacks.transfer_progress = &fetch_progress;
    // clone_opts.fetch_opts.callbacks.credentials = cred_acquire_cb;
    clone_opts.fetch_opts.callbacks.payload = &pd;

    // Do the clone
    error = git_clone(&cloned_repo, url, path, &clone_opts);
    printf("\n");
    if (error != 0) {
        const git_error *err = giterr_last();
        if (err) printf("ERROR %d: %s\n", err->klass, err->message);
        else printf("ERROR %d: no detailed info\n", error);
    } else if (cloned_repo) git_repository_free(cloned_repo);
    return error;
}


int create_repsitory(git_repository **out, const char *path, int bare, void *payload)
{
    int error;

    /*
     * We create the repository ourselves, libgit2 gives us the parameters it would
     * have used to create the repository. In this case we ignore the path passed
	 * to git_clone() and put it under /tmp/
     */
    if ((error = git_repository_init(out, "/tmp/...", bare)) < 0)
        return error;

    /* Further customisation of the repository goes here */

    return 0;
}

int create_remote(git_remote **out, git_repository *repo, const char *name, const char *url, void *payload)
{
    int error;

    /*
     * Like above, we create the repository based on what libgit2 would have used
     * (which is what was passed to git_clone. We could use a different refspec
     * or name.
     */
    if ((error = git_remote_create(out, repo, name, url)) < 0)
	    return error;

    /* Further customisation of the remote goes here */

    return 0;
}

git_repository *repo;
git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
clone_opts.repository_cb = create_repository;
clone_opts.remote_cb     = create_remote;

error = git_clone(&repo, url, path, &clone_opts);

int main(int argc, char** argv) {
    git_repository *repo = NULL;
    
    int error = do_clone(repo, argc, argv);

    cout << "error = " << error << endl;


    auto lerror = giterr_last();
    char *message = lerror->message;
    cout << "error message = " << message << endl;
    return 0;
}

