#include "mainwindow.h"
#include <QApplication>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
//defines for signal
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include "thpool.h"
#include "multicast.h"
#include <boost/bind.hpp>

#define BUFFER_SIZE 1024
#define DATA_UPDATE_HZ 33
#define DATA_UPDATE_USEC 1000000/DATA_UPDATE_HZ
#define MAX_DELAY_TIME_US 5000

// ###### THREAD DATA ######
/// \brief a mutex to avoid multi-thread access to message
pthread_mutex_t message_mutex = PTHREAD_MUTEX_INITIALIZER;
/// \brief thread pool to allow dynamic task assignment
threadpool thpool_t;
///  \brief buffer to hold received data
uint8_t buffer[BUFFER_SIZE];
/// \brief main udp data receiving thread
pthread_t recv_monitor_thread;
/// \brief struct to specify timer options for sending thread
struct itimerval timer;
// #########################

void* udpReceivingThread(void *mcast);
void processInfo(void *packet);
MainWindow *window;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    window = &w;
    Multicast rtdb;

    //Setup Updated timer thread
    // #########################
    /*signal(SIGALRM,sendRobotInformationUpdate);
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = DATA_UPDATE_USEC;
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = DATA_UPDATE_USEC;*/
    // #########################

    //Setup Thread pool and receving thread
    // #########################
    thpool_t = thpool_init(6); //5 threads per agent ?
    pthread_create(&recv_monitor_thread, NULL, udpReceivingThread, &rtdb);

    // Run functions and join threads
    // #########################
    //setitimer (ITIMER_REAL, &timer, NULL);
    a.exec();
    pthread_join(recv_monitor_thread, NULL);
    thpool_destroy(thpool_t);
    rtdb.closeSocket();
    pthread_exit(NULL);
}

/// \brief main udp receiving thread. This thread deals with datagram reception
/// and sends the received data to be dealt by a thread using threadpool
/// \param socket - pointer to socket descriptor to use
void* udpReceivingThread(void *mcast)
{
   Multicast *rtdb = (Multicast*)mcast;
   int recvlen = 0;
   while(1){
      bzero(buffer, BUFFER_SIZE);
      if((recvlen = rtdb->receiveData(buffer,BUFFER_SIZE)) > 0 ){
         udp_packet *relay_packet = new udp_packet;
         relay_packet->packet = new uint8_t[recvlen];
         relay_packet->packet_size = recvlen;
         memcpy(relay_packet->packet,buffer,recvlen);
         thpool_add_work(thpool_t, processInfo, relay_packet);
      }
   }

   return NULL;
}

void processInfo(void *packet)
{
    window->onUpdateFromRobot(packet);
}
