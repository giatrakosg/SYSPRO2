
#include "Client.hpp"
#include <signal.h>
#define SLEEP_PERIOD 3

bool runFlag ;
pid_t failpid ;
void sigstop_handler(int signum,siginfo_t *si,void *data) {
    printf("Signal %d from pid %lu\n", (int)si->si_signo,
       (unsigned long)si->si_pid);
    runFlag = false ;
}
void sigusr_handler(int signum,siginfo_t *si,void *data){
    printf("Signal %d from pid %lu\n", (int)si->si_signo,
       (unsigned long)si->si_pid);
}


int main(int argc, char *argv[]) {
    struct sigaction usr_act ;
    struct sigaction sigint_act ;
    struct sigaction sigstop_act ;
    memset(&usr_act, 0, sizeof(usr_act));
    memset(&sigint_act, 0, sizeof(sigint_act));
    memset(&sigstop_act, 0, sizeof(sigstop_act));

    usr_act.sa_sigaction = sigusr_handler ;
    usr_act.sa_flags = SA_SIGINFO ;

    sigint_act.sa_sigaction = sigstop_handler ;
    sigint_act.sa_flags = SA_SIGINFO ;

    sigstop_act.sa_sigaction = sigstop_handler ;
    sigstop_act.sa_flags = SA_SIGINFO ;

    if (sigaction(SIGUSR1, &usr_act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}


    if (sigaction(SIGQUIT, &sigint_act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}
    if (sigaction(SIGINT, &sigstop_act, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}

    Client *cli = new Client ;
    runFlag = true ;

    cli->getArgs(argc,argv);
    //cli->printArgs();
    int err = cli->parseArgs();
    if (err < 0) {
        delete cli ;
        return -1;
    }
    err = cli->writeID();
    if (err < 0) {
        delete cli ;
        return -1 ;
    }
    while (runFlag == true) {
        sleep(SLEEP_PERIOD);
        cli->detectNewID();
    }

    delete cli ;
    return 0;
}
