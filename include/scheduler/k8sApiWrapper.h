#include <config/kube_config.h>
#include <include/apiClient.h>
#include <api/CoreV1API.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <json/json.h>

#ifdef __cplusplus
extern "C"
{
#endif

    // C-compatible functions that call Kubernetes C API functions

    void create_pod(apiClient_t *apiClient, char *namespace);
    bool create(apiClient_t *apiClient);

#ifdef __cplusplus
}
#endif
