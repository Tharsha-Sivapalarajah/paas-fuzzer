#include "ClusterAccess.h"

namespace scheduler
{
    // create a Deployment
    bool ClusterAccess::createDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const
    {
        v1_deployment_t *deploymentInfo = v1_deployment_parseFromJSON(jsonData);

        v1_deployment_t *adeployment = AppsV1API_createNamespacedDeployment(apiClient, namespc, deploymentInfo, NULL, NULL, NULL, NULL);
        // printf("code=%ld\n", apiClient->response_code);
        v1_deployment_free(adeployment);
        v1_deployment_free(deploymentInfo);
        if (apiClient->response_code == 201 || apiClient->response_code == 200)
            return true;
        return false;
    }

    // delete an existing Deployment
    bool ClusterAccess::deleteDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const
    {
        v1_deployment_t *deploymentInfo = v1_deployment_parseFromJSON(jsonData);

        v1_status_t *deploymentDeleteStatus = AppsV1API_deleteNamespacedDeployment(apiClient, deploymentInfo->metadata->name, namespc, NULL, NULL, 30, 0, NULL, NULL);

        // printf("Status code: %ld\n", apiClient->response_code);
        v1_status_free(deploymentDeleteStatus);
        v1_deployment_free(deploymentInfo);
        if (apiClient->response_code == 200)
            return true;
        return false;
    }

    // create a pod
    bool ClusterAccess::createPod(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const
    {
        v1_pod_t *podInfo = v1_pod_parseFromJSON(jsonData);
        v1_pod_t *apod = CoreV1API_createNamespacedPod(apiClient, namespc, podInfo, NULL, NULL, NULL, NULL);
        // printf("code=%ld\n", apiClient->response_code);
        v1_pod_free(apod);
        v1_pod_free(podInfo);
        if (apiClient->response_code == 201 || apiClient->response_code == 200)
            return true;
        return false;
    }

    // delete an existing pod
    bool ClusterAccess::deletePod(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const
    {
        char *podName = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(jsonData, "metadata"), "name"));
        v1_pod_t *pod = CoreV1API_deleteNamespacedPod(apiClient, podName, namespc, NULL, NULL, 30, 0, NULL, NULL);

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

    // create a Kubernetes API Client
    bool ClusterAccess::createAPI_client(apiClient_t *apiClient) const
    {
        int rc = 0;

        char *baseName = NULL;
        sslConfig_t *sslConfig = NULL;
        list_t *apiKeys = NULL;

        rc = load_kube_config(&baseName, &sslConfig, &apiKeys, NULL);

        if (0 == rc)
        {
            apiClient = apiClient_create_with_base_path(baseName, sslConfig, apiKeys);
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
    bool ClusterAccess::create(cJSON *jsonData, apiClient_t *apiClient, std::string namespc) const
    {
        if (apiClient == NULL)
        {
            if (!createAPI_client(apiClient))
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
                if (createDeployment(apiClient, jsonData, const_cast<char *>(namespc.c_str())))
                {
                    // v1_deployment_t *deploymentInfo = v1_deployment_parseFromJSON(jsonData);

                    // v1_deployment_t depl = AppsV1API_patchNamespacedDeployment(apiClient, deploymentInfo->metadata->name, const_cast<char *>(namespc.c_str()), object_t * body , NULL, NULL, char * fieldManager , char * fieldValidation , int force )
                    std::cout << "Deplyoment successful" << std::endl;
                }
                else
                {
                    std::cout << "Deplyoment failed" << std::endl;
                }
            }
            else if (kindVal == "POD")
            {
                if (createPod(apiClient, jsonData, const_cast<char *>(namespc.c_str())))
                {
                    std::cout << "Deplyoment successful" << std::endl;
                }
                else
                {
                    std::cout << "Deplyoment failed" << std::endl;
                }
            }
            printf("%s\n", kind->valuestring);
        }

        return true;
    }

    // create a patch
    bool ClusterAccess::patch(cJSON *jsonData, apiClient_t *apiClient, std::string namespc) const
    {
        if (apiClient == NULL)
        {
            if (!createAPI_client(apiClient))
            {
                printf("Cannot load kubernetes configuration.\n");
                return false;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    driver::JsonFileHandler fileHandler;
    Json::Value originalData;
    std::string location = "../../../data/output.json";
    if (fileHandler.readJsonFile(originalData, location))
    {
        scheduler::ClusterAccess ca;
        cJSON *jsonData = fileHandler.generateCJSON(originalData);

        ca.create(jsonData, NULL, "default");
        cJSON_Delete(jsonData);
    }
    else
    {
        std::cout << "Read failed" << std::endl;
    }

    return 0;
}