#pragma once

#include "ClusterAccess.h"
#include "Patch.h"

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