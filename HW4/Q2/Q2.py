import threading
import requests
import time

start_time = time.time()
def thread_function():
    global end_time
    requests.get('https://google.com')
    print(time.time() - start_time)


if __name__ == "__main__":
    for _ in range(20):
        x = threading.Thread(target=thread_function, args=())
        x.start()
