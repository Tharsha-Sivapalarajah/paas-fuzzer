#include "ClusterAccess.h"

namespace scheduler
{
    // create a Deployment
    bool ClusterAccess::createDeployment(apiClient_t *apiClient, cJSON *jsonData, char *_namespace) const
    {
        if (!jsonData)
        {
            printf("Invalid cJSON data file");
            return false;
        }
        if (!apiClient)
        {
            printf("Invalid k8s API Client");
            return false;
        }

        v1_deployment_t *deploymentInfo = v1_deployment_parseFromJSON(jsonData);

        v1_deployment_t *adeployment = AppsV1API_createNamespacedDeployment(apiClient, _namespace, deploymentInfo, NULL, NULL, NULL, NULL);
        // printf("code=%ld\n", apiClient->response_code);
        v1_deployment_free(adeployment);
        v1_deployment_free(deploymentInfo);
        if (apiClient->response_code == 201 || apiClient->response_code == 200)
            return true;
        return false;
    }

    // delete an existing Deployment
    bool ClusterAccess::deleteDeployment(apiClient_t *apiClient, cJSON *jsonData, char *_namespace) const
    {
        if (!jsonData)
        {
            printf("Invalid cJSON data file");
            return false;
        }
        if (!apiClient)
        {
            printf("Invalid k8s API Client");
            return false;
        }

        v1_deployment_t *deploymentInfo = v1_deployment_parseFromJSON(jsonData);

        v1_status_t *deploymentDeleteStatus = AppsV1API_deleteNamespacedDeployment(apiClient, deploymentInfo->metadata->name, _namespace, NULL, NULL, 30, 0, NULL, NULL);

        // printf("Status code: %ld\n", apiClient->response_code);
        v1_status_free(deploymentDeleteStatus);
        v1_deployment_free(deploymentInfo);
        if (apiClient->response_code == 200)
            return true;
        return false;
    }

    // create a pod
    bool ClusterAccess::createPod(apiClient_t *apiClient, cJSON *jsonData, char *_namespace) const
    {
        if (!jsonData)
        {
            printf("Invalid cJSON data file");
            return false;
        }
        if (!apiClient)
        {
            printf("Invalid k8s API Client");
            return false;
        }

        v1_pod_t *podInfo = v1_pod_parseFromJSON(jsonData);
        v1_pod_t *apod = CoreV1API_createNamespacedPod(apiClient, _namespace, podInfo, NULL, NULL, NULL, NULL);
        // printf("code=%ld\n", apiClient->response_code);
        v1_pod_free(apod);
        v1_pod_free(podInfo);
        if (apiClient->response_code == 201 || apiClient->response_code == 200)
            return true;
        return false;
    }

    // delete an existing pod
    bool ClusterAccess::deletePod(apiClient_t *apiClient, cJSON *jsonData, char *_namespace) const
    {
        if (!jsonData)
        {
            printf("Invalid cJSON data file");
            return false;
        }
        if (!apiClient)
        {
            printf("Invalid k8s API Client");
            return false;
        }

        char *podName = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(jsonData, "metadata"), "name"));
        v1_pod_t *pod = CoreV1API_deleteNamespacedPod(apiClient, podName, _namespace, NULL, NULL, 30, 0, NULL, NULL);

        if (pod)
        {
            v1_pod_free(pod);
            pod = NULL;
        }

        // printf("The return code of HTTP request=%ld\n", apiClient->response_code);
        if (200 == apiClient->response_code)
            return true;
        return false;
    }

    bool ClusterAccess::get_namespaced_events(apiClient_t *apiClient, std::string pod_name, std::string _namespace) const
    {
        if (apiClient == NULL)
        {
            if (!createAPI_client(&apiClient))
            {
                printf("Cannot load kubernetes configuration.\n");
                return false;
            }
        }
        core_v1_event_list_t *event_list = CoreV1API_listNamespacedEvent(apiClient, const_cast<char *>(_namespace.c_str()), NULL, 0, NULL, NULL, NULL, 0, NULL, NULL, 60, 0, 0);
        std::cout << (char *)(event_list->items->firstEntry->data) << std::endl;
        return true;
    }

    bool ClusterAccess::check_pod_exists(apiClient_t *apiClient, std::string pod_name) const
    {
        if (apiClient == NULL)
        {
            if (!createAPI_client(&apiClient))
            {
                printf("Cannot load kubernetes configuration.\n");
                return false;
            }
        }
        return false;

        // std::string _namespace = "default";
        // v1_pod_list_t *pod_list_t = CoreV1API_listNamespacedPod(apiClient, const_cast<char *>(_namespace.c_str()), NULL, 0, NULL, NULL, NULL, 0, NULL, NULL, 60, 0, 0);

        // list_t *pod_list = pod_list_t->items;

        // listEntry_t *currentListEntry = pod_list->firstEntry;
        // listEntry_t *nextListEntry;

        // if (currentListEntry == NULL)
        // {
        //     return false;
        // }

        // nextListEntry = currentListEntry->nextListEntry;

        // if (std::strcmp(((v1_pod_t *)(currentListEntry->data))->metadata->name, const_cast<char *>(pod_name.c_str())) == 0)
        // {
        //     return true;
        // }

        // currentListEntry = nextListEntry;

        // while (currentListEntry != NULL)
        // {
        //     nextListEntry = currentListEntry->nextListEntry;
        //     if (std::strcmp(((v1_pod_t *)(currentListEntry->data))->metadata->name, const_cast<char *>(pod_name.c_str())) == 0)
        //     {
        //         return true;
        //     }
        //     currentListEntry = nextListEntry;
        // }
        // return false;
    }

    // create a Kubernetes API Client
    bool ClusterAccess::createAPI_client(apiClient_t **apiClient) const
    {
        int rc = 0;

        char *baseName = NULL;
        sslConfig_t *sslConfig = NULL;
        list_t *apiKeys = NULL;

        rc = load_kube_config(&baseName, &sslConfig, &apiKeys, NULL);

        if (0 == rc)
        {
            *apiClient = apiClient_create_with_base_path(baseName, sslConfig, apiKeys);
            return true;
            // printf("Basename: %s\nSSLConfig: %s:%s:%s\n", baseName, sslConfig->clientCertFile, sslConfig->clientKeyFile, sslConfig->CACertFile);
        }
        else
        {
            // printf("Cannot load kubernetes configuration.\n");
            return false;
        }
    }

    // create resource with the provided JSON data
    bool ClusterAccess::create(cJSON *jsonData, apiClient_t *apiClient, std::string _namespace) const
    {
        if (apiClient == NULL)
        {
            if (!createAPI_client(&apiClient))
            {
                printf("Cannot load kubernetes configuration.\n");
                return false;
            }
        }

        cJSON *kind = cJSON_GetObjectItem(jsonData, "kind");
        if (kind != NULL)
        {
            std::string kindVal = kind->valuestring;
            std::transform(kindVal.begin(), kindVal.end(), kindVal.begin(), ::toupper);

            if (kindVal == "DEPLOYMENT")
            {
                if (createDeployment(apiClient, jsonData, const_cast<char *>(_namespace.c_str())))
                {
                    std::cout << "Deployment successful" << std::endl;
                }
                else
                {
                    std::cout << "Deplyoment failed" << std::endl;
                    return false;
                }
            }
            else if (kindVal == "POD")
            {
                if (createPod(apiClient, jsonData, const_cast<char *>(_namespace.c_str())))
                {
                    std::cout << "Deplyoment successful" << std::endl;
                }
                else
                {
                    std::cout << "Deplyoment failed" << std::endl;
                    return false;
                }
            }
        }

        return true;
    }

    // patch the existing Deployment
    bool ClusterAccess::patchDeployment(apiClient_t *apiClient, cJSON *jsonData, char *_namespace, driver::Patch &patch) const
    {
        v1_deployment_t *deploymentInfo = v1_deployment_parseFromJSON(jsonData);

        std::string patchBody = "[{\"op\": \"replace\", \"path\": \"" + patch.getPatchPathString() + "\", \"value\":" + patch.getPatchValue() + "}]";
        genericClient_t *genericClient = genericClient_create(apiClient, "apps", "v1", "deployments");

        list_t *contentType = list_createList();
        list_addElement(contentType, (char *)"application/json-patch+json");

        char *list = Generic_patchNamespacedResource(genericClient, "default", deploymentInfo->metadata->name, const_cast<char *>(patchBody.c_str()), NULL, NULL, NULL, NULL, contentType);
        free(list);

        return true;
    }

    // create a patch
    bool ClusterAccess::patch(cJSON *jsonData, apiClient_t *apiClient, std::string _namespace, driver::Patch &patch) const
    {
        if (apiClient == NULL)
        {
            if (!createAPI_client(&apiClient))
            {
                printf("Cannot load kubernetes configuration.\n");
                return false;
            }
        }

        cJSON *kind = cJSON_GetObjectItem(jsonData, "kind");
        if (kind != NULL)
        {
            std::string kindVal = kind->valuestring;
            std::transform(kindVal.begin(), kindVal.end(), kindVal.begin(), ::toupper);

            if (kindVal == "DEPLOYMENT")
            {
                if (patchDeployment(apiClient, jsonData, const_cast<char *>(_namespace.c_str()), patch))
                {
                    std::cout << "Deployment successful" << std::endl;
                }
                else
                {
                    std::cout << "Deplyoment failed" << std::endl;
                }
            }
            else if (kindVal == "POD")
            {
                if (patchDeployment(apiClient, jsonData, const_cast<char *>(_namespace.c_str()), patch))
                {
                    std::cout << "Deplyoment successful" << std::endl;
                }
                else
                {
                    std::cout << "Deplyoment failed" << std::endl;
                }
            }
        }

        return true;
    }
}

// int main(int argc, char *argv[])
// {
//     driver::JsonFileHandler fileHandler;
//     Json::Value originalData;
//     std::string location = "../../../data/output.json";
//     if (fileHandler.readJsonFile(originalData, location))
//     {
//         scheduler::ClusterAccess ca;
//         cJSON *jsonData = fileHandler.generateCJSON(originalData);

//         Json::Value configFile = originalData;
//         std::vector<driver::Patch> patches = {};
//         fileHandler.createConfigFile(configFile, patches);
//         // ca.patch(jsonData, NULL, "default", patches[0]);

//         ca.get_namespaced_events(NULL, "busybox-deployment-65fd7d9646-8pqvr", "default");

//         cJSON_Delete(jsonData);
//     }
//     else
//     {
//         std::cout << "Read failed" << std::endl;
//     }
//     // scheduler::ClusterAccess ca;
//     // if (ca.check_pod_exists(NULL, "busybox-deployment-65fd7d9646-7hqc7"))
//     // {
//     //     printf("Pod exists\n");
//     // }
//     // else
//     // {
//     //     printf("Pod doesn't exists\n");
//     // }
//     return 0;
// }