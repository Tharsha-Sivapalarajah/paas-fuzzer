from enum import Enum
from kubernetes import client, watch, config
import time
import threading


class DesiredPhase(Enum):
    pending = "Pending"
    succeed = "Succeeded"
    running = "Running"
    failed = "Failed"
    unknown = "Unknown"
    terminating = "Terminating"
    terminated = "Terminated"


def check_if_pod_name_exists(pod_name):
    result = api_instance.list_pod_for_all_namespaces(watch=False)
    for i in result.items:
        if pod_name == i.metadata.name:
            return True
    return False


def calculate_pod_health_time(
    namespace,
    pod_name,
    desired_phase=[DesiredPhase.running.value, DesiredPhase.succeed.value],
    timeout=60,
):
    """
    Calculates the time taken for a pod to reach a desired phase.

    Args:
        namespace (str): The namespace of the pod.
        pod_name (str): The name of the pod.
        desired_phase (list, optional): List of desired phases. Defaults to ['Running', 'Succeeded'].
        timeout (int, optional): Timeout in seconds. Defaults to 60.
    """
    w = watch.Watch()
    start_time = None
    print(api_instance)
    try:
        end_time = time.time() + timeout
        for event in w.stream(api_instance.list_namespaced_pod, namespace):
            pod = event["object"]
            pod_name_from_event = pod.metadata.name
            pod_phase = pod.status.phase

            if pod_name_from_event == pod_name:
                if pod_phase in desired_phase:
                    if start_time is not None:
                        end_time = time.time()
                        health_time = end_time - start_time
                        print(
                            f"Pod {pod_name} became healthy in {health_time:.2f} seconds."
                        )
                        break
                elif pod_phase == "Pending" and start_time is None:
                    start_time = time.time()

            if time.time() > end_time:
                print(
                    f"Timeout: Pod {pod_name} did not become {desired_phase[0]} within {timeout} seconds."
                )
                break

    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    config.load_kube_config()
    api_instance = client.CoreV1Api()

    while True:
        namespace = "default"
        pod_name = input("Enter the pod name: ")
        if pod_name == "quit":
            break

        if not check_if_pod_name_exists(pod_name):
            calculate_pod_health_time(namespace, pod_name, timeout=10)
        else:
            print(f"The Pod with the name {pod_name} is already deployed")
