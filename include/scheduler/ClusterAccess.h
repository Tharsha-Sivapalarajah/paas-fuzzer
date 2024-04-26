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
#include <thread>
#include <chrono>
#include <cstdlib>

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
        bool create(cJSON *jsonData, apiClient_t *apiClient, std::string _namespace, int verbose) const;
        bool patch(cJSON **jsonData, apiClient_t *apiClient, std::string namespc, driver::Patch &patch, int verbose) const;
        bool check_pod_exists(apiClient_t *apiClient, std::string pod_name, int verbose) const;
        bool get_namespaced_events(apiClient_t *apiClient, std::string pod_name, std::string name_space, int verbose) const;
        cJSON *get_namespaced_deployment(cJSON *jsonData, apiClient_t *apiClient, std::string _namespace, int verbose) const;
        bool isPropagationComplete(cJSON *initialConfig, driver::Patch &patch, int verbose);
        bool bugExists(cJSON *initialConfig, apiClient_t *apiClient, std::string _namespace, int verbose) const;
        bool reset(cJSON *initialConfig, apiClient_t *apiClient, std::string _namespace, int verbose) const;
        bool createAPI_client(apiClient_t **apiClient) const;
        bool patchCustom(cJSON **jsonData, char *_namespace, driver::Patch &patch, int verbose) const;

    private:
        bool deletePod(apiClient_t *apiClient, cJSON *jsonData, char *namespc, int verbose) const;
        bool createPod(apiClient_t *apiClient, cJSON *jsonData, char *namespc, int verbose) const;
        bool deleteDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc, int verbose) const;
        bool createDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc, int verbose) const;
        bool patchDeployment(apiClient_t *apiClient, cJSON **jsonData, char *namespc, driver::Patch &patch, int verbose) const;
        bool compareConfigFile(cJSON *baseConfig, cJSON *checkConfig, int verbose) const;
    };
};