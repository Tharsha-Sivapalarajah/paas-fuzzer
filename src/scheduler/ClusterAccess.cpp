#include "ClusterAccess.h"

namespace scheduler
{
    // create a Deployment
    bool ClusterAccess::createDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc) const
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

        std::string _namespace = "default";
        v1_pod_list_t *pod_list_t = CoreV1API_listNamespacedPod(apiClient, const_cast<char *>(_namespace.c_str()), NULL, 0, NULL, NULL, NULL, 0, NULL, NULL, 60, 0, 0);

        list_t *pod_list = pod_list_t->items;

        listEntry_t *currentListEntry = pod_list->firstEntry;
        listEntry_t *nextListEntry;

        if (currentListEntry == NULL)
        {
            return false;
        }

        nextListEntry = currentListEntry->nextListEntry;

        if (std::strcmp(((v1_pod_t *)(currentListEntry->data))->metadata->name, const_cast<char *>(pod_name.c_str())) == 0)
        {
            return true;
        }

        currentListEntry = nextListEntry;

        while (currentListEntry != NULL)
        {
            nextListEntry = currentListEntry->nextListEntry;
            if (std::strcmp(((v1_pod_t *)(currentListEntry->data))->metadata->name, const_cast<char *>(pod_name.c_str())) == 0)
            {
                return true;
            }
            currentListEntry = nextListEntry;
        }
        return false;
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
    bool ClusterAccess::create(cJSON *jsonData, apiClient_t *apiClient, std::string namespc) const
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
                if (createDeployment(apiClient, jsonData, const_cast<char *>(namespc.c_str())))
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

    // object_t *constructPatchBody()
    // {
    // cJSON *patchArray = cJSON_CreateArray();
    // cJSON *patchObject = cJSON_CreateObject();

    // cJSON_AddItemToArray(patchArray, patchObject);

    // cJSON_AddStringToObject(patchObject, "op", "replace");
    // cJSON_AddStringToObject(patchObject, "path", "/spec/replicas");
    // cJSON_AddNumberToObject(patchObject, "value", 3);

    // object_t *patchBody = (object_t *)calloc(1, sizeof(object_t));
    // patchBody->temporary = cJSON_PrintUnformatted(patchArray);

    // cJSON_Delete(patchArray);

    // return patchBody;

    // Create a cJSON object representing a simple JSON structure
    // object_t *patchBody = (object_t *)calloc(1, sizeof(object_t));
    // cJSON *root = cJSON_CreateObject();
    // cJSON *rootArray = cJSON_CreateArray();
    // cJSON_AddArrayToObject(root, );

    // cJSON_Print()
    //     cJSON_AddStringToObject(root, "op", "replace");
    // cJSON_AddStringToObject(root, "path", "/spec/replicas");
    // cJSON_AddNumberToObject(root, "value", 3);

    // Convert the cJSON structure to a JSON-formatted string
    // char *jsonString = cJSON_Print(root);

    // Print the resulting JSON string
    // printf("JSON String:\n%s\n", jsonString);
    // object_t *patchBody = object_parseFromJSON(root);
    // // Free the cJSON structure and the dynamically allocated JSON string
    // cJSON_Delete(root);
    // free(jsonString);
    // return patchBody;
    // }

    // patch the existing Deployment
    bool ClusterAccess::patchDeployment(apiClient_t *apiClient, cJSON *jsonData, char *namespc, driver::Patch &patch) const
    {
        v1_deployment_t *deploymentInfo = v1_deployment_parseFromJSON(jsonData);
        // cJSON *tempJsonSpec = cJSON_CreateObject();
        // cJSON *tempJsonReplica = cJSON_CreateObject();
        // cJSON_AddNumberToObject(tempJsonReplica, "replicas", 3);
        // // cJSON_AddStringToObject(tempJsonReplica, "replicas", "3");
        // cJSON_AddItemToObject(tempJsonSpec, "spec", tempJsonReplica);

        // object_t *body = object_parseFromJSON(tempJsonSpec);

        // driver::JsonFileHandler fh;
        // fh.printCJSON(jsonData->child->next->next->next->child);
        // cJSON *valNumber = cJSON_CreateNumber(3);
        // jsonData->child->next->next->next->child = valNumber;
        // fh.printCJSON(jsonData->child->next->next->next->child);

        // cJSON *root = cJSON_CreateObject();

        // // Create the 'spec' object
        // cJSON *specObject = cJSON_CreateObject();

        // // Add the 'replicas' key-value pair to the 'spec' object
        // cJSON_AddNumberToObject(specObject, "replicas", 2);

        // // Add the 'spec' object to the outer cJSON object
        // cJSON_AddItemToObject(root, "spec", specObject);
        // object_t *body = object_parseFromJSON(root);

        cJSON *jsonArray = cJSON_CreateArray();

        // Create the first cJSON object representing the array element
        cJSON *jsonObject = cJSON_CreateObject();
        cJSON_AddStringToObject(jsonObject, "op", "replace");
        cJSON_AddStringToObject(jsonObject, "path", "/spec/replicas");
        cJSON_AddNumberToObject(jsonObject, "value", 2);

        // Add the cJSON object to the array
        cJSON_AddItemToArray(jsonArray, jsonObject);
        object_t *body = object_parseFromJSON(jsonArray);
        // Call the patchNamespacedDeployment function
        v1_deployment_t *updatedDeployment = AppsV1API_patchNamespacedDeployment(apiClient, deploymentInfo->metadata->name, namespc, body, NULL, NULL, NULL, NULL, 0);

        // Handle the response or errors as needed
        if (updatedDeployment != NULL)
        {
            printf("%s\n", (char *)apiClient->response_code);
            // Handle the updated Deployment
            printf("Deployment updated successfully\n");
            return true;
        }
        else
        {
            // Handle the error
            fprintf(stderr, "Error updating Deployment\n");
        }

        // Cleanup resources
        // freeApiClient(client);

        cJSON_Delete(jsonArray);

        return false;

        // v1_deployment_t *adeployment = AppsV1API_createNamespacedDeployment(apiClient, namespc, deploymentInfo, NULL, NULL, NULL, NULL);
        // // printf("code=%ld\n", apiClient->response_code);
        // v1_deployment_free(adeployment);
        // v1_deployment_free(deploymentInfo);
        // if (apiClient->response_code == 201 || apiClient->response_code == 200)
        //     return true;
        // return false;
    }

    // create a patch
    bool ClusterAccess::patch(cJSON *jsonData, apiClient_t *apiClient, std::string namespc, driver::Patch &patch) const
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
                if (patchDeployment(apiClient, jsonData, const_cast<char *>(namespc.c_str()), patch))
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
                if (patchDeployment(apiClient, jsonData, const_cast<char *>(namespc.c_str()), patch))
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

int main(int argc, char *argv[])
{
    // driver::JsonFileHandler fileHandler;
    // Json::Value originalData;
    // std::string location = "../../../data/output.json";
    // if (fileHandler.readJsonFile(originalData, location))
    // {
    //     scheduler::ClusterAccess ca;
    //     cJSON *jsonData = fileHandler.generateCJSON(originalData);

    //     std::vector<std::string> vec = {};
    //     driver::Patch patch(vec, 0, "30");
    //     ca.patch(jsonData, NULL, "default", patch);

    //     cJSON_Delete(jsonData);
    // }
    // else
    // {
    //     std::cout << "Read failed" << std::endl;
    // }
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