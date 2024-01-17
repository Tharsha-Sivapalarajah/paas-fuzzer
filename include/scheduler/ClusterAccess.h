#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>
#include <cctype>
#include <string>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>

#include <json/json.h>
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
#include "include/generic.h"
}

#include <Patch.h>
#include <JsonFileHandler.h>

namespace scheduler
{
    class ClusterAccess
    {
    public:
        bool create(cJSON *jsonData, apiClient_t *apiClient, std::string _namespace) const;
        bool patch(cJSON **jsonData, apiClient_t *apiClient, std::string namespc, driver::Patch &patch) const;
        bool check_pod_exists(apiClient_t *apiClient, std::string pod_name) const;
        bool get_namespaced_events(apiClient_t *apiClient, std::string pod_name, std::string name_space) const;
        cJSON *get_namespaced_deployment(cJSON *jsonData, apiClient_t *apiClient, std::string _namespace) const;
        bool isPropagationComplete(cJSON *initialConfig, driver::Patch *patch);

    private:
        bool createAPI_client(apiClient_t **apiClient) const;
        bool deletePod(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const;
        bool createPod(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const;
        bool deleteDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const;
        bool createDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const;
        bool patchDeployment(apiClient_t *apiClient, cJSON **jsonData, char *namespc, driver::Patch &patch) const;
    };
};