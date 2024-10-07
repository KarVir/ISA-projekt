/**
 * @file isa-top.c
 * @author Karolína Virágová,xvirag00
 * @brief ISA Project
 * 
 * @date 2024-10-21 
 */

#include "packet_handler.h"
#include "vector.h"
#include "display.h"

thread_args_t* thread_args;

void signal_handler(int sig) {
    if (sig == SIGINT) {
        // Přeruší smyčku pcap
        pcap_breakloop(thread_args->handle);
        thread_args->break_while = true;
    }
}

int main(int argc, char* argv[])
{
    
    signal(SIGINT, signal_handler);
    thread_args = (thread_args_t*)malloc(sizeof(thread_args_t));

    if (thread_args == NULL) {
    fprintf(stderr, "Chyba při alokaci paměti pro thread_args_t.\n");
    exit(1);
    } 

    thread_args->packets = (packet_vector_t*)malloc(sizeof(packet_vector_t));
    if (thread_args->packets == NULL) {
        fprintf(stderr, "Chyba při alokaci paměti pro packets.\n");
        free(thread_args);
        return 1;
    }

    thread_args->break_while = false;
    init_packet_vector(thread_args->packets);
    strcpy(thread_args->args.sorting_criteria, "b");

    pthread_t capture_thread, stats_thread;

    int opt;
    
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
                break;
            case 's':
                if (optarg == NULL) {    
                    break;
                }
                strncpy(thread_args->args.sorting_criteria, optarg, 1);
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
     * @brief check arguments program
     * TODO
     */

    // TODO POPIS
    if (pthread_mutex_init(&thread_args->vector_mutex, NULL) != 0) {
        fprintf(stderr, "Chyba při inicializaci mutexu\n");
        free(thread_args->packets);  // Uvolnění alokované paměti pro packets
        free(thread_args);           // Uvolnění alokované paměti pro thread_args
        return EXIT_FAILURE;
    }

    // TODO POPIS
    if (pthread_create(&capture_thread, NULL, packet_capture_thread, (void*)thread_args) != 0) {
        fprintf(stderr, "Chyba při vytváření vlákna pro zachytávání paketů\n");
        free(thread_args->packets);  // Uvolnění alokované paměti pro packets
        free(thread_args);            // Uvolnění alokované paměti pro thread_args
        return EXIT_FAILURE;
    }

    if (pthread_create(&stats_thread, NULL, statistics_thread, (void*)thread_args) != 0) {
        fprintf(stderr, "Chyba při vytváření vlákna pro statistiky\n");
        free(thread_args->packets);  // Uvolnění alokované paměti pro packets
        free(thread_args);  
        return EXIT_FAILURE;
    }

    // Hlavní vlákno čeká, než uživatel ukončí program (Ctrl+C)
    pthread_join(capture_thread, NULL);
    pthread_join(stats_thread, NULL);

    // Uvolnění prostředků
    free_packet_vector(thread_args->packets);
    pthread_mutex_destroy(&thread_args->vector_mutex);
    free(thread_args->packets);
    free(thread_args);
    return 0;
}