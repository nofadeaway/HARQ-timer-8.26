#include "FuncHead.h"

#define SEND_SIZE 400

using namespace srslte;
using namespace srsue;

extern pthread_barrier_t barrier;
extern pthread_mutex_t pdu_gets;
extern rlc_um rlc_test[];

extern UE_FX ue_test; //map容器

void tongbu()    //用于最开始的对齐
{
    printf("Tong bu!!!\n");
    int port = atoi("4567");
    int st = socket(AF_INET, SOCK_DGRAM, 0);
    int sys=1;
    struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("10.129.4.106"); //目的实际地址
    sendto(st, &sys, sizeof(int), 0, (struct sockaddr *)&addr,sizeof(addr));
}



void handle(union sigval v)
{
    bool flag;
    uint32_t subframe_now = 0;
    int flag_sub;

    subframe_now = ue_test.subframe_now();
    printf("\nNow subframe is NO.%d\n", subframe_now);
    
    flag_sub = ue_test.subframe_process_now();
    if (flag_sub == -1)
    {
        printf("Invaild subframes!\n");
    }
    else if (flag_sub == 0) //1为切换子帧
    {
    }
    else if (flag_sub == 1) //下行帧
    {
        usleep(5000);
        flag = lte_send_udp(NULL);
    }
    else
    {
        flag = lte_rece(NULL); //上行帧
    }
    //printf("\n\nWork done!\n\n");
    ue_test.subframe_end();
}

void *lte_send_recv(void *ptr)
{

    bool flag;
    uint32_t subframe_now = 0;
    int flag_sub;
    ue_test.set_subframe(5);
    pthread_barrier_wait(&barrier);
    sleep(2);

    struct sigevent evp;
    struct itimerspec ts;
    timer_t timer;
    int ret;

    memset (&evp, 0, sizeof (evp));
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = handle;

    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if (ret)
        perror("timer_create");

    ts.it_interval.tv_sec = 1;
    ts.it_interval.tv_nsec = 0;
    ts.it_value.tv_sec = 3;
    ts.it_value.tv_nsec = 0;

    tongbu();

    ret = timer_settime(timer, 0, &ts, NULL);
    if (ret)
        perror("timer_settime");

    while (1)
    {
    }

    printf("Timer over!\n");
    timer_delete(timer);
}