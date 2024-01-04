#pragma once

extern "C"
{
#include <config/kube_config.h>
#include <include/apiClient.h>
#include <api/CoreV1API.h>
#include <api/AppsV1API.h>
#include <model/v1_object_meta.h>
#include <model/object.h>
#include <external/cJSON.h>
#include <model/v1_pod.h>
#include <model/v1_deployment.h>
}

#include <Patch.h>

namespace monitoring
{
    class Monitoring
    {
    public:
        bool check_pod_exists(apiClient_t *apiClient, std::string pod_name) const;
        bool get_namespaced_events(apiClient_t *apiClient, std::string pod_name, std::string name_space) const;

    private:
        bool createAPI_client(apiClient_t **apiClient) const;
    };
}