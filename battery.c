#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <libssh/libssh.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: <executable> <ip-add of iOS device> <path to public ssh key>\n");
        return 1;
    }

    while(1) {
        ssh_session session;
        ssh_channel channel;
        ssh_key public_key;

        int rc = 0;
        int port = 22;
        const char* user = "mobile";

        char buffer[1024];
        unsigned int nbytes;
        FILE* file;

        session = ssh_new();
        if (session == NULL) {
            printf("Error starting session\n");
            continue;
        }

        ssh_options_set(session, SSH_OPTIONS_HOST, argv[1]);
        ssh_options_set(session, SSH_OPTIONS_PORT, &port);
        ssh_options_set(session, SSH_OPTIONS_USER, user);

        rc = ssh_connect(session);
        if (rc != SSH_OK) {
            printf("Connection failed\n");
            continue;
        }

        rc = ssh_pki_import_pubkey_file(argv[2], &public_key);
        if (rc != SSH_OK) {
            printf("Importing pubkey failed\n");
            continue;
        }

        rc = ssh_userauth_publickey_auto(session, user, (const char *)public_key);
        if (rc != SSH_AUTH_SUCCESS) {
            printf("Authentication failed\n");
            continue;
        }

        channel = ssh_channel_new(session);
        if (channel == NULL) {
            printf("Creating channel failed\n");
            continue;
        }

        rc = ssh_channel_open_session(channel);
        if (rc != SSH_OK) {
            printf("Opening session failed\n");
            continue;
        }

        rc = ssh_channel_request_exec(channel, "bstat");
        if (rc != SSH_OK) {
            printf("Remote execution failed\n");
            continue;
        }

        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);  

        file = fopen("data.txt", "w+");
        fwrite(buffer, 1, nbytes, file);
        fclose(file);

        // fwrite(buffer, 1, nbytes, stdout);

        ssh_channel_send_eof(channel);
        ssh_channel_close(channel);
        ssh_channel_free(channel);

        ssh_disconnect(session);
        ssh_free(session);

        sleep(10);
    }

    return 0;
}