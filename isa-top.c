/**
 * @file isa-top.c
 * @author Karolína Virágová
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "packet_handler.h"
#include "vector.h"
#include "display.h"
#include <pcap.h>

thread_args_t* thread_args;

void signal_handler(int sig) {
    if (sig == SIGINT) {
        // Break pcap
        pcap_breakloop(thread_args->handle);
        thread_args->break_while = true;
    }
}

void print_interfaces() {
    pcap_if_t *alldevs, *d;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error finding devices: %s\n", errbuf);
        return;
    }

    printf("Available network interfaces:\n");
    for (d = alldevs; d != NULL; d = d->next) {
        printf("%s", d->name);
        if (d->description) {
            printf(" - %s\n", d->description);
        } else {
            printf(" - No description available\n");
        }
    }

    pcap_freealldevs(alldevs);
}

int main(int argc, char* argv[])
{
    signal(SIGINT, signal_handler);
    thread_args = (thread_args_t*)malloc(sizeof(thread_args_t));

    if (thread_args == NULL) {
        fprintf(stderr, "Error allocating memory for thread_args_t.\n");
        exit(1);
    } 

    thread_args->args.time = 1; 
    strcpy(thread_args->args.sorting_criteria, "b");
    thread_args->packets = (packet_vector_t*)malloc(sizeof(packet_vector_t));
    if (thread_args->packets == NULL) {
        fprintf(stderr, "Error allocating memory for packets.\n");
        free(thread_args);
        return 1;
    }

    thread_args->break_while = false;
    init_packet_vector(thread_args->packets);
    pthread_t capture_thread, stats_thread;

    int opt;
    int interface_provided = 0;  // Track if interface is provided
    
    /**
     * Parsing input arguments
    */
    while ((opt = getopt(argc, argv, "i:s:t:")) != -1) {
        switch (opt) {
            case 'i':
                if (optarg == NULL) {
                    break;
                }
                strncpy(thread_args->args.interface, optarg, 254);
                interface_provided = 1;  // Mark that interface was provided
                break;
            case 's':
                if (optarg == NULL) {    
                    break;
                }
                strncpy(thread_args->args.sorting_criteria, optarg, 2);
                break;
            case 't':
                if (optarg == NULL) {
                    break;
                }
                thread_args->args.time = atoi(optarg);
                break;
            default:
                return EXIT_FAILURE;
        }
    }

    /**
     * @brief Check arguments
     */
    if (!interface_provided) {
        fprintf(stderr, "Error: No interface specified.\n");
        print_interfaces();
        exit(EXIT_FAILURE);
    }

    if (thread_args->args.time <= 0) {
        fprintf(stderr, "Error: Invalid time. Must be greater than 0.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(thread_args->args.sorting_criteria, "b") != 0 && strcmp(thread_args->args.sorting_criteria, "p") != 0) {
        fprintf(stderr, "Error: Invalid sorting criteria. Must be 'b' or 'p'.\n");
        exit(EXIT_FAILURE);
    } 

    // Mutex initialization
    if (pthread_mutex_init(&thread_args->vector_mutex, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex\n");
        free(thread_args->packets); 
        free(thread_args);   
        exit(EXIT_FAILURE);
    }

    // Packet capture thread
    if (pthread_create(&capture_thread, NULL, packet_capture_thread, (void*)thread_args) != 0) {
        fprintf(stderr, "Error creating packet capture thread\n");
        free(thread_args->packets);  
        free(thread_args);           
        exit(EXIT_FAILURE);
    }

    // Statistics dump thread
    if (pthread_create(&stats_thread, NULL, statistics_thread, (void*)thread_args) != 0) {
        fprintf(stderr, "Error creating statistics thread\n");
        free(thread_args->packets); 
        free(thread_args);  
        return EXIT_FAILURE;
    }

    // The main thread waits for the user to exit the program (Ctrl+C)
    pthread_join(capture_thread, NULL);
    pthread_join(stats_thread, NULL);

    // Releasing resources
    pthread_mutex_destroy(&thread_args->vector_mutex);
    free_packet_vector(thread_args->packets);
    free(thread_args->packets);
    free(thread_args);
    return 0;
}
