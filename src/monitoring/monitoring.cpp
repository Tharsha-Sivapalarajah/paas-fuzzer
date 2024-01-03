#include "monitoring.h"

using namespace monitoring;

int main()
{
    scheduler::ClusterAccess ca;

    if (ca.check_pod_exists(NULL, "busybox-deployment-65fd7d9646-7hqc7"))
    {
        printf("Pod exists\n");
    }
    else
    {
        printf("Pod doesn't exists\n");
    }
    return 0;
}